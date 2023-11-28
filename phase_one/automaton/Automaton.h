#ifndef COMPILER_PROJECT_AUTOMATON_H
#define COMPILER_PROJECT_AUTOMATON_H


#include "Types.h"
#include <vector>

/**
 * This class represents a finite automaton.
 * An automaton is a finite representation of a formal language that can be an acceptor or a transducer.
 *
 * remember to use std::make_shared<State>() when creating new State objects to ensure they are managed by smart pointers.
 */
class Automaton {
private:
    // The states of the automaton.
    Types::state_set_t states;

    // The accepting states of the automaton.
    Types::state_set_t accepting;

    // The transitions of the automaton.
    Types::transitions_t transitions;

    // The start state of the automaton.
    std::shared_ptr<State> start;

    // The alphabets of the automaton.
    std::unordered_set<std::string> alphabets;

    // The epsilon symbol of the automaton.
    std::string epsilonSymbol;

    // The regular expression of the automaton.
    std::string regex;

    // maps state (accepting) to their tokens
    // needed in the case where every state has more that on possible token, (final union of automata)
    Types::state_to_string_set_map_t tokens{};

    // The built-in epsilon symbol.
    const std::string BUILT_IN_EPSILON_SYMBOL = "\\L";


public:


    // Default constructor.
    Automaton();

    // Constructor that initializes the automaton with an alphabet, a token name, and an epsilon symbol.
    Automaton(const std::string &alphabet, const std::string &token, const std::string &epsilonSymbol);

    // Adds transitions to the automaton.
    void add_transitions(const std::shared_ptr<State> &currentState, const std::string &transitionSymbol,
                         const Types::state_set_t &nextStates);

    // Adds transitions to the automaton.
    void add_transitions(const Types::transitions_t &extraTransitions);

    // Returns the next states from a given state and transition symbol.
    Types::state_set_t get_next_states(const std::shared_ptr<State> &currentState, const std::string &transitionSymbol);

    // Assigns new ids to all states in the automaton.
    void give_new_ids_all();

    // Assigns new ids to all states in a given set of states.
    void give_new_ids_all(int fromId, bool positive);

    // Sets the token of the automaton.
    void set_token(const std::string &tokenName);

    // Returns the transitions of the automaton in DFA format.
    Types::transitions_dfa_t get_transitions_dfa_format();

    // Sets the transitions of the automaton in DFA format.
    void set_transitions_dfa_format(const Types::transitions_dfa_t &new_transitions);

    // Returns the tokens of the automaton.
    std::string get_tokens_string();

    // Returns the token of the automaton.
    std::string get_token();

    // Returns a state by its id.
    std::shared_ptr<State> get_state_using_id(int id);

    // Adds a state_ptr to the automaton.
    void add_state(const std::shared_ptr<State> &statePtr);

    // Adds states to the automaton.
    void add_states(const Types::state_set_t &extraStates);

    // Adds a final state to the automaton.
    void add_accepting_state(const std::shared_ptr<State> &state);

    // Adds final states to the automaton.
    [[maybe_unused]] void add_accepting_states(const Types::state_set_t &extraStates);

    // Returns the states of the automaton.
    Types::state_set_t &get_states();

    // Returns the alphabets of the automaton.
    std::unordered_set<std::string> &get_alphabets();

    // Adds alphabets to the automaton.
    void add_alphabets(const std::unordered_set<std::string> &extraAlphabets);

    // Adds an alphabet to the automaton.
    void add_alphabet(const std::string &alphabet);

    // Returns the start state of the automaton.
    std::shared_ptr<State> &get_start();

    // Sets the start state of the automaton.
    void set_start(const std::shared_ptr<State> &state);

    // Returns the transitions of the automaton.
    Types::transitions_t &get_transitions();

    // Returns the accepting states of the automaton.
    Types::state_set_t &get_accepting_states();

    // Chicks if a state in the states vector is accepting
    bool has_accepting_state(Types::state_set_t &states_set);

    // Checks if a state_ptr is an accepting state_ptr.
    bool is_accepting_state(const std::shared_ptr<State> &state_ptr);

    // Returns the epsilon symbol of the automaton.
    [[nodiscard]] std::string get_epsilon_symbol() const;

    // Sets the epsilon symbol of the automaton.
    void set_epsilon_symbol(std::string symbol);

    // Sets the regular expression of the automaton.
    void set_regex(std::string string);

    // Returns the regular expression of the automaton.
    std::string get_regex();

    std::string to_string();

    Types::string_set_t get_tokens(const std::shared_ptr<State> &state_ptr);

    void set_tokens(const Types::state_to_string_set_map_t &new_tokens);

    void add_tokens(const std::shared_ptr<State> &state_ptr, const Types::string_set_t &token_set);

    Types::state_to_string_set_map_t get_tokens();

    std::string to_string_transition_table();

    std::string to_json();

    void export_to_file(const std::string &filename);

    static std::shared_ptr<Automaton> import_from_file(const std::string &filename);

    std::vector<std::vector<std::shared_ptr<State>>> matrix_representation();

};


#endif
