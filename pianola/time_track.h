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
///\file tempo.h
///\author FilipeCN (filipedecn@gmail.com)
///\date 2022-05-16
///
///\brief

#ifndef PIANOLA_PIANOLA_TIME_TRACK_H
#define PIANOLA_PIANOLA_TIME_TRACK_H

#include <pianola/midi_utils.h>
#include <vector>

namespace pianola {

class TimeTrack {
public:

  struct TimeSignature {
    size_t tick_count{0};
    int beats_per_measure{4};
    int beat_unit{4};
    int midi_clocks_per_metronome_click{0};
    int thirty_second_notes_per_midi_quarter_note{0};
  };

  struct Tempo {
    size_t tick_count{0};
    int microseconds_per_quarter_note{500000}; // micro-seconds / quarter-note
    float bpm{120}; // beats per minute
    ///
    /// \param ticks
    /// \param divisions ticks / qn
    /// \return
    [[nodiscard]] float ticks2ms(int ticks, float divisions) const;
  };

  TimeTrack();
  ~TimeTrack();

  /// \param tick_count
  /// \return tempo at tick_count
  [[nodiscard]] const Tempo& tempo(size_t tick_count) const;
  void push(const libremidi::message &message, size_t tick_count);
  friend std::ostream &operator<<(std::ostream &o, const TimeTrack &time_track);

private:
  std::vector<TimeSignature> time_signatures_;
  std::vector<Tempo> tempo_changes_;
};

}

#endif //PIANOLA_PIANOLA_TIME_TRACK_H
