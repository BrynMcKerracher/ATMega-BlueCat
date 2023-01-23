cmake_minimum_required(VERSION 2.6.0)

project(ATMega-BlueCat C)

add_executable(ATMega-BlueCat main.c OpCode.h OpBatch.h OpBatch.c Memory.h Memory.c Debug.c Debug.h Value.c Value.h)