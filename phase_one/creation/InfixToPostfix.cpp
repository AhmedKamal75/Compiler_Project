#include <stack>
#include <utility>
#include "InfixToPostfix.h"

InfixToPostfix::InfixToPostfix() : constants() {}

std::string InfixToPostfix::concatExplicit(std::string regex) {
    std::string newRegex;
    for (int i = 0; i < regex.length() - 1; i++) {
        char c1 = regex[i];
        char c2 = regex[i + 1];

        newRegex += c1;
        // If the current character is not an operator or is a closing parenthesis or a Kleene closure or a positive closure
        // and the next character is not an operator or is an opening parenthesis or an escape character, then add a concatenation operator
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


std::string InfixToPostfix::infixToPostfix(std::string regex) {
    std::string postfix;
    std::stack<char> stack;
    std::string formattedRegex = concatExplicit(std::move(regex));
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
            stack.pop();
        } else if (c == constants.ESCAPE) {
            char c1 = formattedRegex[i + 1];
            postfix += c1;
            postfix += formattedRegex[i];
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


std::string InfixToPostfix::evaluatePostfix(std::string postfix) {
    std::stack<std::string> stack;
    for (int i = 0; i < postfix.length(); i++) {
        char c = postfix[i];
        if (!constants.isOperator(c)) {
            stack.emplace(1, c);
        } else {
            if (i < postfix.length() - 1 && constants.ESCAPE == postfix[i + 1]) {
                char c1 = postfix[i + 1];
                stack.push("(" + std::string(1, c1) + c + ")");
                i++;
            } else if (c == constants.KLEENE_CLOSURE) {
                stack.push("(" + stack.top() + ")*"); // replace with your own operation
                stack.pop();
            } else if (c == constants.POSITIVE_CLOSURE) {
                stack.push("(" + stack.top() + ")+"); // replace with your own operation
                stack.pop();
            } else if (c == constants.RANGE) {
                std::string operand2 = stack.top();
                stack.pop();
                std::string operand1 = stack.top();
                stack.pop();
                stack.push("(" + operand1 + "-" + operand2 + ")"); // replace with your own operation
            } else if (c == constants.CONCATENATION) {
                std::string operand2 = stack.top();
                stack.pop();
                std::string operand1 = stack.top();
                stack.pop();
                stack.push("(" + operand1 + "." + operand2 + ")"); // replace with your own operation
            } else if (c == constants.UNION) {
                std::string operand2 = stack.top();
                stack.pop();
                std::string operand1 = stack.top();
                stack.pop();
                stack.push("(" + operand1 + "|" + operand2 + ")"); // replace with your own operation
            } else if (c == constants.ESCAPE) {
                char c1 = postfix[i - 1];
                stack.pop();
                stack.push("(" + std::string(1, c) + c1 + ")");
            }
        }
    }

    return stack.top();
}