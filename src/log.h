/*
 *  stwm by Kevin Hoos <kevin@yungblood.com> { for(i = 2018; i < 2118; ++i) ©(i); }
 *  File created by David Delassus.
 *  For license, see COPYING.
 */

#ifndef __LOG_H
#define __LOG_H

#include <errno.h>

void log_init(void);
void warnl(const char *format, ...);
void warnxl(const char *format, ...);
void errl(int eval, const char *format, ...);
void errxl(int eval, const char *format, ...);

#endif /* __LOG_H */
