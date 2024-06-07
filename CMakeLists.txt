# 動作確認できたCMakeのバージョン
cmake_minimum_required(VERSION 3.16)

# project情報
project(
    PCUnit
    LANGUAGES C
)

#################################################################################

# コンパイル情報をJSON形式で出力
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# in-source ビルドガード
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. ")
endif()

# ビルドフォルダをgitの無視対象にする
file(WRITE ${CMAKE_BINARY_DIR}/.gitignore "*\n")

#################################################################################

add_subdirectory(PCUnit)

option(BUILD_SAMPLE "build sample" OFF)
if(BUILD_SAMPLE)
    add_subdirectory(sample)
endif()

option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    add_subdirectory(test)
endif()
