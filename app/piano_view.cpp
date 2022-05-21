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
///\file piano_view.cpp
///\author FilipeCN (filipedecn@gmail.com)
///\date 2022-05-14
///
///\brief

#include "piano_view.h"

PianoView::PianoView() {
  hermes::Path shaders_path(std::string(SHADERS_PATH));
  keys_.instance_model = circe::Shapes::plane(hermes::Plane::XY(), {}, {0.5, 0, 0}, 1,
                                              circe::shape_options::uv);
  HERMES_CHECK_EXP_WITH_LOG(keys_.instance_program.link(shaders_path, "piano_key"),
                            keys_.instance_program.err);
  keys_.resize(88);
  key_state_.resize(88, false);
}

void PianoView::render(circe::CameraInterface *camera) {
  if (needs_update_) {
    auto instance_data = keys_.instanceData();
    for (size_t i = 0; i < 88; i++) {
      auto color = circe::Color::Red();
      instance_data.at<circe::Color>("color", i) = color;
      auto c = ViewPreferences::keyCenter(i);
      const auto &s = pianola::isKeyBlack(i) ? ViewPreferences::black_key_size : ViewPreferences::white_key_size;
      auto transform =
          hermes::Transform::translate(hermes::vec3(c.x, c.y, pianola::isKeyBlack(i) ? -0.5 : 0)) *
              hermes::Transform::scale(s.x, s.y, 1);
      instance_data.at<hermes::mat4>("transform_matrix", i) = hermes::transpose(transform.matrix());
      needs_update_ = false;
    }
  }
  keys_.draw(camera, {});
}

void PianoView::pressKey(int piano_key) {
  key_state_[piano_key] = true;
  needs_update_ = true;
}

void PianoView::releaseKey(int piano_key) {
  key_state_[piano_key] = false;
  needs_update_ = true;
}

void PianoView::update() {
  needs_update_ = true;
}
