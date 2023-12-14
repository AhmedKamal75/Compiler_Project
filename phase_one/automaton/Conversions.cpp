#include <stack>
#include <algorithm>
#include <queue>
#include "Conversions.h"
#include "Utilities.h"

Conversions::Conversions() : counter(0) {}

void Conversions::prepareForAutomaton(std::shared_ptr<Automaton> &a) {
    epsilon_closures.clear();
    counter = static_cast<int>(a->get_states().size()) + 1;
}

Types::state_set_t Conversions::epsilonClosure(std::shared_ptr<Automaton> &a, const std::shared_ptr<State> &state_ptr) {
    auto iterator = epsilon_closures.find(state_ptr);
    if (iterator != epsilon_closures.end()) {
        return iterator->second;
    }
    Types::state_set_t epsilon_closure_set;
    std::stack<std::shared_ptr<State>> stack;
    stack.push(state_ptr);

    while (!stack.empty()) {
        std::shared_ptr<State> current_state = stack.top();
        stack.pop();
        epsilon_closure_set.insert(current_state);

        for (const std::shared_ptr<State> &next_state: a->get_next_states(current_state, a->get_epsilon_symbol())) {
            if (epsilon_closure_set.find(next_state) == epsilon_closure_set.end()) {
                stack.push(next_state);
            }
        }
    }
    epsilon_closures[state_ptr] = epsilon_closure_set;
    return epsilon_closure_set;
}

[[maybe_unused]] std::shared_ptr<Automaton>
Conversions::removeEpsilonTransitions(std::shared_ptr<Automaton> &automaton) {
    // init a copy of the parameter
    std::shared_ptr<Automaton> a = Utilities::copyAutomaton(automaton);

    // Create a new automaton
    std::shared_ptr<Automaton> nfa = std::make_shared<Automaton>();

    // Copy ε-symbol, the states, alphabets, start state, and accepting states
    nfa->set_epsilon_symbol(a->get_epsilon_symbol());
    nfa->add_states(a->get_states());
    nfa->add_alphabets(a->get_alphabets());
    nfa->set_start(a->get_start());
    nfa->add_accepting_states(a->get_accepting_states());

    // now we work on the transitions, and add new accepting states

    // prepare this Conversions object for new automaton
    this->prepareForAutomaton(a);
    // For each state and each alphabet, compute the set of reachable states, and if they contain any accepting state,
    // we make the state accepting
    for (const std::shared_ptr<State> &state_ptr: a->get_states()) {
        for (const std::string &alphabet: a->get_alphabets()) {
            if (alphabet != a->get_epsilon_symbol()) {
                // x = epsilon_closure(state)
                Types::state_set_t x = epsilonClosure(a, state_ptr);

                // Check if any state in x is an accepting state
                for (const auto &x_state_ptr: x) {
                    if (a->is_accepting_state(x_state_ptr)) {
                        state_ptr->setAccepting(true);
                        state_ptr->setToken(a->get_token());
                        nfa->add_accepting_state(state_ptr);
                        break;
                    }
                }

                // y = δ(x,alphabet)
                Types::state_set_t y{};
                for (const std::shared_ptr<State> &from_state_ptr: x) {
                    Types::state_set_t temp_next_states = a->get_next_states(from_state_ptr, alphabet);
                    Utilities::add_all(y, temp_next_states);
                }

                // z = epsilon_closure(y)
                Types::state_set_t z{};
                for (const std::shared_ptr<State> &statePtr: y) {
                    Types::state_set_t sub_epsilon_closure = epsilonClosure(a, statePtr);
                    Utilities::add_all(z, sub_epsilon_closure);
                }
                nfa->add_transitions(state_ptr, alphabet, z);
            }
        }
    }

    nfa->give_new_ids_all();

    return nfa;
}

std::shared_ptr<State> Conversions::create_dfa_state(Types::state_set_t &state_set,
                                                     std::shared_ptr<Automaton> &a,
                                                     std::shared_ptr<Automaton> &dfa) {
    std::shared_ptr<State> new_state_ptr = std::make_shared<State>(++this->counter, false, "");
    dfa->add_state(new_state_ptr);
    if (a->has_accepting_state(state_set)) {
        new_state_ptr->setAccepting(true);
        new_state_ptr->setToken(a->get_token());
        dfa->add_accepting_state(new_state_ptr);
    }
    return new_state_ptr;
}

std::shared_ptr<State> Conversions::create_dead_state(std::shared_ptr<Automaton> &dfa) {
    std::shared_ptr<State> deadState = std::make_shared<State>(++this->counter, false, "");
    dfa->add_state(deadState);
    for (const std::string &alphabet: dfa->get_alphabets()) {
        dfa->add_transitions(deadState, alphabet, {deadState});
    }
    return deadState;
}


std::shared_ptr<State> Conversions::get_dfa_state(Types::state_set_t &state_set,
                                                  std::vector<std::pair<Types::state_set_t, std::shared_ptr<State>>> &dfa_states) {

    auto it = std::find_if(dfa_states.begin(), dfa_states.end(),
                           [&state_set](const std::pair<Types::state_set_t, std::shared_ptr<State>> &entry) {
                               return Utilities::set_equal(state_set, entry.first);
                           });
    if (it == dfa_states.end()) {
        return nullptr;
    }
    return it->second;
}

[[maybe_unused]] std::shared_ptr<Automaton> Conversions::convertToDFA(std::shared_ptr<Automaton> &automaton, const bool &is_final) {
    // Create a copy of this
    std::shared_ptr<Automaton> a = Utilities::copyAutomaton(automaton);

    // Create a new automaton
    std::shared_ptr<Automaton> dfa = std::make_shared<Automaton>();

    // Copy the alphabets and epsilon symbol
    dfa->add_alphabets(a->get_alphabets());
    dfa->set_epsilon_symbol(a->get_epsilon_symbol());

    std::vector<std::pair<Types::state_set_t, std::shared_ptr<State>>> dfa_states;
    std::queue<Types::state_set_t> queue;

    prepareForAutomaton(a);

    // Compute the epsilon closure of the start state
    Types::state_set_t start_set = epsilonClosure(a, a->get_start());
    queue.push(start_set);
    bool startIsSet = false;
    // loop on the available sets of compatible groups of states
    while (!queue.empty()) {
        Types::state_set_t current_set = queue.front();
        queue.pop();
        std::shared_ptr<State> dfa_state = get_dfa_state(current_set, dfa_states);
        if (dfa_state == nullptr) {
            dfa_state = create_dfa_state(current_set, a, dfa);
            dfa_states.emplace_back(current_set, dfa_state);
        }

        // make dfa_state a part of the dfa states not the automaton was done in the create_dfa_state method
        // now to check if to make it a start state or not!

        if (!startIsSet && (current_set.find(a->get_start()) != current_set.end())) {
            dfa->set_start(dfa_state);
            startIsSet = true;
            // that mean that the dfa will have its start state set one time only, and that
            // is for the correct state
        }
        // now we have a number(size=alphabets) of transitions that needs to be added to the dfa

        // loop on the alphabets to get the next (from the perspective of the current_set) set
        // of compatible groups of states, and then add transitions to them
        for (const std::basic_string<char> &alphabet: a->get_alphabets()) {
            if (alphabet != a->get_epsilon_symbol()) {
                // get the set reachable from current_set(dfa_state) using current alphabet.
                Types::state_set_t immediate_reachable_set{};
                for (const std::shared_ptr<State> &state_ptr: current_set) { // currentSet is already an epsilon closure
                    Types::state_set_t temp_next_states = a->get_next_states(state_ptr, alphabet);
                    Utilities::add_all(immediate_reachable_set, temp_next_states);
                }
                // Compute the epsilon closures of the next immediate_reachable_set
                Types::state_set_t fully_reachable_set{};
                for (const std::shared_ptr<State> &immediate_reachable_state_ptr: immediate_reachable_set) {
                    Types::state_set_t sub_epsilon_closure = epsilonClosure(a, immediate_reachable_state_ptr);
                    Utilities::add_all(fully_reachable_set, sub_epsilon_closure);
                }
                // now we get the state corresponding state to this fully_reachable_set.
                bool new_next_state_created = false;
                std::shared_ptr<State> next_state;
                if (fully_reachable_set.empty()) { // meaning that next state is a dead state
                    next_state = get_dfa_state(fully_reachable_set, dfa_states);
                    if (next_state == nullptr) { // no dead state was found
                        new_next_state_created = true;
                        next_state = create_dead_state(dfa);
                    }
                } else {
                    next_state = get_dfa_state(fully_reachable_set, dfa_states);
                    if (next_state == nullptr) {
                        new_next_state_created = true;
                        next_state = create_dfa_state(fully_reachable_set, a, dfa);
                    }
                }
                // next_state calculated and dfa adjusted to accommodate it, then add the transition
                // we have a current_state(dfa_state) --alphabet--> fully_reachable_set(next_state)
                dfa->add_transitions(dfa_state, alphabet, {next_state});
                // keep the following code in its order
                if (new_next_state_created) {
                    queue.push(fully_reachable_set);
                }
                auto iterator = std::find_if(dfa_states.begin(), dfa_states.end(),
                                             [&fully_reachable_set](
                                                     const std::pair<Types::state_set_t, std::shared_ptr<State>> &entry) {
                                                 return Utilities::set_equal(fully_reachable_set, entry.first);
                                             });
                if (iterator == dfa_states.end()) {
                    dfa_states.emplace_back(fully_reachable_set, next_state);
                }
            }
        }
    }

    // fix tokens in new stats
    if (is_final){
        for (const std::pair<Types::state_set_t, std::shared_ptr<State>> &pair: dfa_states) {
            // Get the set of NFA states and the corresponding DFA state
            const Types::state_set_t &nfa_states = pair.first;
            const std::shared_ptr<State> &dfa_state = pair.second;

            // Check if the DFA state is an accepting state
            if (dfa->is_accepting_state(dfa_state)) {
                // Iterate over the NFA states
                for (const std::shared_ptr<State> &nfa_state: nfa_states) {
                    // If the NFA state is an accepting state, add its tokens to the DFA state
                    if (a->is_accepting_state(nfa_state)) {
                        dfa->add_tokens(dfa_state, {nfa_state->getToken()});
                    }
                }
            }
        }
    }


    dfa->set_regex(a->get_regex());
    dfa->give_new_ids_all();

    return dfa;
}

[[maybe_unused]] std::shared_ptr<Automaton> Conversions::minimizeDFA(std::shared_ptr<Automaton> &automaton) {
    // Step 0: Create a copy of the original automaton because it might encounter change.
    std::shared_ptr<Automaton> dfa = Utilities::copyAutomaton(automaton);
//    if (!automaton->get_tokens().empty()){
//
//    }
    // Step 1: Create a list of groups of states. Initially, there are two sets: accepting states and non-accepting states.
    std::vector<Types::state_set_t> current_group{};
    current_group.push_back(dfa->get_accepting_states());
    Types::state_set_t non_accepting_state{};
    for (const std::shared_ptr<State> &state_ptr: dfa->get_states()) {
        if (!dfa->is_accepting_state(state_ptr)) {
            non_accepting_state.insert(state_ptr);
        }
    }
    current_group.push_back(non_accepting_state);

    // Step 2: Refine the groups until no further refinement is possible.
    while (true) {
        std::vector<Types::state_set_t> next_group = get_next_equivalence(current_group, dfa);
        if (Utilities::group_equal(current_group, next_group)) {
            break;
        }
        current_group = next_group;
    }

    // Step 3: Construct the minimized DFA.
    std::shared_ptr<Automaton> minDFA = std::make_shared<Automaton>();
    // fields that don't need any computations.
    minDFA->set_epsilon_symbol(dfa->get_epsilon_symbol());
    minDFA->add_alphabets(dfa->get_alphabets());
    // calculating the states.
    std::pair<Types::state_set_t, std::pair<std::shared_ptr<State>, Types::state_set_t>> special_data = get_special_data(
            current_group, dfa, minDFA);
    minDFA->add_states(special_data.first);
    minDFA->set_start(special_data.second.first);
    minDFA->add_accepting_states(special_data.second.second);

    create_transitions(dfa, minDFA, current_group);

//    minDFA->set_tokens(dfa->get_tokens());
    minDFA->set_regex(dfa->get_regex());
    minDFA->give_new_ids_all();

    return minDFA;
}

std::vector<Types::state_set_t> Conversions::get_next_equivalence(std::vector<Types::state_set_t> &previous_group,
                                                                  std::shared_ptr<Automaton> &dfa) {
    std::vector<Types::state_set_t> next_group{};

    for (Types::state_set_t &previous_set: previous_group) {
        // in the new groups variable we are storing the next the collective next paired with the states they came from
        // mapping representatives sets to the group of sets they are being mapped into
        std::vector<std::pair<Types::state_set_t, Types::state_set_t>> mapping_previous_set_to_next_set;

        for (const std::shared_ptr<State> &previous_state_ptr: previous_set) {
            // destinations contains the states that are the first state (representative state) in the destination set of the current set
            // that all in the previous group till now.
            Types::state_set_t destinations{};

            for (const std::basic_string<char> &alphabet: dfa->get_alphabets()) {
                // get the next state of the current state we are on
                std::shared_ptr<State> next_state_ptr = *dfa->get_next_states(previous_state_ptr, alphabet).begin();
                for (Types::state_set_t &temp_previous_set: previous_group) {
                    // if a set from the previous group contains the next_state_ptr of the set from the same previous group
                    if (temp_previous_set.find(next_state_ptr) != temp_previous_set.end()) {
                        // add to destinations
                        destinations.insert(*temp_previous_set.begin());
                        break;
                    }
                }
            }
            // now destinations contains the representatives (first state of a set) of the previous sets for every alphabet
            // next we have to see if another state could have mapped to the same set of representatives
            // now add state to be one of the sources of the destinations calculated above.
            auto it = std::find_if(mapping_previous_set_to_next_set.begin(), mapping_previous_set_to_next_set.end(),
                                   [&destinations](const std::pair<Types::state_set_t, Types::state_set_t> &entry) {
                                       return Utilities::set_equal(destinations, entry.first);
                                   });

            if (it == mapping_previous_set_to_next_set.end()) {
                // no other states mapped to the same representatives calculated above
                Types::state_set_t sources = {previous_state_ptr};
                mapping_previous_set_to_next_set.emplace_back(destinations, sources);
            } else {
                // found another group of states that point by the same set of representatives
                it->second.insert(previous_state_ptr);
            }
        }
        // now we collect the states sharing the same representatives
        for (std::pair<Types::state_set_t, Types::state_set_t> &entry: mapping_previous_set_to_next_set) {
            next_group.push_back(entry.second);
        }
    }
    return next_group;
}

std::pair<Types::state_set_t, std::pair<std::shared_ptr<State>, Types::state_set_t>>
Conversions::get_special_data(std::vector<Types::state_set_t> &group,
                              std::shared_ptr<Automaton> &dfa, const std::shared_ptr<Automaton> &minimized_dfa) {
    Types::state_set_t new_states{};
    std::shared_ptr<State> new_start;
    new_start.reset();
    Types::state_set_t new_accepting{};

    bool start_is_set = false;
    // Create new states and map old states to new states
    for (Types::state_set_t &g: group) {
        std::shared_ptr<State> representativeState = *g.begin();  // take the first state of the set as the representative of the set
        new_states.insert(representativeState);

        auto it = g.find(dfa->get_start());
        if (!start_is_set && (it != g.end())) {
            new_start = representativeState;// the new start state is the representative of the group containing the old start state
            start_is_set = true;
        }

        if (dfa->has_accepting_state(g)) {
            // if the group contains an old accepting state, the representative state is dfa new accepting state
            representativeState->setAccepting(true);
            representativeState->setToken(dfa->get_token());
            new_accepting.insert(representativeState);

            // handle tokens
            if (!dfa->get_tokens().empty()) {
                for (const std::shared_ptr<State> &state_ptr: g) {
                    minimized_dfa->add_tokens(representativeState, dfa->get_tokens(state_ptr));
                }
            }
        }

    }
    return std::make_pair(new_states, std::make_pair(new_start, new_accepting));
}

void Conversions::create_transitions(std::shared_ptr<Automaton> &oldDFA,
                                     std::shared_ptr<Automaton> &newDFA,
                                     std::vector<Types::state_set_t> &group) {

    Types::state_to_state_map_t mapping_states_to_representatives{};

    for (const Types::state_set_t &current_set: group) {
        std::shared_ptr<State> representative_state_ptr = *current_set.begin();
        for (const std::shared_ptr<State> &state_ptr: current_set) {
            mapping_states_to_representatives[state_ptr] = representative_state_ptr;
        }
    }

    for (const Types::state_set_t &new_set: group) {
        if (!new_set.empty()) {
            std::shared_ptr<State> representative_state_ptr = *new_set.begin();
            for (const std::string &alphabet: newDFA->get_alphabets()) {
                Types::state_set_t next_set = oldDFA->get_next_states(representative_state_ptr, alphabet);
                newDFA->add_transitions(representative_state_ptr, alphabet,
                                        {mapping_states_to_representatives[*next_set.begin()]});
            }
        }
    }
}
