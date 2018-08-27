#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <float.h>
#include "point.h"
#include "plist.h"
void closest_pair_brute_force(const plist *l, point *p1, point *p2, double *d);
void split_list_x(const plist *l, plist *left, plist *right);
void split_list_y(const plist *l, const plist *x_left, const plist *x_right, plist *y_left, plist *y_right);
void make_middle(const plist *list_y, plist *middle, double left, double right);
void search_middle(const plist *middle, point *p1, point *p2, double *d);
int pointSort(point *p1, point *p2);
int pointCompare(point *p1, point *p2, point *p3, point *p4);
#define MAX_POINTS_TO_CHECK 7


/**
 * Compares the two given points based on x-coordinate.
 *
 * @param p1 a pointer to a point, non-NULL
 * @param p2 a pointer to a point, non-NULL
 * @return a negative number if p1 comes before p2, positive if p1 comes
 * before p2, and 0 if they are the same
 */

 int point_compare_x(const point *p1, const point *p2) {
   double a = p1->x;
   double b = p2->x;
	 if (a==b) {
		 if (p1->y < p2->y) {
			 return -1;
		 }
		 else {
			 return 1;
		 }
	 }
   else if (a>b) {
     return 1;
   }
   else {
     return -1;
   }
}

/**
 * Compares the two given points based on y-coordinate.
 *
 * @param p1 a pointer to a point, non-NULL
 * @param p2 a pointer to a point, non-NULL
 * @return a negative number if p1 comes before p2, positive if p1 comes
 * before p2, and 0 if they are the same
 */

 //does comes before mean greater. fix 	to constant shit
 int point_compare_y(const point *p1, const point *p2) {
   double a = p1->y;
   double b = p2->y;
   if (a==b) {
     if (p1->x < p2->x) {
       return -1;
     }
     else {
       return 1;
     }
   }
   else if (a>b) {
     return 1;
   }
   else {
     return -1;
   }}

/**
 * Reads point from the given stream and adds them to the given list.
 *
 * @param stream a pointer to an input stream
 * @param l a pointer to a list
 * @param n a non-negative integer
 */
void read_points(FILE *stream, plist *l, int n) {
  float x;
  float y;
  // scanf("%d", &n);
  while (n > 0) {
	  if (scanf("%f %f", &x, &y) == 2) {
			point p1 = {x, y};
	    plist_add_end(l, &p1);
	    n--;
		}
		else {
			n = 0;
		}
  }
}

/**
 * Copies the points from the source list to the destination list
 * in the order they appear in the destination list.
 *
 * @param dest a pointer to a list, non-NULL
 * @param source a pointer to a list, non-NULL
 */

void copy_list(plist *dest, const plist *source) {
  point p5;
  for (int i = 0; i < plist_size(source); i++) {
    plist_get(source, i, &p5);
    plist_add_end(dest, &p5);
  }
}

/**
 * Returns the closest pair of points among those on the given list.
 * The closest pair are returned in p1 and p2 and their distance is
 * returned in d.  This implementation uses the O(n log n) divide-and-conquer
 * algorithm.
 *
 * @param l a pointer to a list of at least 2 points
 * @param p1 a pointer to a point, non-NULL
 * @param p2 a pointer to a point, non-NULL
 * @param d a pointer to a double, non-NULL
 */
void closest_pair(const plist *list_x, const plist *list_y, point *p1, point *p2, double *d) {
    int n = plist_size(list_x);

		  // make left/right lists
		plist *x_left = plist_create();
		plist *x_right = plist_create();
		plist *y_left = plist_create();
		plist *y_right = plist_create();

    if (n <= 3)
      {
        closest_pair_brute_force(list_x, p1, p2, d);
				plist_destroy(x_left);
				plist_destroy(x_right);
				plist_destroy(y_left);
				plist_destroy(y_right);
				return;
      }

    // populate left/right lists
    split_list_x(list_x, x_left, x_right);
    split_list_y(list_y, x_left, x_right, y_left, y_right);

    // recursively find closest pair in two halves and keep the closer of those
    point p1_left, p2_left;
    double d_left = DBL_MAX;
    closest_pair(x_left, y_left, &p1_left, &p2_left, &d_left);

    point p1_right, p2_right;
    double d_right = DBL_MAX;
    closest_pair(x_right, y_right, &p1_right, &p2_right, &d_right);

		if ((d_left < d_right) && (d_left < *d)) {
			*d = d_left;
			*p1 = p1_left;
			*p2 = p2_left;
		}
		else if ((d_right < d_left) && (d_right < *d)) {
			*d = d_right;
			*p1 = p1_right;
			*p2 = p2_right;
		}
		else if ((d_left == *d) && (d_right == *d)) {
			pointCompare(&p1_left, &p2_left, &p1_right, &p2_right);
			pointCompare(p1, p2, &p1_left, &p2_left);
		}
		else if (d_left == *d) {
			pointCompare(p1, p2, &p1_left, &p2_left);
		}
		else if (d_right == *d) {
			pointCompare(p1, p2, &p1_right, &p2_right);
		}
    point rightHold;
    point leftHold;
    plist_get(x_left, plist_size(x_left)-1, &rightHold);
    double right = rightHold.x;
    plist_get(x_right, 0, &leftHold);
		double left = leftHold.x;

    double mid = ((right + left)/2);

		plist *middle = plist_create();

    // populate that list
		//fix this. make middle should prob be run twice
    make_middle(list_y, middle, mid - *d, mid + *d); //needs to be sorted by y

    // search the list of points in middle for a closer pair
    search_middle(middle, p1, p2, d); //if closer change the points
		plist_destroy(x_left);
		plist_destroy(x_right);
		plist_destroy(y_left);
		plist_destroy(y_right);
		plist_destroy(middle);
}

/**
 * Returns the closest pair of points among those on the given list.
 * The closest pair are returned in p1 and p2 and their distance is
 * returned in d.  This implementation uses the Theta(n^2) brute force
 * algorithm.
 *
 * @param l a pointer to a list of at least 2 points
 * @param p1 a pointer to a point, non-NULL
 * @param p2 a pointer to a point, non-NULL
 * @param d a pointer to a double, non-NULL
 */

 //run Distance with originally -1 as the distance
 void closest_pair_brute_force(const plist *l, point *p1, point *p2, double *d) {//set d to DBL_MAX
	 for (int i = 0; i < plist_size(l); i++) {
		 for (int j = 0; j < plist_size(l); j++) {
			 if (j == i) {continue;}
			 point p1Temp;
			 point p2Temp;
			 plist_get(l, i, &p1Temp);
			 plist_get(l, j, &p2Temp);
			 double distance = point_distance(&p1Temp, &p2Temp);
			 //if (*d == -1) {distance = &d;}
			 if (distance < *d) {//FIX EQUALS CASE
				 *d = distance;
				 plist_get(l, i, p1);
				 plist_get(l, j, p2);
			 }

			 if (distance == *d) {
				 point p5;
				 plist_get(l, i, &p5);
				 point p6;
				 plist_get(l, i, &p6);
				 pointCompare(p1, p2, &p5, &p6);
			 }

		 }
	 }
 }

/**
 * Splits the given list by adding the leftmost half of the points
 * in order of increasing x-coordinate to the end of left and the
 * rightmost half of the points in order of increasing x-coordinate
 * to to the end of right.
 *
 * @param l a pointer to a list sorted by x-coordinate, non-NULL
 * @param left a pointer to a list, non-NULL
 * @param right a pointer to a list, non-NULL
 */
void split_list_x(const plist *l, plist *left, plist *right) {
  int firstHalf = (int)floor((plist_size(l))/2);
  point pointArb;
  for (int i = 0; i < firstHalf; i++) {
    plist_get(l, i, &pointArb);
    plist_add_end(left, &pointArb);
  }

  for (int i = firstHalf; i < plist_size(l); i++) {
    plist_get(l, i, &pointArb);
    plist_add_end(right, &pointArb);
  }
}

/**
 * Splits the given list by adding the leftmost half of the points
 * in order of increasing y-coordinate to the end of y_left_y and the
 * rightmost half of the points in order of increasing y-coordinate
 * to to the end of y_right.
 *
 * @param l a pointer to a list sorted by y-coordinate, non-NULL
 * @param x_left a pointer to a list containing the leftmost half of the points
 * on l in order of increasing x-coordinate
 * @param x_right a pointer to a list containing the rightmost half of the
 * points on l in order of increasing x-coordinate
 * @param y_left a pointer to a list
 * @param y_right a pointer to a list
 *
 * @param end a pointer to a list, non-NULL
 */

 void split_list_y(const plist *l, const plist *x_left, const plist *x_right, plist *y_left, plist *y_right) {
	 point pointLeftmax;
   point pointLeft;
   point pointArb;
	 plist_get(x_left, plist_size(x_left)-1, &pointLeftmax); //check this line x_left and size - 1
   double xLeftmax = pointLeftmax.x;

	 for (int i = 0; i < plist_size(l); i++) {
     plist_get(l, i, &pointLeft);
		 if (((pointLeft).x <= xLeftmax) && (plist_size(y_left) <= plist_size(x_left))) {
       plist_get(l, i, &pointArb);
       plist_add_end(y_left, &pointArb);
		 }
		 else {
       plist_get(l, i, &pointArb);
			 plist_add_end(y_right, &pointArb);
		 }
	 }
 }

/**
 * Adds the points in the first given list with x-coordinates in the given
 * range to the second list in order of increasing y-coordinate
 *
 * @param list_y a pointer to a list of points sorted by y-coordinate, non-NULL
 * @param middle a pointer to a list, non-NULL
 * @param left a real number
 * @param right a real number greater than or equal to left
 */
void make_middle(const plist *list_y, plist *middle, double left, double right) {
  point pointX;

  for (int i = 0; i < (plist_size(list_y)); i++) {
    plist_get(list_y, i, &pointX);
    double x = pointX.x;
    if ((x >= left) && (x <= right)) {
      plist_add_end(middle, &pointX);
    }
  }
}

/**
 * Searches the given list for a pair of points closer than d units.
 * If such a pair is found, p1 and p2 are updated to the closest such pair
 * and d is updated to their distance.
 *
 * @param middle a pointer to a list of points sorted by y-coordinate, non-NULL
 * @param p1 a pointer to a point, non-NULL
 * @param p2 a pointer to a point, non-NULL
 * @param d a pointer to a positive real number, non-NULL
 */

 void search_middle(const plist *middle, point *p1, point *p2, double *d) {
	 //int jmin;
	 int jmax;

	 for (int i = 0; i < plist_size(middle); i++) {
		 if ((i + MAX_POINTS_TO_CHECK) > plist_size(middle)) {jmax = plist_size(middle);}
		 else {jmax = i + MAX_POINTS_TO_CHECK;}

		 for (int j = 0; j < jmax; j++) {
			 if (j == i) {continue;}

       point p1Temp;
       point p2Temp;
       plist_get(middle, i, &p1Temp);
       plist_get(middle, j, &p2Temp);
       double distance = point_distance(&p1Temp, &p2Temp);

			 if (distance < *d) {
         *d = distance;
         plist_get(middle, i, p1);
         plist_get(middle, j, p2);
			 }

			 if (distance == *d) {
         point p5;
         plist_get(middle, i, &p5);
         point p6;
         plist_get(middle, i, &p6);
         pointCompare(p1, p2, &p5, &p6);
			 }
		 }
	 }
 }

 /*
  * Checks two points and puts the "smaller" one first. It follows
  * this assignment criteria where each coordinate and the distance is output
  * in the default form for printf and the first point listed is the one with the
  * lower x-coordinate (lower y-coordinate if the x-coordinates are the same).
  * If there are multiple pairs of points with the same distance then the one
  * output should be the first one when all such pairs are sorted by first
  * x-coordinate, then first y-coordinate, then second x-coordinate, then second
  * y-coordinate.
  * If such a pair is found, p1 is updated to be the "smaller" first point.
  * @param p1 a pointer to a point, non-NULL
  * @param p2 a pointer to a point, non-NULL
  * @return is an int (1 means the points were switched.
                      0 means the points weren't switched.
                      -1 means there was an error (i.e the points were the same)
  */
int pointSort(point *p1, point *p2) {
	if (p1->x > p2->x) {
		point temp = *p2;
		*p2 = *p1;
		*p1 = temp;
		return 1;
	}
	else if (p1->x == p2->x) {
		if (p1->y < p2->y){
			return 0;
		}

		else if (p1->y > p2->y){
			point temp = *p2;
			*p2 = *p1;
			*p1 = temp;
			return 1;
		}
		else {
			return -1;
		}
	}
	else {
		return 0;
	}
}

  /*
   * Checks two pairs of points and puts the "smaller" pair first.
   * If such a pair is found, p1 and p2 are updated to be
   * the "smaller" first pair.
   * @param p1 a pointer to a point, non-NULL
   * @param p2 a pointer to a point, non-NULL
   * @param p3 a pointer to a point, non-NULL
   * @param p4 a pointer to a point, non-NULL
   * @return is an int
   *        (1 means the pair of points were switched.
   *         0 means the pair of points weren't switched.
   *        -1 means there was an error (i.e a pair of point were the same)
   */

int pointCompare(point *p1, point *p2, point *p3, point *p4) {
	if ((pointSort(p1, p2) != -1) && (pointSort(p3, p4) != -1)){
		if (pointSort(p1, p3) == 1) {
			//p1 and p3 have been switched. this means p3 is smaller. now we switched p4 and p2 and return 1
			point temp = *p4;
			*p4 = *p2;
			*p2 = temp;
			return 1;
		}

		else if (pointSort(p1, p3) == 0) {
			return 1;
			//p1 and p2 are right
		}

		else if (pointSort(p1, p3) == -1) {
			if (pointSort(p2, p4) == 1) {
				//p2 and p4 have been switched. this means p4 is smaller. now we switched p3 and p1 and return 1
				point temp = *p3;
				*p3 = *p1;
				*p1 = temp;
				return 1;
			}

			else if (pointSort(p2, p4) == 0) {
				return 1;
				//p1 and p3 are the same and p2 is smaller than p4
			}

			else if (pointSort(p2, p4) == -1) {
				return 1;
				//they are the same two points
			}

      else {
        return -1;
      }
		}

    else {
      return -1;
    }
	}

	else {
		return -1;
	}
}


int main(int argc, char **argv)
 {
  // create empty lists
  plist *list_x = plist_create();
  plist *list_y = plist_create();

  if (list_x == NULL || list_y == NULL)
    {
      if (list_x != NULL)
	{
	  plist_destroy(list_x);
	}

      if (list_y != NULL)
	{
	  plist_destroy(list_y);
	}

      printf("%s: could not allocate lists\n", argv[0]);
      return 1;
    }

  // read n
  int n;
  fscanf(stdin, "%d", &n);
	// if (isNumeric(n) == false) {
	// 	fprintf(stdout, "%s: failed to read points\n", argv[0]);
	// 	return 0;
	// }
	if (n < 2) {
		fprintf(stdout, "%s: failed to read points\n", argv[0]);
		return 0;
	}

  // read into one list
  read_points(stdin, list_x, n);

  // sort list
  plist_sort(list_x, point_compare_x);

  // check for distinctness
  point pointCheck;
  point pointCheckPrev;
  for (int i = 0; i < plist_size(list_x); i++) {
    plist_get(list_x, i, &pointCheck);
    if (i !=0) {
      plist_get(list_x, i-1, &pointCheckPrev);
      if (point_distance(&pointCheck, &pointCheckPrev) == 0) {
        fprintf(stdout, "Same Point Entered\n");
        return 0;
      }
    }
  }

  // make list_y a copy of list_x
  copy_list(list_y, list_x);

  if (plist_size(list_y) == n)
    {
      // sort the y-list
      plist_sort(list_y, point_compare_y);

      point p1, p2;
      double d = DBL_MAX;

      closest_pair(list_x, list_y, &p1, &p2, &d);
			if (pointSort(&p1, &p2) == -1) {
				fprintf(stdout, "Same Point Entered\n");
				return 0;
			}
			//plist_fprintf(stdout, "%.3f\n", list_x);
      printf("(%f, %f)-(%f, %f)=%f\n", p1.x, p1.y, p2.x, p2.y, d);
    }
  else
    {
      // memory allocation must have failed when copying points
      fprintf(stdout, "%s: failed to read points\n", argv[0]);
    }

  plist_destroy(list_x);
  plist_destroy(list_y);
}
