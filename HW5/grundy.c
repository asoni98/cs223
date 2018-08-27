#include <stdio.h>
#include <stdlib.h>

#include "isset.h"

/**
 * Computes the Grundy numbers for Grundy's game.  The command-line
 * argument should be a positive integer giving the maximum size
 * of the game to compute the Grundy number for.
 *
 * Note that this does not completely test the ISSet ADT.
 *
 * The output should match the sequence given in the Online Encyclopedia
 * of Integer Sequences (OEIS): http://oeis.org/A002188
 *
 * If your output does not match, look at the first value that doesn't match,
 * and then examine all of the operations (several add and one next-excluded)
 * that are done during the corresponding iteration to determine what is going wrong
 * with your tree.
 */

int main(int argc, char **argv)
{
  // read command-line argument
  if (argc < 2)
    {
      fprintf(stderr, "USAGE: %s n\n", argv[0]);
      return 0;
    }

  int n = atoi(argv[1]);
  if (n <= 0)
    {
      fprintf(stderr, "%s: n must be a positive integer\n", argv[0]);
      return 1;
    }

  // create an array of Grundy numbers with the numbers for the final
  // position computed
  int size = n + 1 > 3 ? n + 1 : 3;
  int grundy[size];
  grundy[0] = 0; // unreachable
  grundy[1] = 0; // unreachable
  grundy[2] = 0; // final position -- a win for the player who just moved

  // compute the Grundy numbers for all the other values
  for (int i = 3; i <= n; i++)
    {
      // keep track of the Grundy numbers of states we can reach from i
      isset *reachable = isset_create();

      // for each legal move...
      for (int right, left = 1; left < (right = i - left); left++)
	{
	  // ...compute the Grundy number of the resulting state
	  // and record it
	  // (for gameslike Grundy's game with certain properties,
	  // the Grundy number of a collection of independent games
	  // is the exclusive-or of the Grundy numbers of those games)
	  int xor = grundy[left] ^ grundy[right];
	  isset_add(reachable, xor);
	}

      // the Grundy number of a state is the smallest Grundy number such
      // that no state reachable in one move from that state has that number
      int nex = isset_next_excluded(reachable, 0);

      // remember the Grundy number of i
      grundy[i] = nex;

      // destroy the set of Grundy numbers of reachable states
      isset_destroy(reachable);
    }

  // output the resulting array of Grundy numbers nicely
  printf("[");
  for (int i = 0; i <= n; i++)
    {
      if (i > 0)
	{
	  printf(", ");
	}
      printf("%d", grundy[i]);
    }
  printf("]\n");
}
