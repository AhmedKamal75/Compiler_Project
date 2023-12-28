#include "Parser.h"
#include <iostream>
#include <vector>
#include <fstream>

// Constructor
Parser::Parser(const std::shared_ptr<Table> &table) {
    this->table = table;
}

// Parse the input using the table
void Parser::parse(std::vector<std::string> tokens) {
    std::stack<std::string> parseStack{};
    parseStack.emplace(this->table->get_rules()->get_dollar_symbol());
    parseStack.push(table->get_start_symbol());
    tokens.push_back(this->table->get_rules()->get_dollar_symbol());

    int tokenIndex = 0;

    std::string top = parseStack.top();
    parseStack.pop();
    std::string input_symbol = tokens[tokenIndex];

    std::cout << "######################### parsing started #########################" << '\n';

    while (!parseStack.empty()) {
        if (table->is_terminal(top)) {
            if (top == input_symbol) {
                if (top == this->table->get_rules()->get_dollar_symbol()) {
                    std::cout << GREEN << "Parsing successful" << RESET << '\n';
                    break;
                } else {
                    std::cout << GREEN << "Matched (" << top << ", " << input_symbol << ")" << RESET << '\n';
                    top = parseStack.top();
                    parseStack.pop();
                    input_symbol = tokens[++tokenIndex];
                }
            } else {
                std::cout << RED << "Error: missing {" << top << "}. Inserted " << RESET << '\n';
                top = parseStack.top();
                parseStack.pop();
            }
        } else {
            std::vector<std::string> rule = table->get_rule(top, input_symbol);
            if (!rule.empty()) {
                if ((rule.size() == 1) && (rule[0] == this->table->get_rules()->get_sync_symbol())) {
                    // sync
                    std::cout << RED << "Error: sync" << RESET << '\n';
                    top = parseStack.top();
                    parseStack.pop();
                } else {
                    // output production.
                    // Print the rule that is being applied
                    std::cout << RESET << top << " -> ";
                    for (const auto &symbol: rule) {
                        std::cout << symbol << " ";
                    }
                    std::cout << RESET << "\n";
                    // add production to parse tree vector.
                    this->parse_tree_vector.emplace_back(top, rule);

                    // add production.
                    for (int i = (int) rule.size() - 1; i >= 0; i--) {
                        parseStack.push(rule[i]);
                    }
                    top = parseStack.top();
                    parseStack.pop();
                    while (top == this->table->get_rules()->get_epsilon_symbol()) {
                        top = parseStack.top();
                        parseStack.pop();
                    }
                }
            } else {
                std::cout << RED << "Error: ignoring {" << input_symbol << "}" << RESET << '\n';
                input_symbol = tokens[++tokenIndex];
            }
        }
    }
    std::cout << "########################### parsing ended #########################" << '\n';
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// New version of the parser.

void Parser::parse(const std::shared_ptr<Predictor> &tokenizer, const std::string &parsing_tree_path,
                   const std::string &parsing_output_path) {
    std::stack<std::string> parseStack{};
    parseStack.emplace(this->table->get_rules()->get_dollar_symbol());
    parseStack.push(table->get_start_symbol());

    std::string top = parseStack.top();
    parseStack.pop();
    std::pair<std::string, std::string> input_symbol = get_next_token(tokenizer);
    std::cout << "######################### parsing started #########################" << '\n';
    while (!parseStack.empty()) {
        if (table->is_terminal(top)) {
            if (top == input_symbol.first) {
                if (top == this->table->get_rules()->get_dollar_symbol()) {
                    output_string(parsing_output_path, "Parsing successful");
                    std::cout << GREEN << "Parsing successful" << RESET << '\n';
                    break;
                } else {
                    output_string(parsing_output_path, "Matched (" + top + ", " + input_symbol.second + ")");
                    std::cout << GREEN << "Matched (" << top << ", " << input_symbol.second << ")" << RESET << '\n';
                    top = parseStack.top();
                    parseStack.pop();
                    input_symbol = get_next_token(tokenizer);
                }
            } else {
                output_string(parsing_output_path, "Error: missing {" + top + "}. Inserted ");
                std::cout << RED << "Error: missing {" << top << "}. Inserted " << RESET << '\n';
                top = parseStack.top();
                parseStack.pop();
            }
        } else {
            std::vector<std::string> rule = table->get_rule(top, input_symbol.first);
            if (!rule.empty()) {
                if ((rule.size() == 1) && (rule[0] == this->table->get_rules()->get_sync_symbol())) {
                    // sync
                    output_string(parsing_output_path,
                                  "Error: " + this->table->get_rules()->get_sync_symbol() + " {" + top + "}");
                    std::cout << RED << "Error: " << this->table->get_rules()->get_sync_symbol()
                              << " {" << RESET << top << RED << "}"
                              << RESET << '\n';
                    top = parseStack.top();
                    parseStack.pop();
                } else {
                    // output production.
                    // Print the rule that is being applied
                    output_string(parsing_tree_path, top + " -> " + this->table->get_rules()->vector_to_string(rule));
                    std::cout << RESET << top << " -> ";
                    for (const auto &symbol: rule) {
                        std::cout << symbol << " ";
                    }
                    std::cout << RESET << "\n";
                    // add production to parse tree vector.
                    this->parse_tree_vector.emplace_back(top, rule);

                    // add production.
                    for (int i = (int) rule.size() - 1; i >= 0; i--) {
                        parseStack.push(rule[i]);
                    }
                    top = parseStack.top();
                    parseStack.pop();
                    while (top == this->table->get_rules()->get_epsilon_symbol()) {
                        top = parseStack.top();
                        parseStack.pop();
                    }
                }
            } else {
                output_string(parsing_output_path, "Error: ignoring " + input_symbol.first + " {" + input_symbol.second + "}");
                std::cout << RED << "Error: ignoring " << input_symbol.first << " {" << RESET << input_symbol.second
                          << RED << "}" << RESET << '\n';
                input_symbol = get_next_token(tokenizer);
            }
        }
    }
    std::cout << "########################### parsing ended #########################" << '\n';
}

std::pair<std::string, std::string> Parser::get_next_token(const std::shared_ptr<Predictor> &tokenizer) {
    std::pair<std::string, std::string> entry = tokenizer->next_token();
    if (entry.first.empty() && entry.second.empty()) {
        return {this->table->get_rules()->get_dollar_symbol(), this->table->get_rules()->get_dollar_symbol()};
    }
    return entry;
}

void Parser::output_string(const std::string &parsing_output_path, const std::string &output_string) {
    std::ofstream output_file(parsing_output_path, std::ios::app);
    output_file << output_string << std::endl;
    output_file.close();
}
