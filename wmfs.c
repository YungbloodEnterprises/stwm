/* Copyright (c) 1998, Regents of the University of California
* All rights reserved.
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the University of California, Berkeley nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "local.h"

unsigned int numlockmask = 0;
int taglen[MAXTAG] = {3};

void
arrange(void) {
     Client *c;
     for(c = clients; c; c = c->next)
          if(!ishide(c))
               unhide(c);
          else
               hide(c);
     focus(selbytag[seltag]);

     updatelayout();
     updateall();
}

void
attach(Client *c) {
     if(clients)
          clients->prev = c;
     c->next = clients;
     clients = c;
     return;
}

void
buttonpress(XEvent *event) {
     Client *c;
     int i, j;
     char s[6];
     XButtonPressedEvent *ev = &event->xbutton;

     /* Tbar'n'Button */
     if(conf.ttbarheight) {
          if((c = gettbar(ev->window))) {
               raiseclient(c);
               if(ev->button == Button1)
                    mouseaction(c, ev->x_root, ev->y_root, Move);   /* type 0 for move */
               else if(ev->button == Button2)
                    tile_switch(NULL);
               else if(ev->button == Button3)
                    mouseaction(c, ev->x_root, ev->y_root, Resize); /* type 1 for resize */
          } else if((c = getbutton(ev->window))) {
               if(ev->button == Button1)
                    killclient(NULL);
               else if(ev->button == Button3)
                    togglemax(NULL);
          }
     }

     /* Window */
     if((c = getclient(ev->window))) {
          raiseclient(c);
          if(ev->button == Button1)
               mouseaction(c, ev->x_root, ev->y_root, Move);   /* type 0 for move */
          else if(ev->button == Button2)
               togglemax(NULL);
          else if(ev->button == Button3)
               mouseaction(c, ev->x_root, ev->y_root, Resize); /* type 1 for resize */
     }
     /* Bar */
     /* for tag click */
     else if(ev->window == bar) {
          for(i = 0; i < conf.ntag + 1; ++i) {
               if(ev->x > taglen[i-1]
                  && ev->x < taglen[i]) {
                    ITOA(s, i);
                    if(ev->button == Button1)
                         tag(s);
                    else if(ev->button == Button3)
                         tagtransfert(s);
               }
          }
          /* tag switch with scroll */
          if(ev->x < taglen[conf.ntag]) {
                    if(ev->button == Button4)
                         tag("+1");
                    else if(ev->button == Button5)
                         tag("-1");
          }
          /* layout switch */
          if(ev->x >= taglen[conf.ntag]
             && ev->x < taglen[conf.ntag] +
             (strlen((getlayoutsym(layout[seltag])))*fonty+3)) {
               if(ev->button == Button1
                  || ev->button == Button4)
                    layoutswitch("+");
               else if(ev->button == Button3
                       || ev->button == Button5)
                    layoutswitch("-");
          }
     }
     /* Root */
     /* tag switch */
     else if(ev->window == root) {
          if(ev->button == Button4)
               tag("+1");
          else if(ev->button == Button5)
               tag("-1");
     }
     /* Bar Button */
     for(i=0; i<conf.nbutton ; ++i)
          for(j=0; j<conf.barbutton[i].nmousesec; ++j)
               if(ev->window == conf.barbutton[i].win
                  && ev->button == conf.barbutton[i].mouse[j])
                    if(conf.barbutton[i].func[j])
                         conf.barbutton[i].func[j](conf.barbutton[i].cmd[j]);
}

int
clienthintpertag(int tag) {
     Client *c;
     int i = 0;
     for(c = clients; c; c = c->next)
          if(c->tag == tag && c->hint)
               ++i;
     return i;
}

int
clientpertag(int tag) {
     Client *c;
     int i = 0;
     for(c = clients; c; c = c->next)
          if(c->tag == tag)
               ++i;
     return i;
}

void
configurerequest(XEvent event) {
     Client *c;
     XWindowChanges wc;
     if(layout[seltag] != Free)
          return;
     wc.x = event.xconfigurerequest.x;
     wc.y = event.xconfigurerequest.y;
     wc.width = event.xconfigurerequest.width;
     wc.height = event.xconfigurerequest.height;
     wc.border_width = event.xconfigurerequest.border_width;
     wc.sibling = event.xconfigurerequest.above;
     wc.stack_mode = event.xconfigurerequest.detail;
     XConfigureWindow(dpy, event.xconfigurerequest.window,
                      event.xconfigurerequest.value_mask, &wc);
     if((c = getclient(event.xconfigurerequest.window))) {
          if(wc.y < mw && wc.x < mh) {
               c->free = True;
               c->max = False;

               if(conf.ttbarheight) {
                    XMoveWindow(dpy, c->tbar, wc.x, wc.y - conf.ttbarheight);
                    XResizeWindow(dpy, c->tbar, wc.width, conf.ttbarheight);
                    XMoveWindow(dpy, c->button, wc.x + wc.width - 10, BUTY(wc.y));
               }

               updatetitle(c);
               c->y = wc.y;
               c->x = wc.x;
               c->w = wc.width;
               c->h = wc.height;
          }
     }
}

void
detach(Client *c) {
     if(c->prev) c->prev->next = c->next;
     if(c->next) c->next->prev = c->prev;
     if(c == clients) clients = c->next;
     c->next = c->prev = NULL;
     return;
}

void *
emallocz(unsigned int size) {
     void *res = calloc(1, size);
     if(!res)
          fprintf(stderr,"fatal: could not malloc() %u bytes\n", size);
     return res;
}

int
errorhandler(Display *d, XErrorEvent *event) {
     char mess[512];
     XGetErrorText(d, event->error_code, mess, 128);
     fprintf(stderr, "WMFS error: %s(%d) opcodes %d/%d\n  resource 0x%lx\n", mess,
             event->error_code,
             event->request_code,
             event->minor_code,
             event->resourceid);
     return(1);
}

int
errorhandlerdummy(Display *d, XErrorEvent *event) {
     return 0;
}

void
focus(Client *c) {
     if(sel && sel != c) {
          grabbuttons(sel, False);
          setborder(sel->win, conf.colors.bordernormal);
          if(conf.ttbarheight)
               setborder(sel->tbar, conf.colors.bordernormal);
     }

     if(c)
          grabbuttons(c, True);

     sel = c;
     selbytag[seltag] = c;

     if(c) {
          setborder(c->win, conf.colors.borderfocus);
          if(conf.ttbarheight)
               setborder(sel->tbar, conf.colors.borderfocus);
          if(conf.raisefocus)
               raiseclient(c);
          XSetInputFocus(dpy, c->win, RevertToPointerRoot, CurrentTime);
          updatetitle(c);
     }
     return;
}

void
freelayout(void) {
     layout[seltag] = Free;
     Client *c;
     for(c = clients; c; c = c->next){
          if(!ishide(c)) {
               if(c->max || c->tile)
                    moveresize(c, c->ox, c->oy, c->ow, c->oh, 1);
               c->max = c->tile = False;
          }
     }
     return;
}

Client*
getbutton(Window w) {
     Client *c;
     for(c = clients; c && c->button != w; c = c->next);
     return c;
}

Client*
getclient(Window w) {
     Client *c;
     for(c = clients; c && c->win != w; c = c->next);
     return c;
}

Client*
getnext(Client *c) {
     for(; c; c = c->prev);
     return c;
}

char*
getlayoutsym(int l) {
     char *t;
     switch(layout[seltag]) {
     case Free: t = conf.layouts.free; break;
     case Tile: t = conf.layouts.tile; break;
     case Max:  t = conf.layouts.max;  break;
     }
     return t;
}

void
getstatuscmd(char *cmd, char *buf, size_t bufsize) {
     int i;
     if(!cmd || !buf || !bufsize)
          return;

     FILE *f = popen(cmd, "r");
     fgets(buf, bufsize, f);
     for(i = 0; i< bufsize; ++i)
          if(buf[i] == '\n')
               buf[i] = '\0';
     fclose(f);
     return;
}

Client*
gettbar(Window w) {
     Client *c;
     for(c = clients; c && c->tbar != w; c = c->next);
     return c;
}

void
getevent(void) {
     XWindowAttributes at;
     Client *c;

     struct timeval tv;

     if(QLength(dpy) > 0)
          XNextEvent(dpy, &event);
     else {
          XFlush(dpy);
          FD_ZERO(&fd);
          FD_SET(ConnectionNumber(dpy), &fd);
          event.type = LASTEvent;
          tv.tv_sec = 1;
          tv.tv_usec = 0;
          if(select(FD_SETSIZE, &fd, NULL, NULL, &tv) > 0)
               XNextEvent(dpy, &event);
     }

     switch (event.type) {
     case EnterNotify:
          if(event.xcrossing.mode != NotifyNormal
             || event.xcrossing.detail == NotifyInferior)
               return;
          if((c = getclient(event.xcrossing.window))
             || (c = gettbar(event.xcrossing.window)))
               if(c->win != bar)
                    focus(c);
          break;

     case MapRequest:
          if(!XGetWindowAttributes(dpy, event.xmaprequest.window, &at))  return;
          if(at.override_redirect) return;
          if(!getclient(event.xmaprequest.window))
               manage(event.xmaprequest.window, &at);
          break;

     case MappingNotify: if(event.xmapping.request == MappingKeyboard) grabkeys();  break;

     case PropertyNotify:
          if(event.xproperty.state == PropertyDelete)
               return;
          if(event.xproperty.atom == XA_WM_NAME &&
             event.xproperty.state == PropertyNewValue) {
               if((c = getclient(event.xproperty.window))) {
                    if(c->title) {
                         XFree(c->title);
                         c->title = NULL;
                         updatetitle(c);
                    }
               }
          }
          break;

     case ConfigureRequest: configurerequest(event); break;

     case UnmapNotify:
          if((c = getclient(event.xunmap.window)))
               if(!c->hide)
                    unmanage(c);
          break;

     case DestroyNotify:
          if((c = getclient(event.xdestroywindow.window)))
               unmanage(c);
          break;

     case FocusIn:
          if(sel && event.xfocus.window != sel->win)
               XSetInputFocus(dpy, sel->win, RevertToPointerRoot, CurrentTime);
          break;

     case KeyPress:    keypress(&event); break;
     case ButtonPress: buttonpress(&event); break;
     }
     return;
}

/* Not finish, testing */
void
getstdin(void) {
     if(isatty(STDIN_FILENO))
          return;
     readp = read(STDIN_FILENO, bufbt + offset, len - offset);
     for(ptrb = bufbt + offset; readp > 0; ptrb++, readp--, offset++)
          if(*ptrb == '\n' || *ptrb == '\0') {
               *ptrb = '\0';
               strncpy(bartext, bufbt, len);
               ptrb += readp - 1;
               for(readp = 0; *(ptrb - readp) && *(ptrb - readp) != '\n'; readp++);
              offset = readp;
               if(readp)
                    memmove(bufbt, ptrb - readp + 1, readp);
               break;
          }
     return;
}

void
grabbuttons(Client *c, Bool focused) {
     int i;
     XUngrabButton(dpy, AnyButton, AnyModifier, c->win);
     if(conf.ttbarheight) {
          XUngrabButton(dpy, AnyButton, AnyModifier, c->tbar);
          XUngrabButton(dpy, AnyButton, AnyModifier, c->button);
     }

     if(focused) {
          /* Window */
          XGrabButton(dpy, Button1, ALT, c->win, 0, ButtonMask,GrabModeAsync,GrabModeSync, None, None);
          XGrabButton(dpy, Button1, ALT|LockMask, c->win, 0, ButtonMask,GrabModeAsync, GrabModeSync, None, None);
          XGrabButton(dpy, Button2, ALT, c->win, 0, ButtonMask,GrabModeAsync,GrabModeSync, None, None);
          XGrabButton(dpy, Button2, ALT|LockMask, c->win, 0, ButtonMask,GrabModeAsync, GrabModeSync, None, None);
          XGrabButton(dpy, Button3, ALT, c->win, 0, ButtonMask,GrabModeAsync,GrabModeSync, None, None);
          XGrabButton(dpy, Button3, ALT|LockMask, c->win, False, ButtonMask,GrabModeAsync, GrabModeSync, None, None);
          if(conf.ttbarheight) {
               /* Titlebar */
               XGrabButton(dpy, Button1, AnyModifier, c->tbar, 0, ButtonMask,GrabModeAsync, GrabModeSync, None, None);
               XGrabButton(dpy, Button2, AnyModifier, c->tbar, 0, ButtonMask,GrabModeAsync, GrabModeSync, None, None);
               XGrabButton(dpy, Button3, AnyModifier, c->tbar, 0, ButtonMask,GrabModeAsync, GrabModeSync, None, None);
               /* Titlebar Button */
               XGrabButton(dpy, Button1, AnyModifier, c->button, 0, ButtonMask,GrabModeAsync, GrabModeSync, None, None);
               XGrabButton(dpy, Button3, AnyModifier, c->button, 0, ButtonMask,GrabModeAsync, GrabModeSync, None, None);
          }
          /* Bar Button */
          for(i=0; i< conf.nbutton; ++i)
               XGrabButton(dpy, Button1, AnyModifier, conf.barbutton[i].win, 0, ButtonMask,GrabModeAsync, GrabModeSync, None, None);
     } else {
          XGrabButton(dpy, AnyButton, AnyModifier, c->win, 0, ButtonMask, GrabModeAsync, GrabModeSync, None, None);
          if(conf.ttbarheight) {
               XGrabButton(dpy, AnyButton, AnyModifier, c->tbar, 0, ButtonMask, GrabModeAsync, GrabModeSync, None, None);
               XGrabButton(dpy, AnyButton, AnyModifier, c->button, 0, ButtonMask, GrabModeAsync, GrabModeSync, None, None);
          }
          for(i=0; i< conf.nbutton; ++i)
               XGrabButton(dpy, Button1, AnyModifier, conf.barbutton[i].win, 0, ButtonMask,GrabModeAsync, GrabModeSync, None, None);
     }
}

void
grabkeys(void) {
     unsigned int i;
     KeyCode code;
     XUngrabKey(dpy, AnyKey, AnyModifier, root);
     for(i = 0; i < conf.nkeybind; i++) {
          code = XKeysymToKeycode(dpy, keys[i].keysym);
          XGrabKey(dpy, code, keys[i].mod, root, True, GrabModeAsync, GrabModeAsync);
          XGrabKey(dpy, code, keys[i].mod|numlockmask, root, True, GrabModeAsync, GrabModeAsync);
          XGrabKey(dpy, code, keys[i].mod|LockMask, root, True, GrabModeAsync, GrabModeAsync);
          XGrabKey(dpy, code, keys[i].mod|LockMask|numlockmask, root, True, GrabModeAsync, GrabModeAsync);     }
     return;
}

void
hide(Client *c) {
     if(!c || c->hide) return;
     long data[] = { IconicState, None };

     /*  unmapclient(c); */
     /* Just hide for now... */

     XMoveWindow(dpy, c->win, c->x, c->y+mh*2);
     if(conf.ttbarheight) {
          XMoveWindow(dpy, c->tbar, c->x, c->y+mh*2);
          XMoveWindow(dpy, c->button, c->x, c->y+mh*2);
     }

     c->hide = True;
     XChangeProperty(dpy, c->win, XInternAtom(dpy, "WM_STATE", False),
                     XInternAtom(dpy, "WM_STATE", False),  32,
                     PropModeReplace, (unsigned char *) data, 2);
}

void
init(void) {
     XSetWindowAttributes at;
     XModifierKeymap *modmap;
     int i, j;

     /* FIRST INIT */
     gc = DefaultGC (dpy, screen);
     screen = DefaultScreen (dpy);
     root = RootWindow (dpy, screen);
     mw = DisplayWidth (dpy, screen);
     mh = DisplayHeight (dpy, screen);
     seltag = 1;
     for(i = 0; i< conf.ntag+1; ++i) {
          mwfact[i] = conf.tag[i-1].mwfact;
          layout[i] = conf.tag[i-1].layout;
          nmaster[i] = conf.tag[i-1].nmaster;
     }

     /* INIT FONT */
     font = XLoadQueryFont(dpy, conf.font);
     if(!font){
          fprintf(stderr, "XLoadQueryFont: failed loading font '%s'\n", conf.font);
          exit(0);
     }
     XSetFont(dpy, gc, font->fid);
     fonth = (font->ascent + font->descent) - 1;
     barheight = fonth + 3;
     fonty = (font->ascent + font->descent) / 2;

     font_b = XLoadQueryFont(dpy, conf.buttonfont);
     if(!font_b){
          fprintf(stderr, "XLoadQueryFont: failed loading button font '%s'\n", conf.buttonfont);
          exit(0);
     }

     /* INIT CURSOR */
     cursor[CurNormal] = XCreateFontCursor(dpy, XC_left_ptr);
     cursor[CurResize] = XCreateFontCursor(dpy, XC_sizing);
     cursor[CurMove] = XCreateFontCursor(dpy, XC_fleur);

     /* INIT MODIFIER */
     modmap = XGetModifierMapping(dpy);
     for(i = 0; i < 8; i++)
          for(j = 0; j < modmap->max_keypermod; ++j) {
               if(modmap->modifiermap[i * modmap->max_keypermod + j]
                  == XKeysymToKeycode(dpy, XK_Num_Lock))
                    numlockmask = (1 << i);
          }
     XFreeModifiermap(modmap);

     /* INIT ATOM */
     wm_atom[WMState] = XInternAtom(dpy, "WM_STATE", False);
     wm_atom[WMProtocols] = XInternAtom(dpy, "WM_PROTOCOLS", False);
     wm_atom[WMDelete] = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
     net_atom[NetSupported] = XInternAtom(dpy, "_NET_SUPPORTED", False);
     net_atom[NetWMName] = XInternAtom(dpy, "_NET_WM_NAME", False);
     XChangeProperty(dpy, root, net_atom[NetSupported], XA_ATOM, 32,
                     PropModeReplace, (unsigned char *) net_atom, NetLast);

     /* INIT ROOT */
     at.event_mask = KeyMask | ButtonPressMask | ButtonReleaseMask |
          SubstructureRedirectMask | SubstructureNotifyMask |
          EnterWindowMask | LeaveWindowMask | StructureNotifyMask ;
     at.cursor = cursor[CurNormal];
     XChangeWindowAttributes(dpy, root, CWEventMask | CWCursor, &at);

     /* INIT BAR */
     at.override_redirect = 1;
     at.background_pixmap = ParentRelative;
     at.event_mask = ButtonPressMask | ExposureMask;
     bar = XCreateWindow(dpy, root, 0, 0, mw, barheight, 0, DefaultDepth(dpy, screen),
                         CopyFromParent, DefaultVisual(dpy, screen),
                         CWOverrideRedirect | CWBackPixmap | CWEventMask, &at);
     XSetWindowBackground(dpy, bar, conf.colors.bar);
     XMapRaised(dpy, bar);
     strncpy(bartext, "WMFS-devel", strlen("WMFS-devel"));
     updatebar();
     updatebutton(0);

     /* INIT STUFF */
     XSetErrorHandler(errorhandler);
     grabkeys();
     offset = 0;
     len = sizeof bartext - 1;
     bufbt[len] = bartext[len] = '\0';

     return;
}

Bool
ishide(Client *c) {
     int i;
     for(i = 0; i < conf.ntag+1; ++i)
          if(c->tag == i && seltag == i)
               return False;
     return True;
}

void
keymovex(char *cmd) {
     if(sel
        && cmd
        && !ishide(sel)
        && !sel->max)
     {
          if(layout[seltag] == Tile && !sel->hint)
               return;
          int tmp;
          tmp = sel->x + atoi(cmd);
          moveresize(sel, tmp, sel->y, sel->w, sel->h, 1);
     }
     return;
}

void
keymovey(char *cmd) {
     if(sel
        && cmd
        && !ishide(sel)
        && !sel->max)
     {
          if(layout[seltag] == Tile && !sel->hint)
               return;
          int tmp;
          tmp = sel->y + atoi(cmd);
          moveresize(sel, sel->x, tmp, sel->w, sel->h, 1);
     }
     return;
}

void
keypress(XEvent *e) {
     unsigned int i;
     KeySym keysym;
     XKeyEvent *ev;
     ev = &e->xkey;
     keysym = XKeycodeToKeysym(dpy, (KeyCode)ev->keycode, 0);
     for(i = 0; i < conf.nkeybind; i++)
          if(keysym == keys[i].keysym
             && (keys[i].mod & ~(numlockmask | LockMask)) ==
             (ev->state & ~(numlockmask | LockMask))
             && keys[i].func)
          {
               keys[i].func(keys[i].cmd);
               updateall();
          }
     return;
}

void
keyresize(char *cmd) {
     if(sel && !ishide(sel)
        && !sel->max
        && layout[seltag] != Tile)
     {
          int temph = 0, tempw = 0, modh = 0, modw = 0, tmp = 0;

          switch(cmd[1]) {
          case 'h': tmp = (cmd[0] == '+') ? 5 : -5; modh = tmp; break;
          case 'w': tmp = (cmd[0] == '+') ? 5 : -5; modw = tmp; break;
          }

          temph = sel->h + modh;
          tempw = sel->w + modw;
          temph = (temph < 10) ? 10 : temph;
          tempw = (tempw < 10) ? 10 : tempw;
          moveresize(sel, sel->x, sel->y, tempw, temph, 1);
     }
     return;
}

void
killclient(char *cmd) {
     if(sel && !ishide(sel)) {
          XEvent ev;
          ev.type = ClientMessage;
          ev.xclient.window = sel->win;
          ev.xclient.message_type = wm_atom[WMProtocols];
          ev.xclient.format = 32;
          ev.xclient.data.l[0] = wm_atom[WMDelete];
          ev.xclient.data.l[1] = CurrentTime;
          XSendEvent(dpy, sel->win, False, NoEventMask, &ev);
     }
     arrange();
     return;
}

void
layoutswitch(char *cmd) {
     if(cmd[0] == '+') {
          switch(layout[seltag]) {
          case Free: tile();       break;
          case Tile: maxlayout();  break;
          case Max:  freelayout(); break;
          }
     }
     else if(cmd[0] == '-') {
          switch(layout[seltag]) {
          case Free: maxlayout();  break;
          case Tile: freelayout(); break;
          case Max:  tile(); break;
          }
     }
     return;
}

void
lowerclient(Client *c) {
     if(!c)
          return;
     if(conf.ttbarheight) {
          XLowerWindow(dpy,c->button);
          XLowerWindow(dpy,c->tbar);
     }
     XLowerWindow(dpy,c->win);
     return;
}

void
mapclient(Client *c) {
     if(!c)
          return;
     XMapWindow(dpy, c->win);
     XMapSubwindows(dpy, c->win);
     if(conf.ttbarheight) {
          XMapWindow(dpy, c->tbar);
          XMapWindow(dpy, c->button);
     }
     return;
}

void
manage(Window w, XWindowAttributes *wa) {
     Client *c, *t = NULL;
     Window trans;
     Status rettrans;
     XWindowChanges winc;

     c = emallocz(sizeof(Client));
     c->win = w;
     c->x = wa->x;
     c->y = wa->y + conf.ttbarheight + barheight;
     c->w = wa->width;
     c->h = wa->height;
     c->border = wa->border_width;
     c->tag = seltag;
     switch(layout[seltag]) {
     case Tile: c->tile = True; break;
     case Max:  c->max  = True; break;
     }

     if(conf.ttbarheight) {
          c->tbar =
               XCreateSimpleWindow(dpy, root,
                                   c->x,
                                   c->y - conf.ttbarheight,
                                   c->w,
                                   conf.ttbarheight,
                                   conf.borderheight,
                                   conf.colors.bordernormal,
                                   conf.colors.bar);
          XSelectInput(dpy, c->tbar, ExposureMask | EnterWindowMask);
          c->button =
               XCreateSimpleWindow(dpy, root,
                                   c->x + c->w - 10,
                                   BUTY(c->y),
                                   5,
                                   BUTH,
                                   1,
                                   conf.colors.bordernormal,
                                   conf.colors.borderfocus);
     }

     XConfigureWindow(dpy, w, CWBorderWidth, &winc);
     setborder(w, conf.colors.bordernormal);
     grabbuttons(c, False);
     XSelectInput(dpy, w, EnterWindowMask | FocusChangeMask |
                  PropertyChangeMask | StructureNotifyMask);
     setsizehints(c);
     updatetitle(c);

     if((rettrans = XGetTransientForHint(dpy, w, &trans) == Success))
          for(t = clients; t && t->win != trans; t = t->next);
     if(t)
          c->tag = t->tag;

     attach(c);
     moveresize(c, c->x, c->y, c->w, c->h, 1);

     if(!c->hint)
          raiseclient(c);

     XMoveResizeWindow(dpy, c->win, c->x, c->y, c->w, c->h);
     mapclient(c);
     updatelayout();
     return;
}

void
maxlayout(void) {
     layout[seltag] = Max;
     if(!sel || ishide(sel) || sel->hint)
          return;
     Client *c;
     for(c = clients; c; c = c->next) {
          c->tile = False;
          if(!ishide(c) && !c->free) {
               c->ox = c->x;
               c->oy = c->y;
               c->ow = c->w;
               c->oh = c->h;
               moveresize(c, 0,
                          conf.ttbarheight + barheight,
                          (mw-(conf.borderheight * 2)),
                          (mh-(conf.borderheight * 2)- conf.ttbarheight - barheight), 1);
               raiseclient(sel);
               c->max = True;
          }
     }
     return;
}


/* If the type is 0, this function will move, else,
   this will resize */
void
mouseaction(Client *c, int x, int y, int type) {
     int  ocx, ocy;
     XEvent ev;

     if((c->max && !c->hint)
        || (layout[seltag] == Tile && !c->hint))
          return;
     ocx = c->x;
     ocy = c->y;

     if(XGrabPointer(dpy, root, 0, MouseMask, GrabModeAsync, GrabModeAsync,
                     None, cursor[((type) ?CurResize:CurMove)], CurrentTime) != GrabSuccess) return;
     if(type)
          XWarpPointer(dpy, None, c->win, 0, 0, 0, 0, c->w, c->h);

     for(;;) {
          XMaskEvent(dpy, MouseMask | ExposureMask | SubstructureRedirectMask, &ev);
          if(ev.type == ButtonRelease) {
               if(type)
                    XWarpPointer(dpy, None, c->win, 0, 0, 0, 0, c->w, c->h);
               XUngrabPointer(dpy, CurrentTime);
                  return;
          } else if(ev.type == MotionNotify) {

               XSync(dpy, 0);

               if(type)  /* Resize */
                    moveresize(c, c->x, c->y,
                               ((ev.xmotion.x - ocx <= 0) ? 1 : ev.xmotion.x - ocx),
                               ((ev.xmotion.y - ocy <= 0) ? 1 : ev.xmotion.y - ocy), 1);
               else     /* Move */
                    moveresize(c,
                               (ocx + (ev.xmotion.x - x)),
                               (ocy + (ev.xmotion.y - y)),
                               c->w, c->h, 0);
               if(c->y  < barheight + conf.ttbarheight - 5) {
                    moveresize(c, c->x, barheight+conf.ttbarheight, c->w, c->h, 1);
                    XUngrabPointer(dpy, CurrentTime);
                    return;
               }
          }
     }
     return;
}

void
moveresize(Client *c, int x, int y, int w, int h, bool r) {
     if(c) {
          /* Resize hints {{{ */
          if(r) {
		/* minimum possible */
               if (w < 1)
                    w = 1;
               if (h < 1)
                    h = 1;

               /* base */
               w -= c->basew;
               h -= c->baseh;

               /* aspect */
               if (c->minay > 0
                   && c->maxay > 0
                   && c->minax > 0
                   && c->maxax > 0) {
                    if (w * c->maxay > h * c->maxax)
                         w = h * c->maxax / c->maxay;
                    else if (w * c->minay < h * c->minax)
                          h = w * c->minay / c->minax;
               }

               /* incremental */
               if(c->incw)
                    w -= w % c->incw;
               if(c->inch)
                    h -= h % c->inch;

               /* base dimension */
               w += c->basew;
               h += c->baseh;

               if(c->minw > 0 && w < c->minw)
                    w = c->minw;
               if(c->minh > 0 && h < c->minh)
                    h = c->minh;
               if(c->maxw > 0 && w > c->maxw)
                    w = c->maxw;
               if(c->maxh > 0 && h > c->maxh)
                    h = c->maxh;
               if(w <= 0 || h <= 0)
                    return;
          }
          /* }}} */

          c->max = False;
          if(c->x != x || c->y != y || c->w != w || c->h != h) {
               c->x = x;
               c->y = y;
               c->w = w;
               c->h = h;

               if((y-conf.ttbarheight) <= barheight)
                    y = barheight+conf.ttbarheight;

               XMoveResizeWindow(dpy, c->win, x, y, w ,h);

               if(conf.ttbarheight) {
                    XMoveResizeWindow(dpy, c->tbar, x, y - conf.ttbarheight, w, conf.ttbarheight);
                    XMoveResizeWindow(dpy, c->button,
                                      (x + w - 10),
                                      BUTY(y),
                                      5,
                                      BUTH);
               }
          updateall();
          XSync(dpy, False);
          }
     }
     return;
}

Client *
nexttiled(Client *c) {
	for(; c && (c->hint || ishide(c)); c = c->next);
	return c;
}

void
raiseclient(Client *c) {
     if(!c)
          return;
     XRaiseWindow(dpy,c->win);

     if(conf.ttbarheight) {
          XRaiseWindow(dpy,c->tbar);
          XRaiseWindow(dpy,c->button);
     }
     return;
}

void
scan(void) {
     unsigned int i, num;
     Window *wins, d1, d2;
     XWindowAttributes wa;

     wins = NULL;
     if(XQueryTree(dpy, root, &d1, &d2, &wins, &num)) {
          for(i = 0; i < num; i++) {
               if(!XGetWindowAttributes(dpy, wins[i], &wa))
                    continue;
               if(wa.override_redirect || XGetTransientForHint(dpy, wins[i], &d1))
                    continue;
               if(wa.map_state == IsViewable)
                    manage(wins[i], &wa);
          }
     }
     if(wins)
          XFree(wins);
     arrange();

     return;
}

void
setborder(Window win, int color) {
     XSetWindowBorder(dpy, win, color);
     XSetWindowBorderWidth(dpy, win, conf.borderheight);
     return;
}

void
set_mwfact(char *cmd) {
     double c;
     if(!(sscanf(cmd, "%lf", &c)))
        return;
     if(mwfact[seltag] + c > 0.95
        || mwfact[seltag] + c < 0.05
        || layout[seltag] != Tile)
          return;
     mwfact[seltag] += c;
     arrange();
     return;
}

void
set_nmaster(char *cmd) {
     int n = atoi(cmd);
     if(nmaster[seltag] + n == 0)
          return;
     nmaster[seltag] += n;
     arrange();
     return;
}

void
setsizehints(Client *c) {
	long msize;
	XSizeHints size;

	if(!XGetWMNormalHints(dpy, c->win, &size, &msize) || !size.flags)
             size.flags = PSize;
        /* base */
	if(size.flags & PBaseSize) {
             c->basew = size.base_width;
             c->baseh = size.base_height;
        }
	else if(size.flags & PMinSize) {
             c->basew = size.min_width;
             c->baseh = size.min_height;
	}
	else
             c->basew = c->baseh = 0;
        /* inc */
	if(size.flags & PResizeInc) {
             c->incw = size.width_inc;
             c->inch = size.height_inc;
	}
	else
             c->incw = c->inch = 0;
        /* nax */
	if(size.flags & PMaxSize) {
             c->maxw = size.max_width;
             c->maxh = size.max_height;
	}
	else
             c->maxw = c->maxh = 0;
        /* min */
	if(size.flags & PMinSize) {
             c->minw = size.min_width;
             c->minh = size.min_height;
	}
	else if(size.flags & PBaseSize) {
             c->minw = size.base_width;
             c->minh = size.base_height;
	}
	else
             c->minw = c->minh = 0;
        /* aspect */
	if(size.flags & PAspect) {
             c->minax = size.min_aspect.x;
             c->maxax = size.max_aspect.x;
             c->minay = size.min_aspect.y;
             c->maxay = size.max_aspect.y;
	}
	else
             c->minax = c->maxax = c->minay = c->maxay = 0;
        c->hint = (c->maxw && c->minw && c->maxh && c->minh
                   && c->maxw == c->minw && c->maxh == c->minh);
}

void
spawn(char *cmd) {
     if(!strlen(cmd))
          return;
     if(fork() == 0) {
          if(fork() == 0) {
               if(dpy)
                    close(ConnectionNumber(dpy));
               setsid();
               execl(getenv("SHELL"), getenv("SHELL"), "-c", cmd, (char*)NULL);
               exit(1);
          }
          exit(0);
     }
      return;
}

void
tag(char *cmd) {
     int tmp = atoi(cmd);


     if(cmd[0] == '+' || cmd[0] == '-') {
          if(tmp + seltag < 1
             || tmp + seltag > conf.ntag)
               return;
          seltag += tmp;
     }
     else {
          if(tmp == seltag)
               return;
          seltag = tmp;
     }

     if(selbytag[seltag])
          focus(selbytag[seltag]);

     arrange();
     return;
}

void
tagtransfert(char *cmd) {
     int n = atoi(cmd);

     if(!sel)
          return;
     sel->tag = n;
     arrange();
}

void
tile(void) {
     unsigned int i, n, x, y, w, h, ww, hh, th;
     unsigned int barto, bord, mwf, nm;
     Client *c;

     n       =  clientpertag(seltag) - clienthintpertag(seltag);
     bord    =  conf.borderheight * 2;
     barto   =  conf.ttbarheight + barheight;
     mwf     =  mwfact[seltag] * mw;
     nm      =  nmaster[seltag];

     layout[seltag] = Tile;

     /* window geoms */
     hh = ((n <= nm) ? mh / (n > 0 ? n : 1) : mh / nm) - bord*2;
     ww = (n  <= nm) ? mw : mwf;
     th = (n  >  nm) ? mh / (n - nm) : 0;
     if(n > nm && th < barheight)
          th = mh;

     x = 0;
     y = barto;

     for(i = 0, c = nexttiled(clients); c; c = nexttiled(c->next), i++) {
          c->max = c->free = False;
          c->tile = True;
          c->ox = c->x; c->oy = c->y;
          c->ow = c->w; c->oh = c->h;
          /* MASTER CLIENT */
          if(i < nm) {
               y = barto + i * hh;
               w = ww - bord;
               h = hh;
               /* remainder */
               if(i + 1 == (n < nm ? n : nm))
                    h = (mh - hh*i) - barheight ;
               h -= bord + conf.ttbarheight;
          }
          /* TILE CLIENT */
          else {
               if(i == nm) {
                    y = barto;
                    x += ww;
               }
               w = mw - ww - bord;
               /* remainder */
               if(i + 1 == n)
                    h = (barto + mh) - y - (bord + barto);
               else
                    h = th - (bord + conf.ttbarheight) - bord*2;
          }
          moveresize(c, x, y, w, h, 0);
          if(n > nm && th != mh)
               y = c->y + c->h + bord + conf.ttbarheight;
     }
     return;
}

void
tile_switch(char *cmd) {
     Client *c;
     if(!sel || sel->hint || !sel->tile)
          return;
      if((c = sel) == nexttiled(clients))
          if(!(c = nexttiled(c->next)))
               return;
     detach(c);
     attach(c);
     focus(c);
     arrange();
}

void
togglemax(char *cmd) {
     if(!sel && ishide(sel))
          return;
     if(!sel->max) {
          sel->ox = sel->x;
          sel->oy = sel->y;
          sel->ow = sel->w;
          sel->oh = sel->h;
          moveresize(sel, 0,
                     conf.ttbarheight + barheight,
                     (mw-(conf.borderheight * 2)),
                     (mh-(conf.borderheight * 2)- conf.ttbarheight - barheight), 0);
          raiseclient(sel);
          sel->max = True;
     } else if(sel->max) {
          moveresize(sel,
                     sel->ox,
                     sel->oy,
                     sel->ow,
                     sel->oh, 0);
          sel->max = False;
     }
     return;
}

void
unhide(Client *c) {
     if(!c || !c->hide) return;
     long data[] = { NormalState, None };

     /* mapclient(c); */

     XMoveWindow(dpy,c->win,c->x,c->y);
     if(conf.ttbarheight) {
          XMoveWindow(dpy,c->tbar,c->x, (c->y - conf.ttbarheight));
          XMoveWindow(dpy,c->button, (c->x + c->w -10), (c->y - 9));
     }

     c->hide = False;
     XChangeProperty(dpy, c->win, XInternAtom(dpy, "WM_STATE", False),
                     XInternAtom(dpy, "WM_STATE", False),  32,
                     PropModeReplace, (unsigned char *) data, 2);
}

void
unmanage(Client *c) {
     XSetErrorHandler(errorhandlerdummy);
     sel = (sel == c) ? c->next : NULL;
     selbytag[seltag] = (selbytag[seltag] == c) ? c->next : NULL;
     if(conf.ttbarheight) {
          XUnmapWindow(dpy, c->tbar);
          XDestroyWindow(dpy, c->tbar);
          XUnmapWindow(dpy, c->button);
          XDestroyWindow(dpy, c->button);
     }
     detach(c);
     free(c);
     arrange();
     XSync(dpy, False);
     return;
}

void
updateall(void) {
     Client *c;
     for(c = clients; c; c = c->next)
          if(!ishide(c))
               updatetitle(c);
}

void
updatebar(void) {
     int  i ,j;
     char buf[conf.ntag][100];
     char p[3];


     tm = localtime(&lt);
     lt = time(NULL);

     XClearWindow(dpy, bar);

     XSetForeground(dpy, gc, conf.colors.text);

     for(i=0; i<conf.ntag; ++i) {
          ITOA(p, clientpertag(i+1));
          sprintf(buf[i], "%s<%s> ", conf.tag[i].name, (clientpertag(i+1)) ? p : "");
          taglen[i+1] =  taglen[i] + fonty * ( strlen(conf.tag[i].name) + strlen(buf[i]) - strlen(conf.tag[i].name) ) + fonty-4;
          /* Rectangle for the tag background */
          XSetForeground(dpy, gc, (i+1 == seltag) ? conf.colors.tagselbg : conf.colors.bar);
          XFillRectangle(dpy, bar, gc, taglen[i] - 4, 0, (strlen(buf[i])*fonty), barheight);
          /* Draw tag */
          XSetForeground(dpy, gc, (i+1 == seltag) ? conf.colors.tagselfg : conf.colors.text);
          XDrawString(dpy, bar, gc, taglen[i], fonth, buf[i], strlen(buf[i]));
     }

     updatebutton(1);

     /* Draw layout symbol */
     XSetForeground(dpy, gc, conf.colors.tagselfg);
     XDrawString(dpy, bar, gc, taglen[conf.ntag] + 4,
                 fonth-1,
                 getlayoutsym(layout[seltag]),
                 strlen(getlayoutsym(layout[seltag])));

    /* Draw status */

     sprintf(bartext,"mwfact: %.2f  nmaster: %i - %02i:%02i",
             mwfact[seltag],
             nmaster[seltag],
             tm->tm_hour,
             tm->tm_min);

     /*
          getstatuscmd("/home/martin/status.sh", status, sizeof status);
          sprintf(bartext, "%s", status);
     */
     debug(89);
     j = strlen(bartext);
     XSetForeground(dpy, gc, conf.colors.text);
     XDrawString(dpy, bar, gc, mw - j * fonty, fonth-1 , bartext ,j);
     XDrawLine(dpy, bar, gc, mw- j * fonty-5 , 0 , mw - j * fonty-5, barheight);

     XSync(dpy, False);
     return;
}

/* if c is 0, you can execute this function for the first time
 * else the button is just updated */
void
updatebutton(Bool c) {
     int i, j, p, x, pm = 0;
     XSetWindowAttributes at;

     int y = 3;
     int h = barheight - 5;
     int fonth_l = fonth - 3;

     at.override_redirect = 1;
     at.background_pixmap = ParentRelative;
     at.event_mask = ButtonPressMask | ExposureMask;

     j = taglen[conf.ntag] + ((strlen(getlayoutsym(layout[seltag]))*fonty) + 10);

     XSetFont(dpy, gc, font_b->fid);

     for(i = 0; i < conf.nbutton; ++i) {
          p = strlen(conf.barbutton[i].text);
          if(!conf.barbutton[i].x) {
               if(i)
                    pm += strlen(conf.barbutton[i-1].text) * fonty+1;
               x = (!i) ? j : j + pm;
          } else
               x = conf.barbutton[i].x;

          if(!c) {
               conf.barbutton[i].win = XCreateWindow(dpy, root, x, y, p*fonty+1, h,
                                                     0, DefaultDepth(dpy, screen),
                                                     CopyFromParent, DefaultVisual(dpy, screen),
                                                     CWOverrideRedirect | CWBackPixmap | CWEventMask, &at);

               XSetWindowBackground(dpy, conf.barbutton[i].win, conf.barbutton[i].bg_color);

               XMapRaised(dpy, conf.barbutton[i].win);
               XSetForeground(dpy, gc, conf.barbutton[i].fg_color);
               XDrawString(dpy, conf.barbutton[i].win, gc, 1, fonth_l, conf.barbutton[i].text, p);
          } else {
               if(!conf.barbutton[i].win)
                    return;
               XSetForeground(dpy, gc, conf.barbutton[i].fg_color);
               XMoveWindow(dpy, conf.barbutton[i].win, x, y);
               XDrawString(dpy, conf.barbutton[i].win, gc, 1, fonth_l,
                           conf.barbutton[i].text, strlen(conf.barbutton[i].text));
          }
     }
     XSetFont(dpy, gc, font->fid);
     return;
}

void
updatelayout(void) {
     switch(layout[seltag]) {
     case Tile: tile();       break;
     case Max:  maxlayout();  break;
     case Free: freelayout(); break;
     }
     return;
}

void
unmapclient(Client *c) {
     if(!c)
          return;
     XUnmapWindow(dpy, c->win);
     if(conf.ttbarheight) {
          XUnmapWindow(dpy, c->tbar);
          XUnmapWindow(dpy, c->button);
     }
     XUnmapSubwindows(dpy, c->win);
     return;
}

void
updatetitle(Client *c) {
     XFetchName(dpy, c->win, &(c->title));
     if(!c->title)
          c->title = strdup("WMFS");
     if(conf.ttbarheight) {
          XClearWindow(dpy, c->tbar);
          XSetForeground(dpy, gc, conf.colors.text);
          XDrawString(dpy, c->tbar, gc, 5, fonth-1, c->title, strlen(c->title));
     }
     return;
}

void
wswitch(char *cmd) {
     if(!sel || ishide(sel))
          return;
     Client *c;
     if(cmd[0] == '+') {
          for(c = sel->next; c && ishide(c); c = c->next);
          if(!c)
               for(c = clients; c && ishide(c); c = c->next);
          if(c) {
               focus(c);
               if(!c->tile)
                    raiseclient(c);
          }
          } else if(cmd[0] == '-') {
          for(c = sel->prev; c && ishide(c); c = c->prev);
          if(!c) {
               for(c = clients; c && c->next; c = c->next);
               for(; c && ishide(c); c = c->prev);
          }
          if(c) {
               focus(c);
               if(!c->tile)
                    raiseclient(c);
          }
     }
     updateall();
     return;
}

int
main(int argc,char **argv) {
     dpy = XOpenDisplay(NULL);
     int i;

     static struct option long_options[] ={
          {"help",	 	0, NULL, 'h'},
          {"info",		0, NULL, 'i'},
          {"version",     	0, NULL, 'v'},
          {NULL,		0, NULL, 0}
     };

     while ((i = getopt_long (argc, argv, "hvi", long_options, NULL)) != -1) {
          switch (i) {
          case 'h':
          default:
               printf("Usage: wmfs [OPTION]\n"
                      "   -h, --help         show this page\n"
                      "   -i, --info         show informations\n"
                      "   -v, --version      show WMFS version\n");
               exit(EXIT_SUCCESS);
               break;
          case 'i':
               printf("WMFS - Window Manager From Scratch. By :\n"
                      "   - Martun Duquesnoy (code)\n"
                      "   - Marc Lagrange (build system)\n");
               exit(EXIT_SUCCESS);
               break;
          case 'v':
               printf("WMFS version : "WMFS_VERSION".\n"
                      "  Compilation settings :\n"
                      "    - Flags : "WMFS_COMPILE_FLAGS"\n"
                      "    - Linked Libs : "WMFS_LINKED_LIBS"\n"
                      "    - On "WMFS_COMPILE_MACHINE" by "WMFS_COMPILE_BY".\n");
               exit(EXIT_SUCCESS);
               break;
          }
     }

     if(!dpy) { printf("wmfs: cannot open X server\n"); exit(1); }

     init_conf();
     init();
     scan();
     for(;;) {
          updatebar();
          getevent();
          updateall();
     }

     XCloseDisplay(dpy);
     return 0;
}

