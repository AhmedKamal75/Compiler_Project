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
    std::string concatExplicit(std::string regex);

    /**
     * Convert regular expression from infix to postfix notation using
     * Shunting-yard algorithm.
     *
     * @param regex infix notation
     * @return postfix notation
     */
    std::string infixToPostfix(std::string regex);

    std::string evaluatePostfix(std::string postfix);

};


#endif //COMPILER_PROJECT_INFIXTOPOSTFIX_H
