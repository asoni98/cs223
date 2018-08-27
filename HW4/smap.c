#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
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
  bool used; //is it in use
} entry;

struct smap
{
  int capacity;
  int size;
  int (*hash)(const char *);
  entry *table;
};

#define SMAP_INITIAL_CAPACITY 100

int smap_table_find_key(const entry *table, const char *key, int (*hash)(const char *), int size, int capacity);
void smap_embiggen(smap *m);

smap *smap_create(int (*h)(const char *s))
{
  smap *result = malloc(sizeof(smap));
  if (result != NULL)
    {
      result->size = 0;
      result->hash = h;
      result->table = calloc(SMAP_INITIAL_CAPACITY, sizeof(entry));
      result->capacity = (result->table != NULL ? SMAP_INITIAL_CAPACITY : 0);
    }
  return result;

}

int smap_size(const smap *m)
{
  return m->size;
}

int smap_compute_index(const char *key, int (*hash)(const char *), int cap) {
  return (hash(key) % cap + cap) % cap;
}

bool smap_table_add(entry *table, const char *key, int *value, int (*hash)(const char *), int cap) {
   int i = smap_compute_index(key, hash, cap);
   while (table[i].used) {
     i = (i + 1) % cap;
   }
   char *copy = malloc(strlen(key) + 1);
   strcpy(copy, key);

   if (copy != NULL) {
     table[i].key = copy;
     table[i].value = value;
     table[i].used = true;
     return true;
   }
   else {
     return false;
   }
 }

int smap_table_find_key(const entry *table, const char *key, int (*hash)(const char *), int size, int capacity)
{
  int i = smap_compute_index(key, hash, capacity);

  while (table[i].used && !(strcmp(table[i].key, key) == 0)) {
    i = (i + 1) % capacity;
  }
  return i;
}

bool smap_contains_key(const smap *m, const char *key)
{
  return m->table[smap_table_find_key(m->table, key, m->hash, m->size, m->capacity)].used;
}

int *smap_get(smap *m, const char *key) {
  int i = smap_table_find_key(m->table, key, m->hash, m->size, m->capacity);
  if (m->table[i].used) {return m->table[i].value;}
  else {return NULL;}
}

bool smap_put(smap *m, const char *key, int *value)
{
  if (m != NULL)
    {
      if (m->size + 1 > (m->capacity)/2)
	       {
	       smap_embiggen(m);
	       }

      int index = smap_table_find_key(m->table, key, m->hash, m->size, m->capacity);
      if (m->table[index].used)
	     {
       m->table[index].value = value;
	     return true;
	     }
      else
	     {
	        if (m->size == m->capacity)
	           {
	           return false;
	           }
	  char *copy = malloc(strlen(key) + 1);
	  strcpy(copy, key);
	  if (copy != NULL)
	    {
	      m->table[index].key = copy;
        m->table[index].value = value;
        m->table[index].used = true;
	      m->size++;
	      return true;
	    }
	  else
	    {
	      return false;
	    }

	}
    }
  else
    {
      // NULL matrix
      return false;
    }
}

/**
 * Adds the given key and value into the given table.
 *
 * @param table a hash table with >= 2 unused slots
 * @param key a string not present as a key in table, non-NULL
 * @param value a pointer to an integer, non-NULL
 * @param hash a hash function for strings
 * @param cap the number of slots in table
 */


void smap_embiggen(smap *m) {
  int largerCap = (m->capacity * 2 > SMAP_INITIAL_CAPACITY ? m->capacity * 2 : SMAP_INITIAL_CAPACITY);
  entry *table_copy = malloc(largerCap * sizeof(entry));
  entry *old_table = m->table;

  if (table_copy != NULL)
  {
    m->table = table_copy;
    m->capacity = largerCap;

    for (int i = 0; i < m->size; i++) {
      if (old_table[i].used) {
        smap_table_add(m->table, table_copy[i].key, table_copy[i].value, m->hash, m->capacity);
      }
    }
  }
  for (int i = 0; i < m->size; i++) {
    if (old_table[i].used) {
       free((old_table[i]).value);
       free((old_table[i]).key);
     }
  }
}

void smap_for_each(smap *m, void (*f)(const char *, int *))
{
  for (int i = 0; i < m->capacity; i++) {
    if (m->table[i].used) {
      f(m->table[i].key, m->table[i].value);
    }
  }
}

void smap_for_each_r(smap *m, void (*f)(const char *, int *, void *), void *arg)
{
  for (int i = 0; i < m->capacity; i++) {
    if (m->table[i].used) {
      f(m->table[i].key, m->table[i].value, arg); // Including extra arg argument
    }
  }
}

void smap_destroy(smap *m)
{
  for (int i = 0; i < m->capacity; i++)
  if (m->table[i].used) {
    free((m->table[i]).value);
    free((m->table[i]).key);
  }
}

  // free the table
  free(m->table);
  // free the smap struct
  free(m);
}
