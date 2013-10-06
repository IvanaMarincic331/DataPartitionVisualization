/* Xarea.h
  Holds all the drawing functions. This file does not need changing to complete the Area under the Curve module.
* AUTHOR: Ivana Marincic, Macalester College '15
* DATE: July 2013
* Based on the Xlib tutorial "Anatomy of the most basic Xlib program" by C. Tronche, 
*  	http://tronche.com/gui/x/xlib-tutorial/2nd-program-anatomy.html
*/
#ifndef XAREA_H
#define XAREA_H

/*import libraries*/
#ifndef NO_X11
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include "colors.h"
#endif

#include <stdio.h>    // For sprintf
#include <stdlib.h>   // For exit/EXIT_FAILURE
#include <unistd.h>	// For usleep()
#include "structs.h"

/***************************************************************************
* freeVideo()
*	closes the X display 
***************************************************************************/
void free_video(struct area_t * area) {
  struct display_t * d = &(area->d);
  #ifndef NO_X11
  XCloseDisplay(d->dpy);
  #endif
}

/***************************************************************************
* get_color()
*	based on the rank of the running process and its threads, this function 
*	returns a color specific to this number by calling a corresponding 
*	function in colors.h that sets this color to the foreground
***************************************************************************/
void get_color(struct area_t * area){
  struct display_t * d = &(area->d);
  #ifndef NO_X11
  int id=0;
  #ifdef OMP
  id = omp_get_thread_num();
  #endif
  int order = 2 * area->rank + id;
  switch(order){
    case RED:
      cred(d);
      break;
    case BLUE:
      cblue(d);
      break;
    case YELLOW:
      cyellow(d);
      break;
    case BISQUE:
      cbisque(d);
      break;
    case VIOLET:
      cviolet(d);
      break;
    case CYAN:
      ccyan(d);
      break;
    case OLIVE:
      colive(d);
      break;
    case GRAY:
      cgray(d);
      break;
    case PINK:
      cpink(d);
      break;
    case LAVENDER:
      clavender(d);
      break;
    case ORANGE:
      corange(d);
      break;
    case BROWN:
      cbrown(d);
      break;
    default: 
      get_extra_colors(order,d);
      break;
  }
  #endif
}

/***************************************************************************
* moveWindow()
*	moves each X window to the desired position. If there are multiple
*	processes the X windows should be aligned side by side.
***************************************************************************/
void moveWindow(struct area_t * area){
  struct display_t * d = &(area->d);
  #ifndef NO_X11
  d->posy = 10;
  int gap;
  if(area->gap) gap = 10*area->rank;
  else gap = 0;
  
  double rect_width = area->rect.width;
  if(area->circle) rect_width = area->rect.circle_width;
  else if(area->sin) rect_width = area->rect.sin_width;
  
  int private_num_rect = area->rect.num_rect/area->numProcs;
  int extra_rectangles = area->rect.private_num_rect - private_num_rect;
  int width = d->width;
  if(extra_rectangles == 0){
    if(area->rank == area->numProcs -1) width = d->width - EXTRA_PIXELS;
    d->posx = 100 + width*area->rank + gap;
  }
  else {
    d->posx = 100+ d->width*area->rank+gap;
    if(area->rank == area->numProcs -1) d->posx -= area->rank*(rect_width*extra_rectangles+EXTRA_PIXELS);
    else d->posx -= rect_width*(area->rect.num_rect%area->numProcs);
  }
  XMoveWindow(d->dpy, d->w, d->posx,d->posy);
  #endif
}

/***************************************************************************
 * Define the height of each X window*
 ***************************************************************************/
void setup_window_height(struct area_t * area) {
  struct display_t * d = &(area->d);
  if(area->linear) 
    area->d.height = FUNC(area,area->curve.xright);
  else if(area->circle) 
    area->d.height = scale_circle(area->curve.xleft);
  else if(area->sin)
    area->d.height = AMPLITUDE; 
  else {
    /*define the height of the screen, depending on function used*/
    int x, max=0, h;
    for(x = area->curve.xleft; x<= area->curve.xright; x++){
      h = FUNC(area,x);
      if(h >= max) max = h;
    }
    area->d.height = max;
  }
}

/***************************************************************************
 * Define the width of the X windows and my_xleft for circle and sin functions*
 ***************************************************************************/
void setup_window_width(struct area_t * area){
  struct display_t * d = &(area->d);
  struct rec_t * rect = &(area->rect);
  struct curve_t * curve = &(area->curve);
  /*Calculate the width of the X window of each process*/
  d->width = rect->width*rect->private_num_rect;
  
  int private_num_rect = rect->num_rect / area->numProcs;
  
  if(area->circle){
    if(curve->xright != UNIT) curve->circle_width = curve->width*RADIUS;
    rect->circle_width = curve->circle_width / rect->num_rect;
    curve->circle_my_xleft = curve->my_xleft * RADIUS;
    d->width = rect->circle_width * rect->private_num_rect;
  }
  
  if(area->sin){
    if(curve->xright != pi*FREQ) curve->sin_width = curve->width/FREQUENCY;
    curve->sin_my_xleft = curve->my_xleft / FREQUENCY;
    rect->sin_width = curve->sin_width / rect->num_rect;
    d->width = rect->sin_width * rect->private_num_rect;
  }
  
  /*Add extra width to the last process*/
  if(area->rank == area->numProcs-1) {
    d->width = rect->width*rect->private_num_rect+EXTRA_PIXELS;
    if(area->circle) d->width = rect->circle_width*rect->private_num_rect+EXTRA_PIXELS;
    if(area->sin) d->width = rect->sin_width*rect->private_num_rect+EXTRA_PIXELS;
  }
}

/***************************************************************************
* setupWindow()
*	opens the X windows, maps them and defines the following: the buffer, 
*	the colormap, the graphics context
***************************************************************************/
void setupWindow(struct area_t * area) {
  struct display_t * d = &(area->d);
  #ifndef NO_X11	
  int i;
  XTextProperty xtp;
  Status        xst, xit;
  
  #ifdef OMP
  xit = XInitThreads();
  if(xit == 0) {
    fprintf(stderr,"no threads;\n");
    exit(EXIT_FAILURE);
  }
  #endif
  
  // Generate name object
  char *name="Area Under The Curve";
  xst = XStringListToTextProperty(&name,1,&xtp);
  if (xst == 0) {
    fprintf(stderr, "Error: Insufficient memory for string at %s:%d\n",__FILE__, __LINE__);
    exit(EXIT_FAILURE);
  }
  
  d->dpy = XOpenDisplay(NULL);
  if (d->dpy == NULL) {
    fprintf(stderr, "Error: Could not open display at %s:%d\n",__FILE__, __LINE__);
    exit(EXIT_FAILURE);
  }
  
  d->w = XCreateSimpleWindow(d->dpy, DefaultRootWindow (d->dpy), 0, 0, d->width, d->height, 0, BlackPixel (d->dpy, 0), BlackPixel(d->dpy, 0));
  XSetWMProperties(d->dpy,d->w,&xtp,NULL,NULL,0,NULL,NULL,NULL);
  
  d->buffer = XCreatePixmap(d->dpy,DefaultRootWindow(d->dpy),d->width,d->height,DefaultDepth(d->dpy, DefaultScreen(d->dpy)));
  d->theColormap = XCreateColormap(d->dpy, DefaultRootWindow(d->dpy), DefaultVisual(d->dpy,DefaultScreen(d->dpy)), AllocNone);
  
  // We want to get MapNotify events
  XSelectInput(d->dpy, d->w, StructureNotifyMask | KeyPressMask | ExposureMask);
  
  // "Map" the window (that is, make it appear on the screen) 
  XMapWindow(d->dpy, d->w);
  
  // Create a "Graphics Context"
  d->gc = XCreateGC(d->dpy, d->w, 0, NULL);
   
  /*color initial screen black and fill it*/
  XSetForeground(d->dpy, d->gc, BlackPixel(d->dpy, 0));
  XFillRectangle(d->dpy,d->buffer,d->gc,0,0,d->width,d->height);
  
  // Wait for the MapNotify event
  for(;;) {
    XEvent e;
    XNextEvent(d->dpy, &e);
    if(e.type == MapNotify) break;
    usleep(5000); //give some time to register next event
  } 
  #endif
}

/***************************************************************************
* get_color_for_ranks()
*	called in draw_ranks().
*	based on the rank of the running process and its threads, this function 
*	returns a color specific to this number by calling a corresponding 
*	function in colors.h that sets this color to the foreground
***************************************************************************/
void get_color_for_ranks(struct area_t * area, int id){
  struct display_t * d = &(area->d);
  #ifndef NO_X11
  int order = area->num_threads * area->rank + id;
  switch(order){
    case RED:
      cred(d);
      break;
    case BLUE:
      cblue(d);
      break;
    case YELLOW:
      cyellow(d);
      break;
    case BISQUE:
      cbisque(d);
      break;
    case VIOLET:
      cviolet(d);
      break;
    case CYAN:
      ccyan(d);
      break;
    case OLIVE:
      colive(d);
      break;
    case GRAY:
      cgray(d);
      break;
    case PINK:
      cpink(d);
      break;
    case LAVENDER:
      clavender(d);
      break;
    case ORANGE:
      corange(d);
      break;
    case BROWN:
      cbrown(d);
      break;
    default: 
      get_extra_colors(order,d);
      break;
  }
  #endif
}

/***************************************************************************
* draw_ranks()
*	draws a colored string in the upper right corner, indicating
*	either the rank of processes, or the id of the threads 
***************************************************************************/
void draw_ranks(struct area_t * area) {
  struct display_t * d = &(area->d);
  #ifndef NO_X11
  /******MPI only or serial version******/ 
  #ifndef OMP
  char string[2]; 
  int rank_x      = 12; // X coordinate for rank display#endif
  int rank_y      = 23; // Y coordinate for rank display
  
  sprintf(string,"%d", area->rank);
  
  // Draw the rank display   
  get_color(area); //get color specific to rank
  XDrawString(d->dpy,d->w,d->gc,rank_x,rank_y,string,sizeof(string));
  usleep(100000);
  #endif
  
  /******OpenMP only or MPI+OpenMP version******/
  #ifdef OMP
  char thread_string[2];
  int thread_x,thread_y;
  thread_y      = 23; // Y coordinate for thread display  
  /*Draw the ranks in serial fashion, because of X window lag in LittleFe*/
  int id;
  for(id =0; id<area->num_threads; id++){
    thread_x = 7+(d->width/4)*id; // X coordinate for thread display
    sprintf(thread_string,"%d", id);
    // Draw the thread display 
    get_color_for_ranks(area,id); //get color specific to rank+thread
    XDrawString(d->dpy,d->w,d->gc,thread_x,thread_y,thread_string,sizeof(thread_string));
    usleep(10000);
  }
  #endif
  
  XFlush(d->dpy);
  #endif
}

/***************************************************************************
* draw_rectangle()
*	draws the rectangles under the curve
***************************************************************************/
void draw_rectangle(struct area_t * area, double current_left, double current_height) {
  struct display_t * d = &(area->d);
  #ifndef NO_X11
  struct curve_t * curve = &(area->curve);
  int rank = area->rank;
  double my_xleft = curve->my_xleft;
  double rect_width = area->rect.width;
  if(area->circle) {
    rect_width = area->rect.circle_width;
    my_xleft = curve->circle_my_xleft;
  }
  else if(area->sin) {
    rect_width = area->rect.sin_width;
    my_xleft = curve->sin_my_xleft;
  }
  /*x has to start at 0 and increment by rect_width*/
  int x 	= current_left-my_xleft;
  int HEIGHT 	= d->height;
  int y 	= HEIGHT - current_height;
  int width 	= rect_width;
  int height 	= current_height;
  get_color(area);
  XFillRectangle(d->dpy,d->w,d->gc,x,y,width,current_height);
  usleep(area->time*1000000);
  XFlush(d->dpy);
  #endif
}

/***************************************************************************
* draw_curve()
*	draws the curve defined by the function FUNC() in structs.h. 
*	If there are multiple processes defined, each draws a portion
*	of the curve.
*	NOTE: usleep() is used between each point, because of slow 
*	X forwarding in LittleFe
***************************************************************************/
void draw_curve(struct area_t * area) {
  struct display_t * d = &(area->d);
  #ifndef NO_X11
  struct curve_t * curve = &(area->curve);
  int i, rank = area->rank, time = sleep_time(area);
  /*all x1's need to be 0*/
  double x1=0, x2, y, height = d->height, rect_width = area->rect.width;
  
  /*get the correct rectangle width depending on function*/
  if(area->circle) rect_width = area->rect.circle_width;
  if(area->sin) rect_width = area->rect.sin_width;
  
  /*right boundary of each process*/
  x2 = area->rect.private_num_rect*rect_width;
  if(rank==area->numProcs -1) x2 ++; //last process gets last row of pixels
  
  /*choose the color of the curve*/
  XSetForeground(d->dpy,d->gc,d->green_col.pixel);
  
  /*draw each point on the curve from x1 to x2*/
  for(i=(int)x1;i<(int)x2;i++){
    y = FUNC(area, i + curve->my_xleft);
    if(area->circle) y = scale_circle(i+curve->circle_my_xleft);
    if(area->sin) y = scale_sin(i + curve->sin_my_xleft);
    XDrawPoint(d->dpy,d->w,d->gc, i,(int)(height-y));
    usleep(time);
    XFlush(d->dpy);
  }
  
  draw_ranks(area); 
  #endif
}

/***************************************************************************
* NOTE: call this function if you want non-separated X windows
* draw_process_barrier()
*	draws a line where each process ends, one pixels away from 
*	the right edge, for visibility
***************************************************************************/
void draw_process_barrier(struct area_t * area) {
  struct display_t * d = &(area->d);
  #ifndef NO_X11
  struct curve_t * curve = &(area->curve);
  int x=d->width-2, y1 = 0, y2 = d->height, rank = area->rank;
  XSetForeground(d->dpy,d->gc,WhitePixel(d->dpy, 0));
  XDrawLine(d->dpy,d->w,d->gc,x,y1,x,y2);
  usleep(10000);
  XFlush(d->dpy);
  #endif
}


/***************************************************************************
* sleep_time()
*	adjusts the amount needed to sleep based on the number of 
*	rectangles. 
***************************************************************************/
int sleep_time(struct area_t * area){
 int num_rectangles = area->rect.private_num_rect;
 int min = 6000; //0.006 seconds
 int max = 10000; //0.01 second, maximum time willing to wait
 int constant = 90000; // 0.09 seconds
 int time = constant/num_rectangles;
 if(time <= min) time = min;
 if(time >= max) time = max;
 if(area->numProcs == 1) time = 5000; //serial or np 1
 return time;
}


/***************************************************************************
* displayForever()
*	leaves the display open unless manually terminated
***************************************************************************/
void displayForever(struct area_t * area){
  struct display_t * d = &(area->d);
  #ifndef NO_X11
  if(area->rank==0) printf("\nPress 'q' or Ctr+C to exit the demo.\n");
  while (1)  {
    XNextEvent(d->dpy, &d->e);
    if(d->e.type == KeyPress) {
      usleep(50000); //give enough time to register the key stroke
      if (XLookupKeysym(&d->e.xkey, 0) == XK_q) {
	free_video(area);
	exit(0);
      }
    }
    XFlush(d->dpy);
  }
  #endif
}


#endif
