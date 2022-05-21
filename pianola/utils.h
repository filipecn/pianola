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
///\file utils.h
///\author FilipeCN (filipedecn@gmail.com)
///\date 2022-05-12
///
///\brief

#ifndef PIANOLA_SRC_UTILS_H
#define PIANOLA_SRC_UTILS_H

#include <hermes/common/defs.h>

namespace pianola {

// key midi codes range from 21 to 108
// the first key in the piano corresponds to A0
// each octave has 7 white keys and 5 black keys
// |  ' ' ' '  |  ' ' ' ' ' '  |
// |  ' ' ' '  |  ' ' ' ' ' '  |    keys on a 88 keyboard are:
// |  '-' '-'  |  '-' '-' '-'  |      A#0   C#1
// |   |   |   |   |   |   |   |     A0 B0 C1   ....  B7 C8
// | _ | _ | _ | _ | _ | _ | _ |

static bool isKeyBlack(size_t piano_index) {
  if (piano_index < 3) // it is A0, A#0 or B0
    return piano_index == 1;
  size_t octave_index = (piano_index - 3) % 12;
  return octave_index <= 4 ? octave_index % 2 : octave_index % 2 == 0;
}

static inline int whiteOctaveIndex(int octave_index) {
  int d = octave_index > 4 ? 3 : 0;
  octave_index -= octave_index > 4 ? 5 : 0;
  return d + std::max(0, octave_index - (octave_index / 2));
}

static inline int blackOctaveIndex(int octave_index) {
  int d = octave_index > 4 ? 2 : 0;
  octave_index -= octave_index > 4 ? 5 : 0;
  return d + std::max(0, octave_index / 2);
}

static inline int whiteIndex(int piano_index) {
  if (piano_index < 3) // it is A0, A#0 or B0
    return piano_index - piano_index / 2;
  piano_index -= 3;
  int octave = piano_index / 12;
  int octave_index = piano_index % 12;
  return 2 + octave * 7 + whiteOctaveIndex(octave_index);
}

static inline int blackIndex(int piano_index) {
  if (piano_index < 3) // it is A0, A#0 or B0
    return 0;
  piano_index -= 3;
  int octave = piano_index / 12;
  int octave_index = piano_index % 12;
  return 1 + octave * 5 + blackOctaveIndex(octave_index);
}

}

#endif //PIANOLA_SRC_UTILS_H
