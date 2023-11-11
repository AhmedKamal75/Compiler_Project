#include <iostream>
#include <set>
#include <map>
#include <list>
#include "phase_one/Automaton/State.h"
#include "phase_one/Automaton/Automaton.h"
#include "phase_one/Automaton/Utilities.h"


std::unordered_map<std::string, Automaton *> getAutomata();

void test1();

int main() {

    test1();
    return 0;

}


void test1() {
    std::unordered_map<std::string, Automaton *> automata = getAutomata();
    Automaton aK = *Utilities::positiveClosure(*automata["a"], "");
    std::cout << aK.toString() << std::endl;
    Automaton bK = *Utilities::positiveClosure(*automata["b"], "");
    Automaton aUbK = *Utilities::kleeneClosure(*Utilities::unionAutomata(*automata["a"], *automata["b"], ""), "");
    Automaton aKbKaUbK = *Utilities::concatAutomaton(*Utilities::concatAutomaton(aK, bK, ""), aUbK, "");
    std::cout << aKbKaUbK.toString() << '\n';
    std::cout << aKbKaUbK.getToken() << '\n';

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