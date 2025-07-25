cmake_minimum_required(VERSION 3.20)

project(
    QtTriggerNote
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(external/MidiAppBackend)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia)
find_package(KDDockWidgets-qt6 CONFIG REQUIRED)
find_package(SndFile CONFIG REQUIRED)

qt_standard_project_setup()

qt_add_executable(QtTriggerNote
    src/main.cpp

    include/MainWindow.h src/MainWindow.cpp
    include/WaveformItem.h src/WaveformItem.cpp
    include/core/QtApp.h src/core/QtApp.cpp
    include/core/components/BindingListWidget.h src/core/components/BindingListWidget.cpp
    include/core/components/DevicesListWidget.h src/core/components/DevicesListWidget.cpp
    include/core/components/BindingSetupWidget.h src/core/components/BindingSetupWidget.cpp
    include/core/components/AudioSetupWidget.h src/core/components/AudioSetupWidget.cpp
    include/core/components/AudioListWidget.h src/core/components/AudioListWidget.cpp
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