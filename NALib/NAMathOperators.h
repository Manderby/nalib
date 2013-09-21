
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_MATH_OPERATORS_INCLUDED
#define NA_MATH_OPERATORS_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif

#include "NASystem.h"
#include "NAMathConstants.h"
#include "NARange.h"

// /////////////////////////////////
// This file contains many standard math operators. All functions are defined
// inline. Many checks are performed like division by zero which may slow the
// implementation down. You can disable these checks by defining NDEBUG.
//
// Note that there exist implementations for float and double, but not for
// long double. The reason for this is that the author has observed that the
// additional accuracy teases a programmer to believe that his results become
// more accurate just by introducing long doubles. Often times though the
// systematical or even the numerical errors introduced by algorithms are much
// larger than the accuracy of long doubles. Even more so, they are not
// standardized like floats and doubles and therefore might behave differently
// depending on the system. And they usually are quite slow and take a lot of
// space in the code to write.
//
// If you really, really need long doubles, you will most probably already look
// deep inside the dark pit of numerical uncertainties and likely won't use
// this library anymore, because you already have something much more refined.
// /////////////////////////////////


// Returns the sign. x<0 returns -1, x>=0 returns 1. Never returns 0!
// You can use the result of this function safely as a multiplication factor.
// You can also subtract the result of this function from the comma ASCII
// character and get a string representation of the sign:
// NAUTF8Char signASCII = ',' - naSigni(-42);
NA_INLINE_API NAInt        naSigni  (NAInt x);
NA_INLINE_API int32        naSigni32(int32 x);
NA_INLINE_API int64        naSigni64(int64 x);

// Returns 1/x
NA_INLINE_API float        naInvf(float x);
NA_INLINE_API double       naInv (double x);

// Returns the square root
NA_INLINE_API float        naSqrtf(float x);
NA_INLINE_API double       naSqrt(double x);

// Returns the cubic root
NA_INLINE_API float        naCbrtf(float x);
NA_INLINE_API double       naCbrt(double x);

// Returns the absolute value
NA_INLINE_API float        naAbsf  (float  x);
NA_INLINE_API double       naAbs   (double x);
NA_INLINE_API NAInt        naAbsi  (NAInt  x);
NA_INLINE_API int8         naAbsi8 (int8   x);
NA_INLINE_API int16        naAbsi16(int16  x);
NA_INLINE_API int32        naAbsi32(int32  x);
NA_INLINE_API int64        naAbsi64(int64  x);

// Returns the rounded down value
NA_INLINE_API float        naFloorf(float x);
NA_INLINE_API double       naFloor(double x);

// Returns the rounded up value
NA_INLINE_API float        naCeilf(float x);
NA_INLINE_API double       naCeil(double x);

// Returns the rounded value
NA_INLINE_API float        naRoundf(float x);
NA_INLINE_API double       naRound(double x);

// Returns the sinus
NA_INLINE_API float        naSinf(float x);
NA_INLINE_API double       naSin(double x);

// Returns the cosinus
NA_INLINE_API float        naCosf(float x);
NA_INLINE_API double       naCos(double x);

// Returns the tangens
NA_INLINE_API float        naTanf(float x);
NA_INLINE_API double       naTan(double x);

// Returns the arcus sinus
NA_INLINE_API float        naAsinf(float x);
NA_INLINE_API double       naAsin(double x);

// Returns the arcus cosinus
NA_INLINE_API float        naAcosf(float x);
NA_INLINE_API double       naAcos(double x);

// Returns the arcus tangens in the range [-pi/2, +pi/2]
NA_INLINE_API float        naAtanf(float x);
NA_INLINE_API double       naAtan(double x);

// Returns the arcus tangens of y/x in the range [-pi, +pi]
NA_INLINE_API float        naAtan2f(float y, float x);
NA_INLINE_API double       naAtan2(double y, double x);

// Returns the natural logarithm
NA_INLINE_API float        naLogf(float x);
NA_INLINE_API double       naLog(double x);

// Returns the 10-logarithm
NA_INLINE_API float        naLog10f(float x);
NA_INLINE_API double       naLog10(double x);

// Returns the 2-logarithm
NA_INLINE_API float        naLog2f(float x);
NA_INLINE_API double       naLog2(double x);
// The integer variants of log2, return the rounded down log2. In other words
// the power of 2 which is greaterequal than x.
// - 0 returns 0  -> 2^0 = 1
// - 1 returns 0  -> 2^0 = 1
// - 2 returns 1  -> 2^1 = 2
// - 3 returns 2  -> 2^2 = 4
// - 4 returns 2  -> 2^2 = 4
// - 5 returns 3  -> 2^3 = 8
// - ...
NA_INLINE_API int32        naLog2i32(int32 x);
NA_INLINE_API int64        naLog2i64(int64 x);

// Returns the exponential function e^x
NA_INLINE_API float        naExpf(float x);
NA_INLINE_API double       naExp(double x);

// Returns the naPower of 2 function 2^x
NA_INLINE_API float        naExp2f  (float x);
NA_INLINE_API double       naExp2   (double x);
NA_INLINE_API NAInt        naExp2i  (NAInt x);
NA_INLINE_API int32        naExp2i32(int32 x);
NA_INLINE_API int64        naExp2i64(int64 x);

// Returns the naPower function b^x
NA_INLINE_API float        naPowf(float b, float x);
NA_INLINE_API double       naPow(double b, double x);

// Returns the the angle converted to radiants
NA_INLINE_API float        naDegToRadf(float deg);
NA_INLINE_API double       naDegToRad(double deg);

// Returns the the angle converted to degrees
NA_INLINE_API float        naRadToDegf(float rad);
NA_INLINE_API double       naRadToDeg(double rad);






















// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////



NA_INLINE_API NAInt naSigni(NAInt x){
#if NA_SYSTEM_ADDRESS_BITS == 32
  return naSigni32(x);
#elif NA_SYSTEM_ADDRESS_BITS == 64
  return naSigni64(x);
#endif
}
NA_INLINE_API int32 naSigni32(int32 x){
  #if defined NA_SIGNED_INTEGER_USES_TWOS_COMPLEMENT
    return ((x>>31)<<1)+1;
  #else
    return (x<0)?-1:1;
  #endif
}
NA_INLINE_API int64 naSigni64(int64 x){
  #if defined NA_SIGNED_INTEGER_USES_TWOS_COMPLEMENT
    return ((x>>63)<<1)+1LL;
  #else
    return (x<0)?-1LL:1LL;
  #endif
}




NA_INLINE_API float naInvf(float x){
  #ifndef NDEBUG
    if(x == 0.f)
      naError("naInvf", "Division by zero.");
    if(naAlmostZerof(x))
      naError("naInvf", "Division by almost zero.");
  #endif
  return 1.f / x;
}
NA_INLINE_API double naInv(double x){
  #ifndef NDEBUG
    if(x == 0.)
      naError("naInv", "Division by zero.");
    if(naAlmostZero(x))
      naError("naInv", "Division by almost zero.");
  #endif
  return 1. / x;
}


NA_INLINE_API float naSqrtf(float x){
  #ifndef NDEBUG
    if(x < 0.f){naError("naSqrtf", "naSqrt of negative number.");}
  #endif
  return sqrtf(x);
}
NA_INLINE_API double naSqrt(double x){
  #ifndef NDEBUG
    if(x < 0.){naError("naSqrt", "naSqrt of negative number.");}
  #endif
  return sqrt(x);
}



NA_INLINE_API float naCbrtf(float x){
  #ifndef NDEBUG
    if(x < 0.f){naError("naCbrtf", "naCbrt of negative number.");}
  #endif
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return powf(x, NA_THIRDf);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return cbrtf(x);
  #endif
}

NA_INLINE_API double naCbrt(double x){
  #ifndef NDEBUG
    if(x < 0.){naError("naCbrt", "naCbrt of negative number.");}
  #endif
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return pow(x, NA_THIRD);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return cbrt(x);
  #endif
}


NA_INLINE_API float naAbsf(float x){
  return fabsf(x);
}
NA_INLINE_API double naAbs(double x){
  return fabs(x);
}
NA_INLINE_API NAInt naAbsi(NAInt x){
#if NA_SYSTEM_ADDRESS_BITS == 32
  return naAbsi32(x);
#elif NA_SYSTEM_ADDRESS_BITS == 64
  return naAbsi64(x);
#endif
}
NA_INLINE_API int8 naAbsi8(int8 x){
  return abs(x);
}
NA_INLINE_API int16 naAbsi16(int16 x){
  return abs(x);
}
NA_INLINE_API int32 naAbsi32(int32 x){
  return abs(x);
}
NA_INLINE_API int64 naAbsi64(int64 x){
  return llabs(x);
}


NA_INLINE_API float naFloorf(float x){
  return floorf(x);
}
NA_INLINE_API double naFloor(double x){
  return floor(x);
}


NA_INLINE_API float naCeilf(float x){
  return ceilf(x);
}
NA_INLINE_API double naCeil(double x){
  return ceil(x);
}


NA_INLINE_API float naRoundf(float x){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return floorf(x + .5f);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return roundf(x);
  #endif
}
NA_INLINE_API double naRound(double x){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return floor(x + .5);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return round(x);
  #endif
}


NA_INLINE_API float naSinf(float x){
  return sinf(x);
}
NA_INLINE_API double naSin(double x){
  return sin(x);
}

NA_INLINE_API float naCosf(float x){
  return cosf(x);
}
NA_INLINE_API double naCos(double x){
  return cos(x);
}

NA_INLINE_API float naTanf(float x){
  return tanf(x);
}
NA_INLINE_API double naTan(double x){
  return tan(x);
}



NA_INLINE_API float naAsinf(float x){
  #ifndef NDEBUG
    if(!naInNormIIf(x)){naError("naAsinf", "naAsin of invalid value.");}
  #endif
  return asinf(x);
}
NA_INLINE_API double naAsin(double x){
  #ifndef NDEBUG
    if(!naInNormII(x)){naError("naAsin", "naAsin of invalid value.");}
  #endif
  return asin(x);
}

NA_INLINE_API float naAcosf(float x){
  #ifndef NDEBUG
    if(!naInNormIIf(x)){naError("naAcosf", "naAcos of invalid value.");}
  #endif
  return acosf(x);
}
NA_INLINE_API double naAcos(double x){
  #ifndef NDEBUG
    if(!naInNormII(x)){naError("naAcos", "naAcos of invalid value.");}
  #endif
  return acos(x);
}

NA_INLINE_API float naAtanf(float x){
  return atanf(x);
}
NA_INLINE_API double naAtan(double x){
  return atan(x);
}

NA_INLINE_API float naAtan2f(float y, float x){
  return atan2f(y, x);
}
NA_INLINE_API double naAtan2(double y, double x){
  return atan2(y, x);
}

NA_INLINE_API float naExpf(float x){
  return expf(x);
}
NA_INLINE_API double naExp(double x){
  return exp(x);
}


NA_INLINE_API float naLogf(float x){
  #ifndef NDEBUG
    if(x < 0.f){naError("naLogf", "Logarithm of negative number.");}
  #endif
  return logf(x);
}
NA_INLINE_API double naLog(double x){
  #ifndef NDEBUG
    if(x < 0.){naError("naLog", "Logarithm of negative number.");}
  #endif
  return log(x);
}



NA_INLINE_API float naLog10f(float x){
  #ifndef NDEBUG
    if(x < 0)
      naError("naLog10f", "Logarithm of negative number.");
  #endif
  return log10f(x);
}
NA_INLINE_API double naLog10(double x){
  #ifndef NDEBUG
    if(x < 0)
      naError("naLog10", "Logarithm of negative number.");
  #endif
  return log10(x);
}




NA_INLINE_API float naLog2f(float x){
  #ifndef NDEBUG
    if(x < 0.f){naError("naLog2f", "Logarithm of negative number.");}
  #endif
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return logf(x) / NA_LOGOF2f;
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return log2f(x);
  #endif
}
NA_INLINE_API double naLog2(double x){
  #ifndef NDEBUG
    if(x < 0.){naError("naLog2", "Logarithm of negative number.");}
  #endif
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return log(x) / NA_LOGOF2;
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return log2(x);
  #endif
}

NA_INLINE_API int32 naLog2i32(int32 x){
  int32 retvalue; // Note: declaration before any other code.
  #ifndef NDEBUG
    if(x < 0)
      naError("naLog2i32", "Logarithm of negative number.");
  #endif
  if(x<=0){return 0;}
  x--;
  retvalue = 0;
  while(x){retvalue++; x>>=1;}
  return retvalue;
}
NA_INLINE_API int64 naLog2i64(int64 x){
  int64 retvalue; // Note: declaration before any other code.
  #ifndef NDEBUG
    if(x < 0)
      naError("naLog2i64", "Logarithm of negative number.");
  #endif
  if(x<=0){return 0;}
  x--;
  retvalue = 0;
  while(x){retvalue++; x>>=1;}
  return retvalue;
}



NA_INLINE_API float naExp2f(float x){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return powf(2.f, x);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return exp2f(x);
  #endif
}
NA_INLINE_API double naExp2(double x){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return pow(2., x);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return exp2(x);
  #endif
}
NA_INLINE_API NAInt naExp2i(NAInt x){
#if NA_SYSTEM_ADDRESS_BITS == 32
  return naExp2i32(x);
#elif NA_SYSTEM_ADDRESS_BITS == 64
  return naExp2i64(x);
#endif
}
NA_INLINE_API int32 naExp2i32(int32 x){
  #ifndef NDEBUG
    if(x < 0)
      naError("naExp2i32", "Exponent negative. Will result in 0.");
    if(x >= 32)
      naError("naExp2i32", "Exponent too big. Will result in 0.");
  #endif
  return 1<<x;
}
NA_INLINE_API int64 naExp2i64(int64 x){
  #ifndef NDEBUG
    if(x < 0)
      naError("naExp2i64", "Exponent negative. Will result in 0.");
    if(x >= 64)
      naError("naExp2i64", "Exponent too big. Will result in 0.");
  #endif
  return 1LL<<x;
}



NA_INLINE_API float naPowf(float b, float x){
  #ifndef NDEBUG
    if(b < 0.f)
      naError("naPowf", "Base of naPower function smaller 0.");
  #endif
  return powf(b, x);
}
NA_INLINE_API double naPow(double b, double x){
  #ifndef NDEBUG
    if(b < 0.)
      naError("naPow", "Base of naPower function smaller 0.");
  #endif
  return pow(b, x);
}


NA_INLINE_API float naDegToRadf(float deg){
  return deg * NA_DEGREESf;
}
NA_INLINE_API double naDegToRad(double deg){
  return deg * NA_DEGREES;
}

NA_INLINE_API float naRadToDegf(float rad){
  return rad * NA_DEG_PER_RADf;
}
NA_INLINE_API double naRadToDeg(double rad){
  return rad * NA_DEG_PER_RAD;
}



#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_MATH_OPERATORS_INCLUDED


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
