#ifndef COMPILER_PROJECT_PARSER_H
#define COMPILER_PROJECT_PARSER_H

#include <stack>
#include <vector>
#include "Table.h"
#include "../phase_one/prediction/Predictor.h"


class Parser {
public:
    explicit Parser(const std::shared_ptr<Table> &table);

    void parse(std::vector<std::string> tokens);

    void parse(const std::shared_ptr<Predictor> &tokenizer, const std::string &parsing_tree_path,
               const std::string &parsing_output_path);

    static void output_string(const std::string &parsing_output_path, const std::string &output_string);

    std::pair<std::string, std::string> get_next_token(const std::shared_ptr<Predictor> &tokenizer);

private:
    // ANSI escape codes
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string RESET = "\033[0m";

    std::shared_ptr<Table> table;
    std::vector<std::pair<std::string, std::vector<std::string>>> parse_tree_vector{};
};

#endif
