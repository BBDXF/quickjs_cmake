# The cmake scripts for quickjs build, especially for windows
# Author: BBDXF 
# Url: https://github.com/BBDXF/quickjs_cmake
#
##### quickjs library
message(STATUS ">>> Begain quickjs library build >>>")

# read quickjs version from file
file(STRINGS  "./quickjs/VERSION" QJS_VERSION)
message(STATUS "quickjs version: [${QJS_VERSION}]")

# flags for all
add_definitions(-DCONFIG_BIGNUM)
add_definitions(-DCONFIG_VERSION="${QJS_VERSION}")

# all headers
include_directories("./quickjs/")

# qjs library files
set(QJS_FILES 
    "./quickjs/cutils.c"
    "./quickjs/libbf.c"
    "./quickjs/libregexp.c"
    "./quickjs/libunicode.c"
    "./quickjs/quickjs-libc.c"
    "./quickjs/quickjs.c"

    "./quickjs/cutils.h"
    "./quickjs/libbf.h"
    "./quickjs/libregexp-opcode.h"
    "./quickjs/libregexp.h"
    "./quickjs/libunicode-table.h"
    "./quickjs/libunicode.h"
    "./quickjs/list.h"
    "./quickjs/quickjs-atom.h"
    "./quickjs/quickjs-libc.h"
    "./quickjs/quickjs-opcode.h"
    "./quickjs/quickjs.h"
)

message(STATUS "current dir: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "QJS ${QJS_FILES}")

# quickjs library
add_library(quickjs SHARED ${QJS_FILES} )

# install to copy user used headers
set(QJS_INCLUDE
    "./quickjs/quickjs-libc.h"
    "./quickjs/quickjs.h"
)
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include")
install(FILES ${QJS_INCLUDE} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/include")

##### qjsc
message(STATUS ">>> Begain qjsc build >>>")

set(QJSC_APP
    "./quickjs/qjsc.c"
)
message(STATUS "QJSC_APP ${QJSC_APP}")

add_executable (qjsc ${QJSC_APP})
add_dependencies(qjsc quickjs)
target_link_libraries(qjsc PUBLIC quickjs)

##### qjs
message(STATUS ">>> Begain qjs (qjscalc, repl) build >>>")

add_custom_command(OUTPUT repl.js.c qjscalc.js.c
                   COMMAND qjsc -c -o repl.js.c -m ${CMAKE_CURRENT_SOURCE_DIR}/quickjs/repl.js
                   COMMAND qjsc -c -o qjscalc.js.c -m ${CMAKE_CURRENT_SOURCE_DIR}/quickjs/qjscalc.js
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   DEPENDS qjsc
)

add_executable (qjs "./quickjs/qjs.c" repl.js.c qjscalc.js.c)
target_link_libraries(qjs PUBLIC quickjs)
