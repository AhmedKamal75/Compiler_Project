#include "ReadCFG.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

ReadCFG::ReadCFG(const std::string &file_name) {
    this->rules = readCFG(file_name);
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

bool ReadCFG::is_epsilon_symbol(const std::string &symbol) {
    return this->epsilon_symbol == symbol;
}

bool ReadCFG::contains(const std::set<std::string> &container, const std::string &symbol) {
    return container.find(symbol) != container.end();
}

bool ReadCFG::contains_epsilon(const std::set<std::string> &symbols) {
    return symbols.find(this->epsilon_symbol) != symbols.end();
}

bool ReadCFG::is_non_terminal(const std::string &symbol) {
    return std::find(this->non_terminals.begin(), this->non_terminals.end(), symbol) != this->non_terminals.end();
}

std::set<std::string> ReadCFG::remove_epsilon(const std::set<std::string> &symbols) {
    std::set<std::string> result;
    for (const std::string &symbol: symbols) {
        if (symbol != this->epsilon_symbol) {
            result.insert(symbol);
        }
    }
    return result;
}