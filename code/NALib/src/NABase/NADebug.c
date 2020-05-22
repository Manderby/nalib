
#include "../NABase.h"



#ifndef NDEBUG


  // The error printing method. Errors will be emitted to the stderr output.
  // When NDEBUG is defined, this function is OBSOLETE!

  NA_HDEF void naPrintError(const char* functionSymbol, const char* text){
    if(!na_test_case_running){
      fprintf(stderr, "Error in %s: %s", functionSymbol, text);

      // //////////////////////////
      // Set a breakpoint in the following line to debug.
      // //////////////////////////

      fprintf(stderr, NA_NL);

      // //////////////////////////
    }
  }



  NA_HDEF void naCaptureError(const char* functionSymbol, const char* text){
    na_error_count++;
    naPrintError(NA_DEBUG_FUNCTIONSYMBOL, text);
  }
  
  
  
  NA_HDEF void naCaptureCrash(const char* functionSymbol, const char* text){
    naPrintError(NA_DEBUG_FUNCTIONSYMBOL, text);
    fprintf(stderr, NA_NL "Crashing the application deliberately..." NA_NL);
    exit(EXIT_FAILURE);
  }



#endif



// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>
