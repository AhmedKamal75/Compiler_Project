#ifndef COMPILER_PROJECT_READCFG_H
#define COMPILER_PROJECT_READCFG_H


#include <map>
#include <string>
#include <vector>
#include <set>

class ReadCFG {
public:
    explicit ReadCFG(const std::string &file_name);

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

    static void ltrim(std::string &s);

    static void rtrim(std::string &s);

    static void trim(std::string &s);

private:
    std::map<std::string, std::vector<std::vector<std::string>>> rules{};
    std::set<std::string> terminals{};
    std::vector<std::string> non_terminals{};
    const std::string epsilon_symbol = "\\L";


};


#endif