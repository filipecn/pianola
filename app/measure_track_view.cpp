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
///\file measure_track_view.cpp
///\author FilipeCN (filipedecn@gmail.com)
///\date 2022-05-14
///
///\brief

#include "measure_track_view.h"
#include "view_preferences.h"

MeasureTrackView::MeasureTrackView() = default;

void MeasureTrackView::render(circe::CameraInterface *camera) {
  if (bars_.count())
    bars_.draw(camera, {});
}

void MeasureTrackView::set(const pianola::Track &track, float divisions) {
  bars_.program_handle = *circe::gl::ProgramManager::programHandle("bar");
  auto r = circe::gl::SceneResourceManager::pushModel(
      circe::Shapes::plane(hermes::Plane::XY(), {}, {0.5, 0, 0}, 1, circe::shape_options::uv));
  bars_.model_handle = r.value();
  auto bpm = track.timeTrack().tempo(0).bpm;
  auto beats_per_millisecond = bpm / (60. * 1000.);
  auto track_duration = track.durationInMilliseconds(divisions);
  beat_duration_in_milliseconds = 1. / beats_per_millisecond;
  HERMES_LOG_VARIABLE(track_duration);
  HERMES_LOG_VARIABLE(beats_per_millisecond);
  HERMES_LOG_VARIABLE(beat_duration_in_milliseconds);
  // compute number of beats
  size_t n = std::max((size_t) 10, static_cast<size_t>(beats_per_millisecond * track_duration));
  bars_.resize(n);
  update();
}

void MeasureTrackView::update() {
  if (!bars_.count() || !bars_.good())
    return;
  auto instance_data = bars_.instanceData();
  for (size_t i = 0; i < bars_.count(); i++) {
    auto color = circe::Color::Red();
    instance_data.at<circe::Color>("color", i) = color;
    auto sx = 88 * (ViewPreferences::white_key_size.x + ViewPreferences::key_gap);
    auto transform =
        hermes::Transform::translate(hermes::vec3(sx / 2,
                                                  i * beat_duration_in_milliseconds
                                                      * ViewPreferences::length_per_millisecond,
                                                  10)) *
            hermes::Transform::scale(sx, 0.1, 1);
    instance_data.at<hermes::mat4>("transform_matrix", i) = hermes::transpose(transform.matrix());
  }
}
