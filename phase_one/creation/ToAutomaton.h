#include "Constants.h"
#include "InfixToPostfix.h"
#include "../automaton/Conversions.h"

#ifndef COMPILER_PROJECT_PARSING_H
#define COMPILER_PROJECT_PARSING_H

/**
 * This class is responsible for parsing regular expressions and converting them into minimized DFAs.
 */
class [[maybe_unused]] ToAutomaton {
public:


    /**
     * Parses a regular expression and constructs the corresponding automaton.
     *
     * @param regex         The regular expression to be parsed.
     * @param epsilonSymbol The symbol representing epsilon transitions.
     * @return The automaton equivalent of the regular expression.
     */
    std::shared_ptr<Automaton> regexToMinDFA(std::string regex, const std::string &epsilonSymbol);


    std::shared_ptr<Automaton>
    regularDefinitionToMinDFA(std::string regularDefinition,
                              std::unordered_map<std::string, std::shared_ptr<Automaton>> map,
                              const std::string &epsilonSymbol);

private:

    Constants constants;

    InfixToPostfix infixToPostfix;

    Conversions conversions;

    /**
     * Converts a regular expression into a minimized DFA.
     *
     * @param regex         The regular expression to be converted.
     * @param epsilonSymbol The symbol representing epsilon transitions.
     * @return The minimized DFA equivalent of the regular expression.
     */
    std::shared_ptr<Automaton> parseRegex(std::string regex, const std::string &epsilonSymbol);
};


#endif //COMPILER_PROJECT_PARSING_H
