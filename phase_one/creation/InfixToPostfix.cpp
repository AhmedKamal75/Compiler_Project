#include <stack>
#include <utility>
#include "InfixToPostfix.h"

InfixToPostfix::InfixToPostfix() : constants() {}

std::string InfixToPostfix::concat_explicit(std::string regex) {
    std::string newRegex;
    for (int i = 0; i < regex.length() - 1; i++) {
        char c1 = regex[i];
        char c2 = regex[i + 1];

        newRegex += c1;
        // If the current character is not an operator or is a closing parenthesis or a Kleene closure or a positive closure
        // and the next character is not an operator or is an opening parenthesis or an escape character, then add a concatenation operator
        // if ((the possibilities of c1) && (the possibilities of c2)) add .
        if ((!constants.isOperator(c1) || c1 == constants.CLOSE_PARENTHESIS ||
             c1 == constants.KLEENE_CLOSURE || c1 == constants.POSITIVE_CLOSURE ||
             (constants.isOperator(c1) && i != 0 && regex[i - 1] == constants.ESCAPE)) &&
            (!constants.isOperator(c2) || c2 == constants.OPEN_PARENTHESIS || c2 == constants.ESCAPE)) {
            newRegex += constants.CONCATENATION;
        }
    }
    newRegex += regex[regex.length() - 1];
    return newRegex;
}

std::string InfixToPostfix::regex_infix_to_postfix(std::string regex) {
    std::string postfix;
    std::stack<char> stack;
    std::string formattedRegex = concat_explicit(std::move(regex));
    // could use some buffer to store the operands in if length is > 1
    for (int i = 0; i < formattedRegex.length(); i++) {
        char c = formattedRegex[i];
        if (!constants.isOperator(c)) {
            postfix += c;
        } else if (c == constants.OPEN_PARENTHESIS) {
            stack.push(c);
        } else if (c == constants.CLOSE_PARENTHESIS) {
            while (stack.top() != '(') {
                postfix += stack.top();
                stack.pop();
            }
            stack.pop(); // for '(';
        } else if (c == constants.ESCAPE) {
            char c1 = formattedRegex[i + 1];
            if (constants.isOperator(c1)) { // handle the escape character if it has an operator after it
                postfix += c1;
                postfix += formattedRegex[i];
            } else { // if it is just a regular character.
                postfix += formattedRegex[i];
                postfix += c1;
            }
            i++;
        } else { //  it is an operator
            while (!stack.empty() && stack.top() != constants.OPEN_PARENTHESIS) {
                if (constants.priority(stack.top()) >= constants.priority(c)) {
                    postfix += stack.top();
                    stack.pop();
                } else {
                    break;
                }
            }
            stack.push(c); // Push the current operator onto the stack
        }
    }

    while (!stack.empty()) {
        postfix += stack.top();
        stack.pop();
    }

    return postfix;
}

std::string InfixToPostfix::regex_evaluate_postfix(std::string postfix) {
    std::stack<std::string> stack;
    for (int i = 0; i < postfix.length(); i++) {
        char c = postfix[i];
        if (!constants.isOperator(c)) {
            stack.emplace(1, c);
        } else {
            if ((i < postfix.length() - 1) && (constants.ESCAPE == postfix[i + 1]) && (constants.isOperator(c))) {
                char c1 = postfix[i + 1];
                stack.push("(" + std::string(1, c1) + c + ")");
                i++;
            } else if (c == constants.KLEENE_CLOSURE) {
                std::string temp{};
                temp.append("(").append(stack.top()).append(")*");
                stack.pop();
                stack.push(temp);
            } else if (c == constants.POSITIVE_CLOSURE) {
                std::string temp{};
                temp.append("(").append(stack.top()).append(")+");
                stack.pop();
                stack.push(temp);
            } else if (c == constants.RANGE) {
                std::string operand2 = stack.top();
                stack.pop();
                std::string operand1 = stack.top();
                stack.pop();
                std::string temp{};
                temp.append("(").append(operand1).append("-").append(operand2).append(")");
                stack.push(temp);
            } else if (c == constants.CONCATENATION) {
                std::string operand2 = stack.top();
                stack.pop();
                std::string operand1 = stack.top();
                stack.pop();
                std::string temp{};
                temp.append("(").append(operand1).append(".").append(operand2).append(")");
                stack.push(temp);
            } else if (c == constants.UNION) {
                std::string operand2 = stack.top();
                stack.pop();
                std::string operand1 = stack.top();
                stack.pop();
                std::string temp{};
                temp.append("(").append(operand1).append("|").append(operand2).append(")");
                stack.push(temp);
            } else if (c == constants.ESCAPE) {
                char c1 = postfix[i - 1];
                if (constants.isOperator(c1)){
                    std::string operator_literal = stack.top();
                    stack.pop();
                    std::string temp{};
                    temp.append("(").append(std::to_string(c)).append(operator_literal).append(")");
                    stack.push(temp);
                }
            }
        }
    }

    return stack.top();
}