
/*
<https://github.com/particle-iot/base85>
     
  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
  Copyright (c) 2018 Rafa Garcia <rafagarcia77@gmail.com>.
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

#include "Particle.h"
#include "base85.h"

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(AUTOMATIC);

/* 1) Fill an array named 'binary'.
 * 2) Convert 'binary' in base85 format in an array named 'base85'.
 * 3) Print 'base85'.
 * 4) Convert 'base85' in binary format in an array named 'output'.
 * 5) Check if the conversion was successful.
 * 6) Check the length of 'output'.
 * 7) Compare 'binary' with 'output'. */
void setup() {
    Serial.begin(115200);
    waitFor(Serial.isConnected, 10000);

    char binary[64];
    for( int i = 0; i < 64; ++i )
        binary[i] = i;

    char base85[128] = {};
    bintob85( base85, binary, sizeof(binary) );

    Serial.printlnf( "The base85: '%s'.", base85);

    char output[64] = {};
    auto end = (char*)b85tobin( output, base85 );
    if ( !end ) {
        Serial.println( "Bad base85 format." );
    }

    auto outputlen = (size_t)(end - output);
    if ( outputlen != sizeof(binary) ) {
        Serial.println( "The length of the output is not as expected." );
    }

    auto equal = !memcmp( binary, output, sizeof(binary) );
    if( !equal ) {
        Serial.println( "The output is different from the input." );
    }
}

void loop() {

}