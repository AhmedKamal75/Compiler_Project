
#ifndef COMPILER_PROJECT_AUTOMATON_H
#define COMPILER_PROJECT_AUTOMATON_H

#include <set>
#include <map>
#include <string>
#include <vector>
#include "State.h"

/**
 * This class represents a finite automaton.
 * An automaton is a finite representation of a formal language that can be an acceptor or a transducer.
 */
class Automaton {
public:
    // Default constructor.
    Automaton();

    // Constructor that initializes the automaton with an alphabet, a token name, and an epsilon symbol.
    Automaton(const std::string &alphabet, const std::string &tokenName, const std::string &epsilonSymbol);

    // Adds transitions to the automaton.
    void addTransitions(const State &currentState, const std::string &transitionSymbol,
                        const std::vector<State> &nextStates);

    // Returns the next states from a given state and transition symbol.
    std::vector<State> getNextStates(const State &currentState, const std::string &transitionSymbol);

    // Assigns new ids to all states in the automaton.
    void giveNewIdsAll();

    // Assigns new ids to all states in a given set of states.
    void giveNewIdsAll(std::vector<State> &stateSet, int fromId, bool positive);

    // Sets the token of the automaton.
    void setToken(const std::string &tokenName);

    // Returns the transitions of the automaton in DFA format.
    std::map<std::pair<State, std::string>, State> getTransitionsDFAFormat();

    // Sets the transitions of the automaton in DFA format.
    void setTransitionsDFAFormat(const std::map<std::pair<State, std::string>, State> &newTransitions);

    // Returns the tokens of the automaton.
    std::string getTokens();

    // Returns the token of the automaton.
    std::string getToken();

    // Returns a state by its id.
    State &getStateById(int id);

    // Adds a state to the automaton.
    void addState(const State &state);

    // Adds a final state to the automaton.
    void addFinalState(const State &state);

    // Returns the states of the automaton.
    std::vector<State> &getStates();

    // Returns the alphabets of the automaton.
    std::vector<std::string> &getAlphabets();

    // Returns the start state of the automaton.
    State &getStart();

    // Sets the start state of the automaton.
    void setStart(State &state);

    // Returns the transitions of the automaton.
    std::map<std::pair<State, std::string>, std::vector<State>> &getTransitions();

    // Returns the accepting states of the automaton.
    std::vector<State> &getAccepting();

    // Checks if a state is an accepting state.
    bool isAcceptingState(const State &state);

    // Returns the epsilon symbol of the automaton.
    [[nodiscard]] std::string getEpsilonSymbol() const;

    // Sets the epsilon symbol of the automaton.
    void setEpsilonSymbol(std::string symbol);

    // Sets the regular expression of the automaton.
    void setRegex(std::string string);

    // Returns the regular expression of the automaton.
    std::string getRegex();

    // Returns a JSON representation of the automaton.
    std::string toJson();

    // Returns a string representation of the automaton.
    std::string toString();

private:
    // The states of the automaton.
    std::vector<State> states = {};

    // The alphabets of the automaton.
    std::vector<std::string> alphabets = {};

    // The start state of the automaton.
    State start;

    // The transitions of the automaton.
    std::map<std::pair<State, std::string>, std::vector<State>> transitions = {};

    // The accepting states of the automaton.
    std::vector<State> accepting = {};

    // The epsilon symbol of the automaton.
    std::string epsilonSymbol{};

    // The regular expression of the automaton.
    std::string regex{};

    // The built-in epsilon symbol.
    const std::string BUILT_IN_EPSILON_SYMBOL = "\\L";
};

#endif //COMPILER_PROJECT_AUTOMATON_H
