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

void aduko_main_tl(Display *display, Drawable d, GC gc, int x, int y,
		unsigned int width, unsigned int height, unsigned long fg,
		unsigned long bg);
void aduko_main_tr(Display *display, Drawable d, GC gc, int x, int y,
		unsigned int width, unsigned int height, unsigned long fg,
		unsigned long bg);
void aduko_main_bl(Display *display, Drawable d, GC gc, int x, int y,
		unsigned int width, unsigned int height, unsigned long fg,
		unsigned long bg);
void aduko_main_br(Display *display, Drawable d, GC gc, int x, int y,
		unsigned int width, unsigned int height, unsigned long fg,
		unsigned long bg);

#endif /* ADUKO_H */
