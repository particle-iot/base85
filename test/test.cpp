/**
 * Copyright (c) 2023 Particle Industries, Inc.
 *
 * Licensed under the MIT License
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/license/mit/
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() function

#include "catch.h"
#include "base85.h"  // Include the header file for the Base85 library



TEST_CASE("Base85 Encoding and Decoding") {
    SECTION("Encoding Size") {
        // Basic encoding size check based on
        //    encSize = floor(binSize / 4) * 5 + (binSize % 4) ? 5 : 0
        REQUIRE(0 == bintob85size(0));
        REQUIRE(5 == bintob85size(1));
        REQUIRE(5 == bintob85size(2));
        REQUIRE(5 == bintob85size(3));
        REQUIRE(5 == bintob85size(4));
        REQUIRE(10 == bintob85size(5));
        REQUIRE(10 == bintob85size(6));
        REQUIRE(10 == bintob85size(7));
        REQUIRE(10 == bintob85size(8));

        REQUIRE(15 == bintob85size(11));
        REQUIRE(20 == bintob85size(14));
        REQUIRE(25 == bintob85size(20));
        REQUIRE(30 == bintob85size(24));
        REQUIRE(35 == bintob85size(26));

        REQUIRE(15435 == bintob85size(12345));
    }

    SECTION("Pointer Math") {
        char pchar[16] = {0};
        REQUIRE(0 == b85size(&pchar[0], pchar));
        REQUIRE(3 == b85size(&pchar[3], pchar));
        REQUIRE(4 == b85size(&pchar[4], pchar));
        REQUIRE(9 == b85size(&pchar[9], pchar));

        REQUIRE(0 == b85size(nullptr, pchar));
        REQUIRE(0 == b85size(nullptr, nullptr));
    }

    SECTION("Basic Unpadded Encoding") {
        {
            char input[] = {10, 0, 0, 0};

            char encoded[256] = {0};
            auto encodedNull = bintob85(encoded, input, sizeof(input));
            REQUIRE(nullptr != encodedNull);
            REQUIRE(memcmp(encoded, "\x33\x49", unpaddedB85Size(1)) == 0);
        }
        {
            char input[] = {10, 20, 0, 0};

            char encoded[256] = {0};
            auto encodedNull = bintob85(encoded, input, sizeof(input));
            REQUIRE(nullptr != encodedNull);
            REQUIRE(memcmp(encoded, "\x33\x4b\x52", unpaddedB85Size(2)) == 0);
        }
        {
            char input[] = {10, 20, 30, 0};

            char encoded[256] = {0};
            auto encodedNull = bintob85(encoded, input, sizeof(input));
            REQUIRE(nullptr != encodedNull);
            REQUIRE(memcmp(encoded, "\x33\x4b\x53\x6a", unpaddedB85Size(3)) == 0);
        }
        {
            char input[] = {10, 20, 30, 40};

            char encoded[256] = {0};
            auto encodedNull = bintob85(encoded, input, sizeof(input));
            REQUIRE(nullptr != encodedNull);
            REQUIRE(memcmp(encoded, "\x33\x4b\x53\x6b\x46", unpaddedB85Size(4)) == 0);
        }
        {
            char input[] = {10, 20, 30, 40, 50, 0, 0, 0};

            char encoded[256] = {0};
            auto encodedNull = bintob85(encoded, input, sizeof(input));
            REQUIRE(nullptr != encodedNull);
            REQUIRE(memcmp(encoded, "\x33\x4b\x53\x6b\x46\x47\x35", unpaddedB85Size(5)) == 0);
        }
        {
            char input[] = {10, 20, 30, 40, 50, 60, 0, 0};

            char encoded[256] = {0};
            auto encodedNull = bintob85(encoded, input, sizeof(input));
            REQUIRE(nullptr != encodedNull);
            REQUIRE(memcmp(encoded, "\x33\x4b\x53\x6b\x46\x47\x43\x54", unpaddedB85Size(6)) == 0);
        }
        {
            char input[] = {10, 20, 30, 40, 50, 60, 70, 0};

            char encoded[256] = {0};
            auto encodedNull = bintob85(encoded, input, sizeof(input));
            REQUIRE(nullptr != encodedNull);
            REQUIRE(memcmp(encoded, "\x33\x4b\x53\x6b\x46\x47\x43\x57\x32", unpaddedB85Size(7)) == 0);
        }
        {
            char input[] = {10, 20, 30, 40, 50, 60, 70, 80};

            char encoded[256] = {0};
            auto encodedNull = bintob85(encoded, input, sizeof(input));
            REQUIRE(nullptr != encodedNull);
            REQUIRE(memcmp(encoded, "\x33\x4b\x53\x6b\x46\x47\x43\x57\x33\x35", unpaddedB85Size(8)) == 0);
        }
    }

    SECTION("Encode and then Decode") {
        const char* original = "Hello, World!\0\0\0\0";
        size_t originalSize = strlen(original);

        // Allocate memory for encoded data
        char encoded[256] = {0};
        auto encodedNull = bintob85(encoded, (const uint8_t*)original, originalSize);
        size_t encodedSize = b85size(encodedNull, encoded);
        size_t expectedSize = bintob85size(originalSize);

        REQUIRE(0 < encodedSize);
        REQUIRE(encodedSize == expectedSize);

        // Allocate memory for decoded data
        uint8_t decoded[256] = {0};
        auto decodedNull = (uint8_t*)b85tobin(decoded, encoded);
        REQUIRE(nullptr != decodedNull);
        //size_t decodedSize = b85size(decodedNull, decoded);
        size_t decodedSize = strlen((char*)decoded);

        REQUIRE(decodedSize == originalSize);
        REQUIRE(memcmp(original, decoded, originalSize) == 0);
    }

    SECTION("Decode and then Encode") {
        const char* originalEncoded = R"(FflSSG&MFiI5}ZrV`OD!)";
        const char* expectedDecoded = R"(0123456789abcdef)";
        size_t originalEncodedSize = strlen(originalEncoded);

        // Allocate memory for decoded data
        uint8_t decoded[256] = {0};
        auto decodedNull = (uint8_t*)b85tobin(decoded, originalEncoded);
        REQUIRE(nullptr != decodedNull);

        size_t decodedSize = b85size(decodedNull, decoded);
        auto expectedSize = strlen(expectedDecoded);

        REQUIRE(decodedSize > 0);
        REQUIRE(decodedSize == expectedSize);
        REQUIRE(strcmp((char*)decoded, expectedDecoded) == 0);

        // Allocate memory for re-encoded data
        char reEncoded[256] = {0};
        auto encodedNull = bintob85(reEncoded, decoded, decodedSize);
        size_t reEncodedSize = b85size(encodedNull, reEncoded);

        REQUIRE(reEncodedSize == originalEncodedSize);
        REQUIRE(memcmp(originalEncoded, reEncoded, originalEncodedSize) == 0);
    }

    SECTION("Invalid Decode Input") {
        const char* invalidEncoded = R"(FflSSG&MFiI5}ZrV"OD!)";  // Invalid Base85 string because of quotation, "

        // Allocate memory for decoded data
        uint8_t decoded[256] = {0};

        auto decodedNull = (uint8_t*)b85tobin(decoded, invalidEncoded);
        REQUIRE(nullptr == decodedNull);
    }

    SECTION("Empty Input") {
        char encoded[256] = {0};
        uint8_t decoded[256] = {0};

        REQUIRE(bintob85(encoded, decoded, 0) == encoded);
        REQUIRE(b85tobin(decoded, encoded) == decoded);
    }

    SECTION("All Digits") {
        const char digits[] = R"(0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!#$%&()*+-;<=>?@^_`{|}~)";

        char binary[256] = {0};
        auto end = (char*)b85tobin(binary, digits);
        REQUIRE(nullptr != end);
        REQUIRE(end > binary);

        size_t binarylen = (size_t)(end - binary);
        size_t digitslen = sizeof(digits) - 1;
        size_t calclen   = 4 * digitslen / 5;
        REQUIRE(binarylen == calclen);

        char base85[85] = {0};
        char* nullchar = bintob85(base85, binary, binarylen);
        REQUIRE(nullptr != nullchar);
        REQUIRE('\0' == *nullchar);
        REQUIRE(strcmp(base85, digits) == 0);
    }

    SECTION("Aliasing") {
        const char message[] = "Aliasing";

        char base85[32] = {0};
        memcpy(base85, message, sizeof(message));
        char* nullchar = b85encode( base85, sizeof(message));
        REQUIRE(nullptr != nullchar);
        REQUIRE('\0' == *nullchar);
        size_t len = b85size(nullchar, base85);
        size_t expectlen = bintob85size(sizeof(message));
        REQUIRE(len == expectlen);

        char* end = (char*)b85decode(base85);
        REQUIRE(nullptr != end);
        REQUIRE(strcmp(message, base85) == 0);
    }

    SECTION("Bad Formats - Illegal Characters") {
        const char base85[] = {'0', -1, '1', '2',  '3', 0};
        char binary[32] = {0};
        char* end = (char*)b85tobin(binary, base85);
        REQUIRE(nullptr == end);
    }

    SECTION("Bad Formats - Illegal Encoding") {
        const char base85[] = R"(aaaaaa)";
        char binary[32] = {0};
        char* end = (char*)b85tobin(binary, base85);
        REQUIRE(nullptr == end);
    }
}
