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

#define SMAP_INITIAL_CAPACITY 2

// Given functions
int smap_table_find_key(const entry *table, const char *key, int (*hash)(const char *), int size, int capacity);
void smap_embiggen(smap *m);

// Added functions
int smap_compute_index(const char *key, int (*hash)(const char *), int cap);
bool smap_table_add(entry *table, const char *key, int *value, int (*hash)(const char *), int cap);

smap *smap_create(int (*h)(const char *s)) {
  // printf("SMAP_CREATE\n");
  smap *result = malloc(sizeof(smap));
  if (result != NULL) {
      result->size = 0;
      result->hash = h;
      result->table = malloc(sizeof(entry) * SMAP_INITIAL_CAPACITY);
      result->capacity = (result->table != NULL ? SMAP_INITIAL_CAPACITY : 0);

      // Initialize as false
      // QUESTION: DO WE HAVE TO DO THIS?
      for (int i = 0; i < result->capacity; i++) { // Question: Does this slow the program too much?
        result->table[i].occupied = false;
      }
    }
  return result;
}

int smap_size(const smap *m) {
  // printf("SMAP_SIZE\n");
  return m->size;
}

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
        // Have to make this separate call to account for rehashing
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


//typedef struct
//{
//  char *key; // String hash
//  int* value; // count
//  bool occupied; // added
//} entry;

// NEED TO CHECK OVER THIS
void smap_embiggen(smap *m) {
  printf("SMAP_EMBIGGEN\n");

  // make a list of all the existing elements : QUESTION: HOW
  // go through that list and rehash all of them

  // TO-DO: 60 write and test embiggen for hash table

  int oldCap = m->capacity;
  int largerCap = (m->capacity * 2 > SMAP_INITIAL_CAPACITY ? m->capacity * 2 : SMAP_INITIAL_CAPACITY);
  entry *new_table = malloc(largerCap * sizeof(entry)); // Seperate location
  entry *old_table = m->table; // Pointer to the old table

  if (new_table != NULL) {
    m->table = new_table; // Make the table point to the new table
    m->capacity = largerCap; // Set the new capacity

    // Rehash the variables
    for (int i = 0; i < oldCap; i++) { // Question: Does this slow the program too much?
      if (old_table[i].occupied) {

        // rehash
        smap_table_add(m->table, old_table[i].key, old_table[i].value, m->hash, m->capacity);
      }
    }
  }

  // free old table
  free(old_table);
}


bool smap_contains_key(const smap *m, const char *key) {
  // printf("SMAP_CONTAINS_KEY\n");
  // printf("TABLE[119] OCCUPIED?: %d\n", m->table[119].occupied);

  return m->table[smap_table_find_key(m->table, key, m->hash, m->size, m->capacity)].occupied;
  // return m->table[119].occupied;
  // return smap_table_find_key(m->table, key, m->hash, m->size, m->capacity) < m->size;
}

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

void smap_for_each(smap *m, void (*f)(const char *, int *)) {
  // printf("SMAP_FOR_EACH\n");

  for (int i = 0; i < m->capacity; i++) {
    if (m->table[i].occupied) {
      f(m->table[i].key, m->table[i].value);
    }
  }
}


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
