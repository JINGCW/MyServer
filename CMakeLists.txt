cmake_minimum_required(VERSION 3.14)
set(CMAKE_FIND_PACKAGE_NAME "MyServer")
set(PACKAGE_NAME ${CMAKE_FIND_PACKAGE_NAME})
message(STATUS "PACKAGE_NAME: ${PACKAGE_NAME}")
project(${PACKAGE_NAME} CXX)

include(CheckIncludeFiles)

message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
##################################################################
# Versioning from GIT
# Defaults
set(${PACKAGE_NAME}_version "0.0.0-0-0")
set(${PACKAGE_NAME}_version_major 0)
set(${PACKAGE_NAME}_version_ninor 0)
set(${PACKAGE_NAME}_version_bugfix 0)
set(${PACKAGE_NAME}_version_nrev 0)
set(${PACKAGE_NAME}_version_gid 0)

CHECK_INCLUDE_FILES(alloca.h TINYSERVER_HAS_ALLOCA_H)

# CPU-specific settings.
message(STATUS "CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")

set(SRCS
        common.h
        Connection.h
        Response.h
        Session.h
        Request.h
        Request.cc
#        main.cpp
        Server.cpp
        Server.h
        )

#add_library(${PACKAGE_NAME} ${SRCS})
add_executable(${PACKAGE_NAME} ${SRCS})