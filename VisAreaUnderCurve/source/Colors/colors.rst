******************
Adding More Colors
******************

Xlib Version
############

In case you need more than the available 25 colors for the blocking version that relies on Xlib, follow these steps to add more colors:

1. Go to this site_ and pick the colors from the color name chart.
	.. _site: http://en.wikipedia.org/wiki/X11_color_names
2. Locate the structs.h file.
3. Under display_t and where XColor is declared, add colorname_col to the end of the list, where colorname is the name of the color you choose.
4. Locate the colors.h file.
5. Add #define COLORNAME number to the top of the file where all other colors are defined.
6. Take the hex value of the color from the chart and use it to declare the color as char colorname[] = hex value where all other colors are declared in such way.
6. In the function init_colors() add these two extra lines:
	XParseColor(d->dpy, d->theColormap, colorname, &(d->colorname_col));
	XAllocColor(d->dpy, d->theColormap, &(d->colorname_col));
7. Add an adictional function to the colors.h file such as:
	void ccolorname(struct display_t * d) {
		XSetForeground(d->dpy, d->gc, d->colorname_col.pixel);
	}
8. In the function get_extra_colors() add another case to the switch statement, such as:
	case COLORNAME:
		ccolorname(d);
		break;
9. Repeat this process to add more colors.

MPE Version
###########

In case you need more than the available 25 colors for the striping version that relies on MPE, follow these steps to add more colors:

1. Do step 1 from above.
2. If not already done, do steps 4 and 5 from above.
2. Locate the MPEcolors.h file.
3. Change NUM_COLORS to 25+however many extra colors you are adding
4. In function get_color_mpe() add another case to the switch stament:
	case COLORNAME:
		p_red = *percentage red*
		p_green = *percentage green*
		p_blue = *percentage blue*
		break;
5. Find the percentages of red, green and blue on the chart from step 1. (Take the values from the 4th column and divide by 100)
6. Repeat this process to add more colors.