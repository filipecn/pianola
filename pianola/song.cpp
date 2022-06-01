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
///\file song.cpp
///\author FilipeCN (filipedecn@gmail.com)
///\date 2022-05-08
///
///\brief

#include <libremidi/reader.hpp>
#include <pianola/song.h>
#include <hermes/logging/memory_dump.h>

namespace pianola {

Song Song::fromFile(const hermes::Path &path) {
  libremidi::reader reader;

  // load and parse midi file
  auto bytes = hermes::FileSystem::readBinaryFile(path);
  libremidi::reader::parse_result result{};
  result = reader.parse(bytes);
  HERMES_ASSERT(result == libremidi::reader::validated);
//  hermes::MemoryDumper::dump(bytes.data(),
//                             bytes.size(),
//                             18,
//                             {},
//                             hermes::memory_dumper_options::show_ascii
//                                 | hermes::memory_dumper_options::write_to_console);

  Song song;
  song.divisions_ = reader.ticksPerBeat;
  // extract midi messages
  for (const auto &midi_track : reader.tracks) {
    Track track;
    size_t tick_count = 0;
    for (const auto &event : midi_track) {
      tick_count += event.tick;
      track.push(event, tick_count, song.divisions_);
    }
    song.tracks_.emplace_back(track);
  }

  return song;
}

const std::vector<Track> &Song::tracks() const {
  return tracks_;
}

std::ostream &operator<<(std::ostream &o, const Song &song) {
  o << "[SONG][divisions " << song.divisions_ << "]";
  o << "[" << song.tracks_.size() << " tracks]\n";
  for (const auto &track : song.tracks_)
    o << "[" << track.size() << " notes]";
  o << "\n";
  for (const auto &track : song.tracks_)
    o << track;
  return o;
}

}
