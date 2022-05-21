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
///\file tempo.cpp
///\author FilipeCN (filipedecn@gmail.com)
///\date 2022-05-16
///
///\brief

#include "time_track.h"
#include <hermes/common/debug.h>

namespace pianola {

TimeTrack::TimeTrack() {
  tempo_changes_.emplace_back();
  time_signatures_.emplace_back();
}

TimeTrack::~TimeTrack() = default;

void TimeTrack::push(const libremidi::message &message, size_t tick_count) {
  switch (message.get_meta_event_type()) {
  case libremidi::meta_event_type::TEMPO_CHANGE: {
    Tempo tempo;
    tempo.tick_count = tick_count;
    int value = ((int) message.bytes[3] << 4) |
        ((int) message.bytes[4] << 2) |
        (int) message.bytes[5];
    tempo.microseconds_per_quarter_note = value;
    tempo.bpm = 60000 / value;
    tempo_changes_.emplace_back(tempo);
  }
    break;
  case libremidi::meta_event_type::SMPTE_OFFSET: break;
  case libremidi::meta_event_type::TIME_SIGNATURE: {
    TimeSignature time;
    time.tick_count = tick_count;
    time.beats_per_measure = (int) message.bytes[3];
    time.beat_unit = 1 << (int) message.bytes[4];
    time.midi_clocks_per_metronome_click = (int) message.bytes[5];
    time.thirty_second_notes_per_midi_quarter_note = (int) message.bytes[6];
    time_signatures_.emplace_back(time);
  }
    break;
  case libremidi::meta_event_type::PROPRIETARY: break;
  case libremidi::meta_event_type::UNKNOWN: break;
  default: HERMES_NOT_IMPLEMENTED
  }
}

std::ostream &operator<<(std::ostream &o, const TimeTrack &time_track) {
  for (const auto &time_signature : time_track.time_signatures_) {
    o << "[TIME SIGNATURE][" << hermes::Str::rjust(time_signature.beats_per_measure, 3);
    o << " /" << hermes::Str::rjust(time_signature.beat_unit, 3);
    o << "][MIDI clocks per metronome click " << hermes::Str::rjust(time_signature.midi_clocks_per_metronome_click, 4);
    o << "][32nd-notes per MIDI quarter note "
      << hermes::Str::rjust(time_signature.thirty_second_notes_per_midi_quarter_note, 4) << "]\n";
  }
  for (const auto &tempo_change : time_track.tempo_changes_) {
    o << "[TEMPO CHANGE][" << hermes::Str::rjust(tempo_change.microseconds_per_quarter_note, 5);
    o << " microseconds per quarter-note][" << hermes::Str::rjust(tempo_change.bpm, 3);
    o << " bpm]\n";
  }
  return o;
}

const TimeTrack::Tempo &TimeTrack::tempo(size_t tick_count) {
//  HERMES_LOG_WARNING("INCOMPLETE METHOD (getting default tempo!)");
  return tempo_changes_.front();
}

float TimeTrack::Tempo::ticks2ms(int ticks, float divisions) const {
  return ticks * (microseconds_per_quarter_note / divisions) / 1000.0;
}

}