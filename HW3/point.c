#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#include "point.h"

void point_fprintf(FILE *stream, const char *fmt, const point *p)
{
  // make a copy of format, removing trailing newline if present
  int len = strlen(fmt);
  char copy_fmt[len + 1];
  strcpy(copy_fmt, fmt);
  bool newline = false;
  if (copy_fmt[len - 1] == '\n')
    {
      copy_fmt[len - 1] = '\0';
      newline = true;
      len--;
    }

  char point_format[2 * len + 5 + (newline ? 1 : 0)]; //+5 for (, ) + null
  sprintf(point_format, "(%s, %s)", copy_fmt, copy_fmt);
  if (newline)
    {
      point_format[2 * len + 4] = '\n';
      point_format[2 * len + 5] = '\0';
    }
  fprintf(stream, point_format, p->x, p->y);
}

double point_distance(const point *p1, const point *p2)
{
  return sqrt(pow(p1->x - p2->x, 2) + pow(p1->y - p2->y, 2));
}
