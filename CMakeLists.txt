# avoid policy warning message
if (POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)
# main project
project(PIANOLA)
cmake_minimum_required(VERSION 3.1)
cmake_policy(VERSION 3.1)
set(CMAKE_VERBOSE_MAKEFILE ON)
# path variables
set(ROOT_PROJECT_PATH "${PROJECT_SOURCE_DIR}")
set(BUILD_ROOT ${CMAKE_CURRENT_BINARY_DIR})
##########################################
##               options                ##
##########################################
option(BUILD_ALL "build all targets" OFF)
option(BUILD_TESTS "build library unit tests" OFF)
option(BUILD_DOCS "build library documentation" OFF)
# cmake modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})
# check for specific machine/compiler options.
include(ConfigCompilerFlags)
##########################################
##             dependencies             ##
##########################################
include(ExternalProject)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/ext")
include(hermes)
include(circe)
include(libremidi)
add_subdirectory(ext/pybind11)
##########################################
##               sources                ##
##########################################
set(PIANOLA_HEADERS
        pianola/midi_utils.h
        pianola/note.h
        pianola/song.h
        pianola/time_track.h
        pianola/track.h
        pianola/utils.h
        )
set(PIANOLA_SOURCES
        pianola/note.cpp
        pianola/song.cpp
        pianola/time_track.cpp
        pianola/track.cpp
        )
set(PIANOLA_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${LIBREMIDI_INCLUDES}
        ${HERMES_INCLUDES}
        )
add_library(pianola_lib STATIC ${PIANOLA_HEADERS} ${PIANOLA_SOURCES})
set_target_properties(pianola_lib PROPERTIES
        OUTPUT_NAME "pianola"
        FOLDER "PIANOLA")
target_compile_definitions(pianola_lib PUBLIC
        -DLIBREMIDI_ALSA=1
        -DLIBREMIDI_HEADER_ONLY=1)
target_include_directories(pianola_lib PUBLIC
        ${PIANOLA_INCLUDES}
        )
add_dependencies(pianola_lib hermes)

set(PIANOLA_LIBRARIES
        ${HERMES_LIBRARIES}
        ${LIBREMIDI_LIBRARIES}
        pianola_lib
        )

add_subdirectory(app)

##########################################
##                extra                 ##
##########################################
##########################################
##                tests                 ##
##########################################
if (BUILD_TESTS OR BUILD_ALL)
    include(catch2)
    add_subdirectory(tests)
endif (BUILD_TESTS OR BUILD_ALL)
##########################################
##           documentation              ##
##########################################
if (BUILD_DOCS)
    # check if Doxygen is installed
    # find_package(Doxygen)
endif (BUILD_DOCS)