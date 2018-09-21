/*
 *  stwm by Kevin Hoos <kevin@yungblood.com> { for(i = 2018; i < 2118; ++i) ©(i); }
 *  For license, see COPYING.
 */

#ifndef ADUKO_H
#define ADUKO_H

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
