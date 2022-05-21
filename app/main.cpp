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
///\file pianola.cpp
///\author FilipeCN (filipedecn@gmail.com)
///\date 2022-05-04
///
///\brief

#include <circe/circe.h>
#include <pianola/song.h>
#include "piano_view.h"
#include "track_view.h"
#include "resources_manager.h"

struct PianolaApp : public circe::gl::BaseApp {
  PianolaApp() : circe::gl::BaseApp(800, 800, "Pianola", circe::viewport_options::orthographic) {
    // load resources
    ResourcesManager::addShader("bar");
    ResourcesManager::addShader("note");
    ResourcesManager::addShader("piano_key");
    //
    hermes::profiler::Profiler::setMaxBlockCount(200);
    this->app->viewport().clear_screen_color = circe::Color::Black();
  }

  void render(circe::CameraInterface *camera) override {
    renderMainMenuBar();
    renderDebugWindow();
    renderPreferencesWindow();
    renderMIDISongWindow();
    profiler_view.update();
    grid.draw(camera);
    keyboard_view.render(camera);
    for (auto &track : track_views)
      track.draw(camera);
  }

  void renderMainMenuBar() {
    ImGui::BeginMainMenuBar();
    auto file_pick = circe::ImguiOpenDialog::file_dialog_button("Open", ".mid", "/home/filipecn/Desktop/");
    if (file_pick)
      loadMidiFile(file_pick.path);
    ImGui::EndMainMenuBar();
  }

  void loadMidiFile(const hermes::Path &path) {
    song = pianola::Song::fromFile(path);
//    std::cout << song << std::endl;
    auto tracks = song.tracks();
    for (const pianola::Track &track : tracks)
      track_views.emplace_back(track);
  }

  void renderDebugWindow() {
    ImGui::Begin("Stats");
    profiler_view.render();
    logger_view.render();
    ImGui::End();
  }

  void renderPreferencesWindow() {
    ImGui::Begin("Preferences");
    bool needs_update = false;
#define VIEW_PREFERENCE(A) \
    if(A) needs_update = true
    VIEW_PREFERENCE(ImGui::SliderFloat("key gap", &ViewPreferences::key_gap, 0, 0.1));
    VIEW_PREFERENCE(ImGui::SliderFloat("key width", &ViewPreferences::white_key_size.x, 0.1, 2));
    VIEW_PREFERENCE(ImGui::SliderFloat("key height", &ViewPreferences::white_key_size.y, 0.1, 2));
    VIEW_PREFERENCE(ImGui::SliderFloat("black key width", &ViewPreferences::black_key_size.x, 0.1, 2));
    VIEW_PREFERENCE(ImGui::SliderFloat("black key height", &ViewPreferences::black_key_size.y, 0.1, 2));
    VIEW_PREFERENCE(ImGui::SliderFloat("length per ms", &ViewPreferences::length_per_millisecond, 0.001, 0.01));
#undef VIEW_PREFERENCE
    if (needs_update) {
      keyboard_view.update();
      for (auto &track : track_views)
        track.update();
    }

    ImGui::End();
  }

  void renderMIDISongWindow() {
    ImGui::Begin("MIDI Song");
    // visibility checkboxes
    int track_id = 0;
    for (auto &track_view : track_views) {
      ImGui::Checkbox(hermes::Str::concat("Track ", track_id++, ": ", track_view.name).c_str(), &track_view.visible);
      ImGui::Text("%s", hermes::Str::concat(track_view.track().size(), " notes").c_str());
      ImGui::Separator();
    }
    ImGui::End();
  }

  // midi song
  pianola::Song song;

  // ui
  circe::HProfiler profiler_view;
  circe::HLogger logger_view;

  // vis
  PianoView keyboard_view;
  std::vector<TrackView> track_views;
  circe::gl::helpers::CartesianGrid grid;
};

int main() {
  return PianolaApp().run();
}
