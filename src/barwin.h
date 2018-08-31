/*
 *  stwm by Kevin Hoos <kevin@yungblood.com> { for(i = 2018; i < 2118; ++i) ©(i); }
 *  For license, see COPYING.
 */

#ifndef BARWIN_H
#define BARWIN_H

#include "stwm.h"

#define BARWIN_MASK                                                  \
     (SubstructureRedirectMask | SubstructureNotifyMask              \
      | ButtonMask | MouseMask | ExposureMask | VisibilityChangeMask \
      | StructureNotifyMask)

#define BARWIN_ENTERMASK (EnterWindowMask | LeaveWindowMask | FocusChangeMask)
#define BARWIN_WINCW     (CWOverrideRedirect | CWBackPixmap | CWEventMask)

#define barwin_delete_subwin(b) XDestroySubwindows(W->dpy, b->win)
#define barwin_map_subwin(b)    XMapSubwindows(W->dpy, b->win)
#define barwin_unmap_subwin(b)  XUnmapSubwindows(W->dpy, b->win)
#define barwin_refresh(b)       XCopyArea(W->dpy, b->dr, b->win, W->gc, 0, 0, b->geo.w, b->geo.h, 0, 0)
#define barwin_map(b)           XMapWindow(W->dpy, b->win);
#define barwin_unmap(b)         XUnmapWindow(W->dpy, b->win);
#define barwin_reparent(b, w)   XReparentWindow(W->dpy, b->win, w, 0, 0);

static inline void
barwin_move(struct barwin *b, int x, int y)
{
     XMoveWindow(W->dpy, b->win, (b->geo.x = x), (b->geo.y = y));
}

struct barwin* barwin_new(Window parent, int x, int y, int w, int h, FgColor fg, BgColor bg, bool entermask);
void barwin_draw_text(struct barwin *b, struct theme *t, int x, int y, FgColor fg, const char *str);
void barwin_remove(struct barwin *b);
void barwin_resize(struct barwin *b, int w, int h);
void barwin_refresh_color(struct barwin *b);

#endif /* BARWIN_H */
