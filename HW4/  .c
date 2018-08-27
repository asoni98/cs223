/**
 * Counts words that appear on the command line (up to "-contains") and checks whether
 * words appeared (after "-contains").
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "smap.h"

// frees the value
void free_value(const char *key, int *value);

// prints a (key, value) pair
void print_entry(const char *key, int *value);

// prints a (key, value) pair if the key starts with 'C'
void print_if_c(const char *key, int *value, void *arg);

// a hash function for strings
int really_bad_string_hash(const char *key);

int main(int argc, char **argv)
{
// smap *counts = smap_create(really_bad_string_hash);
// int var1 = 1;
// int var2 = 2;
// char *string = "abc";
// char *string1 = "abd";
// smap_put(counts, string, &var1);
// smap_put(counts, string1, &var2);
// printf("=== Counts ===\n");
// smap_for_each(counts, print_entry);
// printf("SIZE: %d", smap_size(counts));
//
// bool a = smap_contains_key(counts, "abc");
// bool b = smap_contains_key(counts, "abe");
// printf("abc true: %d, abc false: %d\n", a, b);
//
// int *varHold = smap_get(counts, "abd");
// printf("contains abd: %d\n", *varHold);
// int *varHold1 = smap_get(counts, "abq");
// printf("contains abq: %d\n", *varHold1);



//*smap_get(smap *m, const char *key)

  smap *counts = smap_create(really_bad_string_hash);
  if (counts == NULL)
    {
      return 1;
    }

  int a = 1;
  while (a < argc && strcmp(argv[a], "-contains") != 0)
    {
      int *old_count = smap_get(counts, argv[a]);
      if (old_count == NULL)
	{
	  // word was not present
	  int *one = malloc(sizeof(int));
	  *one = 1;

	  if (!smap_put(counts, argv[a], one))
	    {
	      free(one);
	    }
	}
      else
	{
	  // word was already present
	  *old_count = *old_count + 1;
	  smap_put(counts, argv[a], old_count);
	}
      a++;
    }

  printf("=== Counts ===\n");
  smap_for_each(counts, print_entry);

  printf("=== Counts of the words that begin with 'C' ===\n");
  char *start = "C"; // a string, but print_if_c only looks at the first character
  smap_for_each_r(counts, print_if_c, start);

  printf("=== Contains ===\n");
  // advance over "-contains"
  a++;

  while (a < argc)
    {
      printf("%s %s\n", argv[a], smap_contains_key(counts, argv[a]) ? "true" : "false");
      a++;
    }

  // free all the dynamically allocated ints we put in the map as values
  smap_for_each(counts, free_value);

  // destroy the map
  smap_destroy(counts);

  return 0;
}

void free_value(const char *key, int *value)
{
  free(value);
}

void print_entry(const char *key, int *value)
{
  if (key != NULL && value != NULL)
    {
      printf("%s = %d\n", key, *value);
    }
}

void print_if_c(const char *key, int *value, void *arg)
{
  if (key != NULL && value != NULL && arg != NULL && *key == *((char *)arg))
    {
      printf("%s = %d\n", key, *value);
    }
}

int really_bad_string_hash(const char *key)
{
  if (key == NULL)
    {
      return 0;
    }
  else
    {
      // key hashes to ASCII value of first character
      // (this does *not* achieve the goal of spreading all possible keys
      // uniformly and pseudo-randomly across the range of an int)
      // but the current smap implementation does't even use hashing, so whatevs
      return *key;
    }
}
