cmake_minimum_required(VERSION 3.16)

include(ExternalProject)
ExternalProject_Add(
    fmt
    PREFIX ${CMAKE_BINARY_DIR}/fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    EXCLUDE_FROM_ALL TRUE
    GIT_TAG "v1.8.1"
    TIMEOUT 10
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
)

ExternalProject_Add(
    spdlog
    PREFIX ${CMAKE_BINARY_DIR}/spdlog
    GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
    GIT_TAG "v1.8.1"
    TIMEOUT 10
    CONFIGURE_COMMAND ""
    CMAKE_ARGS "-DSPDLOG_FMT_EXTERNAL=ON -DDSPDLOG_FMT_EXTERNAL:string=ON"
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
)