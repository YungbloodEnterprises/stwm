/*
 *  stwm by Kevin Hoos <kevin@yungblood.com> { for(i = 2018; i < 2118; ++i) ©(i); }
 *  For license, see COPYING.
 */

#ifndef ADUKO_H
#define ADUKO_H

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

void aduko_main_tl(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned long fg,
		unsigned long bg);
void aduko_main_tr(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned long fg,
		unsigned long bg);
void aduko_main_bl(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned long fg,
		unsigned long bg);
void aduko_main_br(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned long fg,
		unsigned long bg);


void aduko_main_top(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned long fg,
		unsigned long bg);
void aduko_main_bottom(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned long fg,
		unsigned long bg);
void aduko_main_left(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned long fg,
		unsigned long bg);
void aduko_main_right(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned long fg,
		unsigned long bg);void aduko_main_item_left(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned int gap, unsigned long fg,
		unsigned long bg);

void aduko_main_item_left(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned int gap, unsigned long fg,
		unsigned long bg);
void aduko_main_item_right(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned int gap, unsigned long fg,
		unsigned long bg);
void aduko_main_item_select_left(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned int gap, unsigned long fg,
		unsigned long bg);
void aduko_main_item_select_right(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned int gap, unsigned long fg,
		unsigned long bg);
void aduko_main_item_highlight_left(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned int gap, unsigned long fg,
		unsigned long bg);
void aduko_main_item_highlight_right(Display *dpy, Drawable d, GC gc, int x, int y,
		unsigned int w, unsigned int h, unsigned int gap, unsigned long fg,
		unsigned long bg);

unsigned int aduko_item2y(unsigned int item, unsigned int h, unsigned int gap);

#endif /* ADUKO_H */
