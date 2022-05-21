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
///\file view_preferences.h
///\author FilipeCN (filipedecn@gmail.com)
///\date 2022-05-14
///
///\brief

#ifndef PIANOLA_APP_VIEW_PREFERENCES_H
#define PIANOLA_APP_VIEW_PREFERENCES_H

#include <hermes/geometry/point.h>
#include <hermes/geometry/vector.h>

struct ViewPreferences {
  // tempo
  static float length_per_millisecond;
  // keyboard
  static hermes::vec2 black_key_size;
  static hermes::vec2 white_key_size;
  static float key_gap;

  static float blackKeyCenter(int piano_index);
  static float whiteKeyCenter(int piano_index);
  static float noteCenter(int piano_index);
  static hermes::point2 keyCenter(int piano_index);

  static float ms2length(float milliseconds);

};

#endif //PIANOLA_APP_VIEW_PREFERENCES_H
