#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "Table.h"

// Constructor
Table::Table() = default;

// Constructor
Table::Table(const std::string &file_name, const std::string &parsing_table_output_file_name) {
    ReadCFG read_cfg(file_name);
    this->rules_obj = std::make_shared<ReadCFG>(read_cfg);
    this->rules_obj->convert_to_LL1();
    read_cfg.printCFG();

    FirstFollow first_follow(this->rules_obj);
    this->firstFollow = std::make_shared<FirstFollow>(first_follow);
    first_follow.print_first();
    first_follow.print_follow();

    build_table();
    print_table();
}

// Print Table
void Table::print_table() {
    std::vector<std::string> terminals{};
    for (const auto &terminal: this->rules_obj->get_terminals()) {
        if (terminal != this->rules_obj->get_epsilon_symbol()) {
            terminals.emplace_back(terminal);
        }
    }
    terminals.emplace_back("$");

    // Sort the terminals in reverse order
    std::sort(terminals.begin(), terminals.end(), std::greater<std::string>());

    // Find the maximum length of a production
    size_t max_production_length = 0;
    for (const auto &nt: this->rules_obj->get_non_terminals()) {
        for (const auto &terminal: terminals) {
            if (!parsing_table[{nt, terminal}].empty()) {
                std::string production;
                for (const auto &symbol: parsing_table[{nt, terminal}]) {
                    production += symbol + " ";
                }
                max_production_length = std::max(max_production_length, production.length());
            }
        }
    }

    // Print the terminals
    std::cout << std::setw((int) max_production_length) << " " << "|";
    for (const auto &terminal: terminals) {
        std::cout << std::setw((int) max_production_length) << terminal << "|";
    }
    std::cout << "\n";

    // Print a line separator
    for (size_t i = 0; i < max_production_length; i++) {
        std::cout << " ";
    }
    std::cout << std::string(max_production_length * (terminals.size() + 1), '#') << "\n";

    // For each non-terminal
    for (const auto &nt: this->rules_obj->get_non_terminals()) {
        std::cout << std::setw((int) max_production_length) << nt << ":";
        // For each terminal
        for (const auto &terminal: terminals) {
            if (parsing_table[{nt, terminal}].empty()) {
                std::cout << std::setw((int) max_production_length) << " " << "|";
            } else {
                std::string production;
                for (const auto &symbol: parsing_table[{nt, terminal}]) {
                    production += symbol + " ";
                }
                std::cout << std::setw((int) max_production_length) << production << "|";
            }
        }
        std::cout << "\n";

        // Print a line separator
        for (size_t i = 0; i < max_production_length; i++) {
            std::cout << " ";
        }
        std::cout << std::string(max_production_length * (terminals.size() + 1), '#') << "\n";
    }
}

// Build the parsing table
void Table::build_table() {
    // For each non-terminal.
    for (const auto &nt: rules_obj->get_non_terminals()) {
        // For each production of the non-terminal
        for (const auto &production: rules_obj->get_productions(nt)) {
            // For each terminal in the first set of the production
            std::set<std::string> first_set = firstFollow->get_first(nt, production);
            // If the production is not epsilon, add it to the parsing table
            for (const auto &terminal: first_set) {
                if (!rules_obj->is_epsilon_symbol(terminal)) {
                    parsing_table[{nt, terminal}] = production;
                }
            }
            // If the production is epsilon, add it to the parsing table for each terminal in the follow set
            auto follow_set = firstFollow->get_follow(nt);
            if (rules_obj->contains_epsilon(first_set)) {
                // For each terminal in the follow set of the non-terminal
                for (const auto &terminal: follow_set) {
                    if (parsing_table[{nt, terminal}].empty() || terminal == rules_obj->get_sync_symbol()) {
                        parsing_table[{nt, terminal}] = production;
                    } else {
                        std::cout << "Error: The parsing table is not LL(1)." << '\n';
                        std::cout << "nt: " << nt << '\n';
                        std::cout << "terminal: " << terminal << '\n';
                        std::cout << "old production: " << ReadCFG::vector_to_string(parsing_table[{nt, terminal}])
                                  << '\n';
                        std::cout << "new production: " << ReadCFG::vector_to_string(production) << '\n';
                    }
                }
            }
        }
    }
    // After the parsing table is fully built, add the sync character without overriding anything
    for (const auto &nt: rules_obj->get_non_terminals()) {
        auto follow_set = firstFollow->get_follow(nt);
        for (const auto &terminal: follow_set) {
            if (parsing_table[{nt, terminal}].empty()) {
                parsing_table[{nt, terminal}] = {rules_obj->get_sync_symbol()};
            }
        }
    }
}

std::vector<std::string> Table::get_rule(const std::string &non_terminal, const std::string &terminal) {
    return this->parsing_table.at(std::make_pair(non_terminal, terminal));
}

void Table::export_to_file(const std::string &file_name) {
    std::ofstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Error opening file" << '\n';
        return;
    }
    for (const auto &entry: parsing_table) {
        file << entry.first.first << " " << entry.first.second << " ";
        for (const auto &rule: entry.second) {
            file << rule << " ";
        }
        file << '\n';
    }
    file.close();
}

std::shared_ptr<Table> Table::import_from_file(const std::string &file_name) {
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Error opening file" << '\n';
        return nullptr;
    }

    std::shared_ptr<Table> table = std::make_shared<Table>();

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string non_terminal, terminal;
        std::vector<std::string> rules;

        iss >> non_terminal >> terminal;
        std::string rule{};
        while (iss >> rule) {
            rules.push_back(rule);
        }

        table->parsing_table[{non_terminal, terminal}] = rules;
    }

    file.close();

    return table;
}

std::string Table::get_start_symbol() {
    return *this->rules_obj->get_non_terminals().begin();
}

bool Table::is_terminal(const std::string &symbol) {
    return this->rules_obj->is_terminal(symbol);
}

std::shared_ptr<FirstFollow> Table::get_first_follow() {
    return firstFollow;
}

std::shared_ptr<ReadCFG> Table::get_rules() {
    return rules_obj;
}


