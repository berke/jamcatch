/* CAC3200 Elementary Abstract 32-bit CPU
 *   by Berke Durak <berke@gsu.linux.org.tr>
 *
 * misc.c -- Miscelaneous routines: debugging, etc.
 */

#include <stdlib.h> 
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>

int errorf (int e, char *fmt, ...)
{
	va_list arg;
	
	if (e < 0) e = errno;
	va_start (arg, fmt);
	fprintf (stderr, "Error: ");
	vfprintf (stderr, fmt, arg);
	if (e) fprintf (stderr, " (%s -- error %d)", strerror (errno), errno);
	fputc ('\n', stderr);
	va_end (arg);

	return -1;
}

void informf (char *fmt, ...)
{
	va_list arg;
	
	va_start (arg, fmt);
	vfprintf (stdout, fmt, arg);
	fputc ('\n', stdout);
	va_end (arg);
}

char *msprintf (char *fmt, ...)
{
	char *s;
	va_list arg;
	char buf[1024];
	
	va_start (arg, fmt);
	vsprintf (buf, fmt, arg);
	s = malloc (strlen (buf)+1);
	if (!s) {
		errorf (0, "msprintf: malloc failed");
		exit (EXIT_FAILURE);
	}
	strcpy (s, buf);
	va_end (arg);
	return s;
}
#ifdef DEBUG
void debug_pf (char *fmt, ...)
{
	va_list arg;
	
	va_start (arg, fmt);
	fprintf (stderr, "debug: ");
	vfprintf (stderr, fmt, arg);
	fputc ('\n', stderr);
	va_end (arg);
}
#define debugf(x, y...) debug_pf(x,## y)
#else
#define debugf(x, y...)
#endif
