# 動作確認できたCMakeのバージョン
cmake_minimum_required(VERSION 3.16)

# project情報
project(
    PCUnit_sample
    LANGUAGES C
)

add_executable(
    PCUnit_sample

    main.c

    RingBuf.c
    test_RingBuf.c

    StrToken.c
    test_StrToken.c
)

target_link_libraries(
    PCUnit_sample
    PCUnit
)

target_include_directories(PCUnit_sample PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
