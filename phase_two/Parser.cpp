#include "Parser.h"
#include <iostream>
#include <vector>

// Constructor
Parser::Parser(const std::shared_ptr<Table> &table) {
    this->table = table;
}


// Parse the input using the table
void Parser::parse(std::vector<std::string> tokens) {
    std::stack<std::string> parseStack;
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
// Old version of the parser.

// Parse the input using the table
//void Parser::parse(std::vector<std::string> tokens) {
//    std::stack<std::string> parseStack;
//    parseStack.emplace(this->table->get_rules()->get_dollar_symbol());
//    parseStack.push(table->get_start_symbol());
//    tokens.push_back(this->table->get_rules()->get_dollar_symbol());
//
//    int tokenIndex = 0;
//
//    while (!parseStack.empty()) {
//        std::string top = parseStack.top();
//        parseStack.pop();
//
//        if (top == this->table->get_rules()->get_dollar_symbol() && top == tokens[tokenIndex]) {
//            break;
//        }
//
//
//        if (table->is_terminal(top)) {
//            if (top == tokens[tokenIndex]) {
//                tokenIndex++;
//                std::cout << GREEN << "Matched (" << top << ", " << tokens[tokenIndex - 1] << ")" << RESET << '\n';
//            } else if (top != this->table->get_rules()->get_epsilon_symbol()) {
//                std::cout << RED << "Error: Unmatched terminal symbol " << top << ". Inserting " << top << RESET
//                          << '\n';
//                tokenIndex++;
//            }
//        } else {
//            std::vector<std::string> rule = table->get_rule(top, tokens[tokenIndex]);
//            if (rule.empty()) {
//                std::cout << RED << "Error: No rule found for " << top << ", " << tokens[tokenIndex] << ". Discarding "
//                          << tokens[tokenIndex] << RESET << '\n';
//                tokenIndex++;
//                continue;
//            }
//
//            // Print the rule that is being applied
//            std::cout << RESET << top << " -> ";
//            for (const auto &symbol: rule) {
//                std::cout << symbol << " ";
//            }
//            std::cout << RESET << "\n";
//
//            for (auto it = rule.rbegin(); it != rule.rend(); ++it) {
//                if (*it != this->table->get_rules()->get_epsilon_symbol()) {
//                    parseStack.push(*it);
//                }
//            }
//        }
//    }
//
//    if (tokenIndex != tokens.size() - 1) {
//        std::cout << RED << "Error: Unparsed input remaining" << RESET << '\n';
//    } else {
//        std::cout << GREEN << "Parsing successful" << RESET << '\n';
//    }
//}
