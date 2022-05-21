/// Copyright (c) 2022, FilipeCN.
///
/// The MIT License (MIT)
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to
/// deal in the Software without restriction, including without limitation the
/// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
/// sell copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
/// IN THE SOFTWARE.
///
///\file utils_tests.cpp
///\author FilipeCN (filipedecn@gmail.com)
///\date 2022-05-12
///
///\brief

#include <catch2/catch.hpp>
#include <pianola/utils.h>

using namespace pianola;

TEST_CASE("Octave Keys") {
  //     1   3       6   8   10
  // |  'b' 'b'  |  'b' 'b' 'b'  |
  // |  '0' '1'  |  '2' '3' '4'  |
  // |  '-' '-'  |  '-' '-' '-'  |
  // | w0| w1| w2| w3| w4| w5| w6|
  // | _ | _ | _ | _ | _ | _ | _ |
  //   0   2   4   5   7   9   11
  REQUIRE(whiteOctaveIndex(0) == 0);
  REQUIRE(whiteOctaveIndex(2) == 1);
  REQUIRE(whiteOctaveIndex(4) == 2);
  REQUIRE(whiteOctaveIndex(5) == 3);
  REQUIRE(whiteOctaveIndex(7) == 4);
  REQUIRE(whiteOctaveIndex(9) == 5);
  REQUIRE(whiteOctaveIndex(11) == 6);
  REQUIRE(blackOctaveIndex(1) == 0);
  REQUIRE(blackOctaveIndex(3) == 1);
  REQUIRE(blackOctaveIndex(6) == 2);
  REQUIRE(blackOctaveIndex(8) == 3);
  REQUIRE(blackOctaveIndex(10) == 4);
}

TEST_CASE("Piano Keys") {
  // the first key in the piano corresponds to A0
  // each octave has 7 white keys and 5 black keys
  // |  ' ' ' '  |  ' ' ' ' ' '  |
  // |  ' ' ' '  |  ' ' ' ' ' '  |    keys on a 88 keyboard are:
  // |  '-' '-'  |  '-' '-' '-'  |      A#0   C#1
  // |   |   |   |   |   |   |   |     A0 B0 C1   ....  B7 C8
  // | _ | _ | _ | _ | _ | _ | _ |

  SECTION("is black") {
    REQUIRE(isKeyBlack(0) == false); // A0
    REQUIRE(isKeyBlack(1) == true); // A#0
    REQUIRE(isKeyBlack(2) == false); // B0
    // C1 must have index 3, so:
    for (int octave = 1; octave < 8; ++octave) {
      REQUIRE(isKeyBlack((octave - 1) * 12 + 3 + 0) == false); // C
      REQUIRE(isKeyBlack((octave - 1) * 12 + 3 + 1) == true); // C#
      REQUIRE(isKeyBlack((octave - 1) * 12 + 3 + 2) == false); // D
      REQUIRE(isKeyBlack((octave - 1) * 12 + 3 + 3) == true); // D#
      REQUIRE(isKeyBlack((octave - 1) * 12 + 3 + 4) == false); // E
      REQUIRE(isKeyBlack((octave - 1) * 12 + 3 + 5) == false); // F
      REQUIRE(isKeyBlack((octave - 1) * 12 + 3 + 6) == true); // F#
      REQUIRE(isKeyBlack((octave - 1) * 12 + 3 + 7) == false); // G
      REQUIRE(isKeyBlack((octave - 1) * 12 + 3 + 8) == true); // G#
      REQUIRE(isKeyBlack((octave - 1) * 12 + 3 + 9) == false); // A
      REQUIRE(isKeyBlack((octave - 1) * 12 + 3 + 10) == true); // A#
      REQUIRE(isKeyBlack((octave - 1) * 12 + 3 + 11) == false); // B
    }
  } //

  SECTION("white index") {
    REQUIRE(whiteIndex(0) == 0); // A0
    REQUIRE(whiteIndex(2) == 1); // B0
    // C1 must have index 3, so:
    for (int octave = 1; octave < 8; ++octave) {
      //   0   2   4   5   7   9   11
      REQUIRE(whiteIndex((octave - 1) * 12 + 3 + 0) == (octave - 1) * 7 + 2 + 0); // C
      REQUIRE(whiteIndex((octave - 1) * 12 + 3 + 2) == (octave - 1) * 7 + 2 + 1); // D
      REQUIRE(whiteIndex((octave - 1) * 12 + 3 + 4) == (octave - 1) * 7 + 2 + 2); // E
      REQUIRE(whiteIndex((octave - 1) * 12 + 3 + 5) == (octave - 1) * 7 + 2 + 3); // F
      REQUIRE(whiteIndex((octave - 1) * 12 + 3 + 7) == (octave - 1) * 7 + 2 + 4); // G
      REQUIRE(whiteIndex((octave - 1) * 12 + 3 + 9) == (octave - 1) * 7 + 2 + 5); // A
      REQUIRE(whiteIndex((octave - 1) * 12 + 3 + 11) == (octave - 1) * 7 + 2 + 6); // B
    }
  } //

  SECTION("black index") {
    REQUIRE(blackIndex(1) == 0); // A#0
    // C1 must have index 3, so:
    for (int octave = 1; octave < 8; ++octave) {
      //     1   3       6   8   10
      REQUIRE(blackIndex((octave - 1) * 12 + 3 + 1) == (octave - 1) * 5 + 1 + 0); // C#
      REQUIRE(blackIndex((octave - 1) * 12 + 3 + 3) == (octave - 1) * 5 + 1 + 1); // D#
      REQUIRE(blackIndex((octave - 1) * 12 + 3 + 6) == (octave - 1) * 5 + 1 + 2); // F#
      REQUIRE(blackIndex((octave - 1) * 12 + 3 + 8) == (octave - 1) * 5 + 1 + 3); // G#
      REQUIRE(blackIndex((octave - 1) * 12 + 3 + 10) == (octave - 1) * 5 + 1 + 4); // A#
    }
  } //

}



