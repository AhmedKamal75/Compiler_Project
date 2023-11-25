#ifndef COMPILER_PROJECT_CONSTANTS_H
#define COMPILER_PROJECT_CONSTANTS_H


#include <unordered_map>

#include <unordered_map>
#include <string>

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

    int priority(std::string operatorChar);

    bool is_operator(char c);

    bool is_operator(std::string c);

    bool is_operator(const std::string &str, const char &op);
};


#endif //COMPILER_PROJECT_CONSTANTS_H
