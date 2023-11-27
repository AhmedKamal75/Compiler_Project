#ifndef COMPILER_PROJECT_CONVERSIONS_H
#define COMPILER_PROJECT_CONVERSIONS_H


#include <vector>
#include "Automaton.h"

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
    void prepareForAutomaton(std::shared_ptr<Automaton> &a);

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
    Types::state_set_t epsilonClosure(std::shared_ptr<Automaton> &a, const std::shared_ptr<State> &state_ptr);

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
    [[maybe_unused]] std::shared_ptr<Automaton> removeEpsilonTransitions(std::shared_ptr<Automaton> &automaton);

    /**
     * @brief Converts a given NFA (Non-deterministic Finite automaton) to a DFA (Deterministic Finite automaton).
     *
     * @param automaton A shared pointer to the automaton object that represents the NFA.
     * @param is_final tells the method if the automaton is a final one (at which a final state can have more than one token)
     * so that the method can calculate those tokens an store them in a->tokens
     *
     * @return A shared pointer to the newly created automaton object that represents the DFA.
     *
     * The function first creates a copy of the NFA and a new automaton object for the DFA. It then copies the alphabets and epsilon symbol from the NFA to the DFA.
     * It prepares the NFA for conversion by calling the `prepareForAutomaton` method. This method ensures that the NFA is in the correct format for conversion.
     * It then computes the epsilon closure of the start state of the NFA and adds it to a queue. This queue is used to keep track of the states that need to be processed.
     * The function then enters a loop where it processes each state in the queue. For each state, it retrieves the corresponding DFA state using the `get_dfa_state` method. If no such state exists, it creates a new DFA state using the `create_dfa_state` method and adds it to the DFA.
     * It then checks if the current state is the start state of the NFA. If it is, it sets the corresponding DFA state as the start state of the DFA.
     * For each symbol in the alphabet, it computes the set of states that can be reached from the current state using that symbol. It then computes the epsilon closure of these states. This gives the set of states that can be reached from the current state using the symbol, taking into account epsilon transitions.
     * It then retrieves the DFA state that corresponds to this set of states. If no such state exists, it creates a new DFA state. It then adds a transition from the current DFA state to the new DFA state using the symbol.
     * This process continues until all states have been processed. The resulting DFA is then returned.
     *
     * This function is part of the process of converting an NFA to a DFA.
     * It helps in creating equivalent states in the DFA for each unique combination of states in the NFA.
     * The accepting states in the DFA are determined based on the accepting states in the corresponding NFA states.
     * The token assigned to the accepting states in the DFA is the same as the token of the NFA.
     * This ensures that the DFA accepts the same language as the NFA.
     * The DFA is adjusted according to its new state, which means the transitions of the DFA are updated to include transitions from the new state to other states based on the transitions of the states in `state_vector` in the NFA.
     * This is typically done in a separate function that is called after `create_dfa_state`.
     */
    [[maybe_unused]]  std::shared_ptr<Automaton>
    convertToDFA(std::shared_ptr<Automaton> &automaton, const bool &is_final);

    /**
     * This method minimizes a given DFA (Deterministic Finite LexicalAnalysisGenerator.automaton) using Hopcroft's algorithm.
     * The algorithm works by partitioning the states of the DFA into groups of indistinguishable states,
     * and then collapsing each group of states into a single state. The resulting minimized DFA has the
     * property that it has the smallest possible number of states and is equivalent to the original DFA.
     *
     * @param automaton The DFA to be minimized.
     * @return The minimized DFA.
     */
    [[maybe_unused]] std::shared_ptr<Automaton> minimizeDFA(std::shared_ptr<Automaton> &automaton);


private:

    Types::epsilon_closure_map_t epsilon_closures{};

    int counter{};

    /**
     * @brief Creates dfa new "dead" state in the automaton.
     *
     * @param dfa A shared pointer to the automaton object that represents the DFA.
     *
     * @return A shared pointer to the newly created "dead" State object in the DFA.
     *
     * The function first creates dfa new State object with dfa unique ID and adds it to the DFA. This state is dfa "dead" state, meaning that any transition to this state will not lead to an accepting state.
     * It then iterates over each symbol in the alphabet of the automaton. For each symbol, it adds dfa transition from the "dead" state to itself. This means that once the automaton transitions to the "dead" state, it will stay in the "dead" state regardless of the input symbol.
     * Finally, it returns dfa shared pointer to the newly created "dead" State object.
     *
     * This function is part of the process of converting an NFA to dfa DFA.
     * It helps in handling the case where there is no valid transition for dfa given state and input symbol in the NFA.
     * In the DFA, such cases are handled by transitioning to dfa "dead" state.
     * The "dead" state is dfa non-accepting state, and once the automaton transitions to the "dead" state, it cannot transition to any other state.
     * This ensures that the DFA is complete, meaning that it has dfa valid transition for every state and input symbol.
     */
    std::shared_ptr<State> create_dead_state(std::shared_ptr<Automaton> &dfa);

    /**
     * @brief Retrieves the DFA state corresponding to a vector of NFA states.
     *
     * @param state_vector A vector of shared pointers to State objects. These states are from the NFA.
     * @param dfa_states A vector of pairs, where each pair consists of a vector of NFA states and the corresponding DFA state.
     *
     * @return A shared pointer to the State object in the DFA that corresponds to the vector of NFA states. If no such state exists, it returns a null pointer.
     *
     * The function first searches for `state_vector` in `dfa_states` using the `std::find_if` algorithm. It uses a lambda function to compare `state_vector` with the first element of each pair in `dfa_states`. The lambda function uses the `Utilities::vector_equal` function to check if two vectors are equal.
     * If `state_vector` is found in `dfa_states`, it returns a shared pointer to the corresponding DFA state (the second element of the pair). If `state_vector` is not found, it returns a null pointer.
     *
     * This function is part of the process of converting an NFA to a DFA.
     * It helps in mapping the states of the NFA to the states of the DFA.
     * Each state in the DFA corresponds to a unique combination of states in the NFA.
     * This function retrieves the DFA state that corresponds to a given combination of NFA states.
     * This is useful when creating the transitions of the DFA based on the transitions of the NFA.
     * For each transition in the NFA, the function can be used to find the corresponding transition in the DFA.
     */
    static std::shared_ptr<State> get_dfa_state(Types::state_set_t &state_set,
                                                std::vector<std::pair<Types::state_set_t, std::shared_ptr<State>>> &dfa_states);


    /**
     * @brief Creates a new state in the DFA from a vector of states in the NFA and adjusts the DFA according to its new state.
     *
     * @param state_vector A vector of shared pointers to State objects. These states are from the NFA and will be replaced by the new DFA state.
     * @param a A shared pointer to the automaton object that represents the NFA.
     * @param dfa A shared pointer to the automaton object that represents the DFA. This DFA will be adjusted according to its new state.
     *
     * @return A shared pointer to the newly created State object in the DFA.
     *
     * The function first creates a new State object with a unique ID and adds it to the DFA.
     * It then checks if any of the states in `state_vector` are accepting states in the NFA using the `hasAcceptingState` method of the automaton class.
     * If there is an accepting state, it sets the new state as an accepting state in the DFA and assigns it the token of the NFA.
     * It also adds the new state to the list of final states in the DFA.
     * Finally, it returns a shared pointer to the newly created State object.
     *
     * This function is part of the process of converting an NFA to a DFA.
     * It helps in creating equivalent states in the DFA for each unique combination of states in the NFA.
     * The accepting states in the DFA are determined based on the accepting states in the corresponding NFA states.
     * The token assigned to the accepting states in the DFA is the same as the token of the NFA.
     * This ensures that the DFA accepts the same language as the NFA.
     * The DFA is adjusted according to its new state, which means the transitions of the DFA are updated to include transitions from the new state to other states based on the transitions of the states in `state_vector` in the NFA.
     * This is typically done in a separate function that is called after `create_dfa_state`.
     */
    std::shared_ptr<State> create_dfa_state(Types::state_set_t &state_set, std::shared_ptr<Automaton> &a,
                                            std::shared_ptr<Automaton> &dfa);


    static std::vector<Types::state_set_t>
    get_next_equivalence(std::vector<Types::state_set_t> &entry, std::shared_ptr<Automaton> &dfa);


    static std::pair<Types::state_set_t, std::pair<std::shared_ptr<State>, Types::state_set_t>>
    get_special_data(std::vector<Types::state_set_t> &group, std::shared_ptr<Automaton> &dfa,
                     const std::shared_ptr<Automaton> &minimized_dfa);

    static void create_transitions(std::shared_ptr<Automaton> &oldDFA,
                                   std::shared_ptr<Automaton> &newDFA,
                                   std::vector<Types::state_set_t> &group);


};

#endif