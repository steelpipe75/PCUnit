# 動作確認できたCMakeのバージョン
cmake_minimum_required(VERSION 3.16)

# project情報
project(
    PCUnit_Library
    LANGUAGES C
)

add_library(PCUnit STATIC
    PCUnit.c
    PCU_Libc.c
    PCU_Suite.c
    PCU_Test.c
)

target_include_directories(PCUnit PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)
