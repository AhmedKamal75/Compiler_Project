//
// Created by Delta on 11/10/2023.
//

#include <sstream>
#include <algorithm>
#include <utility>
#include "Automaton.h"

Automaton::Automaton() {
    this->states = std::set<State>();
    this->alphabets = std::set<std::string>();
    this->start = State();
    this->transitions = std::map<std::pair<State, std::string>, std::set<State>>();
    this->accepting = std::set<State>();
    this->epsilonSymbol = BUILT_IN_EPSILON_SYMBOL;
}

Automaton::Automaton(const std::string &alphabet, const std::string &tokenName, const std::string &epsilonSymbol) {
    // Create the states
    State q0 = State(0, false, "");
    State q1 = State(1, true, (tokenName.empty()) ? alphabet : tokenName);

    // Initialize the fields
    this->states = {q0, q1};
    this->alphabets = {alphabet};
    this->start = q0;
    this->accepting = {q1};
    this->epsilonSymbol = (epsilonSymbol.empty()) ? this->BUILT_IN_EPSILON_SYMBOL : epsilonSymbol;

    // Add the transition
    this->addTransitions(q0, alphabet, {q1});
}

void Automaton::addTransitions(const State &currentState, const std::string &transitionSymbol,
                               const std::set<State> &nextStates) {
    std::pair<State, std::string> transitionKey(currentState, transitionSymbol);
    std::set<State> tempNextStates = getNextStates(currentState, transitionSymbol);
    tempNextStates.insert(nextStates.begin(), nextStates.end());
    this->transitions[transitionKey] = tempNextStates;
}

std::set<State> Automaton::getNextStates(const State &currentState, const std::string &transitionSymbol) {
    std::pair<State, std::string> transitionKey = std::make_pair(currentState, transitionSymbol);

    auto iterator = this->transitions.find(transitionKey);

    if (iterator != this->transitions.end()) {
        return iterator->second;
    } else {
        return {};
    }
}

void Automaton::giveNewIdsAll() {
    this->giveNewIdsAll(this->states, 0, true);
}

void Automaton::giveNewIdsAll(std::set<State> &stateSet, int fromId, bool positive) {
    // Create a vector and move elements from the set to the vector
    std::vector<State> tempStates(stateSet.begin(), stateSet.end());
    stateSet.clear();

    // Update the elements in the vector
    int i = fromId;
    for (State &state: tempStates) {
        state.setId(i);
        i = (positive) ? (i + 1) : (i - 1);
    }

    // Move the updated elements back to the set
    stateSet.insert(states.begin(), states.end());
}

void Automaton::setToken(const std::string &tokenName) {
    std::vector<State> temp(this->accepting.begin(), this->accepting.end());
    this->accepting.clear();

    for (State &state: temp) {
        state.setToken(tokenName);
        state.setAccepting(true);
    }

    this->accepting.insert(temp.begin(), temp.end());
}

std::map<std::pair<State, std::string>, State> Automaton::getTransitionsDFAFormat() {
    std::map<std::pair<State, std::string>, State> dfaTransitions;
    for (auto &entry: this->transitions) {
        if (!entry.second.empty()) {
            State nextState = *entry.second.begin();
            dfaTransitions[entry.first] = nextState;
        }
    }

    return dfaTransitions;
}

void Automaton::setTransitionsDFAFormat(const std::map<std::pair<State, std::string>, State> &newTransitions) {
    std::map<std::pair<State, std::string>, std::set<State>> tempTransitions;
    for (auto &entry: newTransitions) {
        std::set<State> newState = {entry.second};
        tempTransitions[entry.first] = newState;
    }
    this->transitions = tempTransitions;
}

std::string Automaton::getTokens() {
    std::string tokens = "{";
    for (const State &finalState: this->accepting) {
        tokens += "\"" + finalState.getToken() + "\", ";
    }

    // Remove the trailing comma and space, then add the closing brace
    if (tokens.length() > 1) {
        tokens = tokens.substr(0, tokens.length() - 2);
    }
    tokens += "}";
    return tokens;
}

std::string Automaton::getToken() {
    return (*this->accepting.begin()).getToken();
}

State &Automaton::getStateById(int id) {
    // Create a vector and move elements from the set to the vector
    std::vector<State> statesCopy(this->states.begin(), this->states.end());

    for (State &state: statesCopy) {
        if (state.getId() == id) {
            return state;
        }
    }
    // If the state with the given ID is not found, throw an exception.
    throw std::runtime_error("State with given ID not found");
}

void Automaton::addState(const State& state) {
    this->states.insert(state);
}

void Automaton::addFinalState(const State& state) {
    this->accepting.insert(state);
}

std::set<State> &Automaton::getStates() {
    return this->states;
}

std::set<std::string> &Automaton::getAlphabets() {
    return this->alphabets;
}

State &Automaton::getStart() {
    return this->start;
}

void Automaton::setStart(State &state) {
    this->start = state;
}

std::map<std::pair<State, std::string>, std::set<State>> &Automaton::getTransitions() {
    return this->transitions;
}

std::set<State> &Automaton::getAccepting() {
    return this->accepting;
}

bool Automaton::isAcceptingState(const State& state) {
    auto it = this->accepting.find(state);
    if (it != this->accepting.end()){
        return true;
    } else {
        return false;
    }
}

std::string Automaton::getEpsilonSymbol() {
    return this->epsilonSymbol;
}

void Automaton::setEpsilonSymbol(std::string symbol) {
    this->epsilonSymbol = std::move(symbol);
}

void Automaton::setRegex(std::string string) {
    this->regex = std::move(string);
}

std::string Automaton::getRegex() {
    return this->regex;
}

std::string Automaton::toJson() {
    std::ostringstream sb;

    sb << R"({"type":"DFA","dfa":{"transitions":{)";

    // Transitions
    for (const auto& entry : transitions) {
        sb << "\"" << entry.first.first.getId() << "\":{\""
           << entry.first.second << "\":\"";
        for (const State& state : entry.second) {
            sb << state.getId();
        }
        sb << "\"},";
    }
    // Remove trailing comma
    if (sb.tellp() > 1) {
        sb.seekp(-1, std::ios_base::end);
    }
    sb << "},";

    // Start State
    sb << R"("startState":")" << start.getId() << "\",";

    // Accept States
    sb << "\"acceptStates\":[";
    for (const State& state : accepting) {
        sb << "\"" << state.getId() << "\",";
    }
    // Remove trailing comma
    if (sb.tellp() > 1) {
        sb.seekp(-1, std::ios_base::end);
    }
    sb << "]}}";

    return sb.str();
}

std::string Automaton::toString() {
    std::stringstream ss;

    ss << "States: ";
    for (const auto &state: this->states) {
        ss << state.getId() << " ";
    }
    ss << "\n";

    ss << "Input Symbols: ";
    for (const auto &symbol: this->alphabets) {
        ss << symbol << " ";
    }
    ss << "\n";

    ss << "Start State: " << this->start.getId() << "\n";

    ss << "Final States: ";
    for (const auto &state: this->accepting) {
        ss << state.getId() << " ";
    }
    ss << "\n";

    ss << "Transition Function: \n";
    // Create a vector of the map's pairs
    std::vector<std::pair<std::pair<State, std::string>, std::set<State>>> sortedTransitions(this->transitions.begin(),
                                                                                             this->transitions.end());
    // Sort the vector
//    std::sort(sortedTransitions.begin(), sortedTransitions.end(),
//              [](const auto &a, const auto &b) {
//                  return a.first.first < b.first.first;
//              });

    std::sort(sortedTransitions.begin(), sortedTransitions.end());


    for (const auto &entry: sortedTransitions) {
        ss << "δ(" << entry.first.first.getId() << ", " << entry.first.second << ") = ";
        for (const auto &state: entry.second) {
            ss << state.getId() << " ";
        }
        ss << "\n";
    }

    return ss.str();
}


