# 動作確認できたCMakeのバージョン
cmake_minimum_required(VERSION 3.16)

# project情報
project(
    PCUnit_Library
    LANGUAGES C
)

#######################################

# オプション
option(NO_VSPRINTF "no vsprintf" OFF)
option(NO_SETJMP "no setjmp" OFF)
option(NO_LIBC "no libc" OFF)
option(NO_STDARG "no stdarg" OFF)
option(NO_FLOATINGPOINT "no floating point" OFF)
option(NO_DIV32 "no div32" OFF)
option(NO_CONSOLE_RUN "no console run" OFF)

option(USE_WCHAR "use wchar" ON)


#######################################

add_library(PCUnit STATIC
    PCUnit.c
    PCU_Libc.c
    PCU_Suite.c
    PCU_Test.c
)

#######################################

target_include_directories(PCUnit PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

#######################################

if(NO_VSPRINTF)
    target_compile_definitions(PCUnit PUBLIC PCU_NO_VSPRINTF)
endif()

if(NO_SETJMP)
    target_compile_definitions(PCUnit PUBLIC PCU_NO_SETJMP)
endif()

if(NO_LIBC)
    target_compile_definitions(PCUnit PUBLIC PCU_NO_LIBC)
endif()

if(NO_STDARG)
    target_compile_definitions(PCUnit PUBLIC PCU_NO_STDARG)
endif()

if(NO_FLOATINGPOINT)
    target_compile_definitions(PCUnit PUBLIC PCU_NO_FLOATINGPOINT)
endif()

if(NO_DIV32)
    target_compile_definitions(PCUnit PUBLIC PCU_NO_DIV32)
endif()

if(NO_CONSOLE_RUN)
    target_compile_definitions(PCUnit PUBLIC PCU_NO_CONSOLE_RUN)
endif()


if(USE_WCHAR)
    target_compile_definitions(PCUnit PUBLIC PCU_USE_WCHAR)
endif()


