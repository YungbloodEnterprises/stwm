/*
 *  stwm by Kevin Hoos <kevin@yungblood.com> { for(i = 2018; i < 2118; ++i) ©(i); }
 *  For license, see COPYING.
 */

#ifndef SYSTRAY_H
#define SYSTRAY_H

#include "stwm.h"

void systray_acquire(void);
void systray_add(Window win);
void systray_del(struct _systray *s);
void systray_state(struct _systray *s);
void systray_freeicons(void);
struct _systray *systray_find(Window win);
int systray_get_width(void);
void systray_update(void);

#endif /* SYSTRAY_H */
