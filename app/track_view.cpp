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
///\file track_view.cpp
///\author FilipeCN (filipedecn@gmail.com)
///\date 2022-05-14
///
///\brief

#include <pianola/utils.h>

#include <utility>
#include "track_view.h"
#include "view_preferences.h"

TrackView::TrackView() = default;

TrackView::TrackView(const TrackView &other) {
  song_track_ = other.song_track_;
  notes_ = other.notes_;
}

TrackView::TrackView(const pianola::Track& song_track) : TrackView() {
  set(song_track);
}

void TrackView::draw(circe::CameraInterface *camera) {
  if (visible)
    notes_.draw(camera, {});
}

void TrackView::set(const pianola::Track &song_track) {
  notes_.program_handle = *circe::gl::ProgramManager::programHandle("piano_key");
  auto r = circe::gl::SceneResourceManager::pushModel(
      circe::Shapes::plane(hermes::Plane::XY(), {}, {0.5, 0, 0}, 1, circe::shape_options::uv));
  notes_.model_handle = r.value();
  song_track_ = std::move(song_track);
  if (song_track_.size())
    init();
}

void TrackView::init() {
  notes_.resize(song_track_.size());
  update();
}

void TrackView::update() {
  if (!notes_.count() || !notes_.good())
    return;
  auto instance_data = notes_.instanceData();
  const auto &notes = song_track_.notes();
  for (size_t i = 0; i < notes.size(); ++i) {
    const auto &note = notes[i];
    instance_data.at<circe::Color>("color", i) = circe::Color::Red();
    // piano note
    int piano_note = notes[i].midi_code - 21;
    auto sx = pianola::isKeyBlack(piano_note) ? ViewPreferences::black_key_size.x : ViewPreferences::white_key_size.x;
    auto sy = note.duration_in_ms * ViewPreferences::length_per_millisecond;
    auto cx = ViewPreferences::noteCenter(piano_note);
    auto cy = note.start_in_ms * ViewPreferences::length_per_millisecond + sy / 2;
    auto transform =
        hermes::Transform::translate(hermes::vec3(cx, cy, pianola::isKeyBlack(i) ? -0.5 : 0)) *
            hermes::Transform::scale(sx, sy, 1);
    instance_data.at<hermes::mat4>("transform_matrix", i) = hermes::transpose(transform.matrix());
  }
}

const pianola::Track &TrackView::track() const {
  return song_track_;
}

std::string TrackView::debugInfo() {
  hermes::Str s;
  s.appendLine("[TRACK VIEW][DEBUG DATA][", name, "][", song_track_.size(), "]");
  s.appendLine("\t[buffer][id ", notes_.buffer().id(), "][", notes_.count(), " instances]");
  s.append(notes_.instanceData().memoryDump());
  return s.str();
}

