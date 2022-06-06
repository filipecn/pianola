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
///\file track.cpp
///\author FilipeCN (filipedecn@gmail.com)
///\date 2022-05-16
///
///\brief

#include "track.h"
#include <pianola/midi_utils.h>
#include <hermes/common/debug.h>

namespace pianola {

Track::Track() {
  note_hash_.resize(128, -1);
}

void Track::push(const libremidi::track_event &event, size_t tick_count, float divisions) {
  if (event.m.is_meta_event())
    pushMetaEvent(event.m, tick_count);
  else
    pushTrackEvent(event.m, tick_count, divisions);
}

std::ostream &operator<<(std::ostream &o, const Track &track) {
  o << "[TRACK]\n";
  o << track.time_;
  for (const auto &note : track.notes_)
    o << "\t" << note << "\n";
  return o;
}

size_t Track::size() const {
  return notes_.size();
}

const std::vector<Note> &Track::notes() const {
  return notes_;
}

void Track::pushTrackEvent(const libremidi::message &event, size_t tick_count, float divisions) {
  switch (event.get_message_type()) {
  case libremidi::message_type::INVALID: break;
  case libremidi::message_type::NOTE_OFF: {
    auto note = Note::fromMessage(event, tick_count);
    // this note must be already present in the stream!
    HERMES_ASSERT(note_hash_[note.midi_code] >= 0);
  }
    break;
  case libremidi::message_type::NOTE_ON: {
    auto note = Note::fromMessage(event, tick_count);
    if (note_hash_[note.midi_code] < 0) {
      // new appearance of this node
      note_hash_[note.midi_code] = notes_.size();
      notes_.emplace_back(note);
    } else {
      // the note is already active
      // here we consider a zero velocity as a note release
      HERMES_ASSERT(note.velocity == 0);
      if (note.velocity == 0) {
        notes_[note_hash_[note.midi_code]].end_in_ticks = tick_count;
        // compute duration in ms
        notes_[note_hash_[note.midi_code]].start_in_ms =
            time_.tempo(tick_count).ticks2ms(notes_[note_hash_[note.midi_code]].start_in_ticks,
                                             divisions);
        notes_[note_hash_[note.midi_code]].duration_in_ms =
            time_.tempo(tick_count).ticks2ms(notes_[note_hash_[note.midi_code]].end_in_ticks -
                notes_[note_hash_[note.midi_code]].start_in_ticks, divisions);
        note_hash_[note.midi_code] = -1;
      }
    }
  }
    break;
  case libremidi::message_type::POLY_PRESSURE: break;
  case libremidi::message_type::CONTROL_CHANGE: break;
  case libremidi::message_type::PROGRAM_CHANGE: break;
  case libremidi::message_type::AFTERTOUCH: break;
  case libremidi::message_type::PITCH_BEND: break;
  case libremidi::message_type::SYSTEM_EXCLUSIVE: break;
  case libremidi::message_type::TIME_CODE: break;
  case libremidi::message_type::SONG_POS_POINTER: break;
  case libremidi::message_type::SONG_SELECT: break;
  case libremidi::message_type::RESERVED1: break;
  case libremidi::message_type::RESERVED2: break;
  case libremidi::message_type::TUNE_REQUEST: break;
  case libremidi::message_type::EOX: break;
  case libremidi::message_type::TIME_CLOCK: break;
  case libremidi::message_type::RESERVED3: break;
  case libremidi::message_type::START: break;
  case libremidi::message_type::CONTINUE: break;
  case libremidi::message_type::STOP: break;
  case libremidi::message_type::RESERVED4: break;
  case libremidi::message_type::ACTIVE_SENSING: break;
  case libremidi::message_type::SYSTEM_RESET: break;
  }
}

void Track::pushMetaEvent(const libremidi::message &message, size_t tick_count) {
  switch (message.get_meta_event_type()) {
  case libremidi::meta_event_type::SEQUENCE_NUMBER: break;
  case libremidi::meta_event_type::TEXT:
  case libremidi::meta_event_type::COPYRIGHT:
  case libremidi::meta_event_type::TRACK_NAME:
  case libremidi::meta_event_type::INSTRUMENT:
  case libremidi::meta_event_type::LYRIC:
  case libremidi::meta_event_type::MARKER:
  case libremidi::meta_event_type::CUE:
  case libremidi::meta_event_type::PATCH_NAME:
  case libremidi::meta_event_type::DEVICE_NAME:break;
  case libremidi::meta_event_type::CHANNEL_PREFIX: break;
  case libremidi::meta_event_type::MIDI_PORT: break;
  case libremidi::meta_event_type::END_OF_TRACK: break;
  case libremidi::meta_event_type::TEMPO_CHANGE:time_.push(message, tick_count);
    break;
  case libremidi::meta_event_type::SMPTE_OFFSET: break;
  case libremidi::meta_event_type::TIME_SIGNATURE:time_.push(message, tick_count);
    break;
  case libremidi::meta_event_type::KEY_SIGNATURE: {
//    HERMES_NOT_IMPLEMENTED
//    HERMES_LOG("");
  }
    break;
  case libremidi::meta_event_type::PROPRIETARY: break;
  case libremidi::meta_event_type::UNKNOWN: break;
  }
}

const TimeTrack &Track::timeTrack() const {
  return time_;
}

float Track::durationInMilliseconds(float divisions) const {
  if (notes_.empty())
    return 0;
  return time_.tempo(0).ticks2ms(notes_.back().end_in_ticks - notes_.front().start_in_ticks, divisions);
}

}
