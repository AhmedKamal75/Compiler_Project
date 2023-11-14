#include <iostream>
#include <set>
#include <list>
#include <vector>
#include "phase_one/Automaton/State.h"
#include "phase_one/Automaton/Automaton.h"
#include "phase_one/Automaton/Utilities.h"
#include "phase_one/Automaton/Conversions.h"

Conversions conversions;

void test1();

void test0();

std::unordered_map<std::string, std::shared_ptr<Automaton>> getAutomata();

int main() {
    test1();
//    test0();
}

void test0() {
    std::shared_ptr<Automaton> a = std::make_shared<Automaton>("a", "", "");
    std::shared_ptr<Automaton> b = std::make_shared<Automaton>("b", "", "");
    std::cout << a->toString() << std::endl;
    std::cout << b->toString() << std::endl;

    std::vector<std::shared_ptr<Automaton>> v = {a, b};
    std::shared_ptr<Automaton> ab = Utilities::unionAutomataSet(v);
    std::cout << ab->toString() << std::endl;
//    ab.giveNewIdsAll(-19,false);
//    std::cout << a.toString() << std::endl;
//    std::cout << b.toString() << std::endl;
//    std::cout << ab.toString() << std::endl;
}


void test1() {
    std::unordered_map<std::string, std::shared_ptr<Automaton>> automata = getAutomata();
    std::shared_ptr<Automaton> aP = Utilities::positiveClosure(automata["a"], "");
    std::shared_ptr<Automaton> bK = Utilities::kleeneClosure(automata["b"], "");
    std::shared_ptr<Automaton> aUb = Utilities::unionAutomata(automata["a"], automata["b"], "");
    std::shared_ptr<Automaton> aPUbK = Utilities::unionAutomata(aP, bK, "");
    std::shared_ptr<Automaton> aPUbKK = Utilities::kleeneClosure(aPUbK, "");
    std::shared_ptr<Automaton> aPUbKKa = Utilities::concatAutomaton(aPUbKK, automata["a"], "");
    std::shared_ptr<Automaton> aPbK = Utilities::concatAutomaton(aP, bK, "");
    std::shared_ptr<Automaton> aPbKaPUbK = Utilities::concatAutomaton(aPbK, aPUbK, "");

    std::cout << aP->toString() << std::endl;
    std::shared_ptr<Automaton> dfa = conversions.convertToDFA(aPUbKK);
    std::cout << dfa->toString() << std::endl;
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