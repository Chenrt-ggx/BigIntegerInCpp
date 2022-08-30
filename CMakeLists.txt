cmake_minimum_required(VERSION 3.19)
project(BigInteger)

set(CMAKE_CXX_STANDARD 14)

include_directories("C:/Program Files/Python39/include")

link_libraries("C:/Program Files/Python39/libs/python39.lib")

add_executable(BigInteger main.cpp lib/BigInteger.cpp lib/BigInteger.h lib/Complex.cpp lib/Complex.h test/test.cpp test/test.h global.h other/constant.cpp other/constant.h)