#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "chomp.h"

char **chomp_states(int rows, int cols, int *n)
{
  if (rows <= 0 || cols <= 0 || rows > 9)
    {
      return NULL;
    }

  // count[i][j] is the number of rows*(i+1) chomp states with exactly j in the last column
  int count[cols + 1][rows + 1];
  for (int r = 0; r <= rows; r++)
    {
      count[0][r] = 1;
    }

  for (int c = 2; c <= cols + 1; c++)
    {
      for (int r = 0; r <= rows; r++)
	{
	  int sum = 0;
	  for (int r2 = r; r2 <= rows; r2++)
	    {
	      sum += count[c - 2][r2];
	      if (sum <= 0)
		{
		  // overflow
		  return NULL;
		}
	    }
	  count[c - 1][r] = sum;
	}
    }

  *n = count[cols][0];

  char **result = malloc(sizeof(char *) * *n);
  if (result != NULL)
    {
      // last state is whole pan
      result[*n - 1] = malloc(sizeof(char) * (cols + 1));
      if (result[*n - 1] != NULL)
	{
	  for (int i = 0; i < cols; i++)
	    {
	      result[*n - 1][i] = '0' + rows;
	    }
	  result[*n - 1][cols] = '\0';
	}

      // each previous state is obtained by decrementing the next
      for (int i = *n - 2; i >= 0; i--)
	{
	  if (result[i + 1] != NULL)
	    {
	      result[i] = malloc(sizeof(char) * (cols + 1));
	      if (result[i] != NULL)
		{
		  strcpy(result[i], result[i + 1]);
		  // find last non-zero (know there will be one)
		  int j = cols - 1;
		  while (result[i][j] == '0')
		    {
		      j--;
		    }
		  
		  // decrement it
		  result[i][j]--;

		  // set all subsequent to equal it
		  for (int k = j + 1; k < cols; k++)
		    {
		      result[i][k] = result[i][j];
		    }
		}
	    }
	}
    }
  else
    {
      *n = 0;
    }

  return result;
}

char **chomp_successors(const char *s, int *n)
{
  return NULL;
}
