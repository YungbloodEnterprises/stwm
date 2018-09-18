/*
 *  stwm by Kevin Hoos <kevin@yungblood.com> { for(i = 2018; i < 2118; ++i) �(i); }
 *  wmfs2 by Martin Duquesnoy <xorg62@gmail.com> { for(i = 2011; i < 2111; ++i) �(i); }
 *  For license, see COPYING.
 */

#include "stwm.h"
#include "barwin.h"
#include "util.h"
#include "draw.h"

/** Create a barwin
 * \param parent Parent window of the BarWindow
 * \param x X position
 * \param y Y position
 * \param w barwin Width
 * \param h barwin Height
 * \param color barwin color
 * \param entermask bool for know if the EnterMask mask is needed
 * \return The BarWindow pointer
*/
struct barwin*
barwin_new(Window parent, int x, int y, int w, int h, FgColor fg, BgColor bg, bool entermask)
{
     struct barwin *b = (struct barwin*)xcalloc(1, sizeof(struct barwin));
     XSetWindowAttributes at =
     {
          .override_redirect = true,
          .background_pixmap = ParentRelative,
          .event_mask = BARWIN_MASK
     };

     if(entermask)
          at.event_mask |= BARWIN_ENTERMASK;

     /* Create window */
     b->win = XCreateWindow(W->dpy, parent,
                            x, y, w, h,
                            0, W->xdepth,
                            CopyFromParent,
                            DefaultVisual(W->dpy, W->xscreen),
                            BARWIN_WINCW,
                            &at);

     b->dr = XCreatePixmap(W->dpy, parent, w, h, W->xdepth);
#ifdef HAVE_XFT
     b->xftdraw = XftDrawCreate(W->dpy, b->dr, DefaultVisual(W->dpy, W->xscreen), DefaultColormap(W->dpy, W->xscreen));
#endif /* HAVE_XFT */

     /* Property */
     b->geo.x = x;
     b->geo.y = y;
     b->geo.w = w;
     b->geo.h = h;
     b->bg = bg;
     b->fg = fg;

     SLIST_INIT(&b->mousebinds);
     SLIST_INIT(&b->statusmousebinds);

     /* Attach */
     SLIST_INSERT_HEAD(&W->h.barwin, b, next);

     return b;
}

/** Draw text in a barwin
 * \param b barwin pointer
 * \param t theme pointer
 * \param x x position
 * \param y y position
 * \param fg Foreground color
 * \param str text string
*/
void
barwin_draw_text(struct barwin *b, struct theme *t, int x, int y, FgColor fg, const char *str)
{
#ifdef HAVE_XFT
     draw_text(b->xftdraw, t, x, y, fg, str);
#else
     draw_text(b->dr, t, x, y, fg, str);
#endif /* HAVE_XFT */
}

/** Delete a barwin
 * \param b barwin pointer
*/
void
barwin_remove(struct barwin *b)
{
     SLIST_REMOVE(&W->h.barwin, b, barwin, next);

     XSelectInput(W->dpy, b->win, NoEventMask);
     XDestroyWindow(W->dpy, b->win);
#ifdef HAVE_XFT
     XftDrawDestroy(b->xftdraw);
#endif /* HAVE_XFT */
     XFreePixmap(W->dpy, b->dr);

     free(b);
}

/** Resize a barwin
 * \param b barwin pointer
 * \param w Width
 * \param h Height
*/
void
barwin_resize(struct barwin *b, int w, int h)
{
     /* Frame */
#ifdef HAVE_XFT
     XftDrawDestroy(b->xftdraw);
#endif /* HAVE_XFT */
     XFreePixmap(W->dpy, b->dr);

     b->dr = XCreatePixmap(W->dpy, W->root, w, h, W->xdepth);
#ifdef HAVE_XFT
     b->xftdraw = XftDrawCreate(W->dpy, b->dr, DefaultVisual(W->dpy, W->xscreen), DefaultColormap(W->dpy, W->xscreen));
#endif /* HAVE_XFT */

     b->geo.w = w;
     b->geo.h = h;

     XResizeWindow(W->dpy, b->win, w, h);
}

/** Refresh the barwin Color
 * \param b barwin pointer
*/
void
barwin_refresh_color(struct barwin *b)
{
     XSetForeground(W->dpy, W->gc, b->bg);
     XFillRectangle(W->dpy, b->dr, W->gc, 0, 0, b->geo.w, b->geo.h);
}
