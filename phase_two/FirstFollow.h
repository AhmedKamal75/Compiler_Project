#ifndef COMPILER_PROJECT_FIRSTFOLLOW_H
#define COMPILER_PROJECT_FIRSTFOLLOW_H


#include <set>
#include <map>
#include <string>
#include <vector>

class FirstFollow {
public:
    static std::map<std::string, std::set<std::string>>
    get_first(const std::map<std::string, std::vector<std::vector<std::string>>> &cfg,
              const std::set<std::string> &terminals, const std::set<std::string> &non_terminals);

    static std::map<std::string, std::set<std::string>>
    get_follow(const std::map<std::string, std::vector<std::vector<std::string>>> &cfg,
               const std::set<std::string> &terminals, const std::set<std::string> &non_terminals);

};


#endif