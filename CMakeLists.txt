cmake_minimum_required(VERSION 3.16)

project(SnakeRewind VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTORCC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

foreach(CONFIG_TYPE Debug Release RelWithDebInfo MinSizeRel)
    string(TOUPPER ${CONFIG_TYPE} CONFIG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_UPPER} ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG_UPPER} ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_UPPER} ${CMAKE_SOURCE_DIR}/bin)
endforeach()

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(SnakeRewind WIN32
    src/main.cpp
    src/GameTypes.h
    src/GameState.h
    src/GameState.cpp
    src/GameBoardWidget.h
    src/GameBoardWidget.cpp
    src/MainWindow.h
    src/MainWindow.cpp
    src/resources.qrc
    src/app_icon.rc
)

target_link_libraries(SnakeRewind PRIVATE Qt6::Widgets)

if (MSVC)
    set_target_properties(SnakeRewind PROPERTIES
        PDB_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
        PDB_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin
        PDB_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin
        PDB_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/bin
        PDB_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_SOURCE_DIR}/bin
    )
endif()
