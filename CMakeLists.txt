cmake_minimum_required(VERSION 3.27)
project(bgm_wordcounter_problem C)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

set(CMAKE_C_STANDARD 23)

add_executable(bgm_wordcounter_problem main.c)
