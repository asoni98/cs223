#include <stdio.h>
#include <stdlib.h>

#include "ldigraph.h"

void search(const ldigraph *g, ldig_search *(method)(const ldigraph *, int), int from, int to);

int main(int argc, char **argv)
{
  ldigraph *g = ldigraph_create(4);

  if (g != NULL)
    {
      ldigraph_add_edge(g, 0, 1);
      ldigraph_add_edge(g, 0, 3);
      ldigraph_add_edge(g, 1, 3);
      ldigraph_add_edge(g, 1, 2);
      ldigraph_add_edge(g, 2, 0);
      ldigraph_add_edge(g, 2, 3);

      search(g, ldigraph_bfs, 0, 3);
      search(g, ldigraph_dfs, 0, 3);
      search(g, ldigraph_ofs, 0, 3);
      
      ldigraph_destroy(g);
    }
}

void search(const ldigraph *g, ldig_search *(method)(const ldigraph *, int), int from, int to)
{
  ldig_search *s = method(g, from);

  if (s != NULL)
    {
      int len;
      int *path = ldig_search_path(s, to, &len);
      if (path != NULL)
	{
	  printf("%d->%d: [", from, to);
	  for (int i = 0; i <= len; i++)
	    {
	      if (i > 0)
		{
		  printf(", ");
		}
	      printf("%d", path[i]);
	    }
	  printf("]\n");
	  free(path);
	}
      else
	{
	  printf("%d->%d: no path\n", from, to);
	}
      
      ldig_search_destroy(s);
    }
}

