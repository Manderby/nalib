
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NAArray.h
// Do not include this file directly! It will automatically be included when
// including "NAArray.h"


#include "../NACoord.h"


struct NAArray{
  NAPtr   ptr;         // The pointer storing the data
  NAInt   count;       // The number of elements.
  NAInt   typesize;    // The size in bytes of the stored type
  NAMutator destructor;
  #ifndef NDEBUG
    NAInt    itercount;   // The number of iterators attached to this array.
  #endif
};



NA_IDEF NAArray* naInitArray(NAArray* array){
  #ifndef NDEBUG
    if(!array)
      naCrash("array is Null-Pointer");
  #endif
  array->ptr = naMakePtrWithDataConst(NA_NULL);
  // The typesize is set to 1 such that calls to naGetArrayCount will not
  // result in bad values.
  array->count = 0;
  array->typesize = 1;
  array->destructor = NA_NULL;
  #ifndef NDEBUG
    array->itercount = 0;
  #endif
  return array;
}



NA_IDEF NAArray* naInitArrayWithCount(NAArray* array, NAInt typesize, NAInt count){
  #ifndef NDEBUG
    if(!array)
      naCrash("array is Null-Pointer");
    if(typesize < 1)
      naError("typesize should not be < 1.");
    if(count < 0)
      naError("count should not be < 0.");
  #endif
  array->typesize = typesize;
  array->count = count;
  array->destructor = (NAMutator)naFree;
  array->ptr = naMakePtrWithDataMutable(naMalloc(count * typesize));
  #ifndef NDEBUG
    array->itercount = 0;
  #endif

  return array;
}



NA_IDEF NAArray* naInitArrayWithDataConst(NAArray* array, const void* data, NAInt typesize, NAInt count){
  #ifndef NDEBUG
    if(!array)
      naCrash("array is Null-Pointer");
    if(typesize < 1)
      naError("typesize should not be < 1");
  #endif
  array->typesize = typesize;
  array->count = count;
  array->destructor = NA_NULL;
  array->ptr = naMakePtrWithDataConst(data);
  #ifndef NDEBUG
    array->itercount = 0;
  #endif

  return array;
}



NA_IDEF NAArray* naInitArrayWithDataMutable(NAArray* array, void* data, NAInt typesize, NAInt count, NAMutator destructor){
  #ifndef NDEBUG
    if(!array)
      naCrash("array is Null-Pointer");
    if(typesize < 1)
      naError("typesize should not be < 1");
  #endif
  array->typesize = typesize;
  array->count = count;
  array->destructor = destructor;
  array->ptr = naMakePtrWithDataMutable(data);
  #ifndef NDEBUG
    array->itercount = 0;
  #endif

  return array;
}



NA_IDEF void naClearArray(NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("array is Null-Pointer.");
    }
    if(array->itercount)
      naError("There are still some iterators operating upon this array. Did you use naClearArrayIterator?");
  #endif
  naCleanupPtr(&(array->ptr), array->destructor);
}



NA_IDEF void naForeachArrayConst(NAArray* array, NAAccessor accessor){
  NAInt count;
  const NAByte* ptr;

  #ifndef NDEBUG
    if(!accessor)
      naCrash("Accessor is Null");
  #endif
  count = naGetArrayCount(array);
  ptr = naGetPtrConst(array->ptr);
  while(count){
    accessor(ptr);
    ptr += array->typesize;
    count--;
  }
}



NA_IDEF void naForeachArrayMutable(NAArray* array, NAMutator mutator){
  NAInt count;
  NAByte* ptr;

  #ifndef NDEBUG
    if(!mutator)
      naCrash("Mutator is Null");
  #endif
  count = naGetArrayCount(array);
  ptr = naGetPtrMutable(array->ptr);
  while(count){
    mutator(ptr);
    ptr += array->typesize;
    count--;
  }
}



NA_IDEF void naForeachArraypConst(NAArray* array, NAAccessor accessor){
  NAInt count;
  const NAByte* ptr;

  #ifndef NDEBUG
    if(!accessor)
      naCrash("Accessor is Null");
  #endif
  count = naGetArrayCount(array);
  ptr = naGetPtrConst(array->ptr);
  while(count){
    accessor(*((const void**)ptr));
    ptr += array->typesize;
    count--;
  }
}



NA_IDEF void naForeachArraypMutable(NAArray* array, NAMutator mutator){
  NAInt count;
  NAByte* ptr;

  #ifndef NDEBUG
    if(!mutator)
      naCrash("Mutator is Null");
  #endif
  count = naGetArrayCount(array);
  ptr = naGetPtrMutable(array->ptr);
  while(count){
    mutator(*((void**)ptr));
    ptr += array->typesize;
    count--;
  }
}



NA_IDEF const void* naGetArrayPointerConst(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("array is Null-Pointer.");
      return NA_NULL;
    }else{
      if(naIsArrayEmpty(array))
        naError("array is empty, returned pointer is NULL");
    }
  #endif
  return naGetPtrConst(array->ptr);
}



NA_IDEF void* naGetArrayPointerMutable(NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("array is Null-Pointer.");
      return NA_NULL;
    }else{
      if(naIsArrayEmpty(array))
        naError("array is empty, returned pointer is NULL");
    }
  #endif
  return naGetPtrMutable(array->ptr);
}



NA_HIDEF NAInt naMakeIndexAbsolute(NAInt indx, NAInt length){
  #ifndef NDEBUG
    if(length < 0)
      naError("length should not be negative");
  #endif
  if(indx < 0){
    indx += length;
    #ifndef NDEBUG
      if(indx < 0)
        naError("positive index is not positive");
      if(indx >= length)
        naError("positive index overflows length");
    #endif
  }
  return indx;
}



NA_IDEF const void* naGetArrayElementConst(const NAArray* array, NAInt indx){
  #ifndef NDEBUG
    if(!array){
      naCrash("array is Null-Pointer.");
      return NA_NULL;
    }else{
      if(naIsArrayEmpty(array))
        naError("array is empty");
    }
  #endif
  indx = naMakeIndexAbsolute(indx, naGetArrayCount(array));
  #ifndef NDEBUG
    if(indx >= naGetArrayCount(array))
      naError("array overflow.");
  #endif
  return &(((NAByte*)naGetPtrConst(array->ptr))[indx * array->typesize]);
}



NA_IDEF void* naGetArrayElementMutable(NAArray* array, NAInt indx){
  #ifndef NDEBUG
    if(!array){
      naCrash("array is Null-Pointer.");
      return NA_NULL;
    }else{
      if(naIsArrayEmpty(array))
        naError("array is empty");
    }
  #endif
  indx = naMakeIndexAbsolute(indx, naGetArrayCount(array));
  #ifndef NDEBUG
    if(indx >= naGetArrayCount(array))
      naError("array overflow.");
  #endif
  return &(((NAByte*)naGetPtrMutable(array->ptr))[indx * array->typesize]);
}



NA_IDEF const void* naGetArrayElementpConst(const NAArray* array, NAInt indx){
  #ifndef NDEBUG
    if(!array){
      naCrash("array is Null-Pointer.");
      return NA_NULL;
    }else{
      if(naIsArrayEmpty(array))
        naError("array is empty");
    }
  #endif
  indx = naMakeIndexAbsolute(indx, naGetArrayCount(array));
  #ifndef NDEBUG
    if(indx >= naGetArrayCount(array))
      naError("array overflow.");
  #endif
  return *((const void**)&(((NAByte*)naGetPtrConst(array->ptr))[indx * array->typesize]));
}



NA_IDEF void* naGetArrayElementpMutable(NAArray* array, NAInt indx){
  #ifndef NDEBUG
    if(!array){
      naCrash("array is Null-Pointer.");
      return NA_NULL;
    }else{
      if(naIsArrayEmpty(array))
        naError("array is empty");
    }
  #endif
  indx = naMakeIndexAbsolute(indx, naGetArrayCount(array));
  #ifndef NDEBUG
    if(indx >= naGetArrayCount(array))
      naError("array overflow.");
  #endif
  return *((void**)&(((NAByte*)naGetPtrMutable(array->ptr))[indx * array->typesize]));
}



NA_IDEF NAInt naGetArrayCount(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("array is Null-Pointer.");
      return 0;
    }
    if((NAInt)(array->count) < 0)
      naError("Array count looks like it was negative when interpreted as signed integer. Do not create Arrays with negative sizes!");
  #endif
  // Note that an empty array has a typesize of 1.
  return array->count;
}



NA_IDEF NAInt naGetArrayMaxIndex(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("array is Null-Pointer.");
      return 0;
    }
    if(naIsArrayEmpty(array))
      naError("array is empty.");
  #endif
  return naMakeMaxWithEndi(naGetArrayCount(array));
}



NA_IDEF NAInt naGetArrayTypesize(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("array is Null-Pointer.");
      return 0;
    }
  #endif
  return array->typesize;
}



NA_IDEF NABool naIsArrayEmpty(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("array is Null-Pointer.");
      return NA_TRUE;
    }
  #endif
  return (array->count == 0);
}






// /////////////////////////////////////////
// Array Iterator
// /////////////////////////////////////////


struct NAArrayIterator{
  NAPtr array;
  NAInt indx;
  #ifndef NDEBUG
    NABool mutator;
  #endif
};



NA_IDEF NAArrayIterator naMakeArrayAccessor(const NAArray* array){
  NAArrayIterator iter;
  #ifndef NDEBUG
    NAArray* mutablearray;
    if(!array)
      naCrash("array is Null pointer");
    mutablearray = (NAArray*)array;
    mutablearray->itercount++;
    iter.mutator = NA_FALSE;
  #endif
  iter.array = naMakePtrWithDataConst(array);
  iter.indx = -1;

  return iter;
}



NA_IDEF NAArrayIterator naMakeArrayMutator(NAArray* array){
  NAArrayIterator iter;
  #ifndef NDEBUG
    NAArray* mutablearray;
    if(!array)
      naCrash("array is Null pointer");
    mutablearray = (NAArray*)array;
    mutablearray->itercount++;
    iter.mutator = NA_TRUE;
  #endif
  iter.array = naMakePtrWithDataMutable(array);
  iter.indx = -1;

  return iter;
}



NA_IDEF void naClearArrayIterator(NAArrayIterator* iterator){
  #ifndef NDEBUG
    NAArray* mutablearray = (NAArray*)naGetPtrConst(iterator->array);
    if(mutablearray->itercount == 0)
      naError("Too many clears. Iterator count is already zero.");
    mutablearray->itercount--;
  #else
    NA_UNUSED(iterator);
  #endif
}



NA_IDEF NABool naLocateArrayFirst(NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(naIsArrayEmpty(naGetPtrConst(iterator->array)))
      naError("Array is empty");
  #endif
  iterator->indx = 0;
  return NA_TRUE;
}



NA_IDEF NABool naLocateArrayLast(NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(naIsArrayEmpty(naGetPtrConst(iterator->array)))
      naError("Array is empty");
  #endif
  iterator->indx = naGetArrayMaxIndex(naGetPtrConst(iterator->array));
  return NA_TRUE;
}



NA_IDEF NABool naLocateArrayData(NAArrayIterator* iterator, const void* data){
  const NAByte* ptr = naGetArrayPointerConst(naGetPtrConst(iterator->array));
  NAInt count = naGetArrayCount(naGetPtrConst(iterator->array));
  NAInt indx = 0;
  while(count){
    if(ptr == data){
      iterator->indx = indx;
      return NA_TRUE;
    }
    ptr += naGetArrayTypesize(naGetPtrConst(iterator->array));
    count--;
    indx++;
  }
  return NA_FALSE;
}



NA_IDEF NABool naLocateArrayIndex(NAArrayIterator* iterator, NAInt indx){
  #ifndef NDEBUG
    if(naIsArrayEmpty(naGetPtrConst(iterator->array)))
      naError("Array is empty");
  #endif
  indx = naMakeIndexAbsolute(indx, naGetArrayCount(naGetPtrConst(iterator->array)));
  iterator->indx = indx;
  if(indx < 0){
    iterator->indx = -1;
    return NA_FALSE;
  }else if(indx >= naGetArrayCount(naGetPtrConst(iterator->array))){
    iterator->indx = -1;
    return NA_FALSE;
  }
  return NA_TRUE;
}



NA_IDEF NABool naIterateArray(NAArrayIterator* iterator, NAInt step){
  if(naIsArrayAtInitial(iterator) && step < 0){iterator->indx += naGetArrayCount(naGetPtrConst(iterator->array));}
  iterator->indx += step;
  if(iterator->indx == naGetArrayCount(naGetPtrConst(iterator->array))){iterator->indx = -1;}
  #ifndef NDEBUG
    if(iterator->indx < -1)
      naError("Iterator underflows");
    if(iterator->indx >= naGetArrayCount(naGetPtrConst(iterator->array)))
      naError("Iterator overflows");
  #endif
  return (iterator->indx != -1);
}



NA_IDEF const void* naGetArrayPrevConst(const NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("iterator is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  if(iterator->indx == 0){
    return NA_NULL;
  }else if(naIsArrayAtInitial(iterator)){
    return naGetArrayElementConst(naGetPtrConst(iterator->array), -1);
  }else{
    return naGetArrayElementConst(naGetPtrConst(iterator->array), iterator->indx - 1);
  }
}



NA_IDEF void* naGetArrayPrevMutable(NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("iterator is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  if(iterator->indx == 0){
    return NA_NULL;
  }else if(naIsArrayAtInitial(iterator)){
    return naGetArrayElementMutable(naGetPtrMutable(iterator->array), -1);
  }else{
    return naGetArrayElementMutable(naGetPtrMutable(iterator->array), iterator->indx - 1);
  }
}



NA_IDEF const void* naGetArrayCurConst(const NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("iterator is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  if(naIsArrayAtInitial(iterator)){
    return NA_NULL;
  }else{
    return naGetArrayElementConst(naGetPtrConst(iterator->array), iterator->indx);
  }
}



NA_IDEF void* naGetArrayCurMutable(NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("iterator is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  if(naIsArrayAtInitial(iterator)){
    return NA_NULL;
  }else{
    return naGetArrayElementMutable(naGetPtrMutable(iterator->array), iterator->indx);
  }
}



NA_IDEF const void* naGetArrayNextConst(const NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("iterator is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  if(iterator->indx == naGetArrayMaxIndex(naGetPtrConst(iterator->array))){
    return NA_NULL;
  }else if(naIsArrayAtInitial(iterator)){
    return naGetArrayElementConst(naGetPtrConst(iterator->array), 0);
  }else{
    return naGetArrayElementConst(naGetPtrConst(iterator->array), iterator->indx + 1);
  }
}



NA_IDEF void* naGetArrayNextMutable(NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("iterator is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  if(iterator->indx == naGetArrayMaxIndex(naGetPtrConst(iterator->array))){
    return NA_NULL;
  }else if(naIsArrayAtInitial(iterator)){
    return naGetArrayElementMutable(naGetPtrMutable(iterator->array), 0);
  }else{
    return naGetArrayElementMutable(naGetPtrMutable(iterator->array), iterator->indx + 1);
  }
}



NA_IDEF NABool naIsArrayAtFirst(const NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("iterator is Null-Pointer.");
      return NA_FALSE;
    }
  #endif
  return (iterator->indx == 0);
}



NA_IDEF NABool naIsArrayAtLast(const NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("iterator is Null-Pointer.");
      return NA_FALSE;
    }
  #endif
  return (iterator->indx == naGetArrayMaxIndex(naGetPtrConst(iterator->array)));
}



NA_IDEF NABool naIsArrayAtIndex(const NAArrayIterator* iterator, NAInt indx){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("iterator is Null-Pointer.");
      return NA_FALSE;
    }
  #endif
  indx = naMakeIndexAbsolute(indx, naGetArrayCount(naGetPtrConst(iterator->array)));
  return (iterator->indx == indx);
}



NA_IDEF NABool naIsArrayAtInitial(const NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("iterator is Null-Pointer.");
      return NA_FALSE;
    }
  #endif
  return (iterator->indx == -1);
}



NA_IDEF NAInt naGetArrayCurIndex(const NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("iterator is Null-Pointer.");
      return NA_INVALID_MEMORY_INDEX;
    }
    if(naIsArrayAtInitial(iterator))
      naError("iterator is at initial position.");
  #endif
  return iterator->indx;
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