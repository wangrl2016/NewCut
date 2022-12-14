cmake_minimum_required(VERSION 3.20)

project(NewCut VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(KEEP_RPATHS)

configure_file(config/user_config.h.in user_config.h)

### Build MultiMediaLibrary library

add_library(MultiMediaLibrary STATIC)
target_sources(MultiMediaLibrary
        PRIVATE
        base/containers/ring_buffer.h
        base/files/file_path.cc
        base/files/file_path.h
        base/files/file_util.cc
        base/files/file_util.h
        base/memory/aligned_memory.cc
        base/memory/aligned_memory.h
        base/memory/atomic_ref_count.h
        base/memory/ref_counted.cc
        base/memory/ref_counted.h
        base/memory/ref_counted_test.cc
        base/misc/bits.h
        base/strings/string_piece.cc
        base/strings/string_piece.h
        base/strings/string_util.cc
        base/strings/string_util.h
        base/strings/string_util_helper.h
        base/strings/string_util_posix.h
        base/strings/string_util_win.cc
        base/strings/string_util_win.h
        base/strings/utf_string_conversions.cc
        base/strings/utf_string_conversions.h
        base/time/time.cc
        base/time/time.h
        config/build_config.h
        media/base/audio_buffer.cc
        media/base/audio_buffer.h
        media/base/audio_bus.cc
        media/base/audio_bus.h
        media/base/audio_codec.cc
        media/base/audio_codec.h
        media/base/audio_parameters.cc
        media/base/audio_parameters.h
        media/base/audio_sample_types.h
        media/base/channel_layout.cc
        media/base/channel_layout.h
        media/base/container_names.cc
        media/base/container_names.h
        media/base/decoder_buffer.cc
        media/base/decoder_buffer.h
        media/base/decoder.cc
        media/base/decoder.h
        media/base/limits.h
        media/base/sample_format.cc
        media/base/sample_format.h
        media/base/test_data_util.cc
        media/base/test_data_util.h
        media/base/vector_math.cc
        media/base/vector_math.h
        media/ffmpeg/ffmpeg_common.cc
        media/ffmpeg/ffmpeg_common.h
        media/ffmpeg/ffmpeg_deleters.cc
        media/ffmpeg/ffmpeg_deleters.h
        media/filters/audio_file_reader.cc
        media/filters/audio_file_reader.h
        media/filters/ffmpeg_glue.cc
        media/filters/ffmpeg_glue.h
        media/filters/memory_url_protocol.cc
        media/filters/memory_url_protocol.h
        modules/audio_graph/audio_node.cc
        modules/audio_graph/audio_node.h
        third_party/icu/icu_utf.h
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
        newcut/actions/action_draw_line_free.cc
        newcut/actions/action_draw_line_free.h
        newcut/actions/action_file_new.cc
        newcut/actions/action_file_new.h
        newcut/actions/action_interface.cc
        newcut/actions/action_interface.h
        newcut/actions/preview_action_interface.cc
        newcut/actions/preview_action_interface.h
        newcut/actions/snapper.cc
        newcut/actions/snapper.h
        newcut/engine/color.cc
        newcut/engine/color.h
        newcut/engine/document.cc
        newcut/engine/document.h
        newcut/engine/entity.cc
        newcut/engine/entity.h
        newcut/engine/entity_container.cc
        newcut/engine/entity_container.h
        newcut/engine/enum_collect.cc
        newcut/engine/enum_collect.h
        newcut/engine/flags.cc
        newcut/engine/flags.h
        newcut/engine/layer.cc
        newcut/engine/layer.h
        newcut/engine/math.cc
        newcut/engine/math.h
        newcut/engine/pen.cc
        newcut/engine/pen.h
        newcut/engine/polyline.cc
        newcut/engine/polyline.h
        newcut/engine/quadratic.cc
        newcut/engine/quadratic.h
        newcut/engine/settings.cc
        newcut/engine/settings.h
        newcut/engine/undoable.cc
        newcut/engine/undoable.h
        newcut/engine/vector.cc
        newcut/engine/vector.h
        newcut/gui/action_factory.cc
        newcut/gui/action_factory.h
        newcut/gui/action_group_manager.cc
        newcut/gui/action_group_manager.h
        newcut/gui/action_handler.cc
        newcut/gui/action_handler.h
        newcut/gui/central_widget.cc
        newcut/gui/central_widget.h
        newcut/gui/widget_factory.cc
        newcut/gui/widget_factory.h
        newcut/gui/graphic_view.cc
        newcut/gui/graphic_view.h
        newcut/gui/mdi_window.cc
        newcut/gui/mdi_window.h
        newcut/application_window.cc
        newcut/application_window.h
        newcut/main_window.cc
        newcut/main_window.h
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
        MACOSX_BUNDLE_GUI_IDENTIFIER www.newcut.com
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

qt_finalize_executable(DiagramExample)

### Build tests

enable_testing()

qt_add_executable(NewCutTest
        MANUAL_FINALIZATION
        base/files/file_path_test.cc
        base/strings/string_util_test.cc
        base/strings/utf_string_conversions_test.cc
        media/base/audio_bus_test.cc
        media/base/audio_sample_types_test.cc
        media/base/container_names_test.cc
        media/filters/audio_file_reader_test.cc
        media/filters/ffmpeg_glue_test.cc
        media/filters/memory_url_protocol_test.cc
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
