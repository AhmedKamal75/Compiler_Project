#ifndef COMPILER_PROJECT_READCFG_H
#define COMPILER_PROJECT_READCFG_H


#include <map>
#include <string>
#include <vector>
#include <set>
#include <stack>

class ReadCFG {
public:
    explicit ReadCFG(const std::string &file_name);

    bool remove_left_recursion();

    bool left_factoring();

    void convert_to_LL1();

    std::map<std::string, std::vector<std::vector<std::string>>> readCFG(const std::string &file_name);

    void printCFG();

    std::set<std::string> get_terminals();

    std::vector<std::string> get_non_terminals();

    std::map<std::string, std::vector<std::vector<std::string>>> get_rules();

    std::vector<std::vector<std::string>> get_productions(const std::string &non_terminal);

    bool is_terminal(const std::string &symbol);

    bool is_non_terminal(const std::string &symbol);

    bool is_epsilon_symbol(const std::string &symbol);

    static bool contains(const std::set<std::string> &container, const std::string &symbol);

    bool contains_epsilon(const std::set<std::string> &symbols);

    std::set<std::string> remove_epsilon(const std::set<std::string> &symbols);

    std::string get_dollar_symbol();

    std::string get_sync_symbol();

    std::string get_epsilon_symbol();

    static void ltrim(std::string &s);

    static std::string vector_to_string(const std::vector<std::string> &vec);

    static std::string set_to_string(const std::set<std::string> &set);

    static std::string stack_to_string(std::stack<std::string> stack);

    static std::vector<std::string>
    add_after_target(const std::vector<std::string> &vec, const std::string &target, const std::string &symbol,
                     bool one_time);

    static void rtrim(std::string &s);

    static void trim(std::string &s);

private:
    std::map<std::string, std::vector<std::vector<std::string>>> rules{};
    std::set<std::string> terminals{};
    std::vector<std::string> non_terminals{};
    const std::string EPSILON_SYMBOL = "\\L";
    const std::string DOLLAR_SYMBOL = "$";
    const std::string SYNC_SYMBOL = "\\SYNC";
    const std::string FACTORING_SYMBOL = "\"";
    const std::string RECURSION_SYMBOL = "'";
};


#endif