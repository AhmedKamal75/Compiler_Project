#ifndef COMPILER_PROJECT_LEXICALRULESHANDLER_H
#define COMPILER_PROJECT_LEXICALRULESHANDLER_H


#include <unordered_map>
#include <memory>
#include <string>
#include <queue>
#include "../automaton/Automaton.h"
#include "ToAutomaton.h"

class LexicalRulesHandler {
public:
    LexicalRulesHandler();

    [[maybe_unused]] std::unordered_map<std::string, std::shared_ptr<Automaton>>
    handleFile(const std::string &filename);


private:
    std::string epsilonSymbol = "\\L";
    ToAutomaton toAutomaton;
    std::unordered_map<std::string, int> attempts{};
    const int MAX_ATTEMPTS = 100;


    void handle_backlog(std::unordered_map<std::string, std::shared_ptr<Automaton>> &automata,
                        std::queue<std::pair<std::string, std::string>> &backlog);

    // trim from start (in place)
    void ltrim(std::string &s);

    // trim from end (in place)
    void rtrim(std::string &s);

    // trim from both ends (in place)
    void trim(std::string &s);

};


#endif