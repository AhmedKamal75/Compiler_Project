#include "Conversions.h"

Conversions::Conversions() : counter(0) {}

void Conversions::prepareForAutomaton(std::shared_ptr<Automaton> &a) {
    epsilon_closures.clear();
    counter = static_cast<int>(a->getStates().size()) + 1;
}

std::vector<std::shared_ptr<State>>
Conversions::epsilonClosure(std::shared_ptr<Automaton> &a, std::shared_ptr<State> state_ptr) {
    auto iterator = std::find_if(this->epsilon_closures.begin(), this->epsilon_closures.end(),
                                 [&state_ptr](
                                         const std::pair<std::shared_ptr<State>, std::vector<std::shared_ptr<State>>> &ptr) {
                                     return *ptr.first == *state_ptr;
                                 });
    if (iterator != this->epsilon_closures.end()) {
        return iterator->second;
    }
    std::vector<std::shared_ptr<State>> epsilon_closure_vector;
    std::stack<std::shared_ptr<State>> stack;
    stack.push(state_ptr);

    while (!stack.empty()) {
        std::shared_ptr<State> currentState = stack.top();
        stack.pop();
        epsilon_closure_vector.push_back(currentState);

        for (std::shared_ptr<State> &next_state: a->getNextStates(currentState, a->getEpsilonSymbol())) {
            auto it = std::find_if(epsilon_closure_vector.begin(), epsilon_closure_vector.end(),
                                   [&next_state](const std::shared_ptr<State> &ptr) { return *ptr == *next_state; });
            if (it == epsilon_closure_vector.end()) {
                stack.push(next_state);
            }
        }
    }

    std::pair<std::shared_ptr<State>, std::vector<std::shared_ptr<State>>> item = std::make_pair(state_ptr,
                                                                                                 epsilon_closure_vector);
    epsilon_closures.push_back(item);
    return epsilon_closure_vector;
}

[[maybe_unused]] std::shared_ptr<Automaton>
Conversions::removeEpsilonTransitions(std::shared_ptr<Automaton> &automaton) {
    // create a copy of the parameter
    std::shared_ptr<Automaton> a = Utilities::copyAutomaton(automaton);
    // Create a new automaton
    std::shared_ptr<Automaton> nfa = std::make_shared<Automaton>();
    // Copy ε-symbol, the states, alphabets, start state, and accepting states
    nfa->setEpsilonSymbol(a->getEpsilonSymbol());
    nfa->addStates(a->getStates());
    nfa->addAlphabets(a->getAlphabets());
    nfa->setStart(a->getStart());
    nfa->addFinals(a->getAccepting());

    // now we work on the transitions, and add new accepting states

    // prepare this Conversions object for new automaton
    this->prepareForAutomaton(a);
    // For each state and each alphabet, compute the set of reachable states, and if they contain any accepting state,
    // we make the state accepting
    for (std::shared_ptr<State> &state_ptr: a->getStates()) {
        for (std::string &alphabet: a->getAlphabets()) {
            if (alphabet != a->getEpsilonSymbol()) {
                // x = epsilon_closure(state)
                std::vector<std::shared_ptr<State>> x = epsilonClosure(a, state_ptr);

                // Check if any state in x is an accepting state
                for (const auto &x_state_ptr: x) {
                    if (a->isAcceptingState(x_state_ptr)) {
                        state_ptr->setAccepting(true);
                        state_ptr->setToken(x_state_ptr->getToken());
                        nfa->addFinal(state_ptr);
                        break;
                    }
                }

                // y = δ(x,alphabet)
                std::vector<std::shared_ptr<State>> y;
                for (std::shared_ptr<State> &from_state_ptr: x) {
                    std::vector<std::shared_ptr<State>> temp_next_states = a->getNextStates(from_state_ptr, alphabet);
                    Utilities::addAll(x, temp_next_states);
                }

                // z = epsilon_closure(y)
                std::vector<std::shared_ptr<State>> z;
                for (auto &s: y) {
                    std::vector<std::shared_ptr<State>> sub_epsilon_closure = epsilonClosure(a, s);
                    Utilities::addAll(z, sub_epsilon_closure);
                }
                nfa->addTransitions(state_ptr, alphabet, z);
            }
        }
    }

    nfa->giveNewIdsAll();

    return nfa;
}

std::shared_ptr<State>
Conversions::createDFAState(std::vector<std::shared_ptr<State>> &state_vector, std::shared_ptr<Automaton> &a,
                            std::shared_ptr<Automaton> &dfa) {
    std::shared_ptr<State> new_state_ptr = std::make_shared<State>(++this->counter, false, "");
    dfa->addState(new_state_ptr);
    if (a->hasAcceptingState(state_vector)) {
        new_state_ptr->setAccepting(true);
        new_state_ptr->setToken(a->getToken());
        dfa->addFinal(new_state_ptr);
    }
    return new_state_ptr;
}

[[maybe_unused]] std::shared_ptr<State> Conversions::getDFAState(std::vector<std::shared_ptr<State>> &state_vector,
                                                                 std::vector<std::pair<std::vector<std::shared_ptr<State>>, std::shared_ptr<State>>> &dfa_states) {
    auto it = std::find_if(dfa_states.begin(), dfa_states.end(),
                           [&state_vector](
                                   std::pair<std::vector<std::shared_ptr<State>>, std::shared_ptr<State>> &entry) {
                               return Utilities::vector_equal(state_vector, entry.first);
                           });
    if (it == dfa_states.end()) {
        return nullptr;
    }
    return it->second;
}

[[maybe_unused]] std::shared_ptr<State> Conversions::createDeadState(std::shared_ptr<Automaton> &a) {
    std::shared_ptr<State> deadState = std::make_shared<State>(++this->counter, false, "");
    a->addState(deadState);
    for (const std::string &alphabet: a->getAlphabets()) {
        a->addTransitions(deadState, alphabet, {deadState});
    }
    return deadState;
}

[[maybe_unused]] std::shared_ptr<Automaton> Conversions::convertToDFA(std::shared_ptr<Automaton> &automaton) {
    // Create a copy of this
    std::shared_ptr<Automaton> a = Utilities::copyAutomaton(automaton);

    // Create a new automaton
    std::shared_ptr<Automaton> dfa = std::make_shared<Automaton>();

    // Copy the alphabets and epsilon symbol
    dfa->addAlphabets(a->getAlphabets());
    dfa->setEpsilonSymbol(a->getEpsilonSymbol());

    std::vector<std::pair<std::vector<std::shared_ptr<State>>, std::shared_ptr<State>>> dfa_states;
    std::queue<std::vector<std::shared_ptr<State>>> queue;

    prepareForAutomaton(a);

    // Compute the epsilon closure of the start state
    std::vector<std::shared_ptr<State>> start_vector = epsilonClosure(a, a->getStart());
    queue.push(start_vector);
    bool startIsSet = false;

    while (!queue.empty()) {
        std::vector<std::shared_ptr<State>> current_vector = queue.front();
        queue.pop();
        std::shared_ptr<State> dfa_state = getDFAState(current_vector, dfa_states);
        if (dfa_state == nullptr) {
            dfa_state = createDFAState(current_vector, a, dfa);
            dfa_states.emplace_back(current_vector, dfa_state);
        }

        // make dfa_state a part of the dfa states not the automaton

        auto it = std::find_if(current_vector.begin(), current_vector.end(),
                               [&a](std::shared_ptr<State> &temp_state_ptr) {
                                   return *a->getStart() == *temp_state_ptr;
                               }); // if a.start in current_vector

        if (!startIsSet && (it != current_vector.end())) {
            dfa->setStart(dfa_state);
            startIsSet = true;
            // that mean that the dfa will have its start state set one time only, and that
            // is for the correct state
        }
        // now we have a new state that needs to be added to the dfa


        for (std::string &alphabet: a->getAlphabets()) {
            if (alphabet != a->getEpsilonSymbol()) {
                // get the set reachable from dfaState(currentState) using current alphabet.
                std::vector<std::shared_ptr<State>> reachable_states_vector;
                for (std::shared_ptr<State> &state_ptr: current_vector) { // currentSet is already an epsilon closure
                    std::vector<std::shared_ptr<State>> temp_next_states = a->getNextStates(state_ptr, alphabet);
                    Utilities::addAll(reachable_states_vector, temp_next_states);
                }
                // Compute the epsilon closures of the next states
                std::vector<std::shared_ptr<State>> epsilon_closure_vector;
                for (std::shared_ptr<State> &state_ptr: reachable_states_vector) {
                    std::vector<std::shared_ptr<State>> sub_epsilon_closure = epsilonClosure(a, state_ptr);
                    Utilities::addAll(epsilon_closure_vector, sub_epsilon_closure);
                }
                // compute the next state from the ε-closure set calculated above.
                std::shared_ptr<State> next_state;
                if (epsilon_closure_vector.empty()) { // meaning that next state is a dead state
                    next_state = getDFAState(epsilon_closure_vector, dfa_states);
                    if (next_state == nullptr) { // no dead state was found
                        next_state = createDeadState(dfa);
                    }
                } else {
                    next_state = getDFAState(epsilon_closure_vector, dfa_states);
                    if (next_state == nullptr) {
                        next_state = createDFAState(epsilon_closure_vector, a, dfa);
                    }
                }
                // next_state calculated and dfa adjusted to accommodate it, then add the transition
                // current_state(dfa_state) --alphabet--> next_state
                dfa->addTransitions(dfa_state, alphabet, {next_state});
                // keep the following code in its order
                if (getDFAState(epsilon_closure_vector, dfa_states) == nullptr) {
                    queue.push(epsilon_closure_vector);
                }
                auto iterator = std::find_if(dfa_states.begin(), dfa_states.end(), [&epsilon_closure_vector](
                        std::pair<std::vector<std::shared_ptr<State>>, std::shared_ptr<State>> &entry) {
                    return Utilities::vector_equal(epsilon_closure_vector, entry.first);
                });
                if (iterator == dfa_states.end()) {
                    dfa_states.emplace_back(epsilon_closure_vector, next_state);
                }
            }
        }
    }

    dfa->giveNewIdsAll();

    return dfa;
}

//
//
//Automaton* Conversions::minimizeDFA(Automaton &automaton) {
//    Automaton dfa = utilities.copyAutomaton(automaton);
//
//    std::vector<std::unordered_set<std::shared_ptr<State>>> currentGroup;
//    currentGroup.push_back(dfa.getAccepting());
//    std::unordered_set<std::shared_ptr<State>> nonAcceptingStates(dfa.getStates());
//    for (auto &state : dfa.getAccepting()) {
//        nonAcceptingStates.erase(state);
//    }
//    currentGroup.push_back(nonAcceptingStates);
//
//    while (true) {
//        std::vector<std::unordered_set<std::shared_ptr<State>>> nextGroup = getNextEquivalence(currentGroup, dfa.getAlphabets(), dfa.getTransitionsDFAFormat());
//        if (currentGroup == nextGroup) {
//            break;
//        }
//        currentGroup = nextGroup;
//    }
//
//    Automaton* minDFA = new Automaton();
//    minDFA->setEpsilonSymbol(dfa.getEpsilonSymbol());
//    minDFA->getAlphabets().insert(dfa.getAlphabets().begin(), dfa.getAlphabets().end());
//
//    auto statesData = getNewStatesAndSpecialStates(currentGroup, dfa);
//    minDFA->getStates().insert(statesData.first.begin(), statesData.first.end());
//    minDFA->setStart(statesData.second.first);
//    minDFA->getAccepting().insert(statesData.second.second.begin(), statesData.second.second.end());
//
//    minDFA->setTransitionsDFAFormat(getNewTransitions(dfa, currentGroup, statesData.first));
//
//    minDFA->giveNewIdsAll(minDFA->getStates(), true);
//
//    return minDFA;
//}
//
//
//std::vector<std::unordered_set<std::shared_ptr<State>>>
//Conversions::getNextEquivalence(std::vector<std::unordered_set<std::shared_ptr<State>>> &prevEquivalence,
//                                std::unordered_set<std::string> &alphabets,
//                                std::unordered_map<std::pair<std::shared_ptr<State>, std::string>, std::shared_ptr<State>> &transitions) {
//    std::vector<std::unordered_set<std::shared_ptr<State>>> nextEquivalence;
//
//    for (auto &group: prevEquivalence) {
//        std::unordered_map<std::unordered_set<std::shared_ptr<State>>, std::unordered_set<std::shared_ptr<State>>> newGroups;
//        for (auto &state: group) {
//            std::unordered_set<std::shared_ptr<State>> key;
//            for (auto &symbol: alphabets) {
//                if (!symbol.equals(a.getEpsilonSymbol())) {
//                    auto nextState = transitions[std::make_pair(state, symbol)];
//                    for (auto &g: prevEquivalence) {
//                        if (g.find(nextState) != g.end()) {
//                            key.insert(g.begin()->get());
//                            break;
//                        }
//                    }
//                }
//            }
//            if (newGroups.find(key) == newGroups.end()) {
//                newGroups[key] = std::unordered_set<std::shared_ptr<State>>();
//            }
//            newGroups[key].insert(state);
//        }
//        for (auto &newGroup: newGroups) {
//            nextEquivalence.push_back(newGroup.second);
//        }
//    }
//
//    return nextEquivalence;
//}
//
//
//std::pair<std::vector<std::shared_ptr<State>>, std::pair<std::shared_ptr<State>, std::unordered_set<std::shared_ptr<State>>>>
//Conversions::getNewStatesAndSpecialStates(std::vector<std::unordered_set<std::shared_ptr<State>>> &group,
//                                          Automaton &automaton) {
//    std::vector<std::shared_ptr<State>> newStates;
//    std::shared_ptr<State> newStartState = nullptr;
//    std::unordered_set<std::shared_ptr<State>> newAcceptingStates;
//
//    int i = 0;
//    for (auto &g: group) {
//        auto representativeState = g.begin()->get();
//        newStates.push_back(representativeState);
//
//        if (g.find(automaton.getStart()) != g.end()) {
//            newStartState = representativeState;
//        }
//
//        for (auto &oldState: g) {
//            if (automaton.getAccepting().find(oldState) != automaton.getAccepting().end()) {
//                newAcceptingStates.insert(representativeState);
//                break;
//            }
//        }
//        i++;
//    }
//
//    return std::make_pair(newStates, std::make_pair(newStartState, newAcceptingStates));
//}
//
//std::unordered_map<std::pair<std::shared_ptr<State>, std::string>, std::shared_ptr<State>>
//Conversions::getNewTransitions(Automaton &oldDFA, std::vector<std::unordered_set<std::shared_ptr<State>>> &group,
//                               std::vector<std::shared_ptr<State>> &newStates) {
//    std::unordered_map<std::pair<std::shared_ptr<State>, std::string>, std::shared_ptr<State>> newTransitions;
//    std::unordered_map<std::shared_ptr<State>, std::shared_ptr<State>> stateToRepresentative;
//
//    int i = 0;
//    for (auto &g: group) {
//        for (auto &oldState: g) {
//            stateToRepresentative[oldState] = newStates[i];
//        }
//        i++;
//    }
//
//    for (auto &newState: newStates) {
//        for (auto &symbol: oldDFA.getAlphabets()) {
//            auto key = std::make_pair(newState, symbol);
//            newTransitions[key] = stateToRepresentative[oldDFA.getTransitionsDFAFormat()[key]];
//        }
//    }
//
//    return newTransitions;
//}
//
