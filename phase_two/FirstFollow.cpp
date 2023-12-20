#include <iostream>
#include <stack>
#include <algorithm>
#include "FirstFollow.h"

FirstFollow::FirstFollow(const std::shared_ptr<ReadCFG> &rules_obj) {
    this->rules_obj = rules_obj;
    // Initialize the first and follow sets.
    this->first = {};
    this->follow = {};

    std::stack<std::string> reverse_non_terminal{};

    for (const std::string &nt: this->rules_obj->get_non_terminals()) {
        reverse_non_terminal.push(nt);
        first[nt] = {};
        follow[nt] = {};
    }

    while (!reverse_non_terminal.empty()) {
        std::string nt = reverse_non_terminal.top();
        reverse_non_terminal.pop();
        auto temp_first = get_first(nt);
    }

    for (const std::string &nt: this->rules_obj->get_non_terminals()) {
        auto temp_follow = get_follow(nt);
    }
}

std::set<std::string> FirstFollow::get_follow(const std::string &nt, const std::string &temp_non_terminal,
                                              const std::vector<std::string> &rule) {
    // Initialize the set to store the firsts.
    std::set<std::string> follows{};

    // if the nt is the first non-terminal, add $ to the set of follows.
    if (nt == *this->rules_obj->get_non_terminals().begin()) {
        follows.insert(this->rules_obj->get_dollar_symbol());
    }

    // Iterate over the symbols in the rule, till you find the nt.
    int i = 0;
    while (i < rule.size()) {
        if (rule[i] == nt) {
            if (i < rule.size() - 1) {
                bool run = false;
                do {
                    if ((i < rule.size() - 1) && (this->rules_obj->is_terminal(rule[i + 1]))) {
                        follows.insert(rule[i + 1]);
                    } else {
                        if (i < rule.size() - 1) {
                            std::set<std::string> temp_firsts = get_first(rule[i + 1]);
                            if (this->rules_obj->contains_epsilon(temp_firsts)) {
                                temp_firsts = this->rules_obj->remove_epsilon(temp_firsts);
                                follows.insert(temp_firsts.begin(), temp_firsts.end());
                                run = true;
                                i++;
                            } else {
                                run = false;
                            }
                        } else {
                            if (this->rules_obj->is_terminal(rule[i])) {
                                follows.insert(rule[i]);
                            } else {
                                if (temp_non_terminal != nt) {
                                    std::set<std::string> temp_follows = get_follow(temp_non_terminal);
                                    follows.insert(temp_follows.begin(), temp_follows.end());
                                }
                            }
                            run = false;
                        }
                    }
                } while (run);
            } else {
                if (this->rules_obj->is_terminal(rule[i])) {
                    follows.insert(rule[i]);
                } else {
                    if (temp_non_terminal != nt) {
                        std::set<std::string> temp_follows = get_follow(temp_non_terminal);
                        follows.insert(temp_follows.begin(), temp_follows.end());
                    }
                }
            }
        }
        i++;
    }
    return follows;
}


std::set<std::string> FirstFollow::get_follow(const std::string &nt) {
    // Check if nt was already calculated.
    if (!this->follow.at(nt).empty()) {
        return this->follow.at(nt);
    }
    // Initialize the set to store the firsts.
    std::set<std::string> follows{};

    for (const auto &entry: this->rules_obj->get_rules()) {
        std::string temp_non_terminal = entry.first;
        for (const std::vector<std::string> &rule: entry.second) {
            std::set<std::string> temp_follows = get_follow(nt, temp_non_terminal, rule);
            follows.insert(temp_follows.begin(), temp_follows.end());
        }
    }
    this->follow[nt] = follows;
    return follows;
}


std::set<std::string> FirstFollow::get_first(const std::string &nt, const std::vector<std::string> &rule) {
    // Initialize the set to store the firsts.
    std::set<std::string> firsts;

    // Iterate over the symbols in the rule.
    int i = 0;
    while (i < rule.size()) {
        // Get the i-th symbol of the production rule.
        const std::string &symbol = rule[i];

        // If the first symbol is a terminal, add it to the set of firsts.
        if (this->rules_obj->is_terminal(symbol)) {
            firsts.insert(symbol);
            break;
        }
        if (symbol == nt) {
            break;
        }
        std::set<std::string> temp_firsts = get_first(symbol);
        bool contains_epsilon = this->rules_obj->contains_epsilon(temp_firsts);
        if (contains_epsilon) {
            temp_firsts = this->rules_obj->remove_epsilon(temp_firsts);
        }
        firsts.insert(temp_firsts.begin(), temp_firsts.end());
        i++;

        if (!contains_epsilon) {
            break;
        }
    }
    // Return the set of firsts.
    return firsts;
}

std::set<std::string> FirstFollow::get_first(const std::string &nt) {
    // Check if nt was terminal
    if (this->rules_obj->is_terminal(nt)) {
        return {nt};
    }
    // Check if nt was already calculated.
    if (!this->first.at(nt).empty()) {
        return this->first.at(nt);
    }

    // Get the rules for the non-terminal.
    const std::vector<std::vector<std::string>> &rules = this->rules_obj->get_productions(nt);

    // Initialize the set to store the firsts.
    std::set<std::string> firsts{};

    // Iterate over the rules.
    for (const std::vector<std::string> &rule: rules) {
        // Iterate over the symbols in the rule.
        std::set<std::string> temp_firsts = this->get_first(nt, rule);
        firsts.insert(temp_firsts.begin(), temp_firsts.end());
    }

    // Return the set of firsts.
    this->first[nt] = firsts;
    return firsts;
}


// Check if the grammar is LL(1)
bool FirstFollow::is_LL1() {
    for (const std::string &nt: this->rules_obj->get_non_terminals()) {
        // Check for common elements in First and Follow sets
        std::set<std::string> intersection{};
        std::set_intersection(this->first.at(nt).begin(), this->first.at(nt).end(),
                              this->follow.at(nt).begin(), this->follow.at(nt).end(),
                              std::inserter(intersection, intersection.begin()));
        if (!intersection.empty()) {
            std::cout << "Grammar is not LL(1) due to common elements in First and Follow sets of [" << nt << "]. "
                      << "they are: {" << ReadCFG::set_to_string(intersection) << "}" << std::endl;
            return false;
        }
    }
    std::cout << "Grammar is LL(1)" << std::endl;
    return true;
}


[[maybe_unused]] std::map<std::string, std::set<std::string>> FirstFollow::get_first() {
    return this->first;
}

[[maybe_unused]] std::map<std::string, std::set<std::string>> FirstFollow::get_follow() {
    return this->follow;
}

void FirstFollow::print_first() {
    std::cout << "############################ First ############################" << '\n';
    for (const auto &nt: this->rules_obj->get_non_terminals()) {
        std::cout << "First(" << nt << ") = {";
        for (auto it = this->first.at(nt).begin(); it != this->first.at(nt).end(); ++it) {
            std::cout << *it;
            if (std::next(it) != this->first.at(nt).end()) {
                std::cout << ", ";
            }
        }
        std::cout << '}' << '\n';
    }
    std::cout << "################################################################";
    std::cout << std::endl;
}

void FirstFollow::print_follow() {
    std::cout << "############################ Follow ############################" << '\n';
    for (const auto &nt: this->rules_obj->get_non_terminals()) {
        std::cout << "Follow(" << nt << ") = {";
        for (auto it = this->follow.at(nt).begin(); it != this->follow.at(nt).end(); ++it) {
            std::cout << *it;
            if (std::next(it) != this->follow.at(nt).end()) {
                std::cout << ", ";
            }
        }
        std::cout << '}' << '\n';
    }
    std::cout << "################################################################";
    std::cout << std::endl;
}



