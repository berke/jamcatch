/* CAC3200 Elementary Abstract 32-bit CPU
 *   by Berke Durak <berke@gsu.linux.org.tr>
 *
 * misc.h -- Header for miscelaneous routines: debugging, etc.
 */

#ifndef MISC_H
#define MISC_H

int errorf (int e, char *fmt, ...);
void informf (char *fmt, ...);
char *msprintf (char *fmt, ...);

#ifdef DEBUG
void debug_pf (char *fmt, ...);
#define debugf(x, y...) debug_pf(x,## y)
#else
#define debugf(x, y...)
#endif

#endif
