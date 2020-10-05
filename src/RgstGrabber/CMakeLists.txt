cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 14)

if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif()

add_executable(RgstGrabber main.cpp)
target_link_libraries(RgstGrabber PRIVATE
    fmt::fmt
    spdlog::spdlog_header_only
    )