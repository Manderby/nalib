
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


#include "NARuntime.h"
#include "NAMemory.h"
#include "NAString.h"
#include "NAList.h"




// Turns out, the pagesize is far too small to result in good speed
// improvements. The custom size can result in up to 2 times faster allocation
// and deallocation on some systems.
#define NA_POOL_SIZE_EQUALS_PAGESIZE
//#define NA_CUSTOM_POOL_SIZE (1 << 16)


typedef struct NACorePool NACorePool;
typedef struct NACoreTypeInfo NACoreTypeInfo;
typedef struct NARuntime NARuntime;


struct NACoreTypeInfo{
  NACorePool* curpool;
  NATypeInfo typeinfo;
};

struct NACorePool{
  NACoreTypeInfo* coretypeinfo;
  NAUInt usedcount;
  void* firstunused;
  void* firstrecycle;
  NACorePool* prevpool;
  NACorePool* nextpool;
  void* dummy1; // used in debugging. Points at first byte of the whole pool
  void* dummy2; // used in debugging. Points at last byte of the whole pool
};

struct NARuntime{
  NAUInt poolsize;
  NAUInt poolsizemask;
};



NARuntime* na_runtime = NA_NULL;


// These are the (hidden) string type identifier for the NALib runtime system.
// Will be set when calling naPrepareStringRuntime or naStartRuntime. This
// will likely change in the near future.
NATypeIdentifier na_NAString_identifier = NA_NULL;
NATypeIdentifier na_NAPointer_identifier = NA_NULL;
NATypeIdentifier na_NAPNG_identifier = NA_NULL;





NA_HIDEF void* naEnhanceCorePool(NACoreTypeInfo* coretypeinfo){
  NACorePool* corepool;
  void** curunused;
  #ifndef NDEBUG
    if(coretypeinfo->typeinfo.typesize < NA_SYSTEM_ADDRESS_BYTES)
      naError("naEnhanceCorePool", "Element is too small");
  #endif
  #ifndef NDEBUG
    if(coretypeinfo->typeinfo.typesize > (na_runtime->poolsize - sizeof(NACorePool)))
      naError("naEnhanceCorePool", "Element is too big");
  #endif

  // We create a new pool.
  corepool = (NACorePool*)naMallocAligned(na_runtime->poolsize, na_runtime->poolsize);
  corepool->coretypeinfo = coretypeinfo;

  #ifndef NDEBUG
    if(((NAUInt)corepool & ~na_runtime->poolsizemask) != 0)
      naError("naEnhanceCorePool", "pool badly aligned");
  #endif

  // Reduce the elemcount by 1 to later set the last entry to NULL
  corepool->usedcount = ((na_runtime->poolsize - sizeof(NACorePool)) / coretypeinfo->typeinfo.typesize) - 1;
  curunused = (void**)(((NAByte*)corepool) + sizeof(NACorePool));
  corepool->firstunused = curunused;
  // Enumerate all positions with the succeeding free position
  while(corepool->usedcount){
    void** nextunused = (void**)((NAByte*)curunused + coretypeinfo->typeinfo.typesize);
    *curunused = nextunused;
    curunused = nextunused;
    corepool->usedcount--;
  }
  // Set the last entry to NULL.
  *curunused = NA_NULL; // note that elemcount has been reduced by 1

  // There is no element ready to be recycled.
  corepool->firstrecycle = NA_NULL;
  
  #ifndef NDEBUG
    corepool->dummy1 = corepool;
    corepool->dummy2 = (NAByte*)((NAUInt)corepool + na_runtime->poolsize - 1);
  #endif

  // Add the new pool after the current pool or se the pool as the first and
  // only pool, if there is none available yet.
  if(coretypeinfo->curpool){
    corepool->prevpool = coretypeinfo->curpool;
    corepool->nextpool = coretypeinfo->curpool->nextpool;
    corepool->prevpool->nextpool = corepool;
    corepool->nextpool->prevpool = corepool;
  }else{
    corepool->prevpool = corepool;
    corepool->nextpool = corepool;
  }
  coretypeinfo->curpool = corepool;
    
  return corepool->firstunused;
}



NA_HIDEF void naShrinkCorePool(NACorePool* corepool){
  if(corepool->coretypeinfo->curpool == corepool){corepool->coretypeinfo->curpool = corepool->nextpool;}
  corepool->prevpool->nextpool = corepool->nextpool;
  corepool->nextpool->prevpool = corepool->prevpool;
  naFreeAligned(corepool);
}




NA_DEF void* naNewStruct(NATypeIdentifier typeidentifier){
  void* pointer;
  NACoreTypeInfo* coretypeinfo = (NACoreTypeInfo*)typeidentifier;
  #ifndef NDEBUG
    if(!na_runtime)
      naCrash("naNew", "Runtime not running. Use naStartRuntime()");
    if(!coretypeinfo)
      {naCrash("naNew", "Given type identifier is Null"); return NA_NULL;}
    if(!coretypeinfo->curpool)
      {naCrash("naNew", "No pool present"); return NA_NULL;}
  #endif
  
  // We get the next unused pointer.
  pointer = coretypeinfo->curpool->firstunused;
  // If none is available, we enhance the pool.
  if(!pointer){pointer = naEnhanceCorePool(typeidentifier);}
  
  // The current pool can now advance the first-pointer to the next one.
  coretypeinfo->curpool->firstunused = *((void**)coretypeinfo->curpool->firstunused);
  coretypeinfo->curpool->usedcount++;
  
  #ifndef NDEBUG
    if(coretypeinfo->curpool != (NACorePool*)((NAUInt)pointer & na_runtime->poolsizemask))
    naError("naNewStruct", "Pointer seems to be outside of pool");
  #endif
  
  // Now, the pointer points to a space which can be constructed.
  return pointer;
}



NA_DEF void naDelete(void* pointer){
  NACorePool* corepool;
  #ifndef NDEBUG
    if(!na_runtime)
      naCrash("naNew", "Runtime not running. Use naStartRuntime()");
  #endif
  // Find the corepool entry at the beginning of the pool.
  corepool = (NACorePool*)((NAUInt)pointer & na_runtime->poolsizemask);
  #ifndef NDEBUG
    if(corepool->dummy1 != corepool)
      naError("naDelete", "Pointer seems not to be from a pool");
    if(corepool->dummy2 != (NAByte*)((NAUInt)corepool + na_runtime->poolsize - 1))
      naError("naDelete", "Pointer seems not to be from a pool");
  #endif
  // Delete the struct with the destructor
  if(corepool->coretypeinfo->typeinfo.destructor){corepool->coretypeinfo->typeinfo.destructor(pointer);}
  *((void**)pointer) = corepool->firstunused;
  corepool->firstunused = pointer;
  corepool->usedcount--;
  if((!corepool->usedcount) && (corepool->nextpool != corepool)){naShrinkCorePool(corepool);}
}




NA_DEF void naStartRuntime(){
  #ifndef NDEBUG
    if(na_runtime)
      naCrash("naStartRuntime", "Runtime already running");
    if(sizeof(NACorePool) != (8 * NA_SYSTEM_ADDRESS_BYTES))
      naError("naStartRuntime", "NACorePool struct encoding misaligned");
  #endif
  na_runtime = naAlloc(NARuntime);
  #if defined NA_POOL_SIZE_EQUALS_PAGESIZE
    na_runtime->poolsize = naGetSystemMemoryPageSize();
    na_runtime->poolsizemask = naGetSystemMemoryPageSizeMask();
  #else
    na_runtime->poolsize = NA_CUSTOM_POOL_SIZE;
    na_runtime->poolsizemask = ~(NAUInt)(NA_CUSTOM_POOL_SIZE - NA_ONE);
  #endif
  naPrepareStringRuntime();
  naPreparePointerRuntime();
  naPrepareListElementRuntime();
}



NA_DEF NATypeIdentifier naManageRuntimeType(NATypeInfo* typeinfo){
  NACoreTypeInfo* coretypeinfo = naAlloc(NACoreTypeInfo);
  coretypeinfo->typeinfo = *typeinfo;
  coretypeinfo->curpool = NA_NULL;
  naEnhanceCorePool(coretypeinfo);
  return coretypeinfo;
}



NA_DEF void naStopRuntime(){
  #ifndef NDEBUG
    if(!na_runtime)
      naCrash("naStopRuntime", "Runtime not running");
  #endif
}





// Copyright (c) NALib, Tobias Stamm, Manderim GmbH
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
// in all copies or substantial portions of the source-code.
//
// In case the source-code of this software is inaccessible to the end-user,
// the above copyright notice and this permission notice shall be included
// in any source-code which is dependent on this software and is accessible
// to the end-user.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.