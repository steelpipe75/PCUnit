# 動作確認できたCMakeのバージョン
cmake_minimum_required(VERSION 3.16)

# project情報
project(
    PCUnit
    LANGUAGES C
)

add_subdirectory(PCUnit)
add_subdirectory(sample)
# add_subdirectory(test) # todo
