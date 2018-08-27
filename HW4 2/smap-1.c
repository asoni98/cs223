#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
// Additional libraries
#include <stdio.h>

#include "smap.h"

/**
 * An implementation of a map of strings as an unsorted fixed-size array.
 * This implementation will not meet the requirements of Assignment #4 and
 * not completely tested itself but should be sufficient for testing your
 * Cooccurrence matrix.
 */

typedef struct
{
  char *key; // String hash
  int* value; // count
  bool occupied; // added
} entry;

struct smap
{
  int capacity;
  int size;
  int (*hash)(const char *);
  entry *table;
};

#define SMAP_INITIAL_CAPACITY 10000000

// Given functions
int smap_table_find_key(const entry *table, const char *key, int (*hash)(const char *), int size, int capacity);
void smap_embiggen(smap *m);

// Added functions
int smap_compute_index(const char *key, int (*hash)(const char *), int cap);
bool smap_table_add(entry *table, const char *key, int *value, int (*hash)(const char *), int cap);

/**
 * Creates an empty map that uses the given hash function.
 *
 * @param h a pointer to takes a string and returns its hash code
 * @return a pointer to the new map or NULL if it could not be created;
 * it is the caller's responsibility to destroy the map
 */
smap *smap_create(int (*h)(const char *s)) {
  // printf("SMAP_CREATE\n");
  smap *result = malloc(sizeof(smap));
  if (result != NULL) {
      result->size = 0;
      result->hash = h;
      result->table = malloc(sizeof(entry) * SMAP_INITIAL_CAPACITY);
      result->capacity = (result->table != NULL ? SMAP_INITIAL_CAPACITY : 0);

      // Initialize as false
      for (int i = 0; i < result->capacity; i++) { // Question: Does this slow the program too much?
        result->table[i].occupied = false;
      }
    }
  return result;
}

/**
 * Returns the number of (key, value) pairs in the given map.
 *
 * @param m a map, non-NULL
 * @return the size of m
 */
int smap_size(const smap *m) {
  // printf("SMAP_SIZE\n");
  return m->size;
}

/**
 * Returns the index where key is located in the given map, or the index
 * where it would go if it is not present.
 *
 * @param table a table with at least one free slot
 * @param key a string, non-NULL
 * @param hash a hash function for strings
 * @param size the size of the table
 * @param capacity the capacity of the table
 * @return the index of key in table, or the index of the empty slot to put it in if it is not present
 */
int smap_table_find_key(const entry *table, const char *key, int (*hash)(const char *), int size, int capacity) {
  // printf("SMAP_TABLE_FIND_KEY\n");

  int i = smap_compute_index(key, hash, capacity);

  /*
  printf("INDEX before WHILE 92: %d\n", i );
  printf("TABLE KEY: %s\n",table[i].key);
  printf("ARGUMENT KEY: %s\n",key);
  if (table[i].occupied)
  {
    printf("STRCOMP (not equal?) RESULT: %d\n", strcmp(table[i].key, key) == 0);
  }
  */

  while (table[i].occupied && !(strcmp(table[i].key, key) == 0)) { // Exits loop if not occupied
    i = (i + 1) % capacity;
  }
  // printf("INDEX after WHILE 96: %d\n", i );
  // printf("\n");
  return i;
}

int smap_compute_index(const char *key, int (*hash)(const char *), int cap) {
  // printf("SMAP_COMPUTE_INDEX\n");

  return (hash(key) % cap + cap) % cap;
}

/**
 * Adds a copy of the given key with value to this map.
 * If the key is already present then the old value is replaced.
 * The caller retains ownership of the value.
 *
 * @param m a map, non-NULL
 * @param key a string, non-NULL
 * @param value a pointer to an integer
 * @return true if the put was successful, false otherwise
 */
bool smap_put(smap *m, const char *key, int *value) {
  // printf("SMAP_PUT\n");

  int index = smap_table_find_key(m->table, key, m->hash, m->size, m->capacity);

  if(m->table[index].occupied) {
    // The key is already present
    m->table[index].value = value;
    return true; // Question: true if it already exists, right?
  }

  else {
    if (m->size + 1 > m->capacity / 2) {
      // grow
      smap_embiggen(m);
      //add to table
      if (m->size + 1 < m->capacity) {
        // Have to make this seperate call to account for rehashing
        return smap_table_add(m->table, key, value, m->hash, m->capacity);
        m->size++;
      }
    }

    else { // no need for size change
      // make a copy of the new key
      char *copy = malloc(strlen(key) + 1);
      strcpy(copy, key);

      if (copy != NULL) {
        m->table[index].key = copy;
        m->table[index].value = value;
        m->table[index].occupied = true;
        m->size++;
        return true;
      }
    }
  }
  return false; // malloc failed
}

/**
 * Adds the given key and value into the given table.
 *
 * @param table a hash table with >= 2 unoccupied slots
 * @param key a string not present as a key in table, non-NULL
 * @param value a pointer to an integer, non-NULL
 * @param hash a hash function for strings
 * @param cap the number of slots in table
 * @return true if the add was successful, false otherwise
 */
bool smap_table_add(entry *table, const char *key, int *value, int (*hash)(const char *), int cap) {
  //printf("SMAP_TABLE_ADD\n");

  int i = smap_compute_index(key, hash, cap);
  while (table[i].occupied) {
    i = (i + 1) % cap;
  }
  char *copy = malloc(strlen(key) + 1);
  strcpy(copy, key);

  if (copy != NULL) {
    table[i].key = copy;
    table[i].value = value;
    table[i].occupied = true;
    return true;
  }
  else {
    return false;
  }
}


// NEED TO CHECK OVER THIS
void smap_embiggen(smap *m) {
  // printf("SMAP_EMBIGGEN\n");

  // make a list of all the existing elements : QUESTION: HOW
  // go through that list and rehash all of them

  // TO-DO: 60 write and test embiggen for hash table

  int *old_count;
  int oldCap = m->capacity;
  int largerCap = (m->capacity * 2 > SMAP_INITIAL_CAPACITY ? m->capacity * 2 : SMAP_INITIAL_CAPACITY);
  smap *new_table = realloc(m->table, largerCap * sizeof(entry));

  if (new_table != NULL) {
    m->table = new_table;
    m->capacity = largerCap;

      // Rehash the variables
      for (int i = 0; i < oldCap; i++) { // Question: Does this slow the program too much?
        if (m->table[i].occupied) {
          *old_count = smap_get(m, m->table[i]);

          if (old_count == NULL) {
            // word was not present
            int *one = malloc(sizeof(int));
            *one = 1;

            if (!smap_put(m, m->table[i], one)) {
              free(one);
            }
          }
          else {
            // Word was already present
            *old_count = *old_count + 1;
            smap_put(m, m->table, old_count);
          }
        }
      }
    }
  return result;
}


  for (int i = 0; i < m->capacity; i++) {
    if (m->table[i].occupied) {

    }
  }

}

/**
 * Determines if the given key is present in this map.
 *
 * @param m a map, non-NULL
 * @param key a string, non-NULL
 * @return true if key is present in this map, false otherwise
 */
bool smap_contains_key(const smap *m, const char *key) {
  // printf("SMAP_CONTAINS_KEY\n");
  // printf("TABLE[119] OCCUPIED?: %d\n", m->table[119].occupied);

  return m->table[smap_table_find_key(m->table, key, m->hash, m->size, m->capacity)].occupied;
  // return m->table[119].occupied;
  // return smap_table_find_key(m->table, key, m->hash, m->size, m->capacity) < m->size;
}

/**
 * Returns the value associated with the given key in this map.
 * If the key is not present in this map then the returned value is
 * NULL.  The value returned is the original value passed to smap_put,
 * and it remains the responsibility of whatever called smap_put to
 * release the value (no ownership transfer results from smap_get).
 *

 * @param m a map, non-NULL
 * @param key a string, non-NULL
 * @return the assocated value, or NULL if they key is not present
 */
int *smap_get(smap *m, const char *key) {
  // printf("SMAP_GET\n");

  int index = smap_table_find_key(m->table, key, m->hash, m->size, m->capacity);
  if (m->table[index].occupied) {
    return m->table[index].value;
  }
  else {
    return NULL;
  }
}

/**
 * Calls the given function for each (key, value) pair in this map.
 *
 * @param m a map, non-NULL
 * @param f a pointer to a function that takes a key and a value, non-NULL
 */
void smap_for_each(smap *m, void (*f)(const char *, int *)) {
  // printf("SMAP_FOR_EACH\n");

  for (int i = 0; i < m->capacity; i++) {
    if (m->table[i].occupied) {
      f(m->table[i].key, m->table[i].value);
    }
  }
}

/**
 * Calls the given function for each (key, value) pair in this map, passing
 * the extra argument as well.
 *
 * @param m a map, non-NULL
 * @param f a pointer to a function that takes a key, a value, and an
 * extra piece of information, non-NULL
 * @param arg a pointer
 */
void smap_for_each_r(smap *m, void (*f)(const char *, int *, void *), void *arg) {
  // printf("SMAP_FOR_EACH_R\n");

  for (int i = 0; i < m->capacity; i++) {
    if (m->table[i].occupied) {
      f(m->table[i].key, m->table[i].value, arg); // Including extra arg argument
    }
  }
}

/**
 * Destroys the given map.
 *
 * @param m a map, non-NULL
 */
void smap_destroy(smap *m) {
  // printf("SMAP_DESTROY\n");

 // not our responsibility to free values

  // free the table
  free(m->table);

  // free the smap struct
  free(m);
}
