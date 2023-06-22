project(BlueCat)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")

add_executable(BlueCat
        main.cpp
        BlueCat/BlueCat.h
        BlueCat/Common.h
        BlueCat/Compiler/Compiler.cpp
        BlueCat/Compiler/Compiler.h
        BlueCat/Debug/Debug.cpp
        BlueCat/Debug/Debug.h
        BlueCat/Hashable.cpp
        BlueCat/Hashable.h
        BlueCat/Hashing.cpp
        BlueCat/Hashing.h
        BlueCat/HashTable.cpp
        BlueCat/HashTable.h
        BlueCat/Local.cpp
        BlueCat/Local.h
        BlueCat/Memory.cpp
        BlueCat/Memory.h
        BlueCat/Object.cpp
        BlueCat/Object.h
        BlueCat/ObjectString.cpp
        BlueCat/ObjectString.h
        BlueCat/OpCode.h
        BlueCat/Parser.cpp
        BlueCat/Parser.h
        BlueCat/Scanner.cpp
        BlueCat/Scanner.h
        BlueCat/Segment.cpp
        BlueCat/Segment.h
        BlueCat/Stack.cpp
        BlueCat/Stack.h
        BlueCat/Token.cpp
        BlueCat/Token.h
        BlueCat/Value.cpp
        BlueCat/Value.h
        BlueCat/VM.cpp
        BlueCat/VM.h
        BlueCat/Compiler/Serialiser.cpp
        BlueCat/Compiler/Serialiser.h
        BlueCat/Compiler/Deserialiser.cpp
        BlueCat/Compiler/Deserialiser.h
        BlueCat/Debug/Printer.cpp
        BlueCat/Debug/Printer.h
        )

cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(FetchContent)
FetchContent_Declare(
        googletest
        DOWNLOAD_EXTRACT_TIMESTAMP
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        FullPipelineTests
        BlueCat/Tests/FullPipeline/FPStrings.cpp

        BlueCat/BlueCat.h
        BlueCat/Common.h
        BlueCat/Compiler/Compiler.cpp
        BlueCat/Compiler/Compiler.h
        BlueCat/Debug/Debug.cpp
        BlueCat/Debug/Debug.h
        BlueCat/Hashable.cpp
        BlueCat/Hashable.h
        BlueCat/Hashing.cpp
        BlueCat/Hashing.h
        BlueCat/HashTable.cpp
        BlueCat/HashTable.h
        BlueCat/Local.cpp
        BlueCat/Local.h
        BlueCat/Memory.cpp
        BlueCat/Memory.h
        BlueCat/Object.cpp
        BlueCat/Object.h
        BlueCat/ObjectString.cpp
        BlueCat/ObjectString.h
        BlueCat/OpCode.h
        BlueCat/Parser.cpp
        BlueCat/Parser.h
        BlueCat/Scanner.cpp
        BlueCat/Scanner.h
        BlueCat/Segment.cpp
        BlueCat/Segment.h
        BlueCat/Stack.cpp
        BlueCat/Stack.h
        BlueCat/Token.cpp
        BlueCat/Token.h
        BlueCat/Value.cpp
        BlueCat/Value.h
        BlueCat/VM.cpp
        BlueCat/VM.h
        BlueCat/Compiler/Serialiser.cpp
        BlueCat/Compiler/Serialiser.h
        BlueCat/Compiler/Deserialiser.cpp
        BlueCat/Compiler/Deserialiser.h
        BlueCat/Debug/Printer.cpp
        BlueCat/Debug/Printer.h
        BlueCat/Tests/FullPipeline/FPNumbers.cpp
        BlueCat/Tests/FullPipeline/FPBytes.cpp
        BlueCat/Tests/FullPipeline/FPBools.cpp
        BlueCat/Tests/FullPipeline/FPControlFlow.cpp
)

target_link_libraries(
        FullPipelineTests
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(FullPipelineTests)