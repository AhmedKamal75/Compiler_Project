#include <iostream>
#include <set>
#include <list>
#include "phase_one/Automaton/Automaton.h"
#include "phase_one/Automaton/Utilities.h"
#include "phase_one/Automaton/Conversions.h"
#include "phase_one/creation/InfixToPostfix.h"

Conversions conversions;
InfixToPostfix infixToPostfix;

void infix_to_postfix_test();

void automaton_test();

std::unordered_map<std::string, std::shared_ptr<Automaton>> getAutomata();

int main() {
//    automaton_test();
    infix_to_postfix_test();
}


void infix_to_postfix_test() {
    std::string regex = "(hello|hi)+(and|or)(welcome|out!)(infinite_loop)*";
    std::string concat_explicit = infixToPostfix.concatExplicit(regex);
    std::string postfix = infixToPostfix.infixToPostfix(concat_explicit);
    std::string evaluated_regex = infixToPostfix.evaluatePostfix(postfix);
    std::cout << "regex    : " << regex << std::endl;
    std::cout << "concat   : " << concat_explicit << std::endl;
    std::cout << "postfix  : " << postfix << std::endl;
    std::cout << "evaluated: " << evaluated_regex << std::endl;

}


void automaton_test() {
    std::unordered_map<std::string, std::shared_ptr<Automaton>> automata = getAutomata();
    std::shared_ptr<Automaton> aP = Utilities::positiveClosure(automata["a"], "");
    std::shared_ptr<Automaton> bK = Utilities::kleeneClosure(automata["b"], "");
    std::shared_ptr<Automaton> aUb = Utilities::unionAutomata(automata["a"], automata["b"], "");
    std::shared_ptr<Automaton> aPUbK = Utilities::unionAutomata(aP, bK, "");
    std::shared_ptr<Automaton> aPUbKK = Utilities::kleeneClosure(aPUbK, "");
    std::shared_ptr<Automaton> aPUbKKa = Utilities::concatAutomaton(aPUbKK, automata["a"], "");
    std::shared_ptr<Automaton> aPbK = Utilities::concatAutomaton(aP, bK, "");
    std::shared_ptr<Automaton> aPbKaPUbK = Utilities::concatAutomaton(aPbK, aPUbK, "");

    std::shared_ptr<Automaton> nfa = Utilities::copyAutomaton(aPbKaPUbK);
    std::cout << "nfa:" << std::endl;
    std::cout << nfa->toString() << std::endl;
    std::shared_ptr<Automaton> dfa = conversions.convertToDFA(nfa);
    std::cout << "dfa:" << std::endl;
    std::cout << dfa->toString() << std::endl;
    std::shared_ptr<Automaton> minDFA = conversions.minimizeDFA(dfa);
    std::cout << "min dfa:" << std::endl;
    std::cout << minDFA->toString() << std::endl;

}


std::unordered_map<std::string, std::shared_ptr<Automaton>> getAutomata() {
    std::list<std::string> alphabets;
    for (char letter = 'a'; letter <= 'z'; letter++) {
        alphabets.emplace_back(1, letter);
    }
    std::unordered_map<std::string, std::shared_ptr<Automaton>> automatas;
    for (const std::string &alphabet: alphabets) {
        automatas[alphabet] = std::make_shared<Automaton>(alphabet, "", "");
    }
    return automatas;
}