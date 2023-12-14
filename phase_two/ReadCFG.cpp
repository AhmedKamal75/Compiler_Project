#include "ReadCFG.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

std::map<std::string, std::vector<std::vector<std::string>>> ReadCFG::readCFG(const std::string &filename) {
    std::map<std::string, std::vector<std::vector<std::string>>> cfg;
    std::ifstream file(filename);
    std::string line;
    std::string last_non_terminal;

    while (std::getline(file, line)) {
        trim(line);
        if (!line.empty() && line[0] == '#') {
            std::istringstream iss(line.substr(1));
            std::getline(iss, last_non_terminal, '=');
            trim(last_non_terminal);
            this->non_terminals.insert(last_non_terminal);
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
                } else {
                    non_terminals.insert(symbol);  // Add to non_terminals
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


void ReadCFG::printCFG(const std::map<std::string, std::vector<std::vector<std::string>>> &cfg) {
    for (const auto &non_terminal: cfg) {
        std::cout << non_terminal.first << " --> ";
        for (const auto &rule: non_terminal.second) {
            std::cout << "[";
            for (size_t i = 0; i < rule.size(); ++i) {
                std::cout << rule[i];
                if (i != rule.size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << "]";
            if (&rule != &non_terminal.second.back()) {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }
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

std::set<std::string> ReadCFG::get_non_terminals() {
    return this->non_terminals;
}
