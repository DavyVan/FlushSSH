cmake_minimum_required(VERSION 2.6)
project(FlushSSH CXX)

include (CheckIncludeFileCXX)

# Version number
set(FlushSSH_VERSION_MAJOR 1)
set(FlushSSH_VERSION_MINOR 1)

message(STATUS "Building version: ${FlushSSH_VERSION_MAJOR}.${FlushSSH_VERSION_MINOR}")

# Include files check
check_include_file_cxx(winsock2.h HAVE_WINSOCK2_H)
check_include_file_cxx(sys/socket.h HAVE_SYS_SOCKET_H)
check_include_file_cxx(netinet/in.h HAVE_NETINET_IN_H)
check_include_file_cxx(sys/select.h HAVE_SYS_SELECT_H)
check_include_file_cxx(unistd.h HAVE_UNISTD_H)
check_include_file_cxx(arpa/inet.h HAVE_ARPA_INET_H)
check_include_file_cxx(sys/time.h HAVE_SYS_TIME_H)
check_include_file_cxx(netdb.h HAVE_NETDB_H)

# configure header file
configure_file(
    "${PROJECT_SOURCE_DIR}/FlushSSHConfig.h.in"
    "${PROJECT_BINARY_DIR}/FlushSSHConfig.h"
)

include_directories("${PROJECT_BINARY_DIR}")

# find libraries
# libssh2
find_package(Libssh2 REQUIRED CONFIG)

# compile & link
add_library(utils utils.cpp)

add_library(clean_up clean.cpp)
target_link_libraries(clean_up Libssh2::libssh2)

add_executable(FlushSSH main.cpp)
target_link_libraries(FlushSSH Libssh2::libssh2 utils clean_up)

# Install rules
install(TARGETS FlushSSH DESTINATION bin)
