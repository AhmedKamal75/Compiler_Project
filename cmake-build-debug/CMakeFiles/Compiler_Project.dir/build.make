# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /snap/clion/250/bin/cmake/linux/x64/bin/cmake

# The command to remove a file.
RM = /snap/clion/250/bin/cmake/linux/x64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Compiler_Project.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Compiler_Project.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Compiler_Project.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Compiler_Project.dir/flags.make

CMakeFiles/Compiler_Project.dir/main.cpp.o: CMakeFiles/Compiler_Project.dir/flags.make
CMakeFiles/Compiler_Project.dir/main.cpp.o: /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/main.cpp
CMakeFiles/Compiler_Project.dir/main.cpp.o: CMakeFiles/Compiler_Project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Compiler_Project.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Compiler_Project.dir/main.cpp.o -MF CMakeFiles/Compiler_Project.dir/main.cpp.o.d -o CMakeFiles/Compiler_Project.dir/main.cpp.o -c /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/main.cpp

CMakeFiles/Compiler_Project.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Compiler_Project.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/main.cpp > CMakeFiles/Compiler_Project.dir/main.cpp.i

CMakeFiles/Compiler_Project.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Compiler_Project.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/main.cpp -o CMakeFiles/Compiler_Project.dir/main.cpp.s

CMakeFiles/Compiler_Project.dir/phase_one/creation/Constants.cpp.o: CMakeFiles/Compiler_Project.dir/flags.make
CMakeFiles/Compiler_Project.dir/phase_one/creation/Constants.cpp.o: /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/creation/Constants.cpp
CMakeFiles/Compiler_Project.dir/phase_one/creation/Constants.cpp.o: CMakeFiles/Compiler_Project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Compiler_Project.dir/phase_one/creation/Constants.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Compiler_Project.dir/phase_one/creation/Constants.cpp.o -MF CMakeFiles/Compiler_Project.dir/phase_one/creation/Constants.cpp.o.d -o CMakeFiles/Compiler_Project.dir/phase_one/creation/Constants.cpp.o -c /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/creation/Constants.cpp

CMakeFiles/Compiler_Project.dir/phase_one/creation/Constants.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Compiler_Project.dir/phase_one/creation/Constants.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/creation/Constants.cpp > CMakeFiles/Compiler_Project.dir/phase_one/creation/Constants.cpp.i

CMakeFiles/Compiler_Project.dir/phase_one/creation/Constants.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Compiler_Project.dir/phase_one/creation/Constants.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/creation/Constants.cpp -o CMakeFiles/Compiler_Project.dir/phase_one/creation/Constants.cpp.s

CMakeFiles/Compiler_Project.dir/phase_one/creation/InfixToPostfix.cpp.o: CMakeFiles/Compiler_Project.dir/flags.make
CMakeFiles/Compiler_Project.dir/phase_one/creation/InfixToPostfix.cpp.o: /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/creation/InfixToPostfix.cpp
CMakeFiles/Compiler_Project.dir/phase_one/creation/InfixToPostfix.cpp.o: CMakeFiles/Compiler_Project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Compiler_Project.dir/phase_one/creation/InfixToPostfix.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Compiler_Project.dir/phase_one/creation/InfixToPostfix.cpp.o -MF CMakeFiles/Compiler_Project.dir/phase_one/creation/InfixToPostfix.cpp.o.d -o CMakeFiles/Compiler_Project.dir/phase_one/creation/InfixToPostfix.cpp.o -c /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/creation/InfixToPostfix.cpp

CMakeFiles/Compiler_Project.dir/phase_one/creation/InfixToPostfix.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Compiler_Project.dir/phase_one/creation/InfixToPostfix.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/creation/InfixToPostfix.cpp > CMakeFiles/Compiler_Project.dir/phase_one/creation/InfixToPostfix.cpp.i

CMakeFiles/Compiler_Project.dir/phase_one/creation/InfixToPostfix.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Compiler_Project.dir/phase_one/creation/InfixToPostfix.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/creation/InfixToPostfix.cpp -o CMakeFiles/Compiler_Project.dir/phase_one/creation/InfixToPostfix.cpp.s

CMakeFiles/Compiler_Project.dir/phase_one/creation/ToAutomaton.cpp.o: CMakeFiles/Compiler_Project.dir/flags.make
CMakeFiles/Compiler_Project.dir/phase_one/creation/ToAutomaton.cpp.o: /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/creation/ToAutomaton.cpp
CMakeFiles/Compiler_Project.dir/phase_one/creation/ToAutomaton.cpp.o: CMakeFiles/Compiler_Project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Compiler_Project.dir/phase_one/creation/ToAutomaton.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Compiler_Project.dir/phase_one/creation/ToAutomaton.cpp.o -MF CMakeFiles/Compiler_Project.dir/phase_one/creation/ToAutomaton.cpp.o.d -o CMakeFiles/Compiler_Project.dir/phase_one/creation/ToAutomaton.cpp.o -c /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/creation/ToAutomaton.cpp

CMakeFiles/Compiler_Project.dir/phase_one/creation/ToAutomaton.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Compiler_Project.dir/phase_one/creation/ToAutomaton.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/creation/ToAutomaton.cpp > CMakeFiles/Compiler_Project.dir/phase_one/creation/ToAutomaton.cpp.i

CMakeFiles/Compiler_Project.dir/phase_one/creation/ToAutomaton.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Compiler_Project.dir/phase_one/creation/ToAutomaton.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/creation/ToAutomaton.cpp -o CMakeFiles/Compiler_Project.dir/phase_one/creation/ToAutomaton.cpp.s

CMakeFiles/Compiler_Project.dir/phase_one/creation/LexicalRulesHandler.cpp.o: CMakeFiles/Compiler_Project.dir/flags.make
CMakeFiles/Compiler_Project.dir/phase_one/creation/LexicalRulesHandler.cpp.o: /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/creation/LexicalRulesHandler.cpp
CMakeFiles/Compiler_Project.dir/phase_one/creation/LexicalRulesHandler.cpp.o: CMakeFiles/Compiler_Project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Compiler_Project.dir/phase_one/creation/LexicalRulesHandler.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Compiler_Project.dir/phase_one/creation/LexicalRulesHandler.cpp.o -MF CMakeFiles/Compiler_Project.dir/phase_one/creation/LexicalRulesHandler.cpp.o.d -o CMakeFiles/Compiler_Project.dir/phase_one/creation/LexicalRulesHandler.cpp.o -c /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/creation/LexicalRulesHandler.cpp

CMakeFiles/Compiler_Project.dir/phase_one/creation/LexicalRulesHandler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Compiler_Project.dir/phase_one/creation/LexicalRulesHandler.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/creation/LexicalRulesHandler.cpp > CMakeFiles/Compiler_Project.dir/phase_one/creation/LexicalRulesHandler.cpp.i

CMakeFiles/Compiler_Project.dir/phase_one/creation/LexicalRulesHandler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Compiler_Project.dir/phase_one/creation/LexicalRulesHandler.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/creation/LexicalRulesHandler.cpp -o CMakeFiles/Compiler_Project.dir/phase_one/creation/LexicalRulesHandler.cpp.s

CMakeFiles/Compiler_Project.dir/phase_one/automaton/Types.cpp.o: CMakeFiles/Compiler_Project.dir/flags.make
CMakeFiles/Compiler_Project.dir/phase_one/automaton/Types.cpp.o: /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/automaton/Types.cpp
CMakeFiles/Compiler_Project.dir/phase_one/automaton/Types.cpp.o: CMakeFiles/Compiler_Project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/Compiler_Project.dir/phase_one/automaton/Types.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Compiler_Project.dir/phase_one/automaton/Types.cpp.o -MF CMakeFiles/Compiler_Project.dir/phase_one/automaton/Types.cpp.o.d -o CMakeFiles/Compiler_Project.dir/phase_one/automaton/Types.cpp.o -c /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/automaton/Types.cpp

CMakeFiles/Compiler_Project.dir/phase_one/automaton/Types.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Compiler_Project.dir/phase_one/automaton/Types.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/automaton/Types.cpp > CMakeFiles/Compiler_Project.dir/phase_one/automaton/Types.cpp.i

CMakeFiles/Compiler_Project.dir/phase_one/automaton/Types.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Compiler_Project.dir/phase_one/automaton/Types.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/automaton/Types.cpp -o CMakeFiles/Compiler_Project.dir/phase_one/automaton/Types.cpp.s

CMakeFiles/Compiler_Project.dir/phase_one/automaton/State.cpp.o: CMakeFiles/Compiler_Project.dir/flags.make
CMakeFiles/Compiler_Project.dir/phase_one/automaton/State.cpp.o: /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/automaton/State.cpp
CMakeFiles/Compiler_Project.dir/phase_one/automaton/State.cpp.o: CMakeFiles/Compiler_Project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/Compiler_Project.dir/phase_one/automaton/State.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Compiler_Project.dir/phase_one/automaton/State.cpp.o -MF CMakeFiles/Compiler_Project.dir/phase_one/automaton/State.cpp.o.d -o CMakeFiles/Compiler_Project.dir/phase_one/automaton/State.cpp.o -c /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/automaton/State.cpp

CMakeFiles/Compiler_Project.dir/phase_one/automaton/State.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Compiler_Project.dir/phase_one/automaton/State.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/automaton/State.cpp > CMakeFiles/Compiler_Project.dir/phase_one/automaton/State.cpp.i

CMakeFiles/Compiler_Project.dir/phase_one/automaton/State.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Compiler_Project.dir/phase_one/automaton/State.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/automaton/State.cpp -o CMakeFiles/Compiler_Project.dir/phase_one/automaton/State.cpp.s

CMakeFiles/Compiler_Project.dir/phase_one/automaton/Automaton.cpp.o: CMakeFiles/Compiler_Project.dir/flags.make
CMakeFiles/Compiler_Project.dir/phase_one/automaton/Automaton.cpp.o: /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/automaton/Automaton.cpp
CMakeFiles/Compiler_Project.dir/phase_one/automaton/Automaton.cpp.o: CMakeFiles/Compiler_Project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/Compiler_Project.dir/phase_one/automaton/Automaton.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Compiler_Project.dir/phase_one/automaton/Automaton.cpp.o -MF CMakeFiles/Compiler_Project.dir/phase_one/automaton/Automaton.cpp.o.d -o CMakeFiles/Compiler_Project.dir/phase_one/automaton/Automaton.cpp.o -c /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/automaton/Automaton.cpp

CMakeFiles/Compiler_Project.dir/phase_one/automaton/Automaton.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Compiler_Project.dir/phase_one/automaton/Automaton.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/automaton/Automaton.cpp > CMakeFiles/Compiler_Project.dir/phase_one/automaton/Automaton.cpp.i

CMakeFiles/Compiler_Project.dir/phase_one/automaton/Automaton.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Compiler_Project.dir/phase_one/automaton/Automaton.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/automaton/Automaton.cpp -o CMakeFiles/Compiler_Project.dir/phase_one/automaton/Automaton.cpp.s

CMakeFiles/Compiler_Project.dir/phase_one/automaton/Utilities.cpp.o: CMakeFiles/Compiler_Project.dir/flags.make
CMakeFiles/Compiler_Project.dir/phase_one/automaton/Utilities.cpp.o: /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/automaton/Utilities.cpp
CMakeFiles/Compiler_Project.dir/phase_one/automaton/Utilities.cpp.o: CMakeFiles/Compiler_Project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/Compiler_Project.dir/phase_one/automaton/Utilities.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Compiler_Project.dir/phase_one/automaton/Utilities.cpp.o -MF CMakeFiles/Compiler_Project.dir/phase_one/automaton/Utilities.cpp.o.d -o CMakeFiles/Compiler_Project.dir/phase_one/automaton/Utilities.cpp.o -c /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/automaton/Utilities.cpp

CMakeFiles/Compiler_Project.dir/phase_one/automaton/Utilities.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Compiler_Project.dir/phase_one/automaton/Utilities.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/automaton/Utilities.cpp > CMakeFiles/Compiler_Project.dir/phase_one/automaton/Utilities.cpp.i

CMakeFiles/Compiler_Project.dir/phase_one/automaton/Utilities.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Compiler_Project.dir/phase_one/automaton/Utilities.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/automaton/Utilities.cpp -o CMakeFiles/Compiler_Project.dir/phase_one/automaton/Utilities.cpp.s

CMakeFiles/Compiler_Project.dir/phase_one/automaton/Conversions.cpp.o: CMakeFiles/Compiler_Project.dir/flags.make
CMakeFiles/Compiler_Project.dir/phase_one/automaton/Conversions.cpp.o: /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/automaton/Conversions.cpp
CMakeFiles/Compiler_Project.dir/phase_one/automaton/Conversions.cpp.o: CMakeFiles/Compiler_Project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/Compiler_Project.dir/phase_one/automaton/Conversions.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Compiler_Project.dir/phase_one/automaton/Conversions.cpp.o -MF CMakeFiles/Compiler_Project.dir/phase_one/automaton/Conversions.cpp.o.d -o CMakeFiles/Compiler_Project.dir/phase_one/automaton/Conversions.cpp.o -c /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/automaton/Conversions.cpp

CMakeFiles/Compiler_Project.dir/phase_one/automaton/Conversions.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Compiler_Project.dir/phase_one/automaton/Conversions.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/automaton/Conversions.cpp > CMakeFiles/Compiler_Project.dir/phase_one/automaton/Conversions.cpp.i

CMakeFiles/Compiler_Project.dir/phase_one/automaton/Conversions.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Compiler_Project.dir/phase_one/automaton/Conversions.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/phase_one/automaton/Conversions.cpp -o CMakeFiles/Compiler_Project.dir/phase_one/automaton/Conversions.cpp.s

# Object files for target Compiler_Project
Compiler_Project_OBJECTS = \
"CMakeFiles/Compiler_Project.dir/main.cpp.o" \
"CMakeFiles/Compiler_Project.dir/phase_one/creation/Constants.cpp.o" \
"CMakeFiles/Compiler_Project.dir/phase_one/creation/InfixToPostfix.cpp.o" \
"CMakeFiles/Compiler_Project.dir/phase_one/creation/ToAutomaton.cpp.o" \
"CMakeFiles/Compiler_Project.dir/phase_one/creation/LexicalRulesHandler.cpp.o" \
"CMakeFiles/Compiler_Project.dir/phase_one/automaton/Types.cpp.o" \
"CMakeFiles/Compiler_Project.dir/phase_one/automaton/State.cpp.o" \
"CMakeFiles/Compiler_Project.dir/phase_one/automaton/Automaton.cpp.o" \
"CMakeFiles/Compiler_Project.dir/phase_one/automaton/Utilities.cpp.o" \
"CMakeFiles/Compiler_Project.dir/phase_one/automaton/Conversions.cpp.o"

# External object files for target Compiler_Project
Compiler_Project_EXTERNAL_OBJECTS =

Compiler_Project: CMakeFiles/Compiler_Project.dir/main.cpp.o
Compiler_Project: CMakeFiles/Compiler_Project.dir/phase_one/creation/Constants.cpp.o
Compiler_Project: CMakeFiles/Compiler_Project.dir/phase_one/creation/InfixToPostfix.cpp.o
Compiler_Project: CMakeFiles/Compiler_Project.dir/phase_one/creation/ToAutomaton.cpp.o
Compiler_Project: CMakeFiles/Compiler_Project.dir/phase_one/creation/LexicalRulesHandler.cpp.o
Compiler_Project: CMakeFiles/Compiler_Project.dir/phase_one/automaton/Types.cpp.o
Compiler_Project: CMakeFiles/Compiler_Project.dir/phase_one/automaton/State.cpp.o
Compiler_Project: CMakeFiles/Compiler_Project.dir/phase_one/automaton/Automaton.cpp.o
Compiler_Project: CMakeFiles/Compiler_Project.dir/phase_one/automaton/Utilities.cpp.o
Compiler_Project: CMakeFiles/Compiler_Project.dir/phase_one/automaton/Conversions.cpp.o
Compiler_Project: CMakeFiles/Compiler_Project.dir/build.make
Compiler_Project: CMakeFiles/Compiler_Project.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX executable Compiler_Project"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Compiler_Project.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Compiler_Project.dir/build: Compiler_Project
.PHONY : CMakeFiles/Compiler_Project.dir/build

CMakeFiles/Compiler_Project.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Compiler_Project.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Compiler_Project.dir/clean

CMakeFiles/Compiler_Project.dir/depend:
	cd /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/cmake-build-debug /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/cmake-build-debug /home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/cmake-build-debug/CMakeFiles/Compiler_Project.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Compiler_Project.dir/depend
