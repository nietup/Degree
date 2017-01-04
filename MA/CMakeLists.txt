cmake_minimum_required(VERSION 3.6)
project(Degree)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES MA.cpp
        Abstract.cpp
        Figure.cpp
        MA.cpp
        MenuState.cpp
        Model.cpp
        NewModelState.cpp
        Primitive.cpp
        Relationship.cpp
        SampleSelectionState.cpp
        Solid.cpp
        State.cpp
        StateMachine.cpp
        WindowPack.cpp
        ModelSelectionState.cpp ModelSelectionState.h)
set(EXECUTABLE_NAME Degree)
add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(SFML 2 REQUIRED system window graphics network audio)
find_package(TGUI 0.7.2 REQUIRED)

if(NOT SFML_FOUND)
    message(FATAL_ERROR "Could not find SFML")
endif()

if(NOT TGUI_FOUND)
    message(FATAL_ERROR "Could not find TGUI")
endif()

target_link_libraries(${EXECUTABLE_NAME} ${TGUI_LIBRARY} ${SFML_LIBRARIES})
