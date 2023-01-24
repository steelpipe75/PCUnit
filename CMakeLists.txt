# 動作確認できたCMakeのバージョン
cmake_minimum_required(VERSION 3.16)

# project情報
project(PCUnit
        LANGUAGES C
)

# CTestを有効にする
enable_testing()

#################################################################################

include_directories(./PCUnit)

#################################################################################

add_library(
    PCUnit
    STATIC
    ./PCUnit/PCUnit.c
    ./PCUnit/PCU_Libc.c
    ./PCUnit/PCU_Suite.c
    ./PCUnit/PCU_Test.c
)
