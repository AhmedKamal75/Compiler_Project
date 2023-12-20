# Compiler Project

## Introduction

This project  is implemented in C++ and uses a variety of techniques, including:
1. A lexical analyzer to tokenize the input program.
2. A parser to parse the tokens into an abstract syntax tree (AST).

## Prerequisites

- CMake version 3.26 or higher
- A C++17 compatible compiler

## Building the Project
1. Open a terminal, clone the repository, and navigate to the project directory.



```shell
git clone https://github.com/ahmed-kamal/Compiler_Project.git
```
2. Navigate to the project directory using `cd`.
3. Enter the `cmake-build-debug` directory with `cd cmake-build-debug`.
4. Build the project with the `make` command.
```shell
make
```
5. Run the project with `./Compiler_Project`, like this command 

```shell
./Compiler_Project ../output/token_list.txt ../inputs/temp_program.txt ../inputs/temp_rules.txt ../inputs/CFG_input_file.txt
```


## Phases done:
1. Lexical analysis (done) [report](https://docs.google.com/document/d/1bXKkk5lQyoX6ByykcY85MEljZOS390rvbRw2BJxWUHE/edit?usp=sharing).
2. LL(1) Parser (done) [report](https://docs.google.com/document/d/1WXNy2-yXSnuaJDfA8Y74dAY78Vf5u5MGoN7OrKiqkEE/edit?usp=sharing)