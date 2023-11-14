#include <iostream>
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

    epsilon_closures.emplace_back(state_ptr, epsilon_closure_vector);
    return epsilon_closure_vector;
}

[[maybe_unused]] [[maybe_unused]] std::shared_ptr<Automaton>
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
        // now we have a number(size=alphabets) of transitions that needs to be added to the dfa


        for (std::string &alphabet: a->getAlphabets()) {
            if (alphabet != a->getEpsilonSymbol()) {
                // get the set reachable from dfa_state(currentState) using current alphabet.
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

    dfa->setToken(a->getToken());
    dfa->giveNewIdsAll();

    return dfa;
}

std::shared_ptr<Automaton> Conversions::minimizeDFA(std::shared_ptr<Automaton> &automaton) {
    // Step 0: Create a copy of the original automaton so that it is changed.
    std::shared_ptr<Automaton> dfa = Utilities::copyAutomaton(automaton);
    // Step 1: Create a list of groups of states. Initially, there are two groups: accepting states and non-accepting states.
    std::vector<std::vector<std::shared_ptr<State>>> current_group;
    current_group.push_back(dfa->getAccepting());
    std::vector<std::shared_ptr<State>> non_accepting_state;
    for (std::shared_ptr<State> &state_ptr: dfa->getStates()) {
        if (!dfa->isAcceptingState(state_ptr)) {
            non_accepting_state.push_back(state_ptr);
        }
    }
    current_group.push_back(non_accepting_state);
    // Step 2: Refine the groups until no further refinement is possible.
    while (true) {
        std::vector<std::vector<std::shared_ptr<State>>> next_group = getNextEquivalence(current_group, dfa);
        if (Utilities::group_equal(current_group, next_group)) {
            break;
        }
        current_group = next_group;
    }

    // Step 3: Construct the minimized DFA.
    std::shared_ptr<Automaton> minDFA = std::make_shared<Automaton>();
    // fields that don't need any computations.
    minDFA->setEpsilonSymbol(dfa->getEpsilonSymbol());
    minDFA->addAlphabets(dfa->getAlphabets());
    // calculating the states.
    std::pair<std::vector<std::shared_ptr<State>>, std::pair<std::shared_ptr<State>, std::vector<std::shared_ptr<State>>>> statesData = getNewStatesAndSpecialStates(
            current_group, dfa);
    minDFA->addStates(statesData.first);
    minDFA->setStart(statesData.second.first);
    minDFA->addFinals(statesData.second.second);

    minDFA->setTransitionsDFAFormat(getNewTransitions(dfa, current_group, statesData.first));

    minDFA->setToken(dfa->getToken());
    minDFA->giveNewIdsAll();

    return minDFA;
}


std::vector<std::vector<std::shared_ptr<State>>>
Conversions::getNextEquivalence(std::vector<std::vector<std::shared_ptr<State>>> &previous_equivalence,
                                std::shared_ptr<Automaton> &dfa) {
    std::vector<std::vector<std::shared_ptr<State>>> nextEquivalence;

    for (std::vector<std::shared_ptr<State>> &group: previous_equivalence) {
        // in the new groups variable we are storing the next the collective next paired with the states they came from
        std::vector<std::pair<std::vector<std::shared_ptr<State>>, std::vector<std::shared_ptr<State>>>> new_groups;

        for (std::shared_ptr<State> &state_ptr: group) {
            // destinations contains the states that are the destinations
            std::vector<std::shared_ptr<State>> destinations;

            for (std::string &alphabet: dfa->getAlphabets()) {
                std::shared_ptr<State> next_state_ptr = *dfa->getNextStates(state_ptr, alphabet).begin();
                for (std::vector<std::shared_ptr<State>> &g: previous_equivalence) {
                    // add new destination
                    auto it = std::find_if(g.begin(), g.end(), [&next_state_ptr](std::shared_ptr<State> &s_ptr) {
                        return *s_ptr == *next_state_ptr;
                    });
                    if (it != g.end()) {
                        destinations.push_back(*g.begin());
                        break;
                    }
                }
            }
            // now add state to be one of the sources of the destinations calculated above.
            auto it = std::find_if(new_groups.begin(), new_groups.end(),
                                   [&destinations](
                                           std::pair<std::vector<std::shared_ptr<State>>, std::vector<std::shared_ptr<State>>> &p) {
                                       return Utilities::vector_equal(destinations, p.first);
                                   });

            if (it == new_groups.end()) {
                std::vector<std::shared_ptr<State>> sources = {state_ptr};
                new_groups.emplace_back(destinations, sources);
            } else {
                it->second.push_back(state_ptr);
            }
        }
        for (std::pair<std::vector<std::shared_ptr<State>>, std::vector<std::shared_ptr<State>>> &pair: new_groups) {
            nextEquivalence.push_back(pair.second);
        }
    }

    return nextEquivalence;
}


std::pair<std::vector<std::shared_ptr<State>>, std::pair<std::shared_ptr<State>, std::vector<std::shared_ptr<State>>>>
Conversions::getNewStatesAndSpecialStates(std::vector<std::vector<std::shared_ptr<State>>> &group,
                                          std::shared_ptr<Automaton> &a) {
    std::vector<std::shared_ptr<State>> new_states;
    std::shared_ptr<State> new_start;
    std::vector<std::shared_ptr<State>> new_accepting;

    bool start_is_set = false;
    // Create new states and map old states to new states
    for (std::vector<std::shared_ptr<State>> &g: group) {
        std::shared_ptr<State> representativeState = g.front();  // take the first state of the set as the representative of the set
        new_states.push_back(representativeState);

        auto it = std::find_if(g.begin(), g.end(),
                               [&a](std::shared_ptr<State> &ptr) { return *ptr == *a->getStart(); });
        if (!start_is_set && (it != g.end())) {
            new_start = representativeState;// the new start state is the representative of the group containing the old start state
            start_is_set = true;
        }

        if (a->hasAcceptingState(g)) {
            // if the group contains an old accepting state, the representative state is a new accepting state
            representativeState->setAccepting(true);
            representativeState->setToken(a->getToken());
            new_accepting.push_back(representativeState);
        }

    }
    return std::make_pair(new_states, std::make_pair(new_start, new_accepting));
}


std::map<std::pair<std::shared_ptr<State>, std::string>, std::shared_ptr<State>>
Conversions::getNewTransitions(std::shared_ptr<Automaton> &oldDFA,
                               std::vector<std::vector<std::shared_ptr<State>>> &group,
                               std::vector<std::shared_ptr<State>> &newStates) {
//    std::map<std::pair<std::shared_ptr<State>, std::string>, std::shared_ptr<State>> oldTransitions = oldDFA->getTransitionsDFAFormat();

    std::vector<std::pair<std::shared_ptr<State>, std::shared_ptr<State>>> state_to_representative;

    // Map old states to their respective representative.
    int i = 0;
    for (auto &g: group) {
        for (auto &old_state: g) {
            state_to_representative.emplace_back(old_state, newStates[i]);
        }
        i++;
    }

    // Create new transitions
    std::map<std::pair<std::shared_ptr<State>, std::string>, std::shared_ptr<State>> newTransitions;

    for (std::shared_ptr<State> &new_state: newStates) {
        for (std::string &alphabet: oldDFA->getAlphabets()) {
            std::pair<std::shared_ptr<State>, std::string> key = std::make_pair(new_state, alphabet);

            std::shared_ptr<State> old_state = *oldDFA->getNextStates(new_state, alphabet).begin();
            auto it = std::find_if(state_to_representative.begin(), state_to_representative.end(),
                                   [&old_state](std::pair<std::shared_ptr<State>, std::shared_ptr<State>> &entry) {
                                       return *entry.first == *old_state;
                                   });

            std::shared_ptr<State> next_state = it->second;
            newTransitions[key] = next_state;
        }
    }
    return newTransitions;
}
