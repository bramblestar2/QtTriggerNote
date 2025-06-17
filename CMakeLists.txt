cmake_minimum_required(VERSION 3.20)

project(
    QtTriggerNote
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(external/MidiAppBackend)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(QtTriggerNote
    src/main.cpp
)

add_dependencies(QtTriggerNote MidiAppCore)

target_link_libraries(QtTriggerNote
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        MidiAppCore
)