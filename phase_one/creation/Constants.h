#ifndef COMPILER_PROJECT_CONSTANTS_H
#define COMPILER_PROJECT_CONSTANTS_H


#include <unordered_map>

#include <unordered_map>

class Constants {
public:
    const char ESCAPE = '\\';
    const char KLEENE_CLOSURE = '*';
    const char POSITIVE_CLOSURE = '+';
    const char RANGE = '-';
    const char CONCATENATION = '.';
    const char UNION = '|';
    const char OPEN_PARENTHESIS = '(';
    const char CLOSE_PARENTHESIS = ')';
    std::unordered_map<char, int> priorities;

    Constants();
    int priority(char operatorChar);
    bool isOperator(char c);
};



#endif //COMPILER_PROJECT_CONSTANTS_H
