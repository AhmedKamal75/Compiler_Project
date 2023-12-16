#include <iostream>
#include "Table.h"

std::vector<std::string> Table::get_rule(const std::string &non_terminal, const std::string &terminal) {
    this->table = {};
    return this->table.at(std::make_pair(non_terminal, terminal));
}

Table::Table(const std::string &file_name) {
    ReadCFG read_cfg(file_name);
    read_cfg.printCFG();
    std::cout << std::endl;
    FirstFollow first_follow(std::make_shared<ReadCFG>(read_cfg));
    first_follow.print_first();
//    first_follow.print_follow();
//    build_table(std::make_shared<ReadCFG>(read_cfg), std::make_shared<FirstFollow>(first_follow));
}

void Table::build_table(const std::shared_ptr<ReadCFG> &rules_obj, const std::shared_ptr<FirstFollow> &firstFollow) {

}
