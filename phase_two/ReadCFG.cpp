#include "ReadCFG.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

ReadCFG::ReadCFG(const std::string &file_name) {
    this->rules = readCFG(file_name);
}


void ReadCFG::convert_to_LL1() {
    int pass = 0;
    bool changed;
    do {
        // First, remove left recursion
        // Then, perform left factoring
        changed = remove_left_recursion() || left_factoring();
        pass += 1;
    } while (changed);
    std::cout << "Grammar converted to LL(1) form after " << pass << " passes" << std::endl;
//     The grammar is now in LL(1) form
}

// Removes left recursion from the grammar
bool ReadCFG::remove_left_recursion() {
    std::vector<std::string> new_non_terminals{};
    std::map<std::string, std::vector<std::vector<std::string>>> new_rules{};

    bool changed = false;

    // Iterate over each non-terminal in the grammar
    for (auto &nt: this->non_terminals) {
        std::vector<std::vector<std::string>> rule = this->rules.at(nt);
        std::vector<std::vector<std::string>> recursive{}, non_recursive{};

        // Split the productions into recursive and non-recursive
        for (auto &production: rule) {
            if (!production.empty() && production[0] == nt) {
                std::vector<std::string> recursive_production(production.begin() + 1, production.end());
                recursive.push_back(recursive_production);
            } else {
                non_recursive.push_back(production);
            }
        }

        // If there are no recursive productions, continue to the next rule
        if (recursive.empty()) {
            new_non_terminals.push_back(nt);
            new_rules[nt] = rule;
            continue;
        }
        changed = true;

        // If there are no non-recursive productions, create a new rule for the recursive productions
        if (non_recursive.empty()) {
            std::vector<std::string> empty_production{};
            non_recursive.push_back(empty_production);
        }

        // Create a new non-terminal for the recursive productions
        std::string new_non_terminal = nt + this->RECURSION_SYMBOL;
        new_non_terminals.push_back(nt);
        new_non_terminals.push_back(new_non_terminal);

        // Adjust the original rule
        for (auto &production: non_recursive) {
            production.push_back(new_non_terminal);
        }

        // Create a new rule for the recursive productions
        std::vector<std::vector<std::string>> new_productions{};
        for (auto &production: recursive) {
            production.push_back(new_non_terminal);
            new_productions.push_back(production);
        }
        new_productions.push_back({EPSILON_SYMBOL});

        // Add the new rules to the grammar
        new_rules[nt] = non_recursive;
        new_rules[new_non_terminal] = new_productions;
    }
    this->rules = new_rules;
    this->non_terminals = new_non_terminals;
    return changed;
}

// Performs left factoring on the grammar
bool ReadCFG::left_factoring() {
    bool changed = false;

    for (auto &rule: rules) {
        auto &productions = rule.second;
        std::map<std::string, std::vector<std::vector<std::string>>> factored_productions;

        // Group productions by their first symbol
        for (const auto &production: productions) {
            if (!production.empty()) {
                std::vector<std::string> factored_production(production.begin() + 1, production.end());
                factored_productions[production[0]].push_back(factored_production);
            }
        }

        // Replace the original productions with the factored ones
        productions.clear();
        std::string s{};
        std::string last_non_terminal = rule.first;
        for (const auto &group: factored_productions) {
            if (group.second.size() == 1) {
                // If there's only one production in the group, add it back directly
                std::vector<std::string> production = {group.first};
                production.insert(production.end(), group.second[0].begin(), group.second[0].end());
                productions.push_back(production);
            } else {
                changed = true;
                // If there are multiple productions, factor them
                s = s.append(this->FACTORING_SYMBOL);
                std::string new_non_terminal = rule.first + s;
                this->non_terminals = add_after_target(this->non_terminals, last_non_terminal,
                                                        new_non_terminal, true);
                last_non_terminal = new_non_terminal;

                // Add a new production for the common prefix
                productions.push_back({group.first, new_non_terminal});

                // Add new productions for the factored parts
                std::vector<std::vector<std::string>> new_sub_production{};
                for (const auto &production: group.second) {
                    if (production.empty()) {
                        new_sub_production.push_back({EPSILON_SYMBOL});
                    } else {
                        std::vector<std::string> new_production(production.begin(), production.end());
                        new_sub_production.push_back(new_production);
                    }
                }
                rules[new_non_terminal] = new_sub_production;
            }
        }
    }
    return changed;
}

std::map<std::string, std::vector<std::vector<std::string>>> ReadCFG::readCFG(const std::string &file_name) {
    std::map<std::string, std::vector<std::vector<std::string>>> cfg;
    std::ifstream file(file_name);
    std::string line;
    std::string last_non_terminal;

    while (std::getline(file, line)) {
        trim(line);
        if (!line.empty() && line[0] == '#') {
            std::istringstream iss(line.substr(1));
            std::getline(iss, last_non_terminal, '=');
            trim(last_non_terminal);
            this->non_terminals.push_back(last_non_terminal);
            line = line.substr(line.find('=') + 1);
        }

        std::istringstream rule_stream(line);
        std::string symbol;
        std::vector<std::string> symbols;

        while (rule_stream >> std::quoted(symbol)) {
            if (symbol == "|") {
                if (!symbols.empty()) {
                    cfg[last_non_terminal].push_back(symbols);
                    symbols.clear();
                }
            } else {
                // If symbol is enclosed in single quotes, remove the quotes
                if (symbol[0] == '\'' && symbol[symbol.size() - 1] == '\'') {
                    symbol = symbol.substr(1, symbol.size() - 2);
                    terminals.insert(symbol);  // Add to terminals
                }
                symbols.push_back(symbol);
            }
        }

        if (!symbols.empty()) {
            cfg[last_non_terminal].push_back(symbols);
        }
    }

    return cfg;
}

// Prints the CFG to the console
void ReadCFG::printCFG() {
    std::cout << "############################ CFG ############################" << '\n';
    for (const std::string &non_terminal: this->non_terminals) {
        std::cout << non_terminal << " --> ";
        for (const std::vector<std::string> &rule: this->rules.at(non_terminal)) {
            std::cout << "[";
            for (size_t i = 0; i < rule.size(); ++i) {
                std::cout << rule[i];
                if (i != rule.size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << "]";
            if (&rule != &this->rules.at(non_terminal).back()) {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "########################################################" << '\n';
}

// trim from start (in place)
void ReadCFG::ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](char &ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
void ReadCFG::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](char &ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
[[maybe_unused]] void ReadCFG::trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

std::set<std::string> ReadCFG::get_terminals() {
    return this->terminals;
}

std::vector<std::string> ReadCFG::get_non_terminals() {
    return this->non_terminals;
}

std::map<std::string, std::vector<std::vector<std::string>>> ReadCFG::get_rules() {
    return this->rules;
}

std::vector<std::vector<std::string>> ReadCFG::get_productions(const std::string &non_terminal) {
    auto it = this->rules.find(non_terminal);
    if (it == rules.end()) {
        return {};
    }
    return it->second;
}

bool ReadCFG::is_terminal(const std::string &symbol) {
    return this->terminals.find(symbol) != this->terminals.end();
}

std::string ReadCFG::get_dollar_symbol() {
    return this->DOLLAR_SYMBOL;
}

std::string ReadCFG::get_sync_symbol() {
    return this->SYNC_SYMBOL;
}

std::string ReadCFG::get_epsilon_symbol() {
    return this->EPSILON_SYMBOL;
}

bool ReadCFG::is_epsilon_symbol(const std::string &symbol) {
    return this->EPSILON_SYMBOL == symbol;
}

bool ReadCFG::contains(const std::set<std::string> &container, const std::string &symbol) {
    return container.find(symbol) != container.end();
}

bool ReadCFG::contains_epsilon(const std::set<std::string> &symbols) {
    return symbols.find(this->EPSILON_SYMBOL) != symbols.end();
}

bool ReadCFG::is_non_terminal(const std::string &symbol) {
    return std::find(this->non_terminals.begin(), this->non_terminals.end(), symbol) != this->non_terminals.end();
}

std::set<std::string> ReadCFG::remove_epsilon(const std::set<std::string> &symbols) {
    std::set<std::string> result;
    for (const std::string &symbol: symbols) {
        if (symbol != this->EPSILON_SYMBOL) {
            result.insert(symbol);
        }
    }
    return result;
}

std::string ReadCFG::vector_to_string(const std::vector<std::string> &vec) {
    std::string result{};
    for (const auto &element: vec) {
        result += element + " ";
    }
    return result;
}

std::string ReadCFG::set_to_string(const std::set<std::string> &set) {
    std::string result{};
    for (const auto &element: set) {
        result += element + " ";
    }
    return result;
}

std::string ReadCFG::stack_to_string(std::stack<std::string> stack) {
    std::string result{};
    while (!stack.empty()) {
        result += stack.top() + " ";
        stack.pop();
    }
    return result;
}

std::vector<std::string> ReadCFG::add_after_target(const std::vector<std::string> &vec, const std::string &target,
                                                   const std::string &symbol, bool one_time) {
    std::vector<std::string> new_vector{};
    bool done = false;
    for (const std::string &str:vec){
        if (!done && (str == target)) {
            if (one_time){
                done = true;
            }
            new_vector.push_back(str);
            new_vector.push_back(symbol);
        } else {
            new_vector.push_back(str);
        }
    }
    return new_vector;
}

