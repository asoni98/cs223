#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <time.h>


int main(int argc, char **argv) {
  int count = 200000;
  //double x;
  double y;
  double z;

  srand ( time(NULL) );

fprintf(stdout, "%d \n", count/2);
  while (count > 0) {
    y = rand();
    z = rand();
    count--;
    fprintf(stdout, "%f \n", (double) ((y/z)));
  }
}
