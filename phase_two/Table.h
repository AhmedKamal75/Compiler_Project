#ifndef COMPILER_PROJECT_TABLE_H
#define COMPILER_PROJECT_TABLE_H


#include <map>
#include <string>
#include <vector>
#include <set>

class Table {
public:
    void init(const std::map<std::string, std::vector<std::vector<std::string>>> &cfg,
              std::set<std::string> &terminals, const std::set<std::string> &non_terminals,
              const std::map<std::string, std::set<std::string>> &first,
              const std::map<std::string, std::set<std::string>> &follow);

    std::vector<std::string> get_rule(const std::string &non_terminal, const std::string &terminal);

private:
    std::map<std::pair<std::string, std::string>, std::vector<std::string>> table{};
};


#endif