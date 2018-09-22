/*
 *  stwm by Kevin Hoos <kevin@yungblood.com> { for(i = 2018; i < 2118; ++i) ©(i); }
 *  For license, see COPYING.
 */

#include <X11/Xutil.h>

#include "aduko.h"

void aduko_main_tl(Display *display, Drawable d, GC gc, int x, int y,
		unsigned int width, unsigned int height, unsigned long fg,
		unsigned long bg) {
	XSetForeground(display, gc, fg);
	XFillRectangle(display, d, gc, x, y, width * 1.35, height * 2);
	XSetForeground(display, gc, bg);
	XFillRectangle(display, d, gc, x, y, width * 0.7, height * 2);
	XFillArc(display, d, gc, x + width, y + height, width * .7, height * 2,
			90 * 64, 90 * 64);
	XSetForeground(display, gc, fg);
	XFillArc(display, d, gc, x, y, width * 1.4, height * 4, 90 * 64, 90 * 64);
}

void aduko_main_tr(Display *display, Drawable d, GC gc, int x, int y,
		unsigned int width, unsigned int height, unsigned long fg,
		unsigned long bg) {
	XSetForeground(display, gc, fg);
	XFillRectangle(display, d, gc, x, y, width * 1.35, height * 2);
	XSetForeground(display, gc, bg);
	XFillRectangle(display, d, gc, x + (width * .65), y, width * 0.7,
			height * 2);
	XFillArc(display, d, gc, x - (width * .35), y + height, width * .7,
			height * 2, 0 * 64, 90 * 64);
	XSetForeground(display, gc, fg);
	XFillArc(display, d, gc, x - (width * .05), y, width * 1.4, height * 4,
			0 * 64, 90 * 64);
}

void aduko_main_bl(Display *display, Drawable d, GC gc, int x, int y,
		unsigned int width, unsigned int height, unsigned long fg,
		unsigned long bg) {
	XSetForeground(display, gc, fg);
	XFillRectangle(display, d, gc, x, y, width * 1.35, height * 2);
	XSetForeground(display, gc, bg);
	XFillRectangle(display, d, gc, x, y, width * 0.7, height * 2);
	XFillArc(display, d, gc, x + width, y - height, width * .7, height * 2,
			180 * 64, 90 * 64);
	XSetForeground(display, gc, fg);
	XFillArc(display, d, gc, x, y - (height * 2), width * 1.4, height * 4,
			180 * 64, 90 * 64);
}

void aduko_main_br(Display *display, Drawable d, GC gc, int x, int y,
		unsigned int width, unsigned int height, unsigned long fg,
		unsigned long bg) {
	XSetForeground(display, gc, fg);
	XFillRectangle(display, d, gc, x, y, width * 1.35, height * 2);
	XSetForeground(display, gc, bg);
	XFillRectangle(display, d, gc, x + (width * .65), y, width * 0.7,
			height * 2);
	XFillArc(display, d, gc, x - (width * .35), y - height, width * .7,
			height * 2, 270 * 64, 90 * 64);
	XSetForeground(display, gc, fg);
	XFillArc(display, d, gc, x - (width * .05), y - (height * 2), width * 1.4,
			height * 4, 270 * 64, 90 * 64);
}

void aduko_main_top(Display *display, Drawable d, GC gc, int x, int y,
		unsigned int width, unsigned int height, unsigned long fg,
		unsigned long bg) {
	int rad = min(width, height)/2;

	XSetForeground(display, gc, fg);
	XFillRectangle(display, d, gc, x, y, width, rad);
	XSetForeground(display, gc, bg);
	XFillRectangle(display, d, gc, x, y, rad, rad);
	XFillRectangle(display, d, gc, x+width-rad, y, rad, rad);
	XSetForeground(display, gc, fg);
	XFillArc(display, d, gc, x, y, rad*2, rad * 2, 90 * 64, 90 * 64);
	XFillArc(display, d, gc, x+width-(rad*2), y, rad*2, rad * 2, 0 * 64, 90 * 64);
}

void aduko_main_bottom(Display *display, Drawable d, GC gc, int x, int y,
		unsigned int width, unsigned int height, unsigned long fg,
		unsigned long bg) {
	int rad = min(width, height)/2;

	XSetForeground(display, gc, fg);
	XFillRectangle(display, d, gc, x, y, width, rad);
	XSetForeground(display, gc, bg);
	XFillRectangle(display, d, gc, x, y, rad, rad);
	XFillRectangle(display, d, gc, x+width-rad, y, rad, rad);
	XSetForeground(display, gc, fg);
	XFillArc(display, d, gc, x, y-rad, rad*2, rad * 2, 180 * 64, 90 * 64);
	XFillArc(display, d, gc, x+width-(rad*2), y-rad, rad*2, rad * 2, 270 * 64, 90 * 64);
}

void aduko_main_left(Display *display, Drawable d, GC gc, int x, int y,
		unsigned int width, unsigned int height, unsigned long fg,
		unsigned long bg) {
	int rad = min(width, height)/2;

	XSetForeground(display, gc, fg);
	XFillRectangle(display, d, gc, x, y, rad, height);
	XSetForeground(display, gc, bg);
	XFillRectangle(display, d, gc, x, y, rad, rad);
	XFillRectangle(display, d, gc, x, y+height-rad, rad, rad);
	XSetForeground(display, gc, fg);
	XFillArc(display, d, gc, x, y, rad*2, rad * 2, 90 * 64, 90 * 64);
	XFillArc(display, d, gc, x, y+height-(rad*2), rad*2, rad * 2, 180 * 64, 90 * 64);
}

void aduko_main_right(Display *display, Drawable d, GC gc, int x, int y,
		unsigned int width, unsigned int height, unsigned long fg,
		unsigned long bg) {
	int rad = min(width, height)/2;

	XSetForeground(display, gc, fg);
	XFillRectangle(display, d, gc, x, y, rad, height);
	XSetForeground(display, gc, bg);
	XFillRectangle(display, d, gc, x, y, rad, rad);
	XFillRectangle(display, d, gc, x, y+height-rad, rad, rad);
	XSetForeground(display, gc, fg);
	XFillArc(display, d, gc, x-rad, y, rad*2, rad * 2, 0 * 64, 90 * 64);
	XFillArc(display, d, gc, x-rad, y+height-(rad*2), rad*2, rad * 2, 270 * 64, 90 * 64);
}

void aduko_main_item_left(Display *display, Drawable d, GC gc, int x, int y,
		unsigned int width, unsigned int height, unsigned int gap, unsigned long fg,
		unsigned long bg) {

	XSetForeground(display, gc, bg);
	XFillRectangle(display, d, gc, x, y, width+(height*2), (gap*2)+1+(height*2));
	XSetForeground(display, gc, fg);
	XFillRectangle(display, d, gc, x, y+gap, width, (height*2)+1);
}

void aduko_main_item_right(Display *display, Drawable d, GC gc, int x, int y,
		unsigned int width, unsigned int height, unsigned int gap, unsigned long fg,
		unsigned long bg) {

	XSetForeground(display, gc, bg);
	XFillRectangle(display, d, gc, x, y, width+(height*2), (gap*2)+1+(height*2));
	XSetForeground(display, gc, fg);
	XFillRectangle(display, d, gc, x+(height*2), y+gap, width, (height*2)+1);
}

void aduko_main_item_select_left(Display *display, Drawable d, GC gc, int x, int y,
		unsigned int width, unsigned int height, unsigned int gap, unsigned long fg,
		unsigned long bg) {

	XSetForeground(display, gc, bg);
	XFillRectangle(display, d, gc, x, y, width+(height*2), (gap*2)+1+(height*2));
	XSetForeground(display, gc, fg);
	XFillRectangle(display, d, gc, x, y+gap, width+(height*2), (height*2)+1);
}

void aduko_main_item_select_right(Display *display, Drawable d, GC gc, int x, int y,
		unsigned int width, unsigned int height, unsigned int gap, unsigned long fg,
		unsigned long bg) {

	XSetForeground(display, gc, bg);
	XFillRectangle(display, d, gc, x, y, width+(height*2), (gap*2)+1+(height*2));
	XSetForeground(display, gc, fg);
	XFillRectangle(display, d, gc, x, y+gap, width+(height*2), (height*2)+1);
}

void aduko_main_item_highlight_left(Display *display, Drawable d, GC gc, int x, int y,
		unsigned int width, unsigned int height, unsigned int gap, unsigned long fg,
		unsigned long bg) {

	XSetForeground(display, gc, bg);
	XFillRectangle(display, d, gc, x, y, width+(height*2), (gap*2)+1+(height*2));
	XSetForeground(display, gc, fg);
	XFillRectangle(display, d, gc, x, y+gap, width, (height*2)+1);
	XFillRectangle(display, d, gc, x+width+gap, y+gap, height-gap, (height*2)+1);
	XFillArc(display, d, gc, x+width, y+gap, height*2, height*2, 270*64, 180*64);
}

void aduko_main_item_highlight_right(Display *display, Drawable d, GC gc, int x, int y,
		unsigned int width, unsigned int height, unsigned int gap, unsigned long fg,
		unsigned long bg) {

	XSetForeground(display, gc, bg);
	XFillRectangle(display, d, gc, x, y, width+(height*2), (gap*2)+1+(height*2));
	XSetForeground(display, gc, fg);
	XFillRectangle(display, d, gc, x+(height*2), y+gap, width, (height*2)+1);
	XFillRectangle(display, d, gc, x+height, y+gap, height-gap, (height*2)+1);
	XFillArc(display, d, gc, x, y+gap, height*2, height*2, 90*64, 180*64);
}

unsigned int aduko_item2y(unsigned int item, unsigned int height, unsigned int gap) {
	unsigned int y = height*3;
	if(item > 0) y += item*(height*2+1+gap);
	return y;
}
