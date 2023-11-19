#ifndef COMPILER_PROJECT_INFIXTOPOSTFIX_H
#define COMPILER_PROJECT_INFIXTOPOSTFIX_H

#include "Constants.h"
#include <string>

class InfixToPostfix {
private:
    Constants constants;

public:
    InfixToPostfix();

    // This method adds explicit concatenation operators to the regular expression
    std::string concat_explicit(std::string regex);

    /**
     * Convert regular expression from infix to postfix notation using
     * Shunting-yard algorithm.
     *
     * @param regex infix notation
     * @return postfix notation
     */
    std::string regex_infix_to_postfix(std::string regex);

    // This method evaluates the postfix expression, i.e. from postfix to infix
    std::string evaluate_postfix(std::string postfix);

};


#endif