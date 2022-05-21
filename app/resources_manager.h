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
///\file graphics_manager.h
///\author FilipeCN (filipedecn@gmail.com)
///\date 2022-05-17
///
///\brief

#ifndef PIANOLA_APP_RESOURCES_MANAGER_H
#define PIANOLA_APP_RESOURCES_MANAGER_H

#include <circe/gl/graphics/shader.h>
#include <map>

class ResourcesManager {
public:
  ResourcesManager(ResourcesManager &&) = delete;
  ResourcesManager(const ResourcesManager &) = delete;
  ResourcesManager &operator=(const ResourcesManager &) = delete;

  static void addShader(const std::string &name);
  static const circe::gl::Program &getShader(const std::string &name);

private:
  ResourcesManager();

  static ResourcesManager instance_;
  std::map<std::string, circe::gl::Program> shaders_;
};

#endif //PIANOLA_APP_RESOURCES_MANAGER_H
