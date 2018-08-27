#ifndef __POINT_H__
#define __POINT_H__

#include <stdio.h>

typedef struct _point
{
  double x;
  double y;
} point;

/**
 * Returns the Euclidean distance between two points.
 *
 * @param p1 a valid point, non-NULL
 * @param p2 a valid point, non-NULL
 * @return the distance between those points
 */
double point_distance(const point *p1, const point *p2);

/**
 * Prints the given point to the given stream using the given format
 * specifier.  The format specifier should be appropriate for a double
 * and the output is of the form (x, y) where x and y are formatted
 * using the given format specifier.  If the format specifier has
 * a newline character ('\n') at the end then that newline is
 * printed once after the closing parenthesis.
 *
 * @param stream a stream, non-NULL
 * @param fmt a format specifier, non-NULL
 * @param p a valid point, non-NULL
 */
void point_fprintf(FILE *stream, const char *fmt, const point *p);

#endif
