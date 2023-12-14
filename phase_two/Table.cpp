#include "Table.h"

void
Table::init(const std::map<std::string, std::vector<std::vector<std::string>>> &cfg, std::set<std::string> &terminals,
            const std::set<std::string> &non_terminals, const std::map<std::string, std::set<std::string>> &first,
            const std::map<std::string, std::set<std::string>> &follow) {

}

std::vector<std::string> Table::get_rule(const std::string &non_terminal, const std::string &terminal){
    this->table = {};
    return this->table.at(std::make_pair(non_terminal,terminal));
}
