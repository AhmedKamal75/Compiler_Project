#include <sstream>
#include <algorithm>
#include "Utilities.h"


std::shared_ptr<Automaton> Utilities::copyAutomaton(std::shared_ptr<Automaton> &originalAutomaton) {
    std::shared_ptr<Automaton> copy = std::make_shared<Automaton>();

    copy->set_epsilon_symbol(originalAutomaton->get_epsilon_symbol());

    for (const std::shared_ptr<State> &state_ptr: originalAutomaton->get_states()) {
        copy->add_state(std::make_shared<State>(state_ptr->copy()));
    }

    for (const auto &alphabet: originalAutomaton->get_alphabets()) {
        copy->add_alphabet(alphabet);
    }

    copy->set_start(copy->get_state_using_id(originalAutomaton->get_start()->getId()));


    for (const std::shared_ptr<State> &originalAcceptingState: originalAutomaton->get_accepting_states()) {
        copy->add_accepting_state(copy->get_state_using_id(originalAcceptingState->getId()));
    }


    for (const auto &entry: originalAutomaton->get_transitions()) {
        std::shared_ptr<State> from_state_ptr = copy->get_state_using_id(entry.first.first->getId());
        std::string symbol = entry.first.second;
        Types::key_t key = std::make_pair(from_state_ptr, symbol);
        Types::state_set_t to_states;
        for (const auto &originalToState: entry.second) {
            std::shared_ptr<State> to_state_ptr = copy->get_state_using_id(originalToState->getId());
            to_states.insert(to_state_ptr);
        }
        copy->add_transitions(from_state_ptr, symbol, to_states);
    }
    // add tokens;
    if (!originalAutomaton->get_tokens().empty()){
        Types::state_to_string_set_map_t copy_tokens{};
        for (const auto &pair: originalAutomaton->get_tokens()) {
            copy_tokens.insert(std::make_pair(copy->get_state_using_id(pair.first->getId()), pair.second));
        }
        copy->set_tokens(copy_tokens);
    }



    copy->set_regex(originalAutomaton->get_regex());

    return copy;
}

std::shared_ptr<Automaton> Utilities::unionAutomata(std::shared_ptr<Automaton> &a1, std::shared_ptr<Automaton> &a2) {
    std::shared_ptr<Automaton> tempA1 = copyAutomaton(a1);
    std::shared_ptr<Automaton> tempA2 = copyAutomaton(a2);
    tempA1->give_new_ids_all(-1, false);
    tempA2->give_new_ids_all(1, true);

    // Create a new automaton
    std::shared_ptr<Automaton> unionAutomaton = std::make_shared<Automaton>();
    // Add the epsilon symbol
    unionAutomaton->set_epsilon_symbol(tempA1->get_epsilon_symbol());

    // Add the states and transitions from the first automaton
    unionAutomaton->add_states(tempA1->get_states());
    unionAutomaton->add_alphabets(tempA1->get_alphabets());
    unionAutomaton->add_transitions(tempA1->get_transitions());
    // Add the states and transitions from the second automaton
    unionAutomaton->add_states(tempA2->get_states());
    unionAutomaton->add_alphabets(tempA2->get_alphabets());
    unionAutomaton->add_transitions(tempA2->get_transitions());

    // Create a new start state with ε-transitions to the start states of the
    // original automata
    std::shared_ptr<State> newStartState = std::make_shared<State>(
            static_cast<int>(unionAutomaton->get_states().size()) + 1, false, "");
    unionAutomaton->add_transitions(newStartState, unionAutomaton->get_epsilon_symbol(),
                                    {tempA1->get_start(), tempA2->get_start()});
    unionAutomaton->set_start(newStartState);
    unionAutomaton->add_state(newStartState);

    // Add the accepting states from both automata
    unionAutomaton->add_accepting_states(tempA1->get_accepting_states());
    unionAutomaton->add_accepting_states(tempA2->get_accepting_states());


    // Update the token names of the accepting states
    unionAutomaton->set_regex(("(" + tempA1->get_regex() + "|" + tempA2->get_regex() + ")"));
    unionAutomaton->give_new_ids_all();

    return unionAutomaton;
}

std::shared_ptr<Automaton> Utilities::concatAutomaton(std::shared_ptr<Automaton> &a1, std::shared_ptr<Automaton> &a2) {
    std::shared_ptr<Automaton> tempA1 = copyAutomaton(a1);
    std::shared_ptr<Automaton> tempA2 = copyAutomaton(a2);
    tempA1->give_new_ids_all(-1, false);
    tempA2->give_new_ids_all(1, true);

    // Create a new automaton
    std::shared_ptr<Automaton> concatAutomaton = std::make_shared<Automaton>();

    // Add the epsilon symbol
    concatAutomaton->set_epsilon_symbol(tempA1->get_epsilon_symbol());

    // Add the states and transitions from the first automaton
    concatAutomaton->add_states(tempA1->get_states());
    concatAutomaton->add_alphabets(tempA1->get_alphabets());
    concatAutomaton->add_transitions(tempA1->get_transitions());
    // Add the states and transitions from the second automaton
    concatAutomaton->add_states(tempA2->get_states());
    concatAutomaton->add_alphabets(tempA2->get_alphabets());
    concatAutomaton->add_transitions(tempA2->get_transitions());

    // Set the start state to the start state of the first automaton
    concatAutomaton->set_start(tempA1->get_start());

    // Add ε-transitions from the accepting states of the first automaton to the
    // start state of the second automaton
    for (const std::shared_ptr<State> &accepting_state_ptr_1: tempA1->get_accepting_states()) {
        accepting_state_ptr_1->setAccepting(false);
        concatAutomaton->add_transitions(accepting_state_ptr_1, concatAutomaton->get_epsilon_symbol(),
                                         {tempA2->get_start()});
    }

    // Set the accepting states to the accepting states of the second automaton
    concatAutomaton->add_accepting_states(tempA2->get_accepting_states());


    // Update the token names of the accepting states
    concatAutomaton->set_regex(("(" + tempA1->get_regex() + tempA2->get_regex() + ")"));
    concatAutomaton->give_new_ids_all();

    return concatAutomaton;
}

std::shared_ptr<Automaton> Utilities::kleeneClosure(std::shared_ptr<Automaton> &a) {
    std::shared_ptr<Automaton> tempA = copyAutomaton(a);
    tempA->give_new_ids_all(0, false);

    // Create a new automaton
    std::shared_ptr<Automaton> kleeneAutomaton = std::make_shared<Automaton>();

    // Add the epsilon symbol
    kleeneAutomaton->set_epsilon_symbol(tempA->get_epsilon_symbol());

    // Add the states and transitions from the original automaton
    kleeneAutomaton->add_states(tempA->get_states());
    kleeneAutomaton->add_alphabets(tempA->get_alphabets());
    kleeneAutomaton->add_transitions(tempA->get_transitions());

    // Create a new start state and a new accepting state
    int i = static_cast<int>(kleeneAutomaton->get_states().size());
    auto new_start_state = std::make_shared<State>(i + 1, false, "");
    auto new_accepting_state = std::make_shared<State>(i + 2, true, "");
    kleeneAutomaton->add_states({new_start_state, new_accepting_state});

    // Set the start state and the accepting states
    kleeneAutomaton->set_start(new_start_state);
    kleeneAutomaton->add_accepting_state(new_accepting_state);

    // Add ε-transitions from the new start state to the new accepting state
    kleeneAutomaton->add_transitions(new_start_state, kleeneAutomaton->get_epsilon_symbol(), {new_accepting_state});

    // Add ε-transitions from the new accepting state to the new start state
    kleeneAutomaton->add_transitions(new_accepting_state, kleeneAutomaton->get_epsilon_symbol(), {new_start_state});

    // Add ε-transitions from the new start state to the start state of the original automaton
    kleeneAutomaton->add_transitions(new_start_state, kleeneAutomaton->get_epsilon_symbol(), {tempA->get_start()});

    // Add ε-transitions from the accepting states of the original automaton to the new accepting state
    for (const std::shared_ptr<State> &accepting_state_ptr: tempA->get_accepting_states()) {
        accepting_state_ptr->setAccepting(false);
        kleeneAutomaton->add_transitions(accepting_state_ptr, kleeneAutomaton->get_epsilon_symbol(),
                                         {new_accepting_state});
    }

    // Update the token names of the accepting states
    kleeneAutomaton->set_regex(("(" + tempA->get_regex() + ")*"));
    kleeneAutomaton->give_new_ids_all();

    return kleeneAutomaton;
}

std::shared_ptr<Automaton> Utilities::positiveClosure(std::shared_ptr<Automaton> &a) {
    std::shared_ptr<Automaton> tempA = copyAutomaton(a);
    tempA->give_new_ids_all(0, false);

    // Create a new automaton
    std::shared_ptr<Automaton> positiveAutomaton = std::make_shared<Automaton>();

    // Add the epsilon symbol
    positiveAutomaton->set_epsilon_symbol(tempA->get_epsilon_symbol());

    // Add the states and transitions from the original automaton
    positiveAutomaton->add_states(tempA->get_states());
    positiveAutomaton->add_alphabets(tempA->get_alphabets());
    positiveAutomaton->add_transitions(tempA->get_transitions());

    // Create a new start state and a new accepting state
    int i = static_cast<int>(positiveAutomaton->get_states().size());
    auto new_start_state = std::make_shared<State>(i + 1, false, "");
    auto new_accepting_state = std::make_shared<State>(i + 2, true, "");
    positiveAutomaton->add_states({new_start_state, new_accepting_state});

    // Set the start state and the accepting states
    positiveAutomaton->set_start(new_start_state);
    positiveAutomaton->add_accepting_state(new_accepting_state);

    // Add ε-transitions from the new start state to the start state of the original automaton
    positiveAutomaton->add_transitions(new_start_state, positiveAutomaton->get_epsilon_symbol(), {tempA->get_start()});

    // Add ε-transitions from the accepting states of the original automaton to the new accepting state
    for (const std::shared_ptr<State> &accepting_state_ptr: tempA->get_accepting_states()) {
        accepting_state_ptr->setAccepting(false);
        positiveAutomaton->add_transitions(accepting_state_ptr, positiveAutomaton->get_epsilon_symbol(),
                                           {new_accepting_state});
    }

    // Add ε-transitions from the new accepting state to the new start state
    positiveAutomaton->add_transitions(new_accepting_state, positiveAutomaton->get_epsilon_symbol(), {new_start_state});

    // Update the token names of the accepting states
    positiveAutomaton->set_regex(("(" + tempA->get_regex() + ")+"));
    positiveAutomaton->give_new_ids_all();

    return positiveAutomaton;
}

std::shared_ptr<Automaton> Utilities::unionAutomataSet(std::vector<std::shared_ptr<Automaton>> &automata) {
    // Create a new automaton
    std::shared_ptr<Automaton> unionAutomaton = std::make_shared<Automaton>();

    // If the vector is empty, return the new (empty) automaton
    if (automata.empty()) {
        return unionAutomaton;
    }

    // Set the epsilon symbol to that of the first automaton in the vector
    unionAutomaton->set_epsilon_symbol(automata[0]->get_epsilon_symbol());

    // Create a new start state
    auto newStartState = std::make_shared<State>(-1, false, "");

    // Set the start state
    unionAutomaton->set_start(newStartState);

    // Add the new start state to the set of states
    unionAutomaton->add_state(newStartState);

    std::string regex{};

    // Iterate over the automata in the vector
    for (std::shared_ptr<Automaton> &tempA: automata) {
        std::shared_ptr<Automaton> a = copyAutomaton(tempA);
        // Give new IDs to the states in the current automaton
        a->give_new_ids_all(static_cast<int>(unionAutomaton->get_states().size()) + 1, true);

        // Add the states and transitions from the current automaton
        unionAutomaton->add_states(a->get_states());
        unionAutomaton->add_alphabets(a->get_alphabets());
        unionAutomaton->add_transitions(a->get_transitions());

        // Add ε-transitions from the new start state to the start state of the current automaton
        unionAutomaton->add_transitions(newStartState, unionAutomaton->get_epsilon_symbol(), {a->get_start()});

        // Add the accepting states from the current automaton
        unionAutomaton->add_accepting_states(a->get_accepting_states());

        // Append the token of the current automaton to the new token
        if (!regex.empty()) {
            regex += "|";
        }
        regex += "(" + a->get_regex() + ")";
    }

    unionAutomaton->set_regex("(" + regex + ")");

    // Give new IDs to all states in the union automaton
    unionAutomaton->give_new_ids_all();

    return unionAutomaton;
}

bool Utilities::set_equal(const Types::state_set_t &x, const Types::state_set_t &y) {
    // if the two empty, then they are equal
    if (x.empty() && y.empty()) {
        return true;
    }
    // all elements in y should be in x
    for (const auto &y_state_ptr: y) {
        if (x.find(y_state_ptr) == x.end()) {
            return false;
        }
    }
    // the two sizes should be the same.
    return y.size() == x.size();
}

bool Utilities::vector_equal(std::vector<std::shared_ptr<State>> &x, std::vector<std::shared_ptr<State>> &y) {
    // if the two empty, then they are equal
    if (x.empty() && y.empty()) {
        return true;
    }
    // all elements in y should be in x
    for (std::shared_ptr<State> y_state_ptr: y) {
        auto it = std::find_if(x.begin(), x.end(), [&y_state_ptr](std::shared_ptr<State> &x_state_ptr) {
            return *y_state_ptr == *x_state_ptr;
        });
        if (it == x.end()) {
            return false;
        }
    }
    // the two sizes should be the same.
    return y.size() == x.size();
}

[[maybe_unused]] void Utilities::add_all(Types::state_set_t &to, Types::state_set_t &from) {
    for (const std::shared_ptr<State> &from_state_ptr: from) {
        to.insert(from_state_ptr);
    }
}

bool Utilities::group_equal(std::vector<Types::state_set_t> &g1, std::vector<Types::state_set_t> &g2) {
    if (g1.size() != g2.size()) {
        return false;
    }
    for (int i = 0; i < g1.size(); i++) {
        if (!set_equal(g1[i], g2[i])) {
            return false;
        }
    }
    return true;
}

void Utilities::group_string(std::vector<Types::state_set_t> &group) {
    std::stringstream ss;
    for (const Types::state_set_t &g: group) {
        ss << "{ ";
        auto it = g.begin();
        if (it != g.end()) {
            ss << "[" << (*it)->getId() << "]";
            ++it;
        }
        for (; it != g.end(); ++it) {
            ss << ", " "[" << (*it)->getId() << "]";
        }
        ss << " }  ";
    }
    ss << std::endl;
    std::cout << ss.str();
}

std::shared_ptr<Automaton> Utilities::get_epsilon_automaton(const std::string &epsilonSymbol) {
    std::shared_ptr<Automaton> a = std::make_shared<Automaton>();
    // Create the states
    std::shared_ptr<State> q0 = std::make_shared<State>(0, false, "");
    std::shared_ptr<State> q1 = std::make_shared<State>(1, true, epsilonSymbol);

    // Initialize the fields
    a->add_states({q0, q1});
    a->set_start(q0);
    a->add_accepting_state(q1);
    a->set_epsilon_symbol(epsilonSymbol);
    a->add_transitions(q0, epsilonSymbol, {q1});
    a->set_regex("(" + epsilonSymbol + ")");

    return a;
}