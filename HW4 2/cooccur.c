#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smap.h"
#include "cooccur.h"
#include <string.h>

struct cooccurrence_matrix
{
  int numWords;
  smap *keywords;
  smap *words;
};

int hash_function(const char* str) {
  int hash = 5381;
  int c;
  while ((c = *str++) != 0)
  hash = ((hash << 5) + hash) + c;
  return hash;
}

/**
 * Creates a cooccurrence matrix that counts cooccurrences of the given keywords
 * and is initialized to 0 for all entries.
 *
 * @param key an array of distinct non-NULL strings, non-NULL
 * @param n the size of that array
 * @return a pointer to a new cooccurrence matrix; the caller is responsible for
 * destroying the matrix
 */
cooccurrence_matrix *cooccur_create(char *key[], int n) {
  cooccurrence_matrix *result = malloc(sizeof(cooccurrence_matrix));
  if (result != NULL)
    {
      result->numWords = n;
      result->keywords = smap_create(hash_function); //change hashing function
      result->words = smap_create(hash_function);

      for (int i = 0; i < n; i++) {
        int *j = malloc(sizeof(int));
        //if number not equal to null
        *j = i;
        smap_put(result->keywords, key[i], j);
      }

      for (int i = 0; i < n; i++) {
        int *z = calloc(n,sizeof(int));
        smap_put(result->words, key[i], z);
      }

      //  int *a = smap_get(result->words, key[n-1]);
        // printf("n %d\n", n);
        // printf("a1 %d\n", a[n-1]);
        // printf("a2 %d\n", ++a[n-1]);

    }
  return result;
}

/**
 * Updates the given cooccurrence matrix by incrementing the counts for
 * each pair of keywords in the given context.
 *
 * @param mat a pointer to a cooccurrence matrix, non-NULL
 * @param context an array of distinct non-NULL strings that are keywords
 * for that matrix, non-NULL
 * @param n the size of that array
 */


void cooccur_update(cooccurrence_matrix *mat, char **context, int n) {//FIX UPDATE WITH AUSTINS
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      // smap *counts = smap_create(really_bad_string_hash);
      // int *varHold = smap_get(counts, "abd");

      int *word_index = smap_get(mat->keywords, context[i]);
      int *other_word_index = smap_get(mat->keywords, context[j]);
      if (word_index != NULL && other_word_index != NULL) {

        // printf("context[i]: %s, %d\n", context[i], *word_index);
        // printf("context[j]: %s, %d\n", context[j], *other_word_index);
        // printf("other_word_index: %d\n",  *other_word_index);
        int *array_to_change = smap_get(mat->words, context[i]);
        int b = *other_word_index;
        //array_to_change[b];
        printf("i: %d j: %d B: %d\n",*word_index, *other_word_index,  b);
        array_to_change[b]++;
        printf("%s", context[i]);
        printf("other_word_index: %d \n",*other_word_index );
        // if (*other_word_index == 1) {printf("WTF\n" );}
        // if (context[0] == "b") {printf("WTF1\n" );}
        //array_to_change[*other_word_index]++;

        // *(index + smap_get(mat->words, context[j]))++;
        // mat->words[*word_index][*other_word_index]++;
      }
      //mat->words[*other_word_index][*word_index]++;
    }
  }
}

/**
 * Reads keywords from the given matrix from the current line of the given stream.
 * A keyword is considered to have been read if it appears on the stream before
 * the next newline surrounded by whitespace, the beginning of input (as seen by
 * this function), or EOF.  The newline is read and removed from the stream.
 *
 * @param mat a pointer to a cooccurrence matrix, non-NULL
 * @param stream a stream, non-NULL
 * @param n a pointer to an int where the size of the returned array will be written
 * @return an array of unique non-NULL strings containing all the keywords read;
 * the caller is responsible for deallocating the array and the strings it contains
 */
 char **cooccur_read_context(cooccurrence_matrix *mat, FILE *stream, int *n) {
   int bytes_read;
   size_t nbytes = 509;
   char *line = NULL;
   int sizeWordsReadIn;

  // line = (char *)malloc(nbytes+1);
   bytes_read =  getline(&line, &nbytes, stream); //read into the line at this point.
   char **array = malloc(sizeof(char *) * 509);
   if (bytes_read!=-1)
   {
   	printf("stream value: %s\n", line);

     char *p = strtok (line, " \n\t");
     int i = 0;
     while (p != NULL)
     {
       printf("%s\n", p);
       array[i++] = p;
       p = strtok (NULL, " \n\t");
     }
     sizeWordsReadIn = i;
   }
   else {
     return NULL;
   }
  //  for (int i = 0; i < 2; i++) {
  //    printf("read words: %s,", array[i]);
  //  }

   smap *keywordsNew = smap_create(hash_function);
   char **holdKeys = malloc(sizeof(char *) * sizeWordsReadIn);
   int count = 0;
   for (int i = 0; i < sizeWordsReadIn; i++) {
     int *j = malloc(sizeof(int));
     //if number not equal to null
     *j = 1;
     if (smap_contains_key(mat->keywords, array[i])) {
       if (smap_get(keywordsNew, array[i]) == NULL) {
         smap_put(keywordsNew, array[i], j);
         holdKeys[count] = array[i];
         count++;
       }
       //smap_put(keywordsNew, array[i], j);
     }}
     //holdKeys[count+1] = NULL;
    //  printf("words:\n");
    //   for (int i = 0; i < count; i++) {
    //     printf("%s ", holdKeys[i]);
    //   }
    //   printf("%d\n",count );
    *n = count;
    smap_destroy(keywordsNew);
    return holdKeys;
 }


 /**
  * Returns the vector (row) for the given word in the given matrix.
  * Values in the returned array correspond to the keywords for the
  * given matrix in the order they were given in the array passed to
  * the call to cooccur_create that created the given matrix.  Each
  * value is the proportion of times that the given keyword appeared in
  * a call to cooccur_update for the given matrix that also contained
  * each other keyword.  If the given word is not a keyword for the
  * given matrix or the keyword has never appeared in a call to
  * cooccur_update for the given matrix, then the returned array
  * contains 0.0 in every entry.
  *
  * @param mat a pointer to a cooccurrence matrix, non-NULL
  * @param word a string, non-NULL
  * @return an array of doubles; it is the caller's responsibility to deallocate that array
  */
 double *cooccur_get_vector(cooccurrence_matrix *mat, const char *word) {
   int *word_index = smap_get(mat->keywords, word);
  //  printf("word: %s ", word);
   int *array_to_change = smap_get(mat->words, word);
   int maxNum = array_to_change[*word_index];
  //  printf("wordIndex: %d ", *word_index);
  //  printf("maxNum: %d\n", maxNum);
   //
  //  for (int i = 0; i < mat->numWords; i++) {
  //    printf("Vector. i: %d num: %d\n", i, array_to_change[i]);
  //  }
  //  int maxNum = 1;
   if (maxNum == 0) {maxNum = 1;}
   double *array_new = malloc(sizeof(double) * mat->numWords);

   for (int i = 0; i < mat->numWords; i++) {
     //if word is 0 then make it 0.0000
     array_new[i] = (double) array_to_change[i]/maxNum;
   }

   return array_new;
 }


 /**
  * Destroys the given matrix.
  *
  * @param mat a pointer to a cooccurrence matrix, non-NULL
  */
void cooccur_destroy(cooccurrence_matrix *mat) {
  smap_destroy(mat->keywords);
  smap_destroy(mat->words);
  free(mat);
}

int main(int argc, char **argv)
{
  if (argc < 2) {
    return 1;
  }

  char **keywords = malloc((sizeof(char *) * argc) + 1);
  int size = 0;

  for (int i = 1; i < argc; i++) {
    keywords[i-1] = argv[i];
    //printf("Key word: %s\n", keywords[i-1]);
    size++;
  }

  cooccurrence_matrix *words = cooccur_create(keywords, size);
  // int *arrayLast = smap_get(words->words, keywords[size-1]);
  // printf("array LAST:\n");
  // for (int i = 0; i < size; i++) {
  //   printf("%d, ", *(arrayLast+i));
  // }
  //
  // printf("%d\n", words->numWords);

  int numKeyWords;
  int c;
  while ((c = getchar()) != EOF) {
    ungetc(c, stdin);
    char **context = cooccur_read_context(words, stdin, &numKeyWords);
    cooccur_update(words, context, numKeyWords);
    free(context);
  }


  for (int i = 0; i < words->numWords; i++) {
    printf("%s: [", keywords[i]);
     double *wordPercentages = cooccur_get_vector(words, keywords[i]);
    //int *wordPercentages = smap_get(words->words, keywords[i]);
    for (int j = 0; j < words->numWords; j++) {
      printf("%lf", wordPercentages[j]);
      if (j != (words->numWords-1)) {printf(", ");}
    }
    printf("]\n");
  }

  cooccur_destroy(words);
  free(keywords);
}
