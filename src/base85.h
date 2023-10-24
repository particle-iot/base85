
/*
<https://github.com/rafagafe/base85>

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
  Copyright (c) 2016-2018 Rafa Garcia <rafagarcia77@gmail.com>.
  Permission is hereby  granted, free of charge, to any  person obtaining a copy
  of this software and associated  documentation files (the "Software"), to deal
  in the Software  without restriction, including without  limitation the rights
  to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
  copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
  IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
  FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
  AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
  LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

*/

#ifndef BASE85_H
#define	BASE85_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

/** @defgroup base85 Base85 Converter.
  * Base85 RFC 1924 version. The character set is, in order, 0–9, A–Z, a–z, and
  * then the 23 characters !#$%&()*+-;<=>?@^_`{|}~.
  * @{ */

/** Convert a binary memory block in a base85 null-terminated string.
  * If the size of the source memory block is not a multiple of four,
  * as many zeros as necessary are added to convert it to a multiple of four.
  * @param dest Destination memory where to put the base85 null-terminated string.
  * @param src Source binary memory block.
  * @param size Size in bytes of source binary memory block.
  * @return A pointer to the null character of the base85 null-terminated string. */
char* bintob85( char* dest, void const* src, size_t size );

/** Convert a base85 string to binary format.
  * @param dest Destination memory block.
  * @param src Source base85 string.
  * @return If success a pointer to the next byte in memory block.
  *         Null if string has a bad format.  */
void* b85tobin( void* dest, char const* src );

/** Convert a base85 string to binary format.
  * @param p Source base85 string and destination memory block.
  * @return If success a pointer to the next byte in memory block.
  *         Null if string has a bad format.  */
static inline void* b85decode( void* p ) {
    return b85tobin( p, (char*)p );
}

static inline char* b85encode( void* p, size_t size ) {
    return bintob85( (char*)p, p, size );
}

/** Calculate the size of the resulting base85 encoding with padding alignment.
  * @param binsz Size of binary input to encode to base85.
  * @return The size of buffer that will encoded given the input size. */
static inline size_t bintob85size( size_t binsz ) {
  return (5 * ( binsz / 4 )) + (( binsz % 4 ) ? 5 : 0);
}

/**
 * Calculate the size of the resulting base85 encoding without padding alignment.
 * Input binary data still needs to by 4-byte aligned and padded with zeros.
 * @param binsz Size of binary input to encode to base85.
 * @return The size of output data that will encoded without extra padding. */
static inline size_t unpaddedB85Size(size_t binsz) {
    return ((binsz + 3) * 5 / 4) - 3;
}

/** Calculate the size of bytes written to a buffer.
  * @param end The end pointer result of binary/base85 operations.
  * @param start The beginning of the buffer from binary/base85 operations.
  * @return The size, in bytes, of the given end pointer from the start of the buffer. */
static inline size_t b85size( void* end, void* start ) {
    uintptr_t pend = (uintptr_t)end;
    uintptr_t pstart = (uintptr_t)start;
    return (pend > pstart) ? (size_t)( pend - pstart ) : 0;
}


/** @ } */

#ifdef __cplusplus
}
#endif

#endif	/* BASE85_H */
