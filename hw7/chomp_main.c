#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "chomp.h"
#include "string_helpers.h"
#include "smap.h"

enum {CURRENT, OTHER};

int main(int argc, char **argv)
{
  if (argc < 2 || strlen(argv[1]) == 0 || argv[1][0] == '0')
    {
      fprintf(stderr, "USAGE: %s state\n", argv[0]);
      return 1;
    }

  int cols = strlen(argv[1]);
  int rows = 1;
  for (char *s = argv[1]; *s; s++)
    {
      if (!isdigit(*s) || *s < *(s + 1))
	{
	  // string contains non-digit or increasing digits
	  fprintf(stderr, "USAGE: %s state\n", argv[0]);
	  return 1;
	}

      // rows is max over all digits in string
      int h = *s - '0';
      if (h > rows)
	{
	  rows = h;
	}
    }

  int count;
  char **states = chomp_states(rows, cols, &count);
  if (states == NULL)
    {
      fprintf(stderr, "%s: error allocating memory\n", argv[0]);
      return 1;
    }

  // create a map from states (strings) to position values

  // set final state to win for current player

  // for each other state
  for (int i = 1; i < count; i++)
    {
      // get successor states
      int succ_count;
      char **succ = chomp_successors(states[i], &succ_count);
      if (succ != NULL)
	{
	  // if all successor states are wins then set states[i] to loss
	  // else set states[i] to win
	}

      string_array_free(succ, succ_count);
    }

  // look up result for argv[1]

  // if a loss then print LOSS
  // else find first successor that is a loss and output WIN: that successor
  
  string_array_free(states, count);
}
