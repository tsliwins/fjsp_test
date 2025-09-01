cmake_minimum_required(VERSION 3.20)
project(fjsp_gen)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DNDEBUG")

add_executable(fjsp_gen
        JobshopData.h
        main.cpp
        Generator.cpp
        Generator.h
        DataExport.cpp
        DataExport.h
)

target_link_libraries(fjsp_gen )
