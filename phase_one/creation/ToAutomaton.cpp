#include <string>
#include <memory>
#include <stack>
#include <utility>
#include "ToAutomaton.h"
#include "../automaton/Utilities.h"

std::shared_ptr<Automaton> ToAutomaton::regex_to_minimized_dfa(std::string regex, const std::string &epsilon_symbol) {
    // Parse the regex and construct the corresponding postfix
    std::string postfix = infixToPostfix.regex_infix_to_postfix(std::move(regex));
    // parse the postfix regex (easier) to an Automaton
    std::shared_ptr<Automaton> nfa = get_automaton_from_regex_postfix(postfix, epsilon_symbol);
    // Convert the regex automaton to a DFA and minimize it
    std::shared_ptr<Automaton> dfa = conversions.convertToDFA(nfa, false);
    // return the minimized dfa
    std::shared_ptr<Automaton> minDFa = conversions.minimizeDFA(dfa);
    /*
     *TODO: see which type of regex do you want the automaton to have
     * this:
     */
    minDFa->set_regex(infixToPostfix.regex_evaluate_postfix(postfix));
    /*TODO:
     * or this:
     */
//    minDFa->set_regex(nfa->get_regex());
    return minDFa;
}

std::shared_ptr<Automaton> ToAutomaton::regular_definition_to_minimized_dfa(const std::string &regular_definition,
                                                                            const std::unordered_map<std::string, std::shared_ptr<Automaton>> &automata,
                                                                            const std::string &epsilon_symbol) {
    // transform the regular definition to tokens to make it easy to handle
    std::vector<std::string> tokens = infixToPostfix.tokenize(regular_definition);
    // Parse the regular definition and construct the corresponding postfix
    std::vector<std::string> rd_postfix = infixToPostfix.regular_definition_infix_to_postfix(tokens);

    std::shared_ptr<Automaton> nfa = get_automaton_from_regular_definition(rd_postfix, automata, epsilon_symbol);
    std::shared_ptr<Automaton> dfa = conversions.convertToDFA(nfa, false);
    std::shared_ptr<Automaton> minimized_dfa = conversions.minimizeDFA(dfa);
    /*
     *TODO: see which type of regex do you want the automaton to have
     * this:
     */
//    minimized_dfa->set_regex(infixToPostfix.regular_definition_evaluate_postfix(rd_postfix));
    /*TODO:
     * or this:
     */
    minimized_dfa->set_regex(nfa->get_regex());

    return minimized_dfa;
}


std::shared_ptr<Automaton>
ToAutomaton::get_automaton_from_regex_postfix(const std::string &postfix, const std::string &epsilonSymbol) {
    std::stack<std::shared_ptr<Automaton>> stack;
    for (int i = 0; i < postfix.length(); i++) {
        char c = postfix[i];
        if (!constants.is_operator(c)) {
            stack.push(std::make_shared<Automaton>(std::string(1, c), "", epsilonSymbol));
        } else {
            if ((i < postfix.length() - 1) && (constants.ESCAPE == postfix[i + 1]) && (constants.is_operator(c))) {
                stack.push(std::make_shared<Automaton>(std::string(1, c), "", epsilonSymbol));
                i++;
            } else if (c == constants.KLEENE_CLOSURE) {
                std::shared_ptr<Automaton> a = Utilities::kleeneClosure(stack.top());
                stack.pop();
                stack.push(a);
            } else if (c == constants.POSITIVE_CLOSURE) {
                std::shared_ptr<Automaton> a = Utilities::positiveClosure(stack.top());
                stack.pop();
                stack.push(a);
            } else if (c == constants.CONCATENATION) {
                std::shared_ptr<Automaton> operand2 = stack.top();
                stack.pop();
                std::shared_ptr<Automaton> operand1 = stack.top();
                stack.pop();
                stack.push(Utilities::concatAutomaton(operand1, operand2));
            } else if (c == constants.UNION) {
                std::shared_ptr<Automaton> operand2 = stack.top();
                stack.pop();
                std::shared_ptr<Automaton> operand1 = stack.top();
                stack.pop();
                stack.push(Utilities::unionAutomata(operand1, operand2));
            } else if (c == constants.RANGE) {
                std::shared_ptr<Automaton> end = stack.top();
                stack.pop();
                std::shared_ptr<Automaton> start = stack.top();
                stack.pop();

                std::shared_ptr<Automaton> unionAll = start;
                for (char letter = start->get_alphabets().begin()->at(0) + 1;
                     letter < end->get_alphabets().begin()->at(0); letter++) {
                    std::shared_ptr<Automaton> tempA = std::make_shared<Automaton>(std::string(1, letter), "",
                                                                                   epsilonSymbol);
                    unionAll = Utilities::unionAutomata(unionAll, tempA);
                }
                stack.push(Utilities::unionAutomata(unionAll, end));
            }
        }
    }

    return stack.top();
}

std::shared_ptr<Automaton> ToAutomaton::get_automaton_from_regular_definition(std::vector<std::string> postfix_tokens,
                                                                              const std::unordered_map<std::string, std::shared_ptr<Automaton>> &map,
                                                                              const std::string &epsilonSymbol) {
    std::stack<std::shared_ptr<Automaton>> stack;
    for (int i = 0; i < postfix_tokens.size(); i++) {
        std::string token = postfix_tokens[i];
        if (!constants.is_operator(token)) {
            if ((i < postfix_tokens.size() - 1) && constants.is_operator(postfix_tokens[i + 1], constants.ESCAPE)) {
                std::string temp = postfix_tokens[i + 1];
                /*TODO: see if you will do something with the escape character that is temp (I did nothing).*/
                std::shared_ptr<Automaton> a = std::make_shared<Automaton>();
                if (token == "L") {
                    // if it is the epsilon character.
                    a = Utilities::get_epsilon_automaton(epsilonSymbol);
                } else {
                    if (token.size() == 1){
                        a = std::make_shared<Automaton>(token,temp + token,epsilonSymbol);
                    } else {
                        a = get_automaton_from_map(token, map, epsilonSymbol);
                    }
                }
                if (a == nullptr) { // that mean that the tokens needs a token that is not defined yet
                    return nullptr;
                }
                stack.push(a);
                i++;
            } else {
                std::shared_ptr<Automaton> a = get_automaton_from_map(token, map, epsilonSymbol);
                if (a == nullptr) { // that mean that the tokens needs a token that is not defined yet
                    return nullptr;
                }
                stack.push(a);
            }
        } else {
            if ((i < postfix_tokens.size() - 1) &&
                (constants.is_operator(postfix_tokens[i + 1], constants.ESCAPE) && constants.is_operator(token))) {
                /*TODO: see if you will uncomment the next line*/
                //std::string token = postfix_tokens[i+1] + token;
                std::shared_ptr<Automaton> a = get_automaton_from_map(token, map, epsilonSymbol);
                if (a == nullptr) { // that mean that the tokens needs a token that is not defined yet
                    return nullptr;
                }
                stack.push(a);
                i++;
            } else if (constants.is_operator(token, constants.KLEENE_CLOSURE)) {
                std::shared_ptr<Automaton> a = Utilities::kleeneClosure(stack.top());
                stack.pop();
                stack.push(a);
            } else if (constants.is_operator(token, constants.POSITIVE_CLOSURE)) {
                std::shared_ptr<Automaton> a = Utilities::positiveClosure(stack.top());
                stack.pop();
                stack.push(a);
            } else if (constants.is_operator(token, constants.RANGE)) {
                std::shared_ptr<Automaton> end = stack.top();
                stack.pop();
                std::shared_ptr<Automaton> start = stack.top();
                stack.pop();

                std::shared_ptr<Automaton> unionAll = start;
                for (char letter = start->get_alphabets().begin()->at(0) + 1;
                     letter < end->get_alphabets().begin()->at(0); letter++) {
                    std::shared_ptr<Automaton> tempA = std::make_shared<Automaton>(std::string(1, letter), "",
                                                                                   epsilonSymbol);
                    unionAll = Utilities::unionAutomata(unionAll, tempA);
                }
                stack.push(Utilities::unionAutomata(unionAll, end));
            } else if (constants.is_operator(token, constants.CONCATENATION)) {
                std::shared_ptr<Automaton> operand2 = stack.top();
                stack.pop();
                std::shared_ptr<Automaton> operand1 = stack.top();
                stack.pop();
                stack.push(Utilities::concatAutomaton(operand1, operand2));
            } else if (constants.is_operator(token, constants.UNION)) {
                std::shared_ptr<Automaton> operand2 = stack.top();
                stack.pop();
                std::shared_ptr<Automaton> operand1 = stack.top();
                stack.pop();
                stack.push(Utilities::unionAutomata(operand1, operand2));
            }
        }
    }

    return stack.top();
}

std::shared_ptr<Automaton> ToAutomaton::get_automaton_from_map(const std::string &token,
                                                               const std::unordered_map<std::string, std::shared_ptr<Automaton>> &map,
                                                               const std::string &epsilonSymbol) {
    auto it = map.find(token);
    if (it != map.end()) {
        // If the token exists in the map, return the corresponding Automaton
        return it->second;
    } else {

        // If the token does not exist in the map:
        if (token.size() > 1) {
            // if its size is bigger than 1, then that means that it is a token to be defined in the future.
            return nullptr;
        }
        // init a new Automaton and return it
        return std::make_shared<Automaton>(token, token, epsilonSymbol);
    }
}
