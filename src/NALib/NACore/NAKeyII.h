
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NAKey.h
// Do not include this file directly! It will automatically be included when
// including "NAKey.h"

#include "NACoord.h"



NA_IDEF void NA_KEY_OP(Assign, double)(void* dst, const void* src){
  *(double*)dst = *(const double*)src;
}
NA_IDEF NABool NA_KEY_OP(LowerEqual, double)(const void* a, const void* b){
  return *(const double*)a <= *(const double*)b;
}
NA_IDEF NABool NA_KEY_OP(Lower, double)(const void* a, const void* b){
  return *(const double*)a < *(const double*)b;
}
NA_IDEF NABool NA_KEY_OP(Equal, double)(const void* a, const void* b){
  return *(const double*)a == *(const double*)b;
}
NA_IDEF NABool NA_KEY_OP(GreaterEqual, double)(const void* a, const void* b){
  return *(const double*)a >= *(const double*)b;
}
NA_IDEF NABool NA_KEY_OP(Greater, double)(const void* 
a, const void* b){
  return *(const double*)a > *(const double*)b;
}
NA_IDEF void NA_KEY_OP(Add, double)(void* dst, const void* src1, const void* src2){
  *(double*)dst = *(const double*)src1 + *(const double*)src2;
}



NA_IDEF void NA_KEY_OP(Assign, float)(void* dst, const void* src){
  *(float*)dst = *(const float*)src;
}
NA_IDEF NABool NA_KEY_OP(LowerEqual, float)(const void* a, const void* b){
  return *(const float*)a <= *(const float*)b;
}
NA_IDEF NABool NA_KEY_OP(Lower, float)(const void* a, const void* b){
  return *(const float*)a < *(const float*)b;
}
NA_IDEF NABool NA_KEY_OP(Equal, float)(const void* a, const void* b){
  return *(const float*)a == *(const float*)b;
}
NA_IDEF NABool NA_KEY_OP(GreaterEqual, float)(const void* a, const void* b){
  return *(const float*)a >= *(const float*)b;
}
NA_IDEF NABool NA_KEY_OP(Greater, float)(const void* a, const void* b){
  return *(const float*)a > *(const float*)b;
}
NA_IDEF void NA_KEY_OP(Add, float)(void* dst, const void* src1, const void* src2){
  *(float*)dst = *(const float*)src1 + *(const float*)src2;
}



NA_IDEF void NA_KEY_OP(Assign, NAInt)(void* dst, const void* src){
  *(NAInt*)dst = *(const NAInt*)src;
}
NA_IDEF NABool NA_KEY_OP(LowerEqual, NAInt)(const void* a, const void* b){
  return *(const NAInt*)a <= *(const NAInt*)b;
}
NA_IDEF NABool NA_KEY_OP(Lower, NAInt)(const void* a, const void* b){
  return *(const NAInt*)a < *(const NAInt*)b;
}
NA_IDEF NABool NA_KEY_OP(Equal, NAInt)(const void* a, const void* b){
  return *(const NAInt*)a == *(const NAInt*)b;
}
NA_IDEF NABool NA_KEY_OP(GreaterEqual, NAInt)(const void* a, const void* b){
  return *(const NAInt*)a >= *(const NAInt*)b;
}
NA_IDEF NABool NA_KEY_OP(Greater, NAInt)(const void* a, const void* b){
  return *(const NAInt*)a > *(const NAInt*)b;
}
NA_IDEF void NA_KEY_OP(Add, NAInt)(void* dst, const void* src1, const void* src2){
  *(NAInt*)dst = *(const NAInt*)src1 + *(const NAInt*)src2;
}



NA_IDEF void NA_KEY_OP(Assign, NAPos)(void* dst, const void* src){
  naCopyPos((NAPos*)dst, (const NAPos*)src);
}
NA_IDEF NABool NA_KEY_OP(Lower, NAPos)(const void* a, const void* b){
  return naLowerPos(*(const NAPos*)a, *(const NAPos*)b);
}
NA_IDEF NABool NA_KEY_OP(LowerEqual, NAPos)(const void* a, const void* b){
  return naLowerEqualPos(*(const NAPos*)a, *(const NAPos*)b);
}
NA_IDEF NABool NA_KEY_OP(Equal, NAPos)(const void* a, const void* b){
  return naEqualPos(*(const NAPos*)a, *(const NAPos*)b);
}
NA_IDEF NABool NA_KEY_OP(GreaterEqual, NAPos)(const void* a, const void* b){
  return naGreaterEqualPos(*(const NAPos*)a, *(const NAPos*)b);
}
NA_IDEF NABool NA_KEY_OP(Greater, NAPos)(const void* a, const void* b){
  return naGreaterPos(*(const NAPos*)a, *(const NAPos*)b);
}
NA_IDEF void NA_KEY_OP(Add, NAPos)(void* dst, const void* src1, const void* src2){
  NAPos* dstPos = (NAPos*)dst; 
  NAPos* src1Pos = (NAPos*)src1; 
  NAPos* src2Pos = (NAPos*)src2; 
  dstPos->x = src1Pos->x + src2Pos->x;
  dstPos->y = src1Pos->y + src2Pos->y;
}



NA_IDEF void NA_KEY_OP(Assign, NAVertex)(void* dst, const void* src){
  naCopyVertex((NAVertex*)dst, (const NAVertex*)src);
}
NA_IDEF NABool NA_KEY_OP(Lower, NAVertex)(const void* a, const void* b){
  return naLowerVertex(*(const NAVertex*)a, *(const NAVertex*)b);
}
NA_IDEF NABool NA_KEY_OP(LowerEqual, NAVertex)(const void* a, const void* b){
  return naLowerEqualVertex(*(const NAVertex*)a, *(const NAVertex*)b);
}
NA_IDEF NABool NA_KEY_OP(Equal, NAVertex)(const void* a, const void* b){
  return naEqualVertex(*(const NAVertex*)a, *(const NAVertex*)b);
}
NA_IDEF NABool NA_KEY_OP(GreaterEqual, NAVertex)(const void* a, const void* b){
  return naGreaterEqualVertex(*(const NAVertex*)a, *(const NAVertex*)b);
}
NA_IDEF NABool NA_KEY_OP(Greater, NAVertex)(const void* a, const void* b){
  return naGreaterVertex(*(const NAVertex*)a, *(const NAVertex*)b);
}
NA_IDEF void NA_KEY_OP(Add, NAVertex)(void* dst, const void* src1, const void* src2){
  NAVertex* dstVertex = (NAVertex*)dst; 
  NAVertex* src1Vertex = (NAVertex*)src1; 
  NAVertex* src2Vertex = (NAVertex*)src2; 
  dstVertex->x = src1Vertex->x + src2Vertex->x;
  dstVertex->y = src1Vertex->y + src2Vertex->y;
  dstVertex->z = src1Vertex->z + src2Vertex->z;
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
