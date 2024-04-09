# 動作確認できたCMakeのバージョン
cmake_minimum_required(VERSION 3.16)

# project情報
project(
    PCUnit_test
    LANGUAGES C
)

add_subdirectory(assert_test)
# add_subdirectory(assert_output_test) # todo
# add_subdirectory(PCU_format_test) # todo
# add_subdirectory(PCU_Libc_test) # todo
# add_subdirectory(pcunit_mock_test) # todo
