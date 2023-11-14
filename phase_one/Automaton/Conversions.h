#ifndef COMPILER_PROJECT_CONVERSIONS_H
#define COMPILER_PROJECT_CONVERSIONS_H


#include "Automaton.h"
#include "Utilities.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>

/**
 * This class provides methods for converting automata.
 * It includes methods for converting epsilon-NFAs to NFAs and NFAs to DFAs.
 */
class Conversions {
public:

    /**
     * Constructs a new Conversion object.
     */
    Conversions();

    /**
     * Prepares the Conversion object for a new automaton.
     * This method should be called before starting to work with a new automaton.
     *
     * @param a the new automaton
     */
    void prepareForAutomaton(Automaton &a);

    /**
     * Computes the epsilon-closure of a state in an automaton.
     * The epsilon-closure of a state is the set of states that can be reached
     * from the state by following epsilon-transitions.
     * it uses the depth first search algorithm.
     *
     * @param a     the automaton
     * @param state the state
     * @return the epsilon-closure of the state
     */
    std::vector<std::shared_ptr<State>> epsilonClosure(Automaton &a, std::shared_ptr<State> state_ptr);

    /**
     * IMPORTANT NOTE: don't use this method it was tested and didn't work correctly.
     * Transforms an epsilon-NFA to a normal NFA.
     * a function to transform this epsilon automata to normal automata (i.e.
     * from epsilon nfa to normal nfa) using this formula:
     * δ --> epsilon nfa transitions
     * δ' --> normal nfa transitions
     * δ'(state, symbol) = epsilon_closure(δ(epsilon_closure(state),symbol))
     * x = epsilon_closure(state)
     * y = δ(x,symbol)
     * z = epsilon_closure(y)
     * δ'(state, symbol) = [z]
     *
     * @param automaton the epsilon-NFA to transform
     * @return a new automaton that is the normal NFA equivalent of the input
     * epsilon-NFA
     */
    Automaton *removeEpsilonTransitions(Automaton &automaton);

    Automaton *convertToDFA(Automaton &automaton);

    Automaton *minimizeDFA(Automaton &automaton);


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

private:

    std::vector<std::pair<std::shared_ptr<State>, std::vector<std::shared_ptr<State>>>> epsilon_closures;

    int counter;
};

#endif //COMPILER_PROJECT_CONVERSIONS_H
