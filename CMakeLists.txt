CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
PROJECT(open_mp)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

FILE(GLOB_RECURSE source_files "source/*")
ADD_LIBRARY(open_mp_lib STATIC ${source_files})
TARGET_INCLUDE_DIRECTORIES(open_mp_lib PUBLIC "include/")


ADD_EXECUTABLE(open_mp main.cpp)

TARGET_LINK_LIBRARIES(open_mp open_mp_lib)

