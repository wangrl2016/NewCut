cmake_minimum_required(VERSION 3.20)

project(NewCut VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(KEEP_RPATHS)

### Build MultiMediaLibrary library

add_library(MultiMediaLibrary STATIC)
target_sources(MultiMediaLibrary
        PRIVATE
        base/memory/aligned_memory.cc
        base/memory/aligned_memory.h
        engine/geometry/flags.cc
        engine/geometry/flags.h
        media/base/audio_bus.cc
        media/base/audio_bus.h
        media/base/audio_parameters.cc
        media/base/channel_layout.cc
        media/base/channel_layout.h
        media/base/container_names.cc
        media/base/container_names.h
        media/base/sample_format.cc
        media/base/sample_format.h
        media/filters/audio_file_reader.cc
        media/filters/audio_file_reader.h
        media/filters/ffmpeg_glue.cc
        media/filters/ffmpeg_glue.h
        )

target_link_libraries(MultiMediaLibrary
        PRIVATE
        ${CONAN_LIBS}
        )

### Build NewCut executable

set(QT_LIBS
        concurrent
        core
        gui
        network
        widgets
        )

find_package(QT NAMES Qt6 Qt5
        REQUIRED COMPONENTS ${QT_LIBS})
find_package(Qt${QT_VERSION_MAJOR}
        REQUIRED COMPONENTS ${QT_LIBS})

set(PROJECT_SOURCES
        resources/application.qrc
        main.cc
        view/params/param_widget.cc
        view/params/param_widget.h
        view/piano/piano_graphics_scene.cc
        view/piano/piano_graphics_scene.h
        view/piano/piano_graphics_view.cc
        view/piano/piano_graphics_view.h
        view/tracks/track_graphics_scene.cc
        view/tracks/track_graphics_scene.h
        view/tracks/track_graphics_view.cc
        view/tracks/track_graphics_view.h
        view/main_window.cc
        view/main_window.h
        )

qt_add_executable(NewCut
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        )

target_link_libraries(NewCut PRIVATE
        ${CONAN_LIBS}
        MultiMediaLibrary
        Qt${QT_VERSION_MAJOR}::Concurrent
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Widgets
        )

set_target_properties(NewCut PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER www.qt.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING
        ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )

qt_finalize_executable(NewCut)

### Build examples

set(DIAGRAM_EXAMPLE_SOURCES
        resources/application.qrc
        examples/diagram/main.cc
        examples/diagram/arrow.cc
        examples/diagram/arrow.h
        examples/diagram/diagram_item.cc
        examples/diagram/diagram_item.h
        examples/diagram/diagram_scene.cc
        examples/diagram/diagram_scene.h
        examples/diagram/diagram_text_item.cc
        examples/diagram/diagram_text_item.h
        examples/diagram/main_window.cc
        examples/diagram/main_window.h
        )

qt_add_executable(DiagramExample
        MANUAL_FINALIZATION
        ${DIAGRAM_EXAMPLE_SOURCES}
        )

target_link_libraries(DiagramExample PRIVATE
        ${CONAN_LIBS}
        MultiMediaLibrary
        Qt${QT_VERSION_MAJOR}::Concurrent
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Widgets
        )

set_target_properties(DiagramExample PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER www.qt.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING
        ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )

qt_finalize_executable(DiagramExample)

### Build tests

enable_testing()

qt_add_executable(NewCutTest
        MANUAL_FINALIZATION
        media/base/container_names_test.cc
        )

target_link_libraries(NewCutTest PRIVATE
        ${CONAN_LIBS}
        MultiMediaLibrary
        Qt${QT_VERSION_MAJOR}::Concurrent
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Widgets
        )

qt_finalize_executable(NewCutTest)

include(GoogleTest)
gtest_discover_tests(NewCutTest)
