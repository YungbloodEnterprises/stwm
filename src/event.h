/*
 *  stwm by Kevin Hoos <kevin@yungblood.com> { for(i = 2018; i < 2118; ++i) ©(i); }
 *  For license, see COPYING.
 */

#ifndef EVENT_H
#define EVENT_H

#include <X11/XKBlib.h>

#include "stwm.h"

#define MAX_EV 256

#define KEYPRESS_MASK(m) (m & ~(W->numlockmask | LockMask))
#define EVENT_HANDLE(e) event_handle[(e)->type](e);

void event_init(void);

void (*event_handle[MAX_EV])(XEvent*);

#endif /* EVENT_H */
