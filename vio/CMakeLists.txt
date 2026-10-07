cmake_minimum_required(VERSION 3.16)
project(vio LANGUAGES CXX)

include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(ZED 5 REQUIRED)
find_package(CUDA REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ZMQ REQUIRED libzmq)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(vio
    vio.cpp
    vio_json.cpp
    zed_vio.cpp
    zmq_publisher.cpp
)
target_include_directories(vio PRIVATE
    ${ZED_INCLUDE_DIRS}
    ${CUDA_INCLUDE_DIRS}
    ${ZMQ_INCLUDE_DIRS}
)

target_link_libraries(vio PRIVATE
    ${ZMQ_LINK_LIBRARIES}
    nlohmann_json::nlohmann_json
    ${ZED_LIBRARIES}
    ${CUDA_LIBRARIES}
)

if(BUILD_TESTING)
    add_executable(vio_json_test
        test_vio_json.cpp
        vio_json.cpp
    )
    target_link_libraries(vio_json_test PRIVATE
        nlohmann_json::nlohmann_json
    )
    add_test(NAME vio_json_test COMMAND vio_json_test)
endif()
