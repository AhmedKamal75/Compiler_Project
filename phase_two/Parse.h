#ifndef COMPILER_PROJECT_PARSER_H
#define COMPILER_PROJECT_PARSER_H

#include <stack>
#include <vector>
#include "Table.h"

class Parser {
public:
    explicit Parser(const std::shared_ptr<Table> &table);

    void parse(std::vector<std::string> tokens);

private:
    // ANSI escape codes
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string RESET = "\033[0m";

    std::shared_ptr<Table> table;
};

#endif
