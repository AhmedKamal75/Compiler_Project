#ifndef COMPILER_PROJECT_LEXICALRULESHANDLER_H
#define COMPILER_PROJECT_LEXICALRULESHANDLER_H


#include <unordered_map>
#include <memory>
#include <string>
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
};


#endif