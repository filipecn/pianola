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
///\file song.h
///\author FilipeCN (filipedecn@gmail.com)
///\date 2022-05-08
///
///\brief

#ifndef PIANOLA_SRC_SONG_H
#define PIANOLA_SRC_SONG_H

#include <pianola/track.h>
#include <vector>
#include <hermes/common/file_system.h>

namespace pianola {

class Song {
public:
  static Song fromFile(const hermes::Path &path);

  [[nodiscard]] const std::vector<Track> &tracks() const;
  [[nodiscard]] float divisions() const;

  friend std::ostream &operator<<(std::ostream &o, const Song &song);

private:
  // ticks per quarter note
  float divisions_{};
  std::vector<Track> tracks_;
};

}

#endif //PIANOLA_SRC_SONG_H
