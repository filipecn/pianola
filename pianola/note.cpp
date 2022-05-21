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
///\file note.cpp
///\author FilipeCN (filipedecn@gmail.com)
///\date 2022-05-08
///
///\brief

#include <hermes/common/str.h>
#include "note.h"
#include "midi_utils.h"

namespace pianola {

Note Note::fromMessage(const libremidi::message &message, size_t start_tick) {
  if (message.is_meta_event() || (
      message.get_message_type() != libremidi::message_type::NOTE_ON &&
          message.get_message_type() != libremidi::message_type::NOTE_OFF))
    return {};
  Note note;
  note.channel = message.get_channel();
  note.start_in_ticks = start_tick;
  note.midi_code = message.bytes[1];
  note.velocity = message.bytes[2];
  return note;
}

std::ostream &operator<<(std::ostream &o, const Note &note) {
  o << "[Note][" << hermes::Str::ljust(noteCode2str(note.midi_code, true), 4) << "]";
  o << "[track " << hermes::Str::rjust(note.track, 2) << "]";
  o << "[channel " << hermes::Str::rjust(note.channel, 2) << "]";
  o << "[velocity " << hermes::Str::rjust(note.velocity, 3) << "]";
  o << "[start " << hermes::Str::rjust(note.start_in_ticks, 6) << " ticks]";
  o << "[duration " << hermes::Str::rjust(note.end_in_ticks - note.start_in_ticks, 6) << " ticks]";
  o << "[duration " << hermes::Str::rjust(note.duration_in_ms, 6) << " ms]";
  return o;
}

}
