#ifndef COMPILER_PROJECT_CONVERSIONS_H
#define COMPILER_PROJECT_CONVERSIONS_H


#include "Automaton.h"
#include "Utilities.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>

class Conversions {
public:
    Conversions();

    void prepareForAutomaton(Automaton &a);

    std::unordered_set<std::shared_ptr<State>> epsilonClosure(Automaton &a, std::shared_ptr<State> state);

    Automaton *removeEpsilonTransitions(Automaton &automaton);

    Automaton *convertToDFA(Automaton &automaton);

    Automaton *minimizeDFA(Automaton &automaton);

private:
    std::unordered_map<std::shared_ptr<State>, std::unordered_set<std::shared_ptr<State>>> epsilonClosures;
    int counter;
    Utilities utilities;

    std::shared_ptr<State>
    createDFAState(std::unordered_set<std::shared_ptr<State>> &stateSet, Automaton &a, Automaton &dfa);

    std::shared_ptr<State> getDFAState(std::unordered_set<std::shared_ptr<State>> &stateSet,
                                       std::unordered_map<std::unordered_set<std::shared_ptr<State>>, std::shared_ptr<State>> &dfaStates);

    std::shared_ptr<State> createDeadState(Automaton &dfa);

    std::vector<std::unordered_set<std::shared_ptr<State>>>
    getNextEquivalence(std::vector<std::unordered_set<std::shared_ptr<State>>> &prevEquivalence,
                       std::unordered_set<std::string> &alphabets,
                       std::unordered_map<std::pair<std::shared_ptr<State>, std::string>, std::shared_ptr<State>> &transitions);

    std::pair<std::vector<std::shared_ptr<State>>, std::pair<std::shared_ptr<State>, std::unordered_set<std::shared_ptr<State>>>>
    getNewStatesAndSpecialStates(std::vector<std::unordered_set<std::shared_ptr<State>>> &group, Automaton &automaton);

    std::unordered_map<std::pair<std::shared_ptr<State>, std::string>, std::shared_ptr<State>>
    getNewTransitions(Automaton &oldDFA, std::vector<std::unordered_set<std::shared_ptr<State>>> &group,
                      std::vector<std::shared_ptr<State>> &newStates);
};

#endif //COMPILER_PROJECT_CONVERSIONS_H
