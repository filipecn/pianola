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
///\file graphics_manager.cpp
///\author FilipeCN (filipedecn@gmail.com)
///\date 2022-05-17
///
///\brief

#include "resources_manager.h"

ResourcesManager ResourcesManager::instance_;

ResourcesManager::ResourcesManager() = default;

void ResourcesManager::addShader(const std::string &name) {
  hermes::Path shaders_path(std::string(SHADERS_PATH));
  circe::gl::Program shader;
  HERMES_CHECK_EXP_WITH_LOG(shader.link(shaders_path, name), shader.err);
  instance_.shaders_["note_bar"] = std::move(shader);
}

const circe::gl::Program &ResourcesManager::getShader(const std::string &name) {
  if (instance_.shaders_.find(name) == instance_.shaders_.end())
    return {};
  return instance_.shaders_[name];
}
