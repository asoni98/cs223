#define _GNU_SOURCE

#include <stdlib.h>
#include <stdbool.h>

#include "ldigraph.h"

struct ldigraph
{
  int n;          // the number of vertices
  int *list_size; // the size of each adjacency list
  int *list_cap;  // the capacity of each adjacency list
  int **adj;      // the adjacency lists
};

struct ldig_search
{
  const ldigraph *g;
  int from;
  int *color;
  int *dist;
  int *pred;
};

enum {WHITE, GRAY, BLACK};

#define LDIGRAPH_ADJ_LIST_INITIAL_CAPACITY 4

/**
 * Resizes the adjacency list for the given vertex in the given graph.
 * 
 * @param g a pointer to a directed graph
 * @param from the index of a vertex in that graph
 */
void ldigraph_list_embiggen(ldigraph *g, int from);

/**
 * Visits the given vertex in the given search of the given graph.
 *
 * @param g a pointer to a directed graph
 * @param s a search in that graph
 * @param from a vertex in that graph
 */
void ldigraph_dfs_visit(const ldigraph* g, ldig_search *s, int from);

/**
 * Prepares a search result for the given graph starting from the given
 * vertex.  It is the responsibility of the caller to destroy the result.
 *
 * @param g a pointer to a directed graph
 * @param from the index of a vertex in that graph
 * @return a pointer to a search result
 */
ldig_search *ldig_search_create(const ldigraph *g, int from);

ldigraph *ldigraph_create(int n)
{
  if (n < 1)
    {
      return NULL;
    }
  
  ldigraph *g = malloc(sizeof(ldigraph));
  if (g != NULL)
    {
      g->n = n;
      g->list_size = malloc(sizeof(int) * n);
      g->list_cap = malloc(sizeof(int) * n);
      g->adj = malloc(sizeof(int *) * n);
      
      if (g->list_size == NULL || g->list_cap == NULL || g->adj == NULL)
	{
	  free(g->list_size);
	  free(g->list_cap);
	  free(g->adj);
	  free(g);

	  return NULL;
	}

      for (int i = 0; i < n; i++)
	{
	  g->list_size[i] = 0;
	  g->adj[i] = malloc(sizeof(int) * LDIGRAPH_ADJ_LIST_INITIAL_CAPACITY);
	  g->list_cap[i] = g->adj[i] != NULL ? LDIGRAPH_ADJ_LIST_INITIAL_CAPACITY : 0;
	}
    }

  return g;
}

int ldigraph_size(const ldigraph *g)
{
  if (g != NULL)
    {
      return g->n;
    }
  else
    {
      return 0;
    }
}

void ldigraph_list_embiggen(ldigraph *g, int from)
{
  if (g->list_cap[from] != 0)
    {
      g->adj[from] = realloc(g->adj[from], sizeof(int*) * g->list_cap[from] * 2);
      g->list_cap[from] = g->adj[from] != NULL ? g->list_cap[from] * 2 : 0;
    }
}

void ldigraph_add_edge(ldigraph *g, int from, int to)
{
  if (g != NULL && from >= 0 && to >= 0 && from < g->n && to < g->n && from != to)
    {
      // make room if necessary
      if (g->list_size[from] == g->list_cap[from])
	{
	  ldigraph_list_embiggen(g, from);
	}

      // add to end of array if there is room
      if (g->list_size[from] < g->list_cap[from])
	{
	  g->adj[from][g->list_size[from]++] = to;
	}
    }
}

bool ldigraph_has_edge(const ldigraph *g, int from, int to)
{
  if (g != NULL && from >= 0 && to >= 0 && from < g->n && to < g->n && from != to)
    {
      // sequential search of from's adjacency list
      int i = 0;
      while (i < g->list_size[from] && g->adj[from][i] != to)
	{
	  i++;
	}
      return i < g->list_size[from];
    }
  else
    {
      return false;
    }
}

ldig_search *ldigraph_bfs(const ldigraph *g, int from)
{
  return NULL;
}

ldig_search *ldigraph_dfs(const ldigraph *g, int from)
{
  if (g == NULL || from < 0 || from >= g->n)
    {
      return NULL;
    }

  ldig_search *s = ldig_search_create(g, from);
  if (s != NULL)
    {
      // start at from
      s->color[from] = GRAY;
      s->dist[from] = 0;
      ldigraph_dfs_visit(g, s, from);
    }
  return s;
}

ldig_search *ldigraph_ofs(const ldigraph *g, int from)
{
  return NULL;
}

void ldigraph_dfs_visit(const ldigraph* g, ldig_search *s, int from)
{
  // get vertices we can reach in one edge from from
  const int *neighbors = g->adj[from];
  
  // iterate over outgoing edges
  for (int i = 0; i < g->list_size[from]; i++)
    {
      int to = neighbors[i];
      if (s->color[to] == WHITE)
	{
	  // found an edge to a new vertex -- explore it
	  s->color[to] = GRAY;
	  s->dist[to] = s->dist[from] + 1;
	  s->pred[to] = from;
	  
	  ldigraph_dfs_visit(g, s, to);
	}
    }
  
  // mark current vertex finished
  s->color[from] = BLACK;
}

void ldigraph_destroy(ldigraph *g)
{
  if (g != NULL)
    {
      for (int i = 0; i < g->n; i++)
	{
	  free(g->adj[i]);
	}
      free(g->adj);
      free(g->list_cap);
      free(g->list_size);
      free(g);
    }
}

ldig_search *ldig_search_create(const ldigraph *g, int from)
{
  if (g != NULL && from >= 0 && from < g->n)
    {
      ldig_search *s = malloc(sizeof(ldig_search));
      
      if (s != NULL)
	{
	  s->g = g;
	  s->from = from;
	  s->color = malloc(sizeof(int) * g->n);
	  s->dist = malloc(sizeof(int) * g->n);
	  s->pred = malloc(sizeof(int) * g->n);

	  if (s->color != NULL && s->dist != NULL && s->pred != NULL)
	    {
	      // initialize all vertices to unseen
	      for (int i = 0; i < g->n; i++)
		{
		  s->color[i] = WHITE;
		  s->dist[i] = -1;
		  s->pred[i] = -1;
		}
	    }
	  else
	    {
	      free(s->pred);
	      free(s->dist);
	      free(s->color);
	      free(s);
	      return NULL;
	    }
	}

      return s;
    }
  else
    {
      return NULL;
    }
}

int *ldig_search_path(const ldig_search *s, int to, int *len)
{
  return NULL;
}

void ldig_search_destroy(ldig_search *s)
{
  if (s != NULL)
    {
      free(s->color);
      free(s->dist);
      free(s->pred);
      free(s);
    }
}
