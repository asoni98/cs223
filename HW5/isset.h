#ifndef __ISSET_H__
#define __ISSET_H__

#include <stdbool.h>

typedef struct isset isset;

/**
 * Creates a set of integers.
 *
 * @return a pointer to a new empty set of integers
 */
isset *isset_create();

/**
 * Returns the total number of integers in the given set.
 *
 * @param s a pointer to a set, non-NULL
 * @return the total number of integers in that set
 */
int isset_size(const isset *s);

/**
 * Returns the minimum number of intervals of consecutive integers
 * the integers in the given set can be partitioned into.
 * 
 * @param s a pointer to a set, non-NULL
 * @return the number of intervals of integers in that set
 */
int isset_count_intervals(const isset *s);

/**
 * Determines whether the given integer is in the given set.
 *
 * @param s a pointer to a set, non-NULL
 * @param item an integer
 */
bool isset_contains(const isset *s, int item);

/**
 * Adds the given integer to the given set.
 *
 * @param s a pointer to a set, non-NULL
 * @param item an integer
 * @return true if and only if the item was not in the set and was added
 */
bool isset_add(isset *s, int item);

/**
 * Removes the given integer from the given set.
 *
 * @param s a pointer to a set, non-NULL
 * @param item an integer
 * @return true if and only if the item was in the set
 */
bool isset_remove(isset *s, int item);

/**
 * Returns the smallest integer greater than or equal to the given
 * integer that is not in the given set.
 *
 * @param s a pointer to a set, non-NULL
 * @param item an integer
 * @return the smallest integer greater than or equal to that integer that
 * is not in that set
 */
int isset_next_excluded(const isset *s, int item);

/**
 * Destroys the given set.
 *
 * @param s a pointer to a set, non-NULL
 */
void isset_destroy(isset *s);

#endif
