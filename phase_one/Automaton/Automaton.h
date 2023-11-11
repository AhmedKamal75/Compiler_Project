
#ifndef COMPILER_PROJECT_AUTOMATON_H
#define COMPILER_PROJECT_AUTOMATON_H

#include <set>
#include <map>
#include <string>
#include "State.h"

class Automaton {
public:
    Automaton();

    Automaton(const std::string &alphabet, const std::string &tokenName, const std::string &epsilonSymbol);

    void
    addTransitions(const State &currentState, const std::string &transitionSymbol, const std::set<State> &nextStates);

    std::set<State> getNextStates(const State& currentState, const std::string& transitionSymbol);

    void giveNewIdsAll();

    void giveNewIdsAll(std::set<State>& stateSet, int fromId, bool positive);

    void setToken(const std::string& tokenName);

    std::map<std::pair<State, std::string>, State> getTransitionsDFAFormat();

    void setTransitionsDFAFormat(const std::map<std::pair<State, std::string>, State>& newTransitions);

    std::string getTokens();

    std::string getToken();

    State &getStateById(int id);

    void addState(const State& state);

    void addFinalState(const State& state);

    std::set<State> &getStates();

    std::set<std::string> &getAlphabets();

    State &getStart();

    void setStart(State &state);

    std::map<std::pair<State, std::string>, std::set<State>> &getTransitions();

    std::set<State> &getAccepting();

    bool isAcceptingState(const State& state);

    std::string getEpsilonSymbol();

    void setEpsilonSymbol(std::string symbol);

    void setRegex(std::string string);

    std::string getRegex();

    std::string toJson();

    std::string toString();

private:
    std::set<State> states = {};
    std::set<std::string> alphabets = {};
    State start;
    std::map<std::pair<State, std::string>, std::set<State>> transitions = {};
    std::set<State> accepting = {};
    std::string epsilonSymbol{};
    std::string regex{};

    const std::string BUILT_IN_EPSILON_SYMBOL = "\\L";
};

#endif //COMPILER_PROJECT_AUTOMATON_H
