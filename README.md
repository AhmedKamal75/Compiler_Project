# Compiler Project

This project is a compiler built with C++17.

## Prerequisites

- CMake version 3.26 or higher
- A C++17 compatible compiler

## Building the Project

1. Open a terminal.
2. Navigate to the project directory using `cd`.
3. Enter the `cmake-build-debug` directory with `cd cmake-build-debug`.
4. Build the project with the `make` command.
5. Run the project with `./Compiler_Project`, like this command `./Compiler_Project ../output/token_list.txt ../inputs/temp_program.txt ../inputs/temp_rules.txt ../inputs/CFG_input_file.txt`


## Phases done:
1. Lexical analysis (done) [report](https://docs.google.com/document/d/1bXKkk5lQyoX6ByykcY85MEljZOS390rvbRw2BJxWUHE/edit?usp=sharing).
2. LL(1) Parser (in progress)