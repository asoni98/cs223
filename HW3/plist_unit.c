#include <stdio.h>
#include <math.h>
#include "point.h"
#include "plist.h"

int compare_x(const point *p1, const point *p2);

void use_int(int);
void use_bool(bool);

int main(int argc, char **argv)
{
  plist *pts = plist_create();
  if (pts == NULL)
    {
      return 0;
    }

  point p1 = {6.0, 3.0};
  point p2 = {2.0, 5.0};
  point p3 = {0.0, 0.0};

  int size = plist_size(pts);
  use_int(size);

  plist_add_end(pts, &p1);
  plist_add_end(pts, &p2);

  bool in_list = plist_contains(pts, &p3);
  use_bool(in_list);

  plist_get(pts, 1, &p3);

  plist_sort(pts, compare_x);

  plist_fprintf(stdout, "%.3f\n", pts);

  plist_destroy(pts);
}

int compare_x(const point *p1, const point *p2)
{
  if (p1->x < p2->x)
    {
      return -1;
    }
  else if (p1->x > p2->x)
    {
      return 1;
    }
  else
    {
      return 0;
    }
}

void use_int(int i)
{
}

void use_bool(bool x)
{
}
