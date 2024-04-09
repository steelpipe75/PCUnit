# 動作確認できたCMakeのバージョン
cmake_minimum_required(VERSION 3.16)

# project情報
project(
    assert_test
    LANGUAGES C
)

add_executable(assert_test
    main.c
    AssertTest.c
    AssertEqualTest.c
    AssertPtrTest.c
    AssertStringTest.c
    AssertStringWTest.c
    AssertMemoryTest.c
    AssertDoubleTest.c
    AssertOperatorTest.c
    FailTest.c
    SetupTest.c
    InitializeTest.c
    CleanupTest.c
)

target_link_libraries(assert_test PCUnit)
