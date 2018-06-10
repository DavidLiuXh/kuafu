# CMakeLists.txt for LightningServer

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

PROJECT(test_stdext)

#============================= check =========================================================
IF (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    MESSAGE(FATAL_ERROR "
    CMake generation for ${PROJECT_NAME} not allowed within the #source directory!
    Remove the CMakeCache.txt file and try again from another folder, e.g.:

    rm -rf CMakeFiles CMakeCache.txt
    mkdir cmake-make (or mkdir build)
    cd cmake-make (or cd build)
    cmake -G \"Unix Makefiles\" ../
    ")
ENDIF (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)

SET(CMAKE_VERBOSE_MAKEFILE TRUE)

SET(CMAKE_USE_RELATIVE_PATHS TRUE)

IF (NOT CMAKE_BUILD_TYPE)

    MESSAGE(STATUS "set default as MINSIZEREL")

    SET(CMAKE_BUILD_TYPE MINSIZEREL)

    SET(CMAKE_CONFIGURARION_TYPES "MINSIZEREL")

ENDIF(NOT CMAKE_BUILD_TYPE)

#============================= sources ========================================================
SET(PROJECT_ALL_FILES
    ./test_stdext.cc
    )

INCLUDE_DIRECTORIES(
    ../../
    )
#=============================== build =========================================================
IF (CMAKE_BUILD_TYPE MATCHES Debug)
    MESSAGE(STATUS "${PROJECT_NAME} Debug ${CMAKE_BINARY_DIR}")
    SET(CMAKE_CONFIGURATION_TYPES "Debug")
    #SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "..")


    ADD_DEFINITIONS(
        )

    if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
    endif(COMMAND cmake_policy)

    SET(CMAKE_FIND_LIBRARY_SUFFIXES .so)

    ADD_EXECUTABLE(${PROJECT_NAME} ${PROJECT_ALL_FILES} )

    FIND_LIBRARY(LOG4CPLUS_LIB NAMES log4cplus PATHS /usr/local/lib /usr/lib)
    MESSAGE(STATUS "log4cplus lib path: ${LOG4CPLUS_LIB}")
    FIND_LIBRARY(KUAFU_LIB NAMES kuafu PATHS ../../lib/debug PATH_SUFFIXES so)
    MESSAGE(STATUS "kuafu lib path: ${KUAFU_LIB}")

    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        ${LOG4CPLUS_LIB}
        ${KUAFU_LIB}
        pthread
        )

include(CheckCXXCompilerFlag)
	CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

    MESSAGE(STATUS "source dir: ${PROJECT_SOURCE_DIR}")
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-O0 -g -Wall -pthread" OUTPUT_NAME ${PROJECT_NAME})

ELSE (CMAKE_BUILD_TYPE MATCHES Debug)
    MESSAGE(STATUS "${PROJECT_NAME} ${CMAKE_BUILD_TYPE}")
    SET(CMAKE_CONFIGURATION_TYPES "Release")
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../lib/release")

    ADD_DEFINITIONS(
        )

    #ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_ALL_FILES} )
    ADD_LIBRARY(${PROJECT_NAME} SHARED ${PROJECT_ALL_FILES} )

    SET(CMAKE_FIND_LIBRARY_SUFFIXES .a)

    FIND_LIBRARY(LOG4CPLUS_LIB NAMES log4cplus PATHS /usr/local/lib )
    MESSAGE(STATUS "log4cplus lib path: ${LOG4CPLUS_LIB}")

    FIND_LIBRARY(LIBRDKAFKA_LIB NAMES rdkafka PATHS ${CMAKE_BINARY_DIR}/../thirdparts/librdkafka/src NO_DEFAULT_PATH)
    MESSAGE(STATUS "librdkafka lib path: ${LIBRDKAFKA_LIB}")

    #set(CMAKE_EXE_LINKER_FLAGS "-static")
    #set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-s")

    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        ${LOG4CPLUS_LIB}
        ${LIBRDKAFKA_LIB}
        stdc++
        pthread
        rt
        )

    #SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-O0 -pthread -static"  OUTPUT_NAME ${PROJECT_NAME})
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-O0 -pthread "  OUTPUT_NAME ${PROJECT_NAME})

ENDIF (CMAKE_BUILD_TYPE MATCHES Debug)

