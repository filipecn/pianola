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
///\file track.h
///\author FilipeCN (filipedecn@gmail.com)
///\date 2022-05-16
///
///\brief

#ifndef PIANOLA_PIANOLA_TRACK_H
#define PIANOLA_PIANOLA_TRACK_H

#include <pianola/note.h>
#include <pianola/time_track.h>

namespace pianola {

class Track {
public:
  Track();
  void push(const libremidi::track_event &event, size_t tick_count, float divisions);
  void pushTrackEvent(const libremidi::message &event, size_t tick_count, float divisions);
  void pushMetaEvent(const libremidi::message& event, size_t tick_count);
  [[nodiscard]] size_t size() const;
  [[nodiscard]] const std::vector<Note> &notes() const;
  friend std::ostream &operator<<(std::ostream &o, const Track &track);
private:
  TimeTrack time_;
  std::vector<Note> notes_;
  std::vector<int> note_hash_;
};

}

#endif //PIANOLA_PIANOLA_TRACK_H
