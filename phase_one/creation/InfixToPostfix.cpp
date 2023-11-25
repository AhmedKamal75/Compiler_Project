#include <stack>
#include <utility>
#include <stdexcept>
#include "InfixToPostfix.h"

InfixToPostfix::InfixToPostfix() : constants() {}

std::vector<std::string> InfixToPostfix::tokenize(const std::string &regular_definition) {
    std::vector<std::string> tokens;
    tokens.reserve(regular_definition.length());  // Reserve space for tokens
    std::string buffer;
    for (char c: regular_definition) {
        if (!constants.is_operator(c) && !std::isspace(c)) {
            buffer.push_back(c);
        } else {
            if (!buffer.empty()) {
                tokens.push_back(buffer);
                buffer.clear();
            }
            if (c == constants.CONCATENATION ||
                c == constants.RANGE) { // . and - are constants, so I add \ before them.
                tokens.emplace_back(1, constants.ESCAPE);
            } else if (std::isspace(c)) {
                continue;
            }
            tokens.emplace_back(1, c);

        }
    }
    if (!buffer.empty()) {
        tokens.push_back(buffer);
    }
    return tokens;
}

std::string InfixToPostfix::concat_explicit(std::string regex) {
    std::string newRegex;
    for (int i = 0; i < regex.length() - 1; i++) {
        char c1 = regex[i];
        char c2 = regex[i + 1];

        newRegex += c1;
        // If the current character is not an operator or is a closing parenthesis or a Kleene closure or a positive closure
        // and the next character is not an operator or is an opening parenthesis or an escape character, then add a concatenation operator
        // if ((the possibilities of c1) && (the possibilities of c2)) add .
        if ((!constants.is_operator(c1) || c1 == constants.CLOSE_PARENTHESIS ||
             c1 == constants.KLEENE_CLOSURE || c1 == constants.POSITIVE_CLOSURE ||
             (constants.is_operator(c1) && i != 0 && regex[i - 1] == constants.ESCAPE)) &&
            (!constants.is_operator(c2) || c2 == constants.OPEN_PARENTHESIS || c2 == constants.ESCAPE)) {
            newRegex += constants.CONCATENATION;
        }
    }
    newRegex += regex[regex.length() - 1];
    return newRegex;
}

std::vector<std::string> InfixToPostfix::concat_explicit_tokens(const std::vector<std::string> &tokens) {
    if (tokens.empty()) {
        throw std::invalid_argument("Input tokens cannot be empty.");
    }

    std::vector<std::string> newTokens;
    newTokens.reserve(tokens.size() * 2);  // Reserve space for new tokens

    for (std::size_t i = 0; i < tokens.size() - 1; ++i) {
        const std::string &token1 = tokens[i];
        const std::string &token2 = tokens[i + 1];

        newTokens.push_back(token1);

        // Define conditions for readability
        bool isToken1Operator = constants.is_operator(token1);
        bool isToken2Operator = constants.is_operator(token2);
        bool isToken1Escaped = i != 0 && constants.is_operator(tokens[i - 1], constants.ESCAPE);

        // Check if a concatenation operator should be added
        if ((!isToken1Operator || constants.is_operator(token1, constants.CLOSE_PARENTHESIS) ||
             constants.is_operator(token1, constants.KLEENE_CLOSURE) ||
             constants.is_operator(token1, constants.POSITIVE_CLOSURE) ||
             (isToken1Escaped)) &&
            (!isToken2Operator || constants.is_operator(token2, constants.OPEN_PARENTHESIS) ||
             constants.is_operator(token2, constants.ESCAPE))) {
            newTokens.emplace_back(1, constants.CONCATENATION);
        }
    }

    newTokens.push_back(tokens.back());

    return newTokens;
}

std::string InfixToPostfix::regex_infix_to_postfix(std::string regex) {
    std::string postfix;
    std::stack<char> stack;
    std::string formatted_regex = concat_explicit(std::move(regex));
    // could use some buffer to store the operands in if length is > 1
    for (int i = 0; i < formatted_regex.length(); i++) {
        char c = formatted_regex[i];
        if (!constants.is_operator(c)) {
            postfix += c;
        } else if (c == constants.OPEN_PARENTHESIS) {
            stack.push(c);
        } else if (c == constants.CLOSE_PARENTHESIS) {
            while (stack.top() != '(') {
                postfix += stack.top();
                stack.pop();
            }
            stack.pop(); // for '(';
        } else if (c == constants.ESCAPE && (i + 1 < formatted_regex.length())) {
            char c1 = formatted_regex[i + 1];
            if (constants.is_operator(c1)) { // handle the escape character if it has an operator after it
                postfix += c1;
                postfix += formatted_regex[i];
            } else { // if it is just a regular character.
                postfix += formatted_regex[i];
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

std::vector<std::string> InfixToPostfix::regular_definition_infix_to_postfix(const std::vector<std::string> &tokens) {
    std::vector<std::string> postfix;
    postfix.reserve(tokens.size() * 2);  // Reserve space for postfix
    std::stack<std::string> stack;
    std::vector<std::string> formatted_tokens = concat_explicit_tokens(tokens);
    formatted_tokens.reserve(tokens.size() * 2);  // Reserve space for formatted_tokens

    for (int i = 0; i < formatted_tokens.size(); i++) {
        std::string token = formatted_tokens[i];
        if (!constants.is_operator(token)) {
            postfix.push_back(token);
        } else if (constants.is_operator(token, constants.OPEN_PARENTHESIS)) {
            stack.push(token);
        } else if (constants.is_operator(token, constants.CLOSE_PARENTHESIS)) {
            while (!constants.is_operator(stack.top(), constants.OPEN_PARENTHESIS)) {
                postfix.push_back(stack.top());
                stack.pop();
            }
            stack.pop(); // for '(';
        } else if (constants.is_operator(token, constants.ESCAPE) && (i + 1 < formatted_tokens.size())) {
            std::string next_token = formatted_tokens[i + 1];
            // handle the escape character if it has an operator or  after it
            postfix.push_back(next_token);
            postfix.push_back(token);
            i++;
        } else { //  it is an operator
            while (!stack.empty() && !constants.is_operator(stack.top(), constants.OPEN_PARENTHESIS)) {
                if (constants.priority(stack.top()) >= constants.priority(token)) {
                    postfix.push_back(stack.top());
                    stack.pop();
                } else {
                    break;
                }
            }
            stack.push(token); // Push the current operator onto the stack
        }
    }

    while (!stack.empty()) {
        postfix.push_back(stack.top());
        stack.pop();
    }

    return postfix;
}

std::string InfixToPostfix::regex_evaluate_postfix(std::string postfix) {
    std::stack<std::string> stack;
    for (int i = 0; i < postfix.length(); i++) {
        char c = postfix[i];
        if (!constants.is_operator(c)) {
            stack.emplace(1, c);
        } else {
            if ((i < postfix.length() - 1) && (constants.ESCAPE == postfix[i + 1]) && (constants.is_operator(c))) {
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
                if (constants.is_operator(c1)) {
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

std::string InfixToPostfix::regular_definition_evaluate_postfix(std::vector<std::string> postfix_tokens) {
    std::stack<std::string> stack;
    for (int i = 0; i < postfix_tokens.size(); i++) {
        std::string token = postfix_tokens[i];
        if (!constants.is_operator(token)) {
            if ((i < postfix_tokens.size() - 1) && constants.is_operator(postfix_tokens[i + 1], constants.ESCAPE)) {
                std::string temp{};
                temp.append("(").append(postfix_tokens[i + 1]).append(token).append(")");
                stack.push(temp);
                i++;
            } else {
                stack.push(token);
            }
        } else {
            if ((i < postfix_tokens.size() - 1) &&
                (constants.is_operator(postfix_tokens[i + 1], constants.ESCAPE) && constants.is_operator(token))) {
                std::string next_token = postfix_tokens[i + 1];
                std::string temp{};
                temp.append("(").append(next_token).append(token).append(")");
                stack.push(temp);
                i++;
            } else if (constants.is_operator(token, constants.KLEENE_CLOSURE)) {
                std::string temp;
                temp.append("(").append(stack.top()).append(")*");
                stack.pop();
                stack.push(temp);
            } else if (constants.is_operator(token, constants.POSITIVE_CLOSURE)) {
                std::string temp;
                temp.append("(").append(stack.top()).append(")+");
                stack.pop();
                stack.push(temp);
            } else if (constants.is_operator(token, constants.RANGE)) {
                std::string operand2 = stack.top();
                stack.pop();
                std::string operand1 = stack.top();
                stack.pop();
                std::string temp;
                temp.append("(").append(operand1).append("-").append(operand2).append(")");
                stack.push(temp);
            } else if (constants.is_operator(token, constants.CONCATENATION)) {
                std::string operand2 = stack.top();
                stack.pop();
                std::string operand1 = stack.top();
                stack.pop();
                std::string temp;
                temp.append("(").append(operand1).append(".").append(operand2).append(")");
                stack.push(temp);
            } else if (constants.is_operator(token, constants.UNION)) {
                std::string operand2 = stack.top();
                stack.pop();
                std::string operand1 = stack.top();
                stack.pop();
                std::string temp;
                temp.append("(").append(operand1).append("|").append(operand2).append(")");
                stack.push(temp);
            } else if (constants.is_operator(token, constants.ESCAPE)) {
                std::string previous_token = postfix_tokens[i - 1];
                if (constants.is_operator(previous_token)) {
                    std::string operator_literal = stack.top();
                    stack.pop();
                    std::string temp;
                    temp.append("(").append(token).append(operator_literal).append(")");
                    stack.push(temp);
                }
            }
        }
    }

    return stack.top();
}
