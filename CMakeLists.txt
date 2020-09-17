cmake_minimum_required(VERSION 3.0.0)
# set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(tcpServer server.cpp)
target_compile_features(tcpServer PUBLIC cxx_std_17)

add_executable(tcpClient client.cpp)
target_compile_features(tcpClient PUBLIC cxx_std_17)
