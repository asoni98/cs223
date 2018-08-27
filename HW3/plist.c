#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "point.h"
#include "plist.h"

#define POINTS_INITIAL_SIZE 2

struct plist {
  int length;
  int capacity;
  point *points;
};

plist *plist_create() {
  plist *list = malloc(sizeof(plist));

  if (list != NULL)
    {
      list->length = 0;
      list->points = malloc(sizeof(point) * POINTS_INITIAL_SIZE);
      list->capacity = (list->points != NULL ? POINTS_INITIAL_SIZE : 0); //or just Points Initial Size
    }

    return list;
}

void plist_destroy(plist *l) {
  free(l->points);
  free(l);
}

int plist_size(const plist *l) {
  return (l->length);
}

bool plist_add_end(plist *l, const point *p) {
    if (l->length == l->capacity) {
      plist_embiggen(l);
    }

    if (l->length < l->capacity) {
      l->points[l->length] = *p;
      l->length++;
      return 1;
    }

    return 0;
}

void plist_embiggen(plist *l) {
  int largerCap = (l->capacity * 2 > POINTS_INITIAL_SIZE ? l->capacity * 2 : POINTS_INITIAL_SIZE);
  point *larger = realloc(l->points, largerCap * sizeof(point));

  if (larger != NULL)
   {
     l->points = larger;
     l->capacity = largerCap;
   }
}

void plist_get(const plist *l, int i, point *p) {
  if (i < (l->length)) {
    *p = l->points[i];
  }
}

bool plist_contains(const plist *l, const point *p) {
  double x = p->x;
  double y = p->y;

  for (int i = 0; i < (l->length); i++) {
    if (((l->points[i]).x == x) && ((l->points[i]).y == y)) {
      return 1;
    }
  }

  return 0;
}

void plist_fprintf(FILE *stream, const char *fmt, const plist *l) {
  printf("[");
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
  bool notLast = true;

  for (int i = 0; i < (l->length); i++) {
    if (i == ((l->length) -1)) {
      notLast = false;
    }

    point_fprintf(stream, copy_fmt, &(l->points[i]));

    if (notLast) {
      printf(", ");
    }
  }
  printf("]");
  if (newline) {
    printf("\n");
  }
}

void plist_sort(plist *l, int (*compare)(const point*, const point*)) {
  qsort(l->points, l->length, sizeof(point), (int (*) (const void*, const void*)) compare);
}
