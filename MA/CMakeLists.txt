cmake_minimum_required(VERSION 3.6)
project(Degree)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES MA.cpp
        #Abstract.cpp
        #Figure.cpp
        #MA.cpp
        #MenuState.cpp
        #Model.cpp
        #NewModelState.cpp
        #Primitive.cpp
        #Relationship.cpp
        #SampleSelectionState.cpp
        #Solid.cpp
        #State.cpp
        #StateMachine.cpp
        #WindowPack.cpp
        #ModelSelectionState.cpp
        #ModelSelectionState.h
        #ImageHandler.cpp
        #ImageHandler.h
        LineWrap.cpp
        LineWrap.h
        PointWrap.cpp
        PointWrap.h
        MatchingSystem.h
        Learning.h Atom.h Part.h Model.h TreeNode.h Matcher.cpp Matcher.h Hipothesis.cpp Hipothesis.h Generator.cpp Generator.h)
set(EXECUTABLE_NAME Degree)
add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(SFML 2 REQUIRED system window graphics network audio)
find_package(TGUI 0.7.2 REQUIRED)
find_package(BLAS)
find_package(LAPACK)

find_library(elsd_location elsd)

target_link_libraries(${EXECUTABLE_NAME} ${TGUI_LIBRARY} ${SFML_LIBRARIES} ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES} ${elsd_location})

