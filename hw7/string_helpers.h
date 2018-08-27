#ifndef __STRING_HELPERS_H__
#define __STRING_HELPERS_H__

/**
 * Frees all the strings in the given array.
 *
 * @param arr an array of strings
 * @param n the size of that array
 */
void string_array_free(char **arr, int n);

/**
 * Returns the hash value of the given string.
 * 
 * @param s a string
 * @return the hash value of that string, a nonnegative integer
 */
int string_hash_java(const char *s);

/**
 * Prints the strings in the given array, one per line.
 *
 * @param arr an array of strings, non-NULL
 * @param n the size of that array
 */
void string_array_fprintf(FILE *fp, char **arr, int n);

#endif
