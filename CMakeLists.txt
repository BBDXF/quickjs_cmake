# The cmake scripts for quickjs build, especially for windows
# Author: BBDXF 
# Url: https://github.com/BBDXF/quickjs_cmake
#
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


