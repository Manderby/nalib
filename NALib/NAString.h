
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_STRING_INCLUDED
#define NA_STRING_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif

#include "NAByteArray.h"
#include <stdarg.h>
#include <stdio.h>
#include <limits.h>

// Note that in NALib, a String is internally always encoded in UTF-8.
// Strings also are considered to contain const values. A user might nontheless
// access and alter the contents using the array.ptr field.

// Note that the definition of an UTF-8 char here is the built-in char type.
// The uint8 or int8 type can not be used as it is defined to be incompatible
// with char. We add the following preprocessor directive to make sure, our
// code can work properly.
#if CHAR_BIT != 8
  #error NALib can not currently work with chars bigger than 8 bits.
#endif
typedef char NAUTF8Char;

// System dependant mapping of string functions and macros
#if NA_SYSTEM == NA_SYSTEM_WINDOWS
//  #include "windows.h"
//  typedef WCHAR SystemChar;
#elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
//  typedef short SystemChar;
#else
#endif



// Mapping of standard library functions. They can be different depending on
// the system compiled.
NA_INLINE_API NAInt naStrlen(           const NAUTF8Char* str);
NA_INLINE_API NAInt naVsnprintf(              NAUTF8Char* buffer,
                                                    NAInt size,
                                        const NAUTF8Char* newstr,
                                                  va_list argumentlist);
NA_INLINE_API NAInt naVarargStringSize( const NAUTF8Char* string,
                                                  va_list args);




typedef enum NANewLine{
  NA_STRING_NEWLINE_NONE = 0,
  NA_STRING_NEWLINE_UNIX,
  NA_STRING_NEWLINE_WIN,
  NA_STRING_NEWLINE_MAC9
} NANewLine;

#define NA_STRING_NULL_TERMINATED           0x0001


// /////////////////////
// Escaping flags.
// /////////////////////

// The backslash escapes any successing char. For example " is escaped by \"
#define NA_STRING_ESCAPE_BACKSLASH                                    0x0100
// When this flag is set, double quotes encapsulate the text within them.
#define NA_STRING_ENCAPSULATE_DOUBLE_QUOTES                           0x0200
// This flag is set when the first or last offset of a string is considered to
// be within double quotes.
#define NA_STRING_IS_INSIDE_DOUBLE_QUOTES                             0x0400
// This flag escapes " by "" but only within double quotes.
#define NA_STRING_ESCAPE_DOUBLE_QUOTE_DOUBLING_WITHIN_DOUBLE_QUOTES   0x0800


// Opaque type. See explanation in readme.txt
typedef struct NAString NAString;
struct NAString{
  struct NAByteArray array;
  uint32             flags;
};
// Note that an NAString is considered empty if the underlying array is empty.
// If that is the case, flags contains garbage values.


// Terminology:
//
// Note that in this implementation, a string has a leading and a trailing end.
// The leading end contains the first character and the trailing end the last.
// Words like "left" or "right" are misleading because there exist languages
// where the first character is not left but right (for example Arabic) or the
// characters are written from top to bottom (for example Chinese).




// ///////////////////////////
// Constructors and Destructors. Read the readme.txt for explanation.
// ///////////////////////////

// Creates or fills a new, EMPTY String and returns the pointer.
// flags is set to 0.
NAString* naCreateString(NAString* string);

// Creates or fills a new String with the desired size and returns the
// pointer. The internal storage will, after this creation, automatically have
// a \0 at the end and therefore have a size of size+1. The programmer will
// nontheless be presented with a usable string of the given size. The string
// itself is marked as null-terminated.
//
// This function allocates storage-memory on the heap which will be freed
// automatically when the string storage is no longer used.
NAString* naCreateStringWithSize(NAString* string, NAInt size);

// Creates a string out of a given C-String encoded in UTF8. This function
// is useful for the programmers which like to write strings in the code
// like "Hello World". This function takes such strings and wraps it into a
// string struct. The string is marked explicitely as read-only. Note that
// this works only reliable if the source-code itself is encoded in UTF-8.
NAString* naCreateStringWithUTF8CString(  NAString* string,
                                  const NAUTF8Char* ptr);

// Creates an NAString just like sprintf.
NAString* naCreateStringWithFormat(NAString* string,
                           const NAUTF8Char* format,
                                             ...);

// Fills deststring with a desired part of srcstring. Does not copy!
// See naCreateByteArrayExtraction for an explanation of all arguments.
// Use offset = 0 and size = -1 to reference the whole string.
NAString* naCreateStringExtraction( NAString* deststring,
                              const NAString* srcstring,
                                        NAInt offset,
                                        NAInt size);

// Clears or deletes the given string.
void naClearString(NAString* string);
void naDestroyString(NAString* string);




// COPIES the contents of the string to a separate storage and decouples it
// from the existing storage. After this function, the decoupled string is
// ensured to be null-terminated. Copies always!
void naDecoupleString(NAString* string);



// Returns the size of the string in BYTES. If the string is Null-terminated,
// the terminating Null-character is NOT counted. Therefore "Hello" always
// returns 5, no matter if the string is null-terminated or not.
NAInt naGetStringSize(const NAString* string);

// Returns a either a const or a non-const pointer to the first character of
// this string. Use the const variant for printf %s arguments but make sure
// the string is null-terminated. Use naDecoupleString if necessary. No debug
// warning will be emitted if the string is not null-terminated!
// When the string is empty, a pointer to a const char '\0' is returned.
const NAUTF8Char* naGetStringConstUTF8Pointer  (const NAString* string);
      NAUTF8Char* naGetStringMutableUTF8Pointer(      NAString* string);

// Returns a pointer to the desired character.
const NAUTF8Char* naGetStringConstChar  (const NAString* string, NAInt index);
      NAUTF8Char* naGetStringMutableChar(      NAString* string, NAInt index);

// Returns NA_TRUE if the string is empty.
NABool naIsStringEmpty(const NAString* string);



// ////////////////////////////
// Parsing methods
// All the following methods go through a const char string or an NAString,
// searching for desired values. Beware: Some functions change NAStrings!
// ////////////////////////////


// naGetStringCharacterEscapeSize TowardsTrailing and TowardsLeading:
// These two functions test if the character at offset is the beginning or the
// end of an escape sequence. A length in bytes is returned which denotes the
// size and orientation of the escape sequence.
//
// This method is the core implementation of NALib's string parsing and can be
// configured using the escaping flags above.
//
// If the returned size is positive, the escape sequence spans to the trailing
// end. If the returned size is negative, the escape sequence spans to the
// leading end. The absolute value of the returned number denotes the number of
// bytes - 1 needed for the escape sequence. Examples:
// When checking the  \  in  \"   1 might be returned.
// When checking the  "  in  \"  -1 might be returned.
// When checking an escaped windows line ending (\-CR-LF), 2 might be returned.
// When checking the last double quote of "Hello World", -12 might be returned.
// If the character is not part of an escape sequence, 0 is returned.
//
// The Trailing-function will search towards the trailing end and will always
// return 0 or a positive number.
// The Leading-function will search towards the leading end and will always
// return 0 or a negative number.
// This distinction is important for example when checking the middle character
// of the string "\\n": Is the backslash an escaped backslash or does it escape
// the successing n?
//
// The offset argument can be positive or negative. A negative offset searches
// the position size-offset. For example -1 denotes the last character.
//
NAInt naGetStringCharacterEscapeSizeTowardsTrailing(NAString* string,
                                                        NAInt offset);
NAInt naGetStringCharacterEscapeSizeTowardsLeading( NAString* string,
                                                        NAInt offset);


// naSkipStringWhitespaces searches from leading to trailing for the first char
// which is > 32 (space) and adjusts offset and size accordingly. The string
// might also empty after this function.
void naSkipStringWhitespaces(NAString* string);

// Returns the next line delimited by CR, LF or CR-LF. The returned line will
// not contain any line-ending characters but all other whitespaces (ord <= 32)
// are still there. When skipempty is NA_TRUE, lines containing nothing but
// whitespaces will be skipped.
// Returns the number of lines read. Will usually be 1 but will be greater than
// 1 when skipping empty lines. Will be 0 when the string is empty.
// Warning: the two arguments shall not be the same as the result is
// undefined.
NAInt naGetStringLine(NAString* line, NAString* string, NABool skipempty);

// Gathers the first token within string which is embraced by whitespaces.
// The returned token will not have any leading or trailing whitespaces.
// The string argument will point to the next character after the token
// not being a whitespace. Both strings might be empty after this function.
// Warning: the two arguments shall not be the same as the result is
// undefined.
void naParseStringToken(NAString* string, NAString* token);

// Gathers the first token within string which ends in the given delimiter.
// The delimiter will not be included. After this function, string will point
// to the first character after the delimiter. Leading or trailing whitespaces
// will not be stripped at all. Both string might also be empty after this
// function. Warning: the two arguments shall not be the same as the result is
// undefined.
void naParseStringTokenWithDelimiter( NAString* string,
                                      NAString* token,
                                     NAUTF8Char delimiter);

// Gathers the string which is encapsulated by the given delimiter pair. The
// Delimiters will not be included. After this function, string will point to
// the first character after the end delimiter. Any character BEFORE the start-
// delimiter will be ignored. Leading or trailing whitespaces WITHIN the
// delimiter-pair will not be stripped at all. Both strings might also be empty
// after this frunction. Warning: The two arguments shall not be the same as
// the result is undefined.
// Further notice: Escaping will only occur WITHIN the delimiter-pair.
//void naParseStringTokenEncapsulatedBy(  NAString* string,
//                              NAString* token,
//                             NAUTF8Char startdelimiter,
//                             NAUTF8Char enddelimiter);
// commented out because the autor does not like this function. todo.

// Parses the given string for decimal digits and accumulates them into an
// unsigned integer. The function will start at the first character and parse
// as long as there is a decimal digit. The number of bytes considered is
// returned. The resulting integer value is returned in retint. If retint is
// a Null-Pointer, the function just returns the number of bytes considered.
//
// The function will not parse more than maxbytecount bytes. If maxbytecount
// is zero, the string is expected to be Null-terminated and the function will
// not parse beyound that Bull-byte.
//
// If the parsed value exceeds max, retint will be max and a warning will be
// emitted when debugging. But note that the returned number of bytes contains
// all digits considered.
NAInt naParseUTF8StringForDecimalUnsignedInteger(const NAUTF8Char* string,
                                                           uint64* retint,
                                                             NAInt maxbytecount,
                                                            uint64 max);
// Same as above but parses a signed integer. Note that there is an addidional
// min parameter.
NAInt naParseUTF8StringForDecimalSignedInteger( const NAUTF8Char* string,
                                                           int64* retint,
                                                            NAInt maxbytecount,
                                                            int64 min,
                                                            int64 max);

// Parses the given string for an integer value and returns the value in the
// desired type. Note: These are parsing functions which alter string such that
// the string points to the limiting character after this function. If you only
// want the value but not alter the string, look for the naGetStringIntXXX
// functions below.
//
// Whe skipdelimiter is true, the limiting character is skipped and the string
// points to the character right after the delimiter.
//
// The unsigned variants only detect digits and assume a positive value. The
// signed variants detect when the first character is '+' or '-'. Multiple
// signes are not detected.
//
// If any of the strings exceeds the type range, the maximal / minimal value
// of that type is returned. When debugging, a warning is emitted.
int8   naParseStringInt8  (NAString* string, NABool skipdelimiter);
int16  naParseStringInt16 (NAString* string, NABool skipdelimiter);
int32  naParseStringInt32 (NAString* string, NABool skipdelimiter);
int64  naParseStringInt64 (NAString* string, NABool skipdelimiter);
uint8  naParseStringUInt8 (NAString* string, NABool skipdelimiter);
uint16 naParseStringUInt16(NAString* string, NABool skipdelimiter);
uint32 naParseStringUInt32(NAString* string, NABool skipdelimiter);
uint64 naParseStringUInt64(NAString* string, NABool skipdelimiter);

// Returns the appropriate representation while leaving the string untouched.
int8   naGetStringInt8  (const NAString* string);
int16  naGetStringInt16 (const NAString* string);
int32  naGetStringInt32 (const NAString* string);
int64  naGetStringInt64 (const NAString* string);
uint8  naGetStringUInt8 (const NAString* string);
uint16 naGetStringUInt16(const NAString* string);
uint32 naGetStringUInt32(const NAString* string);
uint64 naGetStringUInt64(const NAString* string);

// Tests if the content of the given pointer is equal to the contents of string.
NABool naIsStringEqualToUTF8Pointer(const NAString* string,
                                  const NAUTF8Char* ptr);

















// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////

NA_INLINE_API NAInt naStrlen(const NAUTF8Char* str){
  return (NAInt)strlen((const char*)str);
}


NA_INLINE_API NAInt naVsnprintf(  NAUTF8Char* buffer,
                                      NAInt size,
                                      const NAUTF8Char *newstr,
                                    va_list argumentlist){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return (NAInt)_vsnprintf_s(buffer, (size_t)size, (size_t)size, newstr, argumentlist);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return (NAInt)vsnprintf((char*)buffer, (size_t)size, (const char*)newstr, argumentlist);
  #endif
}


NA_INLINE_API NAInt naVarargStringSize(const NAUTF8Char* string, va_list args){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return (NAInt)_vscprintf(string, args);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return (NAInt)naVsnprintf(NA_NULL, 0, string, args);
  #endif
}







#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_STRING_INCLUDED


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
