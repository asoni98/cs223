#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "isset.h"

typedef struct _isset_node
{
  int start;
  int end;
  struct _isset_node *parent;
  struct _isset_node *left;
  struct _isset_node *right;
} isset_node;

typedef struct _isset_search_result
{
  isset_node *parent;
  isset_node *n;
  bool found;
  int dir;
  isset_node *last_left;
  isset_node *last_right;
} isset_search_result;

struct isset
{
  isset_node *root;
  int size;
  int nodes;
};

/**
 * Returns a pointer to a new node containing the interval [item, item].
 *
 * @param item an integer
 * @return a pointer to a new node containing the interval [item, item]
 */
isset_node *isset_create_node(int item);

/**
 * Deletes the given node in the given BST.
 *
 * @param s a pointer to a set, non-NULL
 * @param node a node in that set's tree
 */
void isset_delete_node(isset* s, isset_node *node);

/**
 * Finds the node containing the given integer in the given set's tree,
 * or, if the integer is not there, determines where it would be added.
 *
 * @param s a pointer to a set, non-NULL
 * @param item an integer
 * @param return a pointer to the struct in which to record the results
 * of the search
 */
void isset_find_node(const isset *s, int item, isset_search_result *result);

/**
 * Determines if the given integer comes before, within, or after the given
 * interval.
 *
 * @param s a pointer to a set, non-NULL
 * @param start an integer
 * @param end an integer greater than or equal to start
 * @param item an integer
 * @return a negative integer if item < start, a positive integer if
 * item > start, and 0 if start <= item <= end
 */
int isset_interval_compare(const isset *s, int start, int end, int item);

/**
 * Deletes all of the nodes in the tree rooted at the given node.
 * There is no effect if given pointer is NULL.
 *
 * @param n a pointer to a node
 */
void isset_destroy_subtree(isset_node *n);

/**
 * Prints the contents of the subtree rooted at the given node,
 * or "null" if the given pointer is NULL.
 *
 * @param node a pointer to a node
 * @param level the depth of that node
 */
void isset_print_subtree(const isset_node *node, int level);

isset *isset_create()
{
  isset *s = malloc(sizeof(isset));

  if (s != NULL)
    {
      s->root = NULL;
      s->size = 0;
      s->nodes = 0;
    }

  return s;
}

int isset_size(const isset *s)
{
  if (s != NULL)
    {
      return s->size;
    }
  else
    {
      return 0;
    }
}

int isset_count_intervals(const isset *s)
{
  if (s != NULL)
    {
      return s->nodes;
    }
  else
    {
      return 0;
    }
}

bool isset_contains(const isset *s, int item)
{
  if (s == NULL)
    {
      return false;
    }
  else
    {
      isset_search_result result;
      isset_find_node(s, item, &result);
      
      return result.found;
    }
}

void isset_find_node(const isset *s, int item, isset_search_result *result)
{
  isset_node *parent = NULL;
  isset_node *curr = s->root;
  result->last_left = NULL;
  result->last_right = NULL;
  int dir = 0;
  while (curr != NULL && (dir = isset_interval_compare(s, curr->start, curr->end, item)) != 0)
    {
      parent = curr;
      if (dir < 0)
	{
	  result->last_left = curr;
	  curr = curr->left;
	}
      else
	{
	  result->last_right = curr;
	  curr = curr->right;
	}
    }

  result->parent = parent;
  result->n = curr;
  result->dir = dir;
  result->found = (curr != NULL);
}

int isset_interval_compare(const isset *s, int start, int end, int item)
{
  int left = item - start;
  int right = item - end;

  if (left >= 0 && right <= 0)
    {
      return 0;
    }
  else if (left < 0)
    {
      return left;
    }
  else
    {
      return right;
    }
}

bool isset_add(isset *s, int item)
{
  if (s == NULL)
    {
      return false;
    }
  
  // check if item is already in the set
  isset_search_result result;
  isset_find_node(s, item, &result);
  if (result.found)
    {
      // in the set already; nothing more to do
      return false;
    }

  // check if new item can be added to beginning/end of the node
  // where search fell off the tree
  if (result.dir < 0)
    {
      // moved left; see if can prepend to parent
      if (result.parent->start == item + 1)
	{
	  result.parent->start = item;
	  s->size++;

	  return true;
	}
    }
  else if (result.dir > 0)
    {
      // moved right; see if can append to parent
      if (result.parent->end == item - 1)
	{
	  result.parent->end = item;
	  s->size++;

	  return true;
	}
    }

  // need to create a whole new node
  isset_node **incoming;
  if (result.parent == NULL)
    {
      incoming = &s->root;
    }
  else if (result.dir < 0)
    {
      incoming = &result.parent->left;
    }
  else
    {
      incoming = &result.parent->right;
    }
  
  isset_node *new_node = isset_create_node(item);
  *incoming = new_node;

  if (new_node != NULL)
    {
      new_node->parent = result.parent;
      s->size++;
      s->nodes++;
  
      return true;
    }
  else
    {
      return false;
    }
}

isset_node *isset_create_node(int item)
{
  isset_node *result = (isset_node *)malloc(sizeof(isset_node));

  if (result != NULL)
    {
      result->right = NULL;
      result->left = NULL;
      result->start = item;
      result->end = item;
    }

  return result;
}

bool isset_remove(isset *s, int item)
{
  return false;
}

void isset_delete_node(isset *s, isset_node *node)
{
  // find pointer to the node to delete
  isset_node **incoming;
  if (node->parent == NULL)
    {
      incoming = &s->root;
    }
  else if (node->parent->left == node)
    {
      incoming = &node->parent->left;
    }
  else
    {
      incoming = &node->parent->right;
    }

  if (node->left == NULL && node->right == NULL)
    {
      *incoming = NULL;
    }
  else if (node->left == NULL)
    {
      *incoming = node->right;
      node->right->parent = node->parent;
    }
  else if (node->right == NULL)
    {
      *incoming = node->left;
      node->left->parent = node->parent;
    }
  else
    {
      // TWO CHILDREN CASE (not needed until we do isset_remove)
    }

  // free space for node
  free(node);
  s->nodes--;
}

int isset_next_excluded(const isset *s, int item)
{
  while (isset_contains(s, item))
    {
      item++;
    }
  return item;
}

void isset_destroy(isset *s)
{
  isset_destroy_subtree(s->root);
  s->size = 0;
  s->nodes = 0;
  s->root = NULL;
  free(s);
}

void isset_destroy_subtree(isset_node *n)
{
  if (n != NULL)
    {
      isset_destroy_subtree(n->left);
      isset_destroy_subtree(n->right);
      free(n);
    }
}

void isset_print_subtree(const isset_node *node, int level)
{
  for (int i = 0; i < level; i++)
    {
      printf(" ");
    }
  if (node != NULL)
    {
      printf("[%d-%d]\n", node->start, node->end);
      isset_print_subtree(node->left, level + 1);
      isset_print_subtree(node->right, level + 1);
    }
  else
    {
      printf("null\n");
    }
}
