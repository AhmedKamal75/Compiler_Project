#include <string>
#include <memory>
#include <stack>
#include <utility>
#include "ToAutomaton.h"
#include "../automaton/Utilities.h"

std::shared_ptr<Automaton> ToAutomaton::regex_to_minimized_dfa(std::string regex, const std::string &epsilonSymbol) {
    // Parse the regex and construct the corresponding automaton
    std::string postfix = infixToPostfix.regex_infix_to_postfix(std::move(regex));
    // parse the postfix regex (easier) to an Automaton
    std::shared_ptr<Automaton> regexAutomaton = get_automaton_from_regex_postfix(postfix, epsilonSymbol);
    // Convert the regex automaton to a DFA and minimize it
    std::shared_ptr<Automaton> dfa = conversions.convertToDFA(regexAutomaton);
    // return the minimized dfa
    std::shared_ptr<Automaton> minDFa = conversions.minimizeDFA(dfa);
    //TODO
    // delete the following line
    minDFa->set_regex(infixToPostfix.regex_evaluate_postfix(postfix));
    /*TODO:
     * uncomment the following line.
     */
//    minDFa->setRegex(regex);
    return minDFa;
}

std::shared_ptr<Automaton>
ToAutomaton::get_automaton_from_regex_postfix(const std::string &postfix, const std::string &epsilonSymbol) {
    std::stack<std::shared_ptr<Automaton>> stack;
    for (int i = 0; i < postfix.length(); i++) {
        char c = postfix[i];
        if (!constants.isOperator(c)) {
            stack.push(std::make_shared<Automaton>(std::string(1, c), "", epsilonSymbol));
        } else {
            if ((i < postfix.length() - 1) && (constants.ESCAPE == postfix[i + 1]) && (constants.isOperator(c))) {
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

std::shared_ptr<Automaton> ToAutomaton::regular_definition_to_min_dfa(std::string regularDefinition,
                                                                      std::unordered_map<std::string, std::shared_ptr<Automaton>> map,
                                                                      const std::string &epsilonSymbol) {
    std::stack<std::shared_ptr<Automaton>> stack;
    std::string name;
    bool build = true;
    for (int i = 0; i < regularDefinition.length(); i++) {
        char c = regularDefinition[i];
        if (!constants.isOperator(c)) {
            stack.push(std::make_shared<Automaton>(std::string(1, c), "", epsilonSymbol));
        } else {
            if (i < regularDefinition.length() - 1 && constants.ESCAPE == regularDefinition[i + 1]) {
                stack.push(std::make_shared<Automaton>(std::string(1, c), "", epsilonSymbol));
                i++;
            } else if (c == constants.KLEENE_CLOSURE) {
                stack.push(Utilities::kleeneClosure(stack.top())); // replace with your own operation
                stack.pop();
            } else if (c == constants.POSITIVE_CLOSURE) {
                stack.push(Utilities::positiveClosure(stack.top())); // replace with your own operation
                stack.pop();
            } else if (c == constants.RANGE) {
                char end = stack.top()->get_alphabets().begin()->at(0);
                stack.pop();
                char start = stack.top()->get_alphabets().begin()->at(0);
                stack.pop();
                std::shared_ptr<Automaton> unionAll = std::make_shared<Automaton>(std::string(1, start), "",
                                                                                  epsilonSymbol);
                for (char letter = start + 1; letter <= end; letter++) {
                    std::shared_ptr<Automaton> a = std::make_shared<Automaton>(std::string(1, letter), "",
                                                                               epsilonSymbol);
                    unionAll = Utilities::unionAutomata(unionAll, a);
                }
                stack.push(unionAll);
            } else if (c == constants.CONCATENATION) {
                std::shared_ptr<Automaton> a2 = stack.top();
                stack.pop();
                std::shared_ptr<Automaton> a1 = stack.top();
                stack.pop();
                stack.push(Utilities::concatAutomaton(a1, a2)); // replace with your own operation
            } else if (c == constants.UNION) {
                std::shared_ptr<Automaton> a2 = stack.top();
                stack.pop();
                std::shared_ptr<Automaton> a1 = stack.top();
                stack.pop();
                stack.push(Utilities::unionAutomata(a1, a2)); // replace with your own operation
            } else if (c == constants.ESCAPE) {
                char c1 = regularDefinition[i - 1];
                std::shared_ptr<Automaton> a = stack.top();
                stack.pop();
                stack.push(a);
            }
        }
    }

    return stack.top();
}

