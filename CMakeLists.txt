# 動作確認できたCMakeのバージョン
cmake_minimum_required(VERSION 3.16)

# project情報
project(
    PCUnit
    LANGUAGES C
)

add_subdirectory(PCUnit)

option(BUILD_SAMPLE "build sample" OFF)
if(BUILD_SAMPLE)
    add_subdirectory(sample)
endif()

option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    add_subdirectory(test)
endif()
