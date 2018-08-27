#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "isset.h"

/**
 * A command-line driven test of isset.  The arguments is a sequence of operations
 * to perform on a set.  The operations are
 * a num to add an integer
 * r num to remove an integer
 * c num to test if an integer in in the set
 * x num to find the next excluded integer
 * n to compute the number of integers in the set
 * i to compute the number of intervals the set
 */

int main(int argc, char **argv)
{
  isset *s = isset_create();

  for (int i = 1; i < argc; i++)
    {
      if (strlen(argv[i]) > 0)
	{
	  int num;
	  
	  switch (argv[i][0])
	    {
	    case 'a':
	      if (i + 1 < argc)
		{
		  num = atoi(argv[i + 1]);
		  if (isset_add(s, num))
		    {
		      printf("%d added\n", num);
		    }
		  else
		    {
		      printf("%d already present\n", num);
		    }
		}
	      break;

	    case 'r':
	      if (i + 1 < argc)
		{
		  num = atoi(argv[i + 1]);
		  if (isset_remove(s, num))
		    {
		      printf("%d removed\n", num);
		    }
		  else
		    {
		      printf("%d not present\n", num);
		    }
		}
	      break;

	    case 'c':
	      if (i + 1 < argc)
		{
		  num = atoi(argv[i + 1]);
		  if (isset_contains(s, num))
		    {
		      printf("%d present\n", num);
		    }
		  else
		    {
		      printf("%d not present\n", num);
		    }
		}
	      break;

	    case 'x':
	      if (i + 1 < argc)
		{
		  num = atoi(argv[i + 1]);
		  printf("next_excluded(%d) = %d\n", num, isset_next_excluded(s, num));
		}
	      break;

	    case 'n':
	      printf("element count = %d\n", isset_size(s));
	      break;

	    case 'i':
	      printf("interval count = %d\n", isset_count_intervals(s));
	      break;
	    }
	}
    }	

  isset_destroy(s);
}
