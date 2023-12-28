#ifndef COMPILER_PROJECT_TABLE_H
#define COMPILER_PROJECT_TABLE_H


#include <map>
#include <string>
#include <vector>
#include <set>
#include <memory>
#include <unordered_map>
#include "ReadCFG.h"
#include "FirstFollow.h"

class Table {
public:
    Table();

    explicit Table(const std::string &file_name, const std::string &parsing_table_output_file_name);

    std::vector<std::string> get_rule(const std::string &non_terminal, const std::string &terminal);

    void build_table();

    void print_table();

    void export_to_file(const std::string &file_name);

    static std::shared_ptr<Table> import_from_file(const std::string &file_name);

    std::string get_start_symbol();

    bool is_terminal(const std::string &symbol);

    std::shared_ptr<FirstFollow> get_first_follow();

    std::shared_ptr<ReadCFG> get_rules();

private:
    struct pair_hash {
        template<class T1, class T2>
        std::size_t operator()(const std::pair<T1, T2> &p) const {
            auto h1 = std::hash<T1>{}(p.first);
            auto h2 = std::hash<T2>{}(p.second);
            h1 ^= h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2);

            return h1 ^ h2;
        }
    };

    std::unordered_map<std::pair<std::string, std::string>, std::vector<std::string>, pair_hash> parsing_table;
    std::shared_ptr<ReadCFG> rules_obj;
    std::shared_ptr<FirstFollow> firstFollow;
};


#endif