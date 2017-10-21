cmake_minimum_required(VERSION 2.8)
project(threslogic_optimize)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
    -Wall -Wextra
    -O3 -std=c++11")

set(BASEPATH ${CMAKE_SOURCE_DIR})
set(INCPATH ${BASEPATH}/include)

include_directories(${BASEPATH} ${INCPATH})

set(SRC
  src/main.cpp
  src/common/arghandler.cpp
  src/logic/network.cpp
)
add_executable(thres-opt ${SRC})
