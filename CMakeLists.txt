cmake_minimum_required (VERSION 2.6)
project(AntTracking)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")

set(FILE_VIDEO "" CACHE FILEPATH "Path to video sequence")
set(FILE_MASK "" CACHE FILEPATH "Path to mask image")

if(FILE_VIDEO STREQUAL "")
    message(SEND_ERROR "no path to video sequence given")
endif()

if(FILE_MASK STREQUAL "")
    message(SEND_ERROR "no path to mask image given")
endif()

set(OPENCV_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/externals/OpenCV/include" CACHE PATH "OpenCV include dir")
set(OPENCV_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/externals/OpenCV/lib" CACHE PATH "OpenCV library dir")
set(OPENCV_BINARY "${CMAKE_CURRENT_SOURCE_DIR}/externals/OpenCV/bin" CACHE PATH "OpenCV binary dir")

set(ZMQ_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/externals/libzmq/include" CACHE PATH "ZMQ include dir")
set(ZMQ_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/externals/libzmq/lib" CACHE PATH "ZMQ library dir")
set(ZMQ_BINARY "${CMAKE_CURRENT_SOURCE_DIR}/externals/libzmq/lib" CACHE PATH "ZMQ binary dir")

include_directories("${OPENCV_INCLUDE}")
include_directories("${ZMQ_INCLUDE}")

link_directories("${OPENCV_LIBRARY}")
link_directories("${ZMQ_LIBRARY}")

file(GLOB TRACKING_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(AntTracking ${TRACKING_SRC})

target_link_libraries(AntTracking "${OPENCV_LIBRARY}/*.lib")
target_link_libraries(AntTracking "${ZMQ_LIBRARY}/*.lib")

STRING(REGEX REPLACE "/" "\\\\\\\\" FILE_MASK \"${FILE_MASK}\")
STRING(REGEX REPLACE "/" "\\\\\\\\" FILE_VIDEO \"${FILE_VIDEO}\")

target_compile_definitions(AntTracking PRIVATE FILE_VIDEO=${FILE_VIDEO} FILE_MASK=${FILE_MASK})

STRING(REGEX REPLACE "/" "\\\\" OPENCV_COPY_SOURCE \"${OPENCV_BINARY}/*.dll\")
STRING(REGEX REPLACE "/" "\\\\" ZMQ_COPY_SOURCE \"${ZMQ_BINARY}/*.dll\")
STRING(REGEX REPLACE "/" "\\\\" COPY_DEST \"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>\")

add_custom_command(TARGET AntTracking POST_BUILD
    COMMAND COMMAND copy ${OPENCV_COPY_SOURCE} ${COPY_DEST}
    COMMAND COMMAND copy ${ZMQ_COPY_SOURCE} ${COPY_DEST}
    COMMENT "Copying OpenCV DLLs...\n"
)

set(BUILD_ANTCLIENT false CACHE BOOL "Create ZMQ client buildfiles")
if(BUILD_ANTCLIENT)
    add_executable(AntClient
        "${CMAKE_CURRENT_SOURCE_DIR}/src/client/AntClient.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ant.hpp"
    )
    target_link_libraries(AntClient "${OPENCV_LIBRARY}/*.lib")
    target_link_libraries(AntClient "${ZMQ_LIBRARY}/*.lib")
endif()

set(BUILD_ANTSERVER false CACHE BOOL "Create ZMQ server buildfiles")
if(BUILD_ANTSERVER)
    add_executable(AntServer
        "${CMAKE_CURRENT_SOURCE_DIR}/src/server/AntServer.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ant.hpp"
    )
    target_link_libraries(AntServer "${OPENCV_LIBRARY}/*.lib")
    target_link_libraries(AntServer "${ZMQ_LIBRARY}/*.lib")
endif()
