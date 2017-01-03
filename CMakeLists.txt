cmake_minimum_required(VERSION 2.6)
project(FlushSSH CXX)

# Version number
set(FlushSSH_VERSION_MAJOR 0)
set(FlushSSH_VERSION_MINOR 1)

#message(STATUS "Building version: ${FlushSSH_VERSION_MAJOR}.${FlushSSH_VERSION_MINOR}")
# configure header file
configure_file(
    "${PROJECT_SOURCE_DIR}/FlushSSHConfig.h.in"
    "${PROJECT_BINARY_DIR}/FlushSSHConfig.h"
)

# search path
include_directories("${PROJECT_BINARY_DIR}")
link_directories("D:/libssh/bin")
add_library(ssh)

add_executable(FlushSSH main.cpp)
target_link_libraries(FlushSSH ssh)

# Install rules
install(TARGETS FlushSSH DESTINATION bin)
