cmake_minimum_required(VERSION 3.16)
project(vio LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ZED 5 REQUIRED)
find_package(CUDA REQUIRED)

add_executable(vio
    vio.cpp
    zed_vio.cpp
)

target_include_directories(vio PRIVATE
    ${ZED_INCLUDE_DIRS}
    ${CUDA_INCLUDE_DIRS}
)

target_link_libraries(vio
    ${ZED_LIBRARIES}
    ${CUDA_LIBRARIES}
)