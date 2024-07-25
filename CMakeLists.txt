# The cmake scripts for quickjs build, especially for windows
# Author: BBDXF 
# Url: https://github.com/BBDXF/quickjs_cmake
#

cmake_minimum_required(VERSION 3.14)
project(quickjs_cmake)

set(CMAKE_CXX_STANDARD 11)

# default CMAKE_BUILD_TYPE
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message("Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)

  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

message(STATUS "current used C compiler: ${CMAKE_C_COMPILER}, CXX compiler: ${CMAKE_CXX_COMPILER}.")

# load quickjs first
include("quickjs.cmake")
message(STATUS "quickjs library and binary build finished.")

###################################################
# quickjs app demo
message(STATUS ">>> Begain demo app build >>>")

set(DEMO_SRC
    "demo.c"
    "demo.h"
)
message(STATUS "DEMO_SRC ${DEMO_SRC}")

add_executable (demo ${DEMO_SRC})
target_link_libraries(demo PUBLIC quickjs)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET demo PROPERTY CXX_STANDARD 20)
endif()


