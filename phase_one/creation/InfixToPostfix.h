#ifndef COMPILER_PROJECT_INFIXTOPOSTFIX_H
#define COMPILER_PROJECT_INFIXTOPOSTFIX_H

#include "Constants.h"
#include <string>
#include <vector>

class InfixToPostfix {
private:
    Constants constants;

public:
    InfixToPostfix();

    // takes a regular definition as a string and return a vector of the tokens it contained in the infix notations
    std::vector<std::string> tokenize(const std::string &regular_definition);

    // This method adds explicit concatenation operators to the regular expression
    std::string concat_explicit(std::string regex);

    // same as above but for tokens
    std::vector<std::string> concat_explicit_tokens(const std::vector<std::string> &tokens);


    /**
     * Convert regular expression from infix to postfix notation using
     * Shunting-yard algorithm.
     *
     * @param regex infix notation
     * @return postfix notation
     */
    std::string regex_infix_to_postfix(std::string regex);

    std::vector<std::string> regular_definition_infix_to_postfix(const std::vector<std::string> &tokens);

    // This method evaluates the postfix expression, i.e. from postfix to infix
    std::string regex_evaluate_postfix(std::string postfix);

    // This method evaluates the postfix regular definition, i.e. from postfix to infix
    std::string regular_definition_evaluate_postfix(std::vector<std::string> postfix_tokens);
};


#endif