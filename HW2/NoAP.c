#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
bool canItbeAdded(int nextNumber, int arr[], int size);
bool isArithmeticprogression(int a, int b, int c);
int maxElement(int list[], int size);
int greedy(int maxNumber, int arrayFill[], int sizeArr);
int backwards(int maxNumber, int arrayFill[], int sizeArr);
int skip(int maxNumber, int arrayFill[], int sizeArr, int first, int skip);
int cmpfunc (const void * a, const void * b);
void copyArray (int realArr[], int size, int tempArr[]);
void printArray (int realArr[], int size);
void printArray1 (bool realArr[], int size);
bool isNumeric(char str[]);
void onoffFun(int maxNumber, int insert, int array1[], bool onoff[], int arrayLen);
void opt(int maxNumber, int start, int partial[], bool onoff[], int partialLen);
void copyArrayBool (bool realArr[], int size, bool tempArr[]);
bool isvalueinarray(int val, int *arr, int size);
int optHeuristic(int start, bool onoff[], int size);

int *best;
int bestLen;
int minSize;

int main(int argc, char **argv) {

  int maxNumber;
  int first;
  int skipVar;
  int reqNumbersize = 0;
  int sizeArr;
  bool inStateSkip;
  int maxReqnumber;

  if (argc < 3) { //print nothing if there is only one number inputted (this helps prevent a few segment errors)
    return 0;
  }

  if (atoi(argv[1]) <= 0) { //check if is negative or 0
    printf("NoAP: n must not be negative; was %d\n", atoi(argv[1]));
    return 0;
  }

  maxNumber = atoi(argv[1]); //this is N
  int array[maxNumber]; //holds the array of required numbers
  int temp[maxNumber]; //temporary array for performing multiple actions (holds array[maxNumber] temporarily)

  bool onoff[maxNumber]; //for opt: list of on/off depending on available numbers that can be added
  int arrayGreedy[maxNumber]; //for the heuristic on opt - length of greedy on number is the lower bound
  int tempFortemp[maxNumber];

  best = (int *) malloc(maxNumber * sizeof(int));
  minSize = greedy(maxNumber, arrayGreedy, 0);

  for (int i = 2; i < argc; ++i) { //scans till it hits a nonnumber (i.e. -opt, -backward, -skip, or -greedy)
    if ((atoi(argv[i]) < 0) || (atoi(argv[i]) > (maxNumber - 1))) {
      printf("NoAP: integer out of range %d\n", atoi(argv[i]));
      return 0;
    }

    if (argv[i][0] == '-') { //double check if -something else
      reqNumbersize = (i-2);
      break;
    }

    if (isNumeric(argv[i]) == false) {
      printf("NoAP: required number must be a positive integer; was %s\n", argv[i]);
      return 0;
    }

    array[i - 2] = atoi(argv[i]);
  }


  bool alreadyProgression = false; //checks if the required numbers already form a progression
  for (int i = 0; i < reqNumbersize; i++) {
    if (canItbeAdded(array[i], temp, i)) {;}
    else {alreadyProgression = true;}
    temp[i] = array[i];
  }

int counterSkip = 0;
  for (int i = reqNumbersize + 2; i < argc; i++) {//matches opt, backward, skip, and greedy
    copyArray(array, maxNumber, temp); //temp = array
    if (strcmp(argv[i], "-greedy") == 0) {
      inStateSkip = false;
      counterSkip = 0;

      if (alreadyProgression) {printf("-greedy: 0 []\n"); continue;}
      sizeArr = greedy(maxNumber, temp, reqNumbersize);
      printf("-greedy: %d [", sizeArr);
      qsort(temp, sizeArr, sizeof(int), cmpfunc);
      printArray(temp, sizeArr);
      printf("\n");

    } else if (strcmp(argv[i], "-backward") == 0) {
      inStateSkip = false;
      counterSkip = 0;

      if (alreadyProgression) {printf("-backward: 0 []\n"); continue;}
      sizeArr = backwards(maxNumber, temp, reqNumbersize);
      printf("-backward: %d [", sizeArr);
      qsort(temp, sizeArr, sizeof(int), cmpfunc);
      printArray(temp, sizeArr);
      printf("\n");

    } else if (strcmp(argv[i], "-skip") == 0) {//error handling for skip
      inStateSkip = true;
      counterSkip = 0;

      if ((i + 3) > argc) {
        printf("NoAP: there are not enough arguments for skip\n");
        return 0;
      }

      if (isNumeric(argv[i+1]) == false)  {
        printf("NoAP: first must be a positive integer; was %s\n", argv[i+1]);
        return 0;
      }

      if (isNumeric(argv[i+2]) == false)  {
        printf("NoAP: step must be a positive integer; was %s\n", argv[i+2]);
        return 0;
      }

      first = atoi(argv[i + 1]);
      skipVar = atoi(argv[i + 2]);
      maxReqnumber = maxElement(temp, reqNumbersize);

      if ((first < (maxReqnumber + 1)) || first > (maxNumber - 1)) {
        printf("NoAP: invalid first %d\n", first);
        return 0;
      }

      if ((skipVar < 0.99) || skipVar > (maxNumber - maxReqnumber - 1)) {
        printf("NoAP: invalid step %d\n", skipVar);
        return 0;
      }

      if (alreadyProgression) {printf("-skip: 0 []\n"); continue;}

      sizeArr = skip(maxNumber, temp, reqNumbersize, first, skipVar);
      printf("-skip %d %d: %d [", first, skipVar, sizeArr);
      qsort(temp, sizeArr, sizeof(int), cmpfunc);
      printArray(temp, sizeArr);
      printf("\n");

    } else if (strcmp(argv[i], "-opt") == 0) {
      inStateSkip = false;
      if (alreadyProgression) {printf("-opt: 0 []\n"); continue;}

      maxReqnumber = maxElement(temp, reqNumbersize);
      copyArray(temp, maxNumber, tempFortemp);

      for (int i = 0; i < maxNumber; i++) {
        onoff[i] = 1;
      }

      if (maxReqnumber != -1) {   //if max required number is -1 do something else
        int list[maxReqnumber];
        for (int i = 0; i < reqNumbersize; i++) {
          onoffFun(maxNumber, tempFortemp[i], list, onoff, i);
          list[i] = tempFortemp[i];
        }
      }

      //asdf
      if (maxReqnumber == -1) {maxReqnumber = 0; reqNumbersize = 0;}
      opt(maxNumber, maxReqnumber, temp, onoff, reqNumbersize);
      printf("-opt: %d [", bestLen);
      printArray(best, bestLen);
      printf("\n");
    }

    else {//CHECKS ALL INBETWEEN ERRORS
      if (isNumeric(argv[i]) == false) {printf("NoAP: invalid input\n"); return 0;}
      if (inStateSkip == false) {printf("NoAP: invalid input\n"); return 0;}
      if ((inStateSkip == true) && (counterSkip > 1)) {printf("NoAP: invalid input\n"); return 0;}
      if ((inStateSkip == true) && (counterSkip <= 1)) {counterSkip++;}
    }
  }
  //freeing pointer
  free(best);
}

//implements opt search
void opt(int maxNumber, int start, int partial[], bool onoff[], int partialLen) {

  int count = 0;
  count = optHeuristic(start, onoff, maxNumber);
  bool endCondition = true;
  if (maxNumber > 15) {
    if ((count + partialLen) < (minSize)) {
      return;
    }
  }

  for (int i = start; i < maxNumber; i++) {

    if (onoff[i]) {
      endCondition = false;
      bool tempOnoff[maxNumber];
      int tempPartial[maxNumber];
      int tempPartialLen;
      copyArrayBool(onoff, maxNumber, tempOnoff);
      copyArray(partial, partialLen, tempPartial);
      tempPartialLen = partialLen;

      onoffFun(maxNumber, i, tempPartial, tempOnoff, partialLen);
      tempPartial[tempPartialLen] = i;
      tempPartialLen = (tempPartialLen + 1);
      opt(maxNumber, i+1, tempPartial, tempOnoff, tempPartialLen);
    }
  }
  if (endCondition == true) {
    if (partialLen > bestLen) {
      copyArray(partial, partialLen, best);
      bestLen = partialLen;
    }
    return;
  }
}

//creates a list of true false whether future numbers can be added based on an insertion into the list
void onoffFun(int maxNumber, int insert, int partial[], bool onoff[], int arrayLen) {
  int index;

  if (arrayLen == 0) {
    onoff[insert] = false;
    return;
  }
  for (int i = 0; i < arrayLen; i++) {
    if (insert <= partial[i]) {
      return; //should be an error
    }
    index = ((insert - partial[i]) + insert);
    onoff[partial[i]] = false;
    if (index >= maxNumber) {continue;}
    else {
      onoff[index] = false;
    }
  }
  onoff[insert] = false;
  return;
}

//optimizes opt to only look at cases above "count"
int optHeuristic(int start, bool onoff[], int size) {
  int count = 0;
  for (int i = start; i < size; i++) {
    if ((i + 2) <= size) {
      if ((onoff[i] == true) && (onoff[i+1] == true) && (onoff[i+2] == true)) {
        count = count + 2;
        i = i + 2;
      }
      else {if (onoff[i] == true) {count = count + 1;}}
    }
    else {if (onoff[i] == true) {count = count + 1;}}
  }
  return count;
}

//implements greedy search
int greedy(int maxNumber, int arrayFill[], int sizeArr) {
  int start = maxElement(arrayFill, sizeArr);
  start = start + 1;

  for (int i = start; i < maxNumber; i++) {
    if (canItbeAdded(i, arrayFill, sizeArr)) {
      arrayFill[sizeArr] = i;
      sizeArr = sizeArr + 1;
    }
  }

  return sizeArr;
}

//implements backward search
int backwards(int maxNumber, int arrayFill[], int sizeArr) {
  int start = maxElement(arrayFill, sizeArr);
  start = start + 1;

  for (int i = maxNumber-1; i > start - 1; i--) {
    if (canItbeAdded(i, arrayFill, sizeArr)) {
      arrayFill[sizeArr] = i;
      sizeArr = sizeArr + 1;
    }
  }

  return sizeArr;
}

//implements skip search
int skip(int maxNumber, int arrayFill[], int sizeArr, int first, int skip) {
  int start = maxElement(arrayFill, sizeArr);
  start = start + 1;
  int maxLoops = maxNumber - start + 2;

  for (int i = first; maxLoops > 0; i = i + skip) {
    if (i >= maxNumber) {
      i = ((i%maxNumber) + start);
    }
    maxLoops = maxLoops - 1;

    if (canItbeAdded(i, arrayFill, sizeArr)) {
      arrayFill[sizeArr] = i;
      sizeArr = sizeArr + 1;
    }
  }

  return sizeArr;
}

bool canItbeAdded(int nextNumber, int arr[], int size) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (isArithmeticprogression(arr[i], nextNumber, arr[j])) {
        return false;
        break;
      }
    }
  }

  return true;
}

bool isArithmeticprogression(int a, int b, int c){
/* SIMPLE SORTING FUNCTION BETWEEN THREE NUMBERS*/
  if (a > b){
    int flip = a;
    a = b;
    b = flip;
  }
  if (a > c){
    int flip = a;
    a =c ;
    c = flip;
  }

  if (b > c ){
    int flip = b;
    b = c;
    c = flip;}

    if ((a-b) == (b-c)) {
      return true;
    }

    else {
      return false;
    }
  }

  //copies an integer array
  void copyArray (int realArr[], int size, int tempArr[]) {
    for (int i = 0; i < size; i++) {
      tempArr[i] = realArr[i];
    }
  }

  //copies a boolean array
  void copyArrayBool (bool realArr[], int size, bool tempArr[]) {
    for (int i = 0; i < size; i++) {
      tempArr[i] = realArr[i];
    }
  }

  //prints integer arrays
  void printArray (int realArr[], int size) {
    for (int i = 0; i < size; i++) {
      if ((i + 1) == size) {printf("%d]", realArr[i]); break;}
      printf("%d, ", realArr[i]);
    }
  }

  //prints boolean arrays
  void printArray1 (bool realArr[], int size) {
    for (int i = 0; i < size; i++) {
      if ((i + 1) == size) {printf("%d]", realArr[i]); break;}
      printf("%d, ", realArr[i]);
    }
  }

  //maxElement calculates the maximum element in an array. If the array is empty it outputs -1.
  //Cite: StackOverflow
  int maxElement(int list[], int size) {
    int max = -1;
    for (int i = 0; i < size; i++) {
      if (max == -1) {max = list[i];}
      if (list[i] > max) {max = list[i];}
    }
    return max;
  }

  //checks if a string/character is an integer
  bool isNumeric(char str[]) {
    int stringLength = strlen(str);
    for (int i = 0; i < stringLength; ++i) {
      if (!(isdigit(str[i]))) { // If it is not a digit
        return false;
      }
    }
    return true;
  }

  //Compare function for the qsort library function
  int cmpfunc (const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
  }
