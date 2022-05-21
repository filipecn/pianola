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
///\file midi_utils.h
///\author FilipeCN (filipedecn@gmail.com)
///\date 2022-05-05
///
///\brief

#ifndef PIANOLA_SRC_MIDI_UTILS_H
#define PIANOLA_SRC_MIDI_UTILS_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <hermes/common/str.h>
#include <libremidi/message.hpp>

namespace pianola {

inline double ticks2ms(int ticks, int tempo, int divisions) {
  return ticks * (static_cast<double>(tempo) / divisions) / 1000.0;
}

inline std::string noteCode2str(unsigned char code, bool use_sharp) {
  static const char *note_names[2][12] = {
      {"A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab"},
      {"A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"}};
  // code 21 is A0 (piano key number 1)
  int offset = (int) code - 21;
  // compute octave
  int octave = offset / 12;
  int octave_note = offset % 12;
  return hermes::Str::concat(note_names[use_sharp][octave_note], octave);
}

inline std::string messageType2str(const libremidi::message_type &type) {
#define STRFROM(MESSAGE_TYPE)                                  \
  if(type == libremidi::message_type::MESSAGE_TYPE)            \
  return #MESSAGE_TYPE;

  STRFROM(INVALID)
  STRFROM(NOTE_OFF)
  STRFROM(NOTE_ON)
  STRFROM(POLY_PRESSURE)
  STRFROM(CONTROL_CHANGE)
  STRFROM(PROGRAM_CHANGE)
  STRFROM(AFTERTOUCH)
  STRFROM(PITCH_BEND)
  STRFROM(SYSTEM_EXCLUSIVE)
  STRFROM(TIME_CODE)
  STRFROM(SONG_POS_POINTER)
  STRFROM(SONG_SELECT)
  STRFROM(RESERVED1)
  STRFROM(RESERVED2)
  STRFROM(TUNE_REQUEST)
  STRFROM(EOX)
  STRFROM(TIME_CLOCK)
  STRFROM(RESERVED3)
  STRFROM(START)
  STRFROM(CONTINUE)
  STRFROM(STOP)
  STRFROM(RESERVED4)
  STRFROM(ACTIVE_SENSING)
  STRFROM(SYSTEM_RESET)

#undef STRFROM
}

inline std::string metaEventType2str(libremidi::meta_event_type type) {
#define STRFROM(META_EVENT_TYPE)         \
  if(type == libremidi::meta_event_type::META_EVENT_TYPE)            \
  return #META_EVENT_TYPE;

  STRFROM(SEQUENCE_NUMBER)
  STRFROM(TEXT)
  STRFROM(COPYRIGHT)
  STRFROM(TRACK_NAME)
  STRFROM(INSTRUMENT)
  STRFROM(LYRIC)
  STRFROM(MARKER)
  STRFROM(CUE)
  STRFROM(PATCH_NAME)
  STRFROM(DEVICE_NAME)
  STRFROM(CHANNEL_PREFIX)
  STRFROM(MIDI_PORT)
  STRFROM(END_OF_TRACK)
  STRFROM(TEMPO_CHANGE)
  STRFROM(SMPTE_OFFSET)
  STRFROM(TIME_SIGNATURE)
  STRFROM(KEY_SIGNATURE)
  STRFROM(PROPRIETARY)
  STRFROM(UNKNOWN)

#undef STRFROM
}

inline std::string messageBytes2str(const libremidi::message &message) {
  hermes::Str s;
  std::vector<std::string> hex_bytes;
  s += '[';
  for (auto b : message)
    hex_bytes.emplace_back(hermes::Str::toHex((i32) b));
  s += hermes::Str::join(hex_bytes, " ");
  s += "]";
  return s.str();
}

inline std::string metaEvent2str(const libremidi::message &message) {
  hermes::Str s;
  s += hermes::Str::ljust(metaEventType2str(message.get_meta_event_type()), 20);
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
  case libremidi::meta_event_type::DEVICE_NAME:s.append("[");
    for (size_t i = 3; i < message.bytes.size(); ++i)
      if (message.bytes[i] == 0xA)
        s.append("[NL]");
      else
        s.append(message.bytes[i]);
    s.append("]");
    break;
  case libremidi::meta_event_type::CHANNEL_PREFIX: break;
  case libremidi::meta_event_type::MIDI_PORT: break;
  case libremidi::meta_event_type::END_OF_TRACK: break;
  case libremidi::meta_event_type::TEMPO_CHANGE: {
    int value = ((int) message.bytes[3] << 4) |
        ((int) message.bytes[4] << 2) |
        (int) message.bytes[5];
    s.append("[", hermes::Str::rjust(value, 5), " microseconds per quarter-note][",
             hermes::Str::rjust(60000 / value, 3), " bpm]");
  }
    break;
  case libremidi::meta_event_type::SMPTE_OFFSET: break;
  case libremidi::meta_event_type::TIME_SIGNATURE:
    s.append("[", hermes::Str::rjust((int) message.bytes[3], 3), " /",
             hermes::Str::rjust(1 << (int) message.bytes[4], 3), "][",
             "MIDI clocks per metronome click ", hermes::Str::rjust((int) message.bytes[5], 4), "][",
             "32nd-notes per MIDI quarter note ", hermes::Str::rjust((int) message.bytes[6], 4), "]");
    break;
  case libremidi::meta_event_type::KEY_SIGNATURE: {
    int sf = 0xffffff00 | message.bytes[3];
    s.append("[");
    if (sf == 0)
      s.append("key of C");
    else
      s.append(sf < 0 ? -sf : sf, sf < 0 ? " flat" : " sharp", std::abs(sf) > 1 ? "s" : "");
    s.append("][", message.bytes[4] ? "minor" : "major", " key]");
  }
    break;
  case libremidi::meta_event_type::PROPRIETARY: break;
  case libremidi::meta_event_type::UNKNOWN: break;
  }
  return s.str();
}

inline std::string message2str(const libremidi::message &message) {
  hermes::Str s;
  s += hermes::Str::ljust(messageType2str(message.get_message_type()), 20);
  s += "[ch" + hermes::Str::rjust(message.get_channel(), 3) + "]";
  switch (message.get_message_type()) {
  case libremidi::message_type::INVALID: break;
  case libremidi::message_type::NOTE_OFF:
  case libremidi::message_type::NOTE_ON: {
    s.append("[note ", hermes::Str::ljust(noteCode2str(message.bytes[1], true), 4), "]");
    s.append("[velocity ", hermes::Str::rjust((int) message.bytes[2], 4), "]");
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

  return s.str();
}

inline std::string event2str(const libremidi::track_event &event) {
  hermes::Str s;
  s = s << hermes::Str::rjust(event.tick, 10) << " ";
  return s.str() + (event.m.is_meta_event() ? metaEvent2str(event.m) : message2str(event.m))
      + messageBytes2str(event.m);
}

}

inline std::ostream &operator<<(std::ostream &o, libremidi::message_type message) {
  o << pianola::messageType2str(message);
  return o;
}

inline std::stringstream &operator<<(std::stringstream &s, libremidi::message_type message) {
  return s;
}

#endif //PIANOLA_SRC_MIDI_UTILS_H
