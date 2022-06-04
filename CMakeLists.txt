cmake_minimum_required(VERSION 3.16)
project(TATextEditor)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS -pthread)

# Set include dir
include_directories(gtest/include)

# Set Library dir
link_directories(gtest/lib)

# Set source files for RELEASE target
set(RELEASE_SOURCE_FILES main.cpp
        Text.h  Text.cpp
        Sentence.h  Sentence.cpp
        Word.h  Word.cpp
        DFA.h  DFA.cpp
        ENFA.h  ENFA.cpp
        json.hpp
        NFA.h  NFA.cpp
        RE.h  RE.cpp
        SpecialCharacter.h SpecialCharacter.cpp
        )

# Set source files for DEBUG target
set(DEBUG_SOURCE_FILES mainTests.cpp
        Text.h  Text.cpp
        Sentence.h  Sentence.cpp
        Word.h  Word.cpp
        DFA.h  DFA.cpp
        ENFA.h  ENFA.cpp
        json.hpp
        NFA.h  NFA.cpp
        RE.h  RE.cpp
        SpecialCharacter.h SpecialCharacter.cpp
        )

# Create RELEASE target
add_executable(TATextEditor ${RELEASE_SOURCE_FILES})

# Create DEBUG target
add_executable(TATextEditor_debug ${DEBUG_SOURCE_FILES})

# Link library
target_link_libraries(TATextEditor_debug gtest)