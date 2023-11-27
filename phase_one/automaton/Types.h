#ifndef COMPILER_PROJECT_TYPES_H
#define COMPILER_PROJECT_TYPES_H


#include <iostream>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include "State.h"


class Types {
public:
    template<class T>
    inline static void hash_combine(std::size_t &seed, const T &v) {
        std::hash<T> h;
        seed ^= h(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }


    struct pair_hash {
        std::size_t operator()(const std::pair<std::shared_ptr<State>, std::string> &p) const {
            auto h1 = p.first->hash();  // Use the 'hash' method of State
            auto h2 = std::hash<std::string>{}(p.second);  // Hash the string

            // Mainly for demonstration purposes, i.e. works but is overly simple
            // In the real world, use sth. like boost.hash_combine
            std::size_t seed = 0;
            hash_combine(seed, h1);
            hash_combine(seed, h2);
            return seed;
        }
    };

    using key_t = std::pair<std::shared_ptr<State>, std::string>;

    struct pair_equal {
        bool operator()(const key_t &a, const key_t &b) const {
            return *(a.first) == *(b.first) && a.second == b.second;
        }
    };

    struct Hash {
        std::size_t operator()(const std::shared_ptr<State> &k) const {
            return k->hash();  // Use the 'hash' method of State
        }
    };

    struct Equal {
        bool operator()(const std::shared_ptr<State> &a, const std::shared_ptr<State> &b) const {
            return *a == *b;  // Use the '==' operator of State
        }
    };

    struct map_hash {
        std::size_t operator()(const std::shared_ptr<State> &p) const {
            std::size_t seed = 0;
            hash_combine(seed, p->hash());
            return seed;
        }
    };

    struct map_equal {
        bool operator()(const std::shared_ptr<State> &a, const std::shared_ptr<State> &b) const {
            return *(a) == *(b);
        }
    };

    struct string_hash {
        std::size_t operator()(const std::string &str) const {
            return std::hash<std::string>{}(str);
        }
    };

    struct string_equal {
        bool operator()(const std::string &a, const std::string &b) const {
            return a == b;
        }
    };

    using string_set_t = std::unordered_set<std::string, string_hash, string_equal>;

    using state_to_string_set_map_t = std::unordered_map<std::shared_ptr<State>, string_set_t, Types::map_hash, Types::map_equal>;

    using state_set_t = std::unordered_set<std::shared_ptr<State>, Hash, Equal>;

    using transitions_t = std::unordered_map<key_t, state_set_t, pair_hash, pair_equal>;

    using transitions_dfa_t = std::unordered_map<key_t, std::shared_ptr<State>, pair_hash, pair_equal>;

    using epsilon_closure_map_t = std::unordered_map<std::shared_ptr<State>, state_set_t, map_hash, map_equal>;

    using state_to_state_map_t = std::unordered_map<std::shared_ptr<State>, std::shared_ptr<State>, map_hash, map_equal>;

};


#endif