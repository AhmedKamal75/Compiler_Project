#include <iostream>
#include "Table.h"

// Constructor
Table::Table(const std::string &file_name) {
    ReadCFG read_cfg(file_name);
    FirstFollow first_follow(std::make_shared<ReadCFG>(read_cfg));
    read_cfg.printCFG();
    first_follow.print_first();
    first_follow.print_follow();
    build_table(std::make_shared<ReadCFG>(read_cfg), std::make_shared<FirstFollow>(first_follow));
    print_table();
}

void Table::print_table() {
    std::cout << "############################ Parsing Table ############################" << '\n';
    for (const auto &entry : parsing_table) {
        std::cout << "Table[" << entry.first.first << ", " << entry.first.second << "] = {";
        for (auto it = entry.second.begin(); it != entry.second.end(); ++it) {
            std::cout << *it;
            if (std::next(it) != entry.second.end()) {
                std::cout << ", ";
            }
        }
        std::cout << '}' << '\n';
    }
    std::cout << "################################################################" << '\n';
}


void Table::build_table(const std::shared_ptr<ReadCFG> &rules_obj, const std::shared_ptr<FirstFollow> &firstFollow) {
    for (const auto &nt : rules_obj->get_non_terminals()) {
        for (const auto &production : rules_obj->get_productions(nt)) {
            auto first_set = firstFollow->get_first(nt, production);
            for (const auto &terminal : first_set) {
                if (!rules_obj->is_epsilon_symbol(terminal)) {
                    parsing_table[{nt, terminal}] = production;
                }
            }
            if (rules_obj->contains_epsilon(first_set)) {
                auto follow_set = firstFollow->get_follow(nt);
                for (const auto &terminal : follow_set) {
                    // If the entry in the parsing table is empty, update it
                    if (parsing_table[{nt, terminal}].empty()) {
                        parsing_table[{nt, terminal}] = production;
                    }
                }
            }
        }
    }
}


std::vector<std::string> Table::get_rule(const std::string &non_terminal, const std::string &terminal) {
    return this->parsing_table.at(std::make_pair(non_terminal, terminal));
}
