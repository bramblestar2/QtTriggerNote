cmake_minimum_required(VERSION 3.20)

project(
    QtTriggerNote
    VERSION 1.0
    LANGUAGES CXX
)



set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIBREMIDI_HEADER_ONLY ON CACHE BOOL "Use header‑only mode" FORCE)

set(KDDockWidgets_FRONTENDS "qtwidgets" CACHE STRING "KDDockWidgets frontends" FORCE)
set(KDDockWidgets_QT6 ON CACHE BOOL "Use Qt6" FORCE)


# find_package(KDDockWidgets-qt6 CONFIG REQUIRED)
find_package(SndFile CONFIG REQUIRED)

include(FetchContent)

FetchContent_Declare(
    MidiApp
    GIT_REPOSITORY https://github.com/bramblestar2/MidiAppBackend.git
    GIT_TAG origin/audio-revamp
)

FetchContent_MakeAvailable(MidiApp)

FetchContent_Declare(
    KDDockWidgets
    GIT_REPOSITORY https://github.com/KDAB/KDDockWidgets.git
    GIT_TAG v2.2.5
)

FetchContent_MakeAvailable(KDDockWidgets)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

qt_standard_project_setup()

qt_add_executable(QtTriggerNote
    src/main.cpp

    resources/resources.qrc

    include/MainWindow.h src/MainWindow.cpp
    include/WaveformItem.h src/WaveformItem.cpp
    
    include/core/QtApp.h src/core/QtApp.cpp
    include/core/Application.h src/core/Application.cpp
    include/core/AppState.h src/core/AppState.cpp

    include/core/windows/ProjectWindow.h src/core/windows/ProjectWindow.cpp

    include/core/components/AssetManagerWidget.h src/core/components/AssetManagerWidget.cpp
    include/core/components/AudioLoader.h src/core/components/AudioLoader.cpp
    
    include/core/pages/AudioEditorWidget.h src/core/pages/AudioEditorWidget.cpp
)

add_dependencies(QtTriggerNote MidiAppCore)

target_link_libraries(QtTriggerNote
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Multimedia

        KDAB::kddockwidgets
        SndFile::sndfile

        MidiAppCore
)

target_include_directories(QtTriggerNote
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

include(GNUInstallDirs)

install(TARGETS QtTriggerNote
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

if (KDDW_DLL_PATH)
    install(FILES ${KDDW_DLL_PATH} DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()