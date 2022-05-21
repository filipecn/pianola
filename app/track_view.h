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
///\file track_view.h
///\author FilipeCN (filipedecn@gmail.com)
///\date 2022-05-14
///
///\brief

#ifndef PIANOLA_APP_TRACK_VIEW_H
#define PIANOLA_APP_TRACK_VIEW_H

#include <pianola/song.h>
#include <circe/circe.h>

struct TrackView {
  TrackView();
  explicit TrackView(pianola::Track song_track);
  TrackView(const TrackView &other);

  void draw(circe::CameraInterface *camera);
  void update();
  const pianola::Track &track() const;

  bool visible{true};
  std::string name;

private:
  void init();

  circe::gl::InstanceSet notes_;
  pianola::Track song_track_;
};

#endif //PIANOLA_APP_TRACK_VIEW_H
