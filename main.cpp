#include <iostream>
#include <set>
#include <map>
#include <list>
#include <vector>
#include <algorithm>
#include "phase_one/Automaton/State.h"
#include "phase_one/Automaton/Automaton.h"
#include "phase_one/Automaton/Utilities.h"

void test1();

void test0();

std::unordered_map<std::string, Automaton *> getAutomata();


int main() {
    test1();
//    test0();
}

void test0() {
    Automaton a("a", "", "");
    Automaton b("b", "", "");
    std::cout << a.toString() << std::endl;
    std::cout << b.toString() << std::endl;

    std::vector<Automaton> v = {a, b};
    Automaton ab = *Utilities::unionAutomataSet(v);
    std::cout << ab.toString() << std::endl;
//    ab.giveNewIdsAll(-19,false);
//    std::cout << a.toString() << std::endl;
//    std::cout << b.toString() << std::endl;
//    std::cout << ab.toString() << std::endl;
}


void test1() {
    std::unordered_map<std::string, Automaton *> automata = getAutomata();
    Automaton aP = *Utilities::positiveClosure(*automata["a"], "");
    Automaton bK = *Utilities::kleeneClosure(*automata["b"], "");
    Automaton aUbK = *Utilities::kleeneClosure(*Utilities::unionAutomata(*automata["a"], *automata["b"], ""), "");
    std::cout << aUbK.toString() << std::endl;
    Automaton aKbKaUbK = *Utilities::concatAutomaton(*Utilities::concatAutomaton(aP, bK, ""), aUbK, "");
    std::cout << aKbKaUbK.toString() << std::endl;

}


std::unordered_map<std::string, Automaton *> getAutomata() {
    std::list<std::string> alphabets;
    for (char letter = 'a'; letter <= 'z'; letter++) {
        alphabets.emplace_back(1, letter);
    }
    std::unordered_map<std::string, Automaton *> automatas;
    for (const std::string &alphabet: alphabets) {
        automatas[alphabet] = new Automaton(alphabet, "", "");
    }
    return automatas;
}