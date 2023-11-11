#include <sstream>
#include <algorithm>
#include <utility>
#include "Automaton.h"

Automaton::Automaton() {
    this->states = std::vector<State>();
    this->alphabets = std::vector<std::string>();
    this->start = State();
    this->transitions = std::map<std::pair<State, std::string>, std::vector<State>>();
    this->accepting = std::vector<State>();
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
                               const std::vector<State> &nextStates) {
    std::pair<State, std::string> transitionKey(currentState, transitionSymbol);
    std::vector<State> tempNextStates = getNextStates(currentState, transitionSymbol);
    tempNextStates.insert(tempNextStates.end(), nextStates.begin(), nextStates.end());
    this->transitions[transitionKey] = tempNextStates;
}

std::vector<State> Automaton::getNextStates(const State &currentState, const std::string &transitionSymbol) {
    std::pair<State, std::string> transitionKey = std::make_pair(currentState, transitionSymbol);
    auto iterator = this->transitions.find(transitionKey);

    if (iterator != this->transitions.end()) {
        return iterator->second;
    } else {
        return std::vector<State>();
    }
}

void Automaton::giveNewIdsAll() {
    this->giveNewIdsAll(0, true);
}

void Automaton::giveNewIdsAll(int fromId, bool positive) {
    int i = fromId;
    for (auto &state: this->states) {
        state.setId(i);
        i = (positive) ? (i + 1) : (i - 1);
    }
}

void Automaton::setToken(const std::string &tokenName) {
    for (auto &state: this->accepting) {
        state.setAccepting(true);
        state.setToken(tokenName);
    }
}

std::map<std::pair<State, std::string>, State> Automaton::getTransitionsDFAFormat() {
    std::map<std::pair<State, std::string>, State> dfaTransitions;
    for (auto &entry: transitions) {
        if (!entry.second.empty()) {
            State nextState = *entry.second.begin();
            dfaTransitions[entry.first] = nextState;
        }
    }

    return dfaTransitions;
}

void Automaton::setTransitionsDFAFormat(const std::map<std::pair<State, std::string>, State> &newTransitions) {
    std::map<std::pair<State, std::string>, std::vector<State>> tempTransitions;
    for (auto &entry: newTransitions) {
        std::vector<State> newState = {entry.second};
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
    return this->accepting[0].getToken();
}

State &Automaton::getStateById(int id) {
    for (auto &state: this->states) {
        if (state.getId() == id) {
            return state;
        }
    }
    // If the state with the given ID is not found, throw an exception.
    throw std::runtime_error("State with given ID not found");
}

void Automaton::addState(const State &state) {
    this->states.push_back(state);
}

void Automaton::addFinalState(const State &state) {
    this->accepting.push_back(state);
}

std::vector<State> &Automaton::getStates() {
    return this->states;
}

std::vector<std::string> &Automaton::getAlphabets() {
    return this->alphabets;
}

State &Automaton::getStart() {
    return this->start;
}

void Automaton::setStart(State &state) {
    this->start = state;
}

std::map<std::pair<State, std::string>, std::vector<State>> &Automaton::getTransitions() {
    return this->transitions;
}

std::vector<State> &Automaton::getAccepting() {
    return this->accepting;
}

bool Automaton::isAcceptingState(const State &state) {
    return std::find(this->accepting.begin(), this->accepting.end(), state) != this->accepting.end();
}

std::string Automaton::getEpsilonSymbol() const {
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
    for (const auto &entry: transitions) {
        sb << "\"" << entry.first.first.getId() << "\":{\""
           << entry.first.second << "\":\"";
        for (const State &state: entry.second) {
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
    for (const State &state: accepting) {
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
    std::vector<std::pair<std::pair<State, std::string>, std::vector<State>>> sortedTransitions(
            this->transitions.begin(),
            this->transitions.end());
    // Sort the vector
    std::sort(sortedTransitions.begin(), sortedTransitions.end());

    for (const auto &entry: sortedTransitions) {
        ss << "f(" << entry.first.first.getId() << ", " << entry.first.second << ") = ";
        for (const auto &state: entry.second) {
            ss << state.getId() << " ";
        }
        ss << "\n";
    }

    return ss.str();
}


