/*MPEarea.h
*	Hold all drawing functions using the MPE graphics library.
* AUTHOR: Ivana Marincic, Macalester College '15
* DATE: July, 2013
*/
#ifndef MPEAREA_H
#define MPEAREA_H

#include <mpi.h>
#include <mpe.h>
#include <mpe_graphics.h>

#include <stdio.h> //for printf
#include <unistd.h> //for usleep
#include <stdlib.h> //for getenv()
#include <string.h> // for strncmp()

#include "structs.h"
#include "MPEcolors.h"

/***************************************************************************
 * Retrieves the display *
 ***************************************************************************/
char* getDisplay(struct area_t * area) {
  struct display_t * d = &(area->d);
  #ifdef STRIPING
  d->display = getenv("DISPLAY");
  if(strncmp(d->display, "(null)", 7) == 0) {
    fprintf(stderr, "\n*** Fatal: DISPLAY variable not set.\n");
    exit(1);
  }
  return d->display;
  #else
  return NULL;
  #endif
  
  
}

/***************************************************************************
 * Opens the display, allocates colors, and sets black background *
 ***************************************************************************/
void setupWindow_mpe(struct area_t * area) {
  struct display_t * d = &(area->d);
  #ifdef STRIPING
  /*Determine window width*/
  d->w_width = area->curve.width;
  if(area->circle) d->w_width = area->curve.circle_width;
  if(area->sin) d->w_width = area->curve.sin_width;
  
  /*Allocate color array and open the display*/
  d->colors = NULL;
  MPE_Open_graphics(&d->canvas, MPI_COMM_WORLD, getDisplay(area), 5,5,d->w_width,d->height,0);
  d->colors = malloc(NUM_COLORS * sizeof(MPE_Color));
  
  /*Color the background of the window black*/
  if(area->rank == 0) MPE_Fill_rectangle(d->canvas,0,0,d->w_width,d->height,MPE_BLACK);
  MPE_Update(d->canvas);
  #endif
}

/***************************************************************************
 * Draws a rectangle*
 ***************************************************************************/
void draw_rectangle_mpe(struct area_t * area, double current_left, double current_height, double rect_width) {
  struct display_t * d = &(area->d);
  #ifdef STRIPING
  get_color_mpe(area);
  MPE_Fill_rectangle(d->canvas,current_left,d->height-current_height,rect_width,current_height,d->myColor);
  MPE_Update(d->canvas);
  usleep(area->time*1000000);
  
  #endif
}

/***************************************************************************
 * Draws the curve *
 ***************************************************************************/
void draw_curve_mpe(struct area_t * area){
  struct display_t * d = &(area->d);
  struct curve_t * curve = &(area->curve);
  #ifdef STRIPING
  int x1;
  int x2 = d->w_width;
  double y;
  if(area->rank == 0){
    for(x1=0;x1<=x2;x1++){
      y = FUNC(area, x1+curve->xleft);
      if(area->circle) y = scale_circle(x1+curve->circle_my_xleft);
      if(area->sin) y = scale_sin(x1 + curve->sin_my_xleft);
      MPE_Draw_point(d->canvas,x1,d->height-y,MPE_GREEN); 
      MPE_Update(d->canvas);
      usleep(5000);
    }
  }
  #endif
}

/***************************************************************************
 * Draws vertical white lines that visually separate the processes*
 ***************************************************************************/
void draw_process_barrier_mpe(struct area_t * area){
  struct display_t * d = &(area->d);
  #ifdef STRIPING
  if(area->circle) MPE_Draw_line(d->canvas,area->curve.circle_my_xleft,0,area->curve.circle_my_xleft,d->height,MPE_GRAY);
  else if(area->sin) MPE_Draw_line(d->canvas,area->curve.sin_my_xleft,0,area->curve.sin_my_xleft,d->height,MPE_GRAY);
  else MPE_Draw_line(d->canvas,area->curve.my_xleft,0,area->curve.my_xleft,d->height,MPE_GRAY);
  MPE_Update(d->canvas);
  #endif
}

/***************************************************************************
 * Draws a string specifying what process or thread is what color*
 ***************************************************************************/
void draw_ranks_mpe(struct area_t * area){
  struct display_t * d = &(area->d);
  #ifdef STRIPING
  char string[2];
  int x, y = 10+20*area->rank;
  #ifndef OMP
  x = area->numProcs;
  if(area->circle) x = d->w_width - x;
  sprintf(string,"%d",area->rank);
  MPE_Draw_string(d->canvas,x,y,d->myColor,string);
  MPE_Update(d->canvas);
  #else
  int id,color_index;
  for(id=0;id<area->num_threads;id++){
    x = 10 + 25*id;
    if(area->circle) x = d->w_width - x;
    color_index = 2*area->rank+id;
    d->myColor = d->colors[color_index];
    sprintf(string,"%d",id);
    MPE_Draw_string(d->canvas,x,y,d->myColor,string);
    MPE_Update(d->canvas);
  }
  #endif
  #endif
}

/***************************************************************************
 * Closes the display upon a mouse click*
 ***************************************************************************/
void free_graphics(struct area_t * area) {
  struct display_t * d = &(area->d);
  #ifdef STRIPING
  if(area->rank==0) printf("\n\nClick on the window or hit Ctrl+C to exit the demo.\n\n");
  int x = 0, y = 0, button = -1;
  if(area->rank == 0) MPE_Get_mouse_press(d->canvas,&x,&y,&button);
  MPE_Close_graphics(&d->canvas);
  #endif
}


#endif