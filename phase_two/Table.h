#ifndef COMPILER_PROJECT_TABLE_H
#define COMPILER_PROJECT_TABLE_H


#include <map>
#include <string>
#include <vector>
#include <set>
#include <memory>
#include "ReadCFG.h"
#include "FirstFollow.h"

class Table {
public:
    Table(const std::string &file_name);

    std::vector<std::string> get_rule(const std::string &non_terminal, const std::string &terminal);

    void build_table(const std::shared_ptr<ReadCFG> &rules_obj, const std::shared_ptr<FirstFollow> &firstFollow);


private:

    std::map<std::pair<std::string, std::string>, std::vector<std::string>> table{};
};


#endif