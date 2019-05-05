
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


//
// This is just a small file to get you started using NALib.
//
// This file should compile and run and print some version notes on the screen.
//

#include "NATree.h"
#include "NAHeap.h"
#include "NAHeap2.h"
#include "NARandom.h"

#define TESTSIZE    10000000
#define SUBTESTSIZE  100000.

int compare(const void* a, const void* b)
{
  if(*(double*)a > *(double*)b){return 1;}
  else if(*(double*)a < *(double*)b){return -1;}
  return 0;  
}

void testQSort(double* keys){
  NADateTime t1, t2;

  t1 = naMakeDateTimeNow();
  qsort(keys, TESTSIZE, sizeof(keys[0]), compare);
  t2 = naMakeDateTimeNow();
  printf("QSort: %f\n", naGetDateTimeDifference(&t2, &t1));

}



void testHeap(const double* keys){
  NAHeap heap;

  naInitHeap(&heap, -1, NA_HEAP_USES_DOUBLE_KEY);
  
  NADateTime t1 = naMakeDateTimeNow();
  for(int i=0; i<TESTSIZE; i++){
    naInsertHeapElementConst(&heap, &(keys[i]), &(keys[i]), NA_NULL);
  }

  NADateTime t2 = naMakeDateTimeNow();

  double prevkey = 0.;
  for(int i=0; i<TESTSIZE; i++){
    const double* curkey = naRemoveHeapRootConst(&heap);
    if(prevkey > *curkey){
      printf("Error in sorting");
    }
    prevkey = *curkey;
//    printf("%f\n", *curkey);
  }

  NADateTime t3 = naMakeDateTimeNow();

  printf("Heap Insert: %f\n", naGetDateTimeDifference(&t2, &t1));
  printf("Heap Iterate: %f\n", naGetDateTimeDifference(&t3, &t2));

  naClearHeap(&heap);
}


void testHeap2(const double* keys){
  NAHeap2 heap;

  naInitHeap2(&heap, -1, NA_HEAP_USES_DOUBLE_KEY);
  
  NADateTime t1 = naMakeDateTimeNow();
  for(int i=0; i<TESTSIZE; i++){
    naInsertHeap2ElementConst(&heap, &(keys[i]), &(keys[i]), NA_NULL);
  }

  NADateTime t2 = naMakeDateTimeNow();

  double prevkey = 0.;
  for(int i=0; i<TESTSIZE; i++){
    const double* curkey = naRemoveHeap2RootConst(&heap);
    if(prevkey > *curkey){
      printf("Error in sorting");
    }
    prevkey = *curkey;
//    printf("%f\n", *curkey);
  }

  NADateTime t3 = naMakeDateTimeNow();

  printf("Heap2 Insert: %f\n", naGetDateTimeDifference(&t2, &t1));
  printf("Heap2 Iterate: %f\n", naGetDateTimeDifference(&t3, &t2));

  naClearHeap2(&heap);
}



//void testTree(){
//  NATreeConfiguration* config = naCreateTreeConfiguration(NA_TREE_KEY_DOUBLE | NA_TREE_BALANCE_AVL);
//  NATree tree;
//  naInitTree(&tree, config); 
//  NADateTime t1, t2;
//  NATreeIterator iter;
//
//  t1 = naMakeDateTimeNow();
//  iter = naMakeTreeModifier(&tree);
//  for(int i=0; i<TESTSIZE; i++){
////    NAInt key = (NAInt)(naUniformRandZE() * TESTSIZE);
//    double key = naUniformRandZE();
////    double key = (int)(naUniformRandZE() * SUBTESTSIZE) / SUBTESTSIZE;
////    double key = (double)i / TESTSIZE;
////    printf("%f\n", key);
//    naAddTreeKeyConst(&iter, &key, NA_NULL, NA_TRUE);
//  }
//  naClearTreeIterator(&iter);
//  t2 = naMakeDateTimeNow();
//  printf("Tree Insert: %f\n", naGetDateTimeDifference(&t2, &t1));
//
//  t1 = naMakeDateTimeNow();
//  iter = naMakeTreeAccessor(&tree);
//  for(int i=0; i<TESTSIZE; i++){
////    NAInt key = (NAInt)(naUniformRandZE() * TESTSIZE);
////    double key = naUniformRandZE();
//    double key = (int)(naUniformRandZE() * SUBTESTSIZE) / SUBTESTSIZE;
////    double key = (double)i / TESTSIZE;
////    double key = (i % 10000) / 10000.;
////    printf("%f\n", key);
////    naAddTreeKeyConst(&iter, &key, NA_NULL, NA_TRUE);
//    naLocateTreeKey(&iter, &key, NA_FALSE);
//  }
//  naClearTreeIterator(&iter);
//  t2 = naMakeDateTimeNow();
//  printf("Tree Locate: %f\n", naGetDateTimeDifference(&t2, &t1));
//
////  NAInt prevkey = -1.;
//  double prevkey = -1.;
//  t1 = naMakeDateTimeNow();
//  iter = naMakeTreeModifier(&tree);
//  int i = 0;
//  while(naIterateTree(&iter)){
//    i++;
////    const NAInt* key = naGetTreeCurKey(&iter);
////    if(*key < prevkey){
////      printf("Wrong sorting: %d: %"NA_PRIi", %"NA_PRIi"\n", i, *key, prevkey);
////    }
////    prevkey = *key;
////    if(i < 5){printf("%"NA_PRIi"\n", *key);}
//
//    const double* key = naGetTreeCurKey(&iter);
//    if(*key < prevkey){
//      printf("Wrong sorting: %d: %f, %f\n", i, *key, prevkey);
//    }
//    prevkey = *key;
////    if(i < 5){printf("%f\n", *key);}
//  }
//  naClearTreeIterator(&iter);
//  t2 = naMakeDateTimeNow();
//  printf("Tree Iterate (%d): %f\n", i, naGetDateTimeDifference(&t2, &t1));
//
//  // ////////////////
//
////  naEmptyTree(&tree);
//  t1 = naMakeDateTimeNow();
//  iter = naMakeTreeModifier(&tree);
//  for(i=0; i<TESTSIZE; i++){
////    NAInt key = (NAInt)(naUniformRandZE() * TESTSIZE);
//    double key = (int)(naUniformRandZE() * SUBTESTSIZE) / SUBTESTSIZE;
////    double key = (double)i / TESTSIZE;
//    if(i%5 != 0){
//      naAddTreeKeyConst(&iter, &key, NA_NULL, NA_TRUE);
//    }else{
//      NABool found = naLocateTreeKey(&iter, &key, NA_FALSE);
//      if(found){naRemoveTreeCur(&iter, NA_FALSE);}
//    }
//  }
//  naClearTreeIterator(&iter);
//  t2 = naMakeDateTimeNow();
//  printf("Tree Insert/Remove: %f\n", naGetDateTimeDifference(&t2, &t1));
//
//  naClearTree(&tree);
//  naReleaseTreeConfiguration(config);
//}




#include "../NALib/NABase.h"
#include <stdio.h>

int main(void){
  printf("NALib Version: %d ", NA_VERSION);
  #ifndef NDEBUG
    printf("(Debug ");
  #else
    printf("(Release ");
  #endif
  printf("%d Bits Addresses, %d Bits Integers)\n", NA_SYSTEM_ADDRESS_BITS, NA_TYPE_NAINT_BITS);

  naStartRuntime();
    double* keys = naMalloc(TESTSIZE * sizeof(double*));
    for(int i=0; i<TESTSIZE; i++){
      keys[i] = naUniformRandZE();
//      keys[i] = TESTSIZE - i;
//      keys[i] = i;
    }
//    testTree();
    testHeap(keys);
    testHeap2(keys);
    testHeap(keys);
    testHeap2(keys);
//    testQSort(keys);
    naFree(keys);
  naStopRuntime();

  #if NA_OS == NA_OS_WINDOWS
    printf("Finished.\n");
    NA_UNUSED(getchar());
  #endif

  return 0;
}


// Copyright (c) NALib, Tobias Stamm
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

