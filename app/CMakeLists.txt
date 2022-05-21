set(HEADERS
        resources_manager.h
        measure_track_view.h
        piano_view.h
        track_view.h
        view_preferences.h
        )
set(SOURCES
        main.cpp
        resources_manager.cpp
        measure_track_view.cpp
        piano_view.cpp
        track_view.cpp
        view_preferences.cpp
        )

add_executable(pianola ${SOURCES} ${HEADERS})
target_compile_definitions(pianola_lib PUBLIC
        -DSHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets"
        -DLIBREMIDI_ALSA=1
        -DLIBREMIDI_HEADER_ONLY=1)
target_include_directories(pianola PUBLIC
        ${PIANOLA_INCLUDES}
        ${CIRCE_INCLUDES}
        )
target_link_libraries(pianola PUBLIC
        ${CIRCE_LIBRARIES}
        ${PIANOLA_LIBRARIES}
        )

add_dependencies(pianola hermes circe pianola_lib)
