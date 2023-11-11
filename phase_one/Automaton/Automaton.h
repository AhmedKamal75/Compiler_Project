#ifndef COMPILER_PROJECT_AUTOMATON_H
#define COMPILER_PROJECT_AUTOMATON_H

#include <set>
#include <map>
#include <string>
#include <vector>
#include <memory>
#include "State.h"

/**
 * This class represents a finite automaton.
 * An automaton is a finite representation of a formal language that can be an acceptor or a transducer.
 *
 * remember to use std::make_shared<State>() when creating new State objects to ensure they are managed by smart pointers.
 */
class Automaton {
public:
    // Default constructor.
    Automaton();

    // Constructor that initializes the automaton with an alphabet, a token name, and an epsilon symbol.
    Automaton(const std::string &alphabet, const std::string &tokenName, const std::string &epsilonSymbol);

    // Adds transitions to the automaton.
    void addTransitions(const std::shared_ptr<State> &currentState, const std::string &transitionSymbol,
                        const std::vector<std::shared_ptr<State>> &nextStates);

    // Adds transitions to the automaton.
    void addTransitions(
            const std::map<std::pair<std::shared_ptr<State>, std::string>, std::vector<std::shared_ptr<State>>> &extraTransitions);

    // Returns the next states from a given state and transition symbol.
    std::vector<std::shared_ptr<State>>
    getNextStates(const std::shared_ptr<State> &currentState, const std::string &transitionSymbol);

    // Assigns new ids to all states in the automaton.
    void giveNewIdsAll();

    // Assigns new ids to all states in a given set of states.
    void giveNewIdsAll(int fromId, bool positive);

    // Sets the token of the automaton.
    void setToken(const std::string &tokenName);

    // Returns the transitions of the automaton in DFA format.
    std::map<std::pair<std::shared_ptr<State>, std::string>, std::shared_ptr<State>> getTransitionsDFAFormat();

    // Sets the transitions of the automaton in DFA format.
    void setTransitionsDFAFormat(
            const std::map<std::pair<std::shared_ptr<State>, std::string>, std::shared_ptr<State>> &newTransitions);

    // Returns the tokens of the automaton.
    std::string getTokens();

    // Returns the token of the automaton.
    std::string getToken();

    // Returns a state by its id.
    std::shared_ptr<State> getStateById(int id);

    // Adds a state to the automaton.
    void addState(const std::shared_ptr<State> &state);

    // Adds states to the automaton.
    void addStates(const std::vector<std::shared_ptr<State>> &extraStates);

    // Adds a final state to the automaton.
    void addFinal(const std::shared_ptr<State> &state);

    // Adds final states to the automaton.
    [[maybe_unused]] void addFinals(const std::vector<std::shared_ptr<State>> &extraStates);

    // Returns the states of the automaton.
    std::vector<std::shared_ptr<State>> &getStates();

    // Returns the alphabets of the automaton.
    std::vector<std::string> &getAlphabets();

    // Adds alphabets to the automaton.
    void addAlphabets(const std::vector<std::string> &extraAlphabets);

    // Adds an alphabet to the automaton.
    void addAlphabet(const std::string &alphabet);

    // Returns the start state of the automaton.
    std::shared_ptr<State> &getStart();

    // Sets the start state of the automaton.
    void setStart(const std::shared_ptr<State> &state);

    // Returns the transitions of the automaton.
    std::map<std::pair<std::shared_ptr<State>, std::string>, std::vector<std::shared_ptr<State>>> &getTransitions();

    // Returns the accepting states of the automaton.
    std::vector<std::shared_ptr<State>> &getAccepting();

    // Checks if a state is an accepting state.
    bool isAcceptingState(const std::shared_ptr<State> &state);

    // Returns the epsilon symbol of the automaton.
    [[nodiscard]] std::string getEpsilonSymbol() const;

    // Sets the epsilon symbol of the automaton.
    void setEpsilonSymbol(std::string symbol);

    // Sets the regular expression of the automaton.
    void setRegex(std::string string);

    // Returns the regular expression of the automaton.
    std::string getRegex();

    std::string toString();

    std::string toJson();

private:

    // The states of the automaton.
    std::vector<std::shared_ptr<State>> states;

    // The accepting states of the automaton.
    std::vector<std::shared_ptr<State>> accepting;

    // The transitions of the automaton.
    std::map<std::pair<std::shared_ptr<State>, std::string>, std::vector<std::shared_ptr<State>>> transitions;

    // The start state of the automaton.
    std::shared_ptr<State> start;

    // The alphabets of the automaton.
    std::vector<std::string> alphabets;

    // The epsilon symbol of the automaton.
    std::string epsilonSymbol;

    // The regular expression of the automaton.
    std::string regex;

    // The built-in epsilon symbol.
    const std::string BUILT_IN_EPSILON_SYMBOL = "\\L";
};

#endif //COMPILER_PROJECT_AUTOMATON_H
