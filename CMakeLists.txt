cmake_minimum_required(VERSION 3.24)
project(Deber_calculo_pi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
add_executable(Deber_calculo_pi main.cpp)
find_package(benchmark REQUIRED)
target_link_libraries(Deber_calculo_pi  benchmark)

target_link_libraries(Deber_calculo_pi benchmark::benchmark_main)