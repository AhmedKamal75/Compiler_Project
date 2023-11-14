cmake_minimum_required(VERSION 3.26)
project(Compiler_Project)

set(CMAKE_CXX_STANDARD 17)

add_executable(Compiler_Project main.cpp
        phase_one/automaton/Automaton.cpp
        phase_one/automaton/Automaton.h
        phase_one/automaton/State.cpp
        phase_one/automaton/State.h
        phase_one/automaton/Utilities.cpp
        phase_one/automaton/Utilities.h
        phase_one/automaton/Conversions.cpp
        phase_one/automaton/Conversions.h
        phase_one/creation/Constants.cpp
        phase_one/creation/Constants.h
        phase_one/creation/InfixToPostfix.cpp
        phase_one/creation/InfixToPostfix.h
)
