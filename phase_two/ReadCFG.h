#ifndef COMPILER_PROJECT_READCFG_H
#define COMPILER_PROJECT_READCFG_H


#include <map>
#include <string>
#include <vector>
#include <set>

class ReadCFG {
public:
    std::map<std::string, std::vector<std::vector<std::string>>> readCFG(const std::string &filename);

    static void printCFG(const std::map<std::string, std::vector<std::vector<std::string>>> &cfg);

    std::set<std::string> get_terminals();

    std::set<std::string> get_non_terminals();

    static void ltrim(std::string &s);

    static void rtrim(std::string &s);

    static void trim(std::string &s);

private:
    std::set<std::string> terminals;
    std::set<std::string> non_terminals;
};


#endif