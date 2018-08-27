#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "string_helpers.h"

void string_array_free(char **arr, int n)
{
  if (arr != NULL)
    {
      for (int i = 0; i < n; i++)
	{
	  free(arr[i]);
	}
      free(arr);
    }
}

int string_hash_java(const char *s)
{
  // from Java's String.hashCode()
  // https://docs.oracle.com/javase/7/docs/api/java/lang/String.html#hashCode()
  if (s == NULL)
    {
      return 0;
    }
  
  int power = 31;
  int hash = 0;
  const char *p = s + strlen(s) - 1; // start at end of string
  while (p >= s)
    {
      hash += power * *p;
      power *= 31;
      p--;
    }

  // make sure hash value is nonnegative so careless maps don't break
  if (hash < 0)
    {
      hash = -hash;
      if (hash < 0)
	{
	  hash = 0;
	}
    }
  return hash;
}

void string_array_fprintf(FILE *fp, char **arr, int n)
{
  if (arr != NULL)
    {
      for (int i = 0; i < n; i++)
	{
	  fprintf(fp, "%s\n", arr[i]);
	}
    }
}

  
