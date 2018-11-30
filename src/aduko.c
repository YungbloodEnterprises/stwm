/*
 *  stwm by Kevin Hoos <kevin@yungblood.com> { for(i = 2018; i < 2118; ++i) ©(i); }
 *  For license, see COPYING.
 */

#include <X11/Xutil.h>

#include "aduko.h"

void aduko_main_tl(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned long fg,
		unsigned long bg) {
	XSetForeground(dpy, gc, fg);
	XFillRectangle(dpy, d, gc, x, y, w * 1.35, h * 2);
	XSetForeground(dpy, gc, bg);
	XFillRectangle(dpy, d, gc, x, y, w * 0.7, h * 2);
	XFillArc(dpy, d, gc, x + w, y + h, w * .7, h * 2, 90 * 64, 90 * 64);
	XSetForeground(dpy, gc, fg);
	XFillArc(dpy, d, gc, x, y, w * 1.4, h * 4, 90 * 64, 90 * 64);
}

void aduko_main_tr(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned long fg,
		unsigned long bg) {
	XSetForeground(dpy, gc, fg);
	XFillRectangle(dpy, d, gc, x, y, w * 1.35, h * 2);
	XSetForeground(dpy, gc, bg);
	XFillRectangle(dpy, d, gc, x + (w * .65), y, w * 0.7, h * 2);
	XFillArc(dpy, d, gc, x - (w * .35), y + h, w * .7, h * 2, 0 * 64, 90 * 64);
	XSetForeground(dpy, gc, fg);
	XFillArc(dpy, d, gc, x - (w * .05), y, w * 1.4, h * 4, 0 * 64, 90 * 64);
}

void aduko_main_bl(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned long fg,
		unsigned long bg) {
	XSetForeground(dpy, gc, fg);
	XFillRectangle(dpy, d, gc, x, y, w * 1.35, h * 2);
	XSetForeground(dpy, gc, bg);
	XFillRectangle(dpy, d, gc, x, y, w * 0.7, h * 2);
	XFillArc(dpy, d, gc, x + w, y - h, w * .7, h * 2, 180 * 64, 90 * 64);
	XSetForeground(dpy, gc, fg);
	XFillArc(dpy, d, gc, x, y - (h * 2), w * 1.4, h * 4, 180 * 64, 90 * 64);
}

void aduko_main_br(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned long fg,
		unsigned long bg) {
	XSetForeground(dpy, gc, fg);
	XFillRectangle(dpy, d, gc, x, y, w * 1.35, h * 2);
	XSetForeground(dpy, gc, bg);
	XFillRectangle(dpy, d, gc, x + (w * .65), y, w * 0.7, h * 2);
	XFillArc(dpy, d, gc, x - (w * .35), y - h, w * .7, h * 2, 270 * 64, 90 * 64);
	XSetForeground(dpy, gc, fg);
	XFillArc(dpy, d, gc, x - (w * .05), y - (h * 2), w * 1.4, h * 4, 270 * 64, 90 * 64);
}

void aduko_main_top(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned long fg,
		unsigned long bg) {
	int rad = min(w, h)/2;

	XSetForeground(dpy, gc, fg);
	XFillRectangle(dpy, d, gc, x, y, w, rad);
	XSetForeground(dpy, gc, bg);
	XFillRectangle(dpy, d, gc, x, y, rad, rad);
	XFillRectangle(dpy, d, gc, x+w-rad, y, rad, rad);
	XSetForeground(dpy, gc, fg);
	XFillArc(dpy, d, gc, x, y, rad*2, rad * 2, 90 * 64, 90 * 64);
	XFillArc(dpy, d, gc, x+w-(rad*2), y, rad*2, rad * 2, 0 * 64, 90 * 64);
}

void aduko_main_bottom(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned long fg,
		unsigned long bg) {
	int rad = min(w, h)/2;

	XSetForeground(dpy, gc, fg);
	XFillRectangle(dpy, d, gc, x, y, w, rad);
	XSetForeground(dpy, gc, bg);
	XFillRectangle(dpy, d, gc, x, y, rad, rad);
	XFillRectangle(dpy, d, gc, x+w-rad, y, rad, rad);
	XSetForeground(dpy, gc, fg);
	XFillArc(dpy, d, gc, x, y-rad, rad*2, rad * 2, 180 * 64, 90 * 64);
	XFillArc(dpy, d, gc, x+w-(rad*2), y-rad, rad*2, rad * 2, 270 * 64, 90 * 64);
}

void aduko_main_left(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned long fg,
		unsigned long bg) {
	int rad = min(w, h)/2;

	XSetForeground(dpy, gc, fg);
	XFillRectangle(dpy, d, gc, x, y, rad, h);
	XSetForeground(dpy, gc, bg);
	XFillRectangle(dpy, d, gc, x, y, rad, rad);
	XFillRectangle(dpy, d, gc, x, y+h-rad, rad, rad);
	XSetForeground(dpy, gc, fg);
	XFillArc(dpy, d, gc, x, y, rad*2, rad * 2, 90 * 64, 90 * 64);
	XFillArc(dpy, d, gc, x, y+h-(rad*2), rad*2, rad * 2, 180 * 64, 90 * 64);
}

void aduko_main_right(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned long fg,
		unsigned long bg) {
	int rad = min(w, h)/2;

	XSetForeground(dpy, gc, fg);
	XFillRectangle(dpy, d, gc, x, y, rad, h);
	XSetForeground(dpy, gc, bg);
	XFillRectangle(dpy, d, gc, x, y, rad, rad);
	XFillRectangle(dpy, d, gc, x, y+h-rad, rad, rad);
	XSetForeground(dpy, gc, fg);
	XFillArc(dpy, d, gc, x-rad, y, rad*2, rad * 2, 0 * 64, 90 * 64);
	XFillArc(dpy, d, gc, x-rad, y+h-(rad*2), rad*2, rad * 2, 270 * 64, 90 * 64);
}

void aduko_main_item_left(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned int gap, unsigned long fg,
		unsigned long bg) {

	XSetForeground(dpy, gc, bg);
	XFillRectangle(dpy, d, gc, x, y, w+(h*2), (gap*2)+1+(h*2));
	XSetForeground(dpy, gc, fg);
	XFillRectangle(dpy, d, gc, x, y+gap, w, (h*2)+1);
}

void aduko_main_item_right(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned int gap, unsigned long fg,
		unsigned long bg) {

	XSetForeground(dpy, gc, bg);
	XFillRectangle(dpy, d, gc, x, y, w+(h*2), (gap*2)+1+(h*2));
	XSetForeground(dpy, gc, fg);
	XFillRectangle(dpy, d, gc, x+(h*2), y+gap, w, (h*2)+1);
}

void aduko_main_item_select_left(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned int gap, unsigned long fg,
		unsigned long bg) {

	XSetForeground(dpy, gc, bg);
	XFillRectangle(dpy, d, gc, x, y, w+(h*2), (gap*2)+1+(h*2));
	XSetForeground(dpy, gc, fg);
	XFillRectangle(dpy, d, gc, x, y+gap, w+(h*2), (h*2)+1);
}

void aduko_main_item_select_right(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned int gap, unsigned long fg,
		unsigned long bg) {

	XSetForeground(dpy, gc, bg);
	XFillRectangle(dpy, d, gc, x, y, w+(h*2), (gap*2)+1+(h*2));
	XSetForeground(dpy, gc, fg);
	XFillRectangle(dpy, d, gc, x, y+gap, w+(h*2), (h*2)+1);
}

void aduko_main_item_highlight_left(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned int gap, unsigned long fg,
		unsigned long bg) {

	XSetForeground(dpy, gc, bg);
	XFillRectangle(dpy, d, gc, x, y, w+(h*2), (gap*2)+1+(h*2));
	XSetForeground(dpy, gc, fg);
	XFillRectangle(dpy, d, gc, x, y+gap, w, (h*2)+1);
	XFillRectangle(dpy, d, gc, x+w+gap, y+gap, h-gap, (h*2)+1);
	XFillArc(dpy, d, gc, x+w, y+gap, h*2, h*2, 270*64, 180*64);
}

void aduko_main_item_highlight_right(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned int gap, unsigned long fg,
		unsigned long bg) {

	XSetForeground(dpy, gc, bg);
	XFillRectangle(dpy, d, gc, x, y, w+(h*2), (gap*2)+1+(h*2));
	XSetForeground(dpy, gc, fg);
	XFillRectangle(dpy, d, gc, x+(h*2), y+gap, w, (h*2)+1);
	XFillRectangle(dpy, d, gc, x+h, y+gap, h-gap, (h*2)+1);
	XFillArc(dpy, d, gc, x, y+gap, h*2, h*2, 90*64, 180*64);
}

unsigned int aduko_item2y(unsigned int item, unsigned int h, unsigned int gap) {
	unsigned int y = h*3;
	if(item > 0) y += item*(h*2+1+gap);
	return y;
}
