#ifndef __LDIGRAPH_H__
#define __LDIGRAPH_H__

#include <stdbool.h>

typedef struct ldigraph ldigraph;
typedef struct ldig_search ldig_search;

/**
 * Creates a new directed graph with the given number of vertices.  The
 * vertices will be numbered 0, ..., n-1.
 *
 * @param n a nonnegative integer
 * @return a pointer to the new graph
 */
ldigraph *ldigraph_create(int n);

/**
 * Returns the number of vertices in the given graph.
 *
 * @param g a pointer to a directed graph
 * @return the number of vertices in that graph
 */
int ldigraph_size(const ldigraph *g);

/**
 * Adds the given directed edge to this graph.  The behavior is
 * undefined if the edge already exists.
 *
 * @param g a pointer to a directed graph, non-NULL
 * @param from the index of a vertex in the given graph
 * @param to the index of a vertex in the given graph, not equal to from
 */
void ldigraph_add_edge(ldigraph *g, int from, int to);

/**
 * Determines if the given graph contains an edge from the given
 * from vertex to the given to vertex.
 *
 * @param g a pointer to a directed graph, non-NULL
 * @param from the index of a vertex in the given graph
 * @param to the index of a vertex in the given graph, not equal to from
 * @return true if and only if the edge exists
 */
bool ldigraph_has_edge(const ldigraph *g, int from, int to);

/**
 * Returns the result of running breadth-first search on the given
 * graph starting with the given vertex.  When the search arrives
 * at a vertex, its neighbors are considered in the order the
 * corresponding edges were added to the graph.
 * It is the caller's responsibility to destroy to result.
 *
 * @param g a pointer to a directed graph, non-NULL
 * @param from the index of a vertex in the given graph
 * @return the result of the search
 */
ldig_search *ldigraph_bfs(const ldigraph *g, int from);

/**
 * Returns the result of running depth-first search on the given
 * graph starting with the given vertex.  When the search arrives
 * at a vertex, its neighbors are considered in the order the
 * corresponding edges were added to the graph.
 * It is the caller's responsibility to destroy to result.
 *
 * @param g a pointer to a directed graph, non-NULL
 * @param from the index of a vertex in the given graph
 * @return the result of the search
 */
ldig_search *ldigraph_dfs(const ldigraph *g, int from);

/**
 * Returns the result of running best-first search on the given
 * graph starting with the given vertex. "Best" here means depth-first
 * where the neighbors of a vertex are considered in order of
 * increasing outdegree with ties broken in favor of lower index.
 * It is the caller's responsibility to destroy to result.
 *
 * @param g a pointer to a directed graph, non-NULL
 * @param from the index of a vertex in the given graph
 * @return the result of the search
 */
ldig_search *ldigraph_ofs(const ldigraph *g, int from);

/**
 * Destroys the given directed graph.
 *
 * @param g a pointer to a directed graph, non-NULL
 */
void ldigraph_destroy(ldigraph *g);

/**
 * Returns the path from the staring point of the given search to the
 * given vertex.  If no such path exists then the return value is NULL
 * and the length is -1.  The length of the path is set to the number
 * of edges on the path (not the number of edges).  It is the caller's
 * responsibility to free the returned array.
 *
 * @param s a pointer to a search result, non-NULL
 * @param to a vertex in the graph the search was performed in
 * @param len a pointer to an int, non-NULL
 * @return an array containing the vertices on the path, or NULL if there
 * is no such path
 */
int *ldig_search_path(const ldig_search *s, int to, int *len);

/**
 * Destroys the given search result.
 *
 * @param s a pointer to a search result, non-NULL
 */
void ldig_search_destroy(ldig_search *s);

#endif
