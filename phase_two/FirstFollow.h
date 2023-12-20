#ifndef FIRSTFOLLOW_H
#define FIRSTFOLLOW_H

#include <map>
#include <set>
#include <vector>
#include <memory>
#include "ReadCFG.h"

class FirstFollow {
public:
    explicit FirstFollow(const std::shared_ptr<ReadCFG> &rules_obj);

    std::set<std::string> get_first(const std::string &nt);

    std::set<std::string> get_follow(const std::string &nt);

    [[maybe_unused]] std::map<std::string, std::set<std::string>> get_first();

    [[maybe_unused]] std::map<std::string, std::set<std::string>> get_follow();

    std::set<std::string> get_first(const std::string &nt, const std::vector<std::string> &rule);

    std::set<std::string>
    get_follow(const std::string &nt, const std::string &temp_non_terminal, const std::vector<std::string> &rule);

    bool is_LL1();

    [[maybe_unused]] void print_first();

    [[maybe_unused]] void print_follow();

private:
    std::shared_ptr<ReadCFG> rules_obj{};
    std::map<std::string, std::set<std::string>> first{};
    std::map<std::string, std::set<std::string>> follow{};
};

#endif