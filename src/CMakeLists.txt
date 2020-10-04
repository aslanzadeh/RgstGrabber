cmake_minimum_required(VERSION 3.16)
project(RgstGrabber)

add_subdirectory(RgstGrabber)
add_subdirectory(libRgstGrabber)
if (RGSTG_BUILD_TESTS)
    add_subdirectory(test)
endif ()
