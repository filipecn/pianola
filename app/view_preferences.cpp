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
///\file view_preferences.cpp
///\author FilipeCN (filipedecn@gmail.com)
///\date 2022-05-14
///
///\brief

#include "view_preferences.h"
#include <pianola/utils.h>

float ViewPreferences::length_per_millisecond = 0.1;
hermes::vec2 ViewPreferences::black_key_size = {0.54, 1.16};
hermes::vec2 ViewPreferences::white_key_size = {0.66, 1.66};
float ViewPreferences::key_gap = 0.05;

float ViewPreferences::whiteKeyCenter(int piano_index) {
  int key_index = pianola::whiteIndex(piano_index);
  return key_index * (key_gap + white_key_size.x) + (key_gap + white_key_size.x) / 2;
}

hermes::point2 ViewPreferences::keyCenter(int piano_index) {
  auto white_key_center = whiteKeyCenter(piano_index);
  if (pianola::isKeyBlack(piano_index)) {
    return {(whiteKeyCenter(piano_index - 1) + white_key_center) / 2, -black_key_size.y / 2};
  }
  return {white_key_center, -white_key_size.y / 2};
}

float ViewPreferences::ms2length(float milliseconds) {
  return milliseconds * length_per_millisecond;
}

float ViewPreferences::blackKeyCenter(int piano_index) {
  auto white_key_center = whiteKeyCenter(piano_index);
  return (whiteKeyCenter(piano_index - 1) + white_key_center) / 2;
}

float ViewPreferences::noteCenter(int piano_index) {
  if (pianola::isKeyBlack(piano_index))
    return blackKeyCenter(piano_index);
  return whiteKeyCenter(piano_index);
}
