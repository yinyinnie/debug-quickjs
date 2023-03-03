cmake_minimum_required(VERSION 3.20)
project(debug_quickjs)

set(CMAKE_CXX_STANDARD 14)

option(LEAK_TRIGGER "Add a leak trigger" FALSE)

# from quickjs VERSION file
set(CONFIG_VERSION "2021-03-27")

if (LEAK_TRIGGER)
    set(COMMON_FLAGS -DLEAK_TRIGGER)
else ()
    set(COMMON_FLAGS)
endif (LEAK_TRIGGER)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CONFIG_CC clang)
else()
    set(CONFIG_CC gcc)
endif()

set(COMMON_FLAGS -D_GNU_SOURCE -DCONFIG_VERSION=\"${CONFIG_VERSION}\" -DCONFIG_CC=\"${CONFIG_CC}\" -DCONFIG_PREFIX=\"/usr/local\" -DCONFIG_BIGNUM)

message("=============" + ${COMMON_FLAGS})

set(QUICKJS_LIB_SOURCES
        quickjs.c
        libregexp.c
        libunicode.c
        libbf.c
        cutils.c
        )

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(COMMON_FLAGS ${COMMON_FLAGS} -DDUMP_LEAKS)
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

if (LEAK_TRIGGER)
    # Use printf as leak_trigger
    set(COMMON_FLAGS ${COMMON_FLAGS} -Dprintf=leak_trigger)
endif (LEAK_TRIGGER)

add_executable(debug_quickjs main.cpp ${QUICKJS_LIB_SOURCES})
target_compile_options(debug_quickjs PRIVATE ${COMMON_FLAGS})