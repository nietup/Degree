cmake_minimum_required(VERSION 3.6)
project(Degree)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES MA.cpp
        LineWrap.cpp
        LineWrap.h
        PointWrap.cpp
        PointWrap.h
        MatchingSystem.h
        Learning.h Vertex.h Edge.h Model.h TreeNode.h)
set(EXECUTABLE_NAME Degree)
add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(SFML 2 REQUIRED system window graphics network audio)
find_package(TGUI 0.7.2 REQUIRED)
find_package(BLAS)
find_package(LAPACK)

find_library(elsd_location elsd)

target_link_libraries(${EXECUTABLE_NAME} ${TGUI_LIBRARY} ${SFML_LIBRARIES} ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES} ${elsd_location})

