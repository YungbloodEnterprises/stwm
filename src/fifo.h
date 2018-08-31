/*
 *  stwm by Kevin Hoos <kevin@yungblood.com> { for(i = 2018; i < 2118; ++i) ©(i); }
 *  File created by David Delassus.
 *  For license, see COPYING.
 */

#ifndef __FIFO_H
#define __FIFO_H

#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

void fifo_init(void);
void fifo_read(void);

#endif /* __FIFO_H */
