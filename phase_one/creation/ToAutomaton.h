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
     * @param epsilon_symbol The symbol representing epsilon transitions.
     * @return The automaton equivalent of the regular expression.
     */
    std::shared_ptr<Automaton> regex_to_minimized_dfa(std::string regex, const std::string &epsilon_symbol);


    std::shared_ptr<Automaton>
    regular_definition_to_minimized_dfa(const std::string &regular_definition,
                                        const std::unordered_map<std::string, std::shared_ptr<Automaton>> &automaton,
                                        const std::string &epsilon_symbol);


private:

    Constants constants;

    InfixToPostfix infixToPostfix;

    Conversions conversions;

    /**
     * Converts a regular expression into a minimized DFA.
     *
     * @param postfix         The regular expression to be converted, in the postfix notation.
     * @param epsilonSymbol The symbol representing epsilon transitions.
     * @return The minimized DFA equivalent of the regular expression.
     */
    std::shared_ptr<Automaton>
    get_automaton_from_regex_postfix(const std::string &postfix, const std::string &epsilonSymbol);

    std::shared_ptr<Automaton> get_automaton_from_regular_definition(std::vector<std::string> postfix_tokens,
                                                                     const std::unordered_map<std::string, std::shared_ptr<Automaton>> &map,
                                                                     const std::string &epsilonSymbol);

    std::shared_ptr<Automaton> get_automaton_from_map(const std::string &token,
                                                      const std::unordered_map<std::string, std::shared_ptr<Automaton>> &map,
                                                      const std::string &epsilonSymbol);
};


#endif //COMPILER_PROJECT_PARSING_H
