cmake_minimum_required(VERSION 3.26)
project(Compiler_Project)

set(CMAKE_CXX_STANDARD 17)

add_executable(Compiler_Project main.cpp
        phase_one/Automaton/Automaton.cpp
        phase_one/Automaton/Automaton.h
        phase_one/Automaton/State.cpp
        phase_one/Automaton/State.h)
