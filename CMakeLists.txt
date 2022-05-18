cmake_minimum_required(VERSION 3.16)
project(TATextEditor)

set(CMAKE_CXX_STANDARD 14)

add_executable(
        TATextEditor
        main.cpp
        Text.h
        Text.cpp
        Sentence.h
        Sentence.cpp
        Word.h
        Word.cpp
)
