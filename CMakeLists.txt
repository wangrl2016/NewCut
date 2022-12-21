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
        media/base/audio_bus.cc
        media/base/audio_bus.h
        media/base/audio_parameters.cc
        media/base/channel_layout.cc
        media/base/channel_layout.h
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
        application.qrc
        main.cc
        src/view/arrow.cc
        src/view/arrow.h
        src/view/diagram_item.cc
        src/view/diagram_item.h
        src/view/diagram_scene.cc
        src/view/diagram_scene.h
        src/view/diagram_text_item.cc
        src/view/diagram_text_item.h
        src/view/main_window.cc
        src/view/main_window.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(NewCut
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
endif()

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

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(NewCut)
endif()

### Build examples

set (EXAMPLES
        )

foreach(EXAMPLE ${EXAMPLES}

        )

endforeach()

### Build tests
