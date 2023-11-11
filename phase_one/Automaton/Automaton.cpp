#include <sstream>
#include <algorithm>
#include <utility>
#include "Automaton.h"

Automaton::Automaton() {
    this->states = std::vector<std::shared_ptr<State>>();
    this->alphabets = std::vector<std::string>();
    this->start = std::make_shared<State>();
    this->transitions = std::map<std::pair<std::shared_ptr<State>, std::string>, std::vector<std::shared_ptr<State>>>();
    this->accepting = std::vector<std::shared_ptr<State>>();
    this->epsilonSymbol = BUILT_IN_EPSILON_SYMBOL;
}

Automaton::Automaton(const std::string &alphabet, const std::string &tokenName, const std::string &epsilonSymbol) {
    // Create the states
    auto q0 = std::make_shared<State>(0, false, "");
    auto q1 = std::make_shared<State>(1, true, (tokenName.empty()) ? alphabet : tokenName);

    // Initialize the fields
    this->states = {q0, q1};
    this->alphabets = {alphabet};
    this->start = q0;
    this->accepting = {q1};
    this->epsilonSymbol = (epsilonSymbol.empty()) ? this->BUILT_IN_EPSILON_SYMBOL : epsilonSymbol;

    // Add the transition
    this->addTransitions(q0, alphabet, {q1});
}

void Automaton::addTransitions(const std::shared_ptr<State> &currentState, const std::string &transitionSymbol,
                               const std::vector<std::shared_ptr<State>> &nextStates) {
    std::pair<std::shared_ptr<State>, std::string> transitionKey(currentState, transitionSymbol);
    std::vector<std::shared_ptr<State>> tempNextStates = getNextStates(currentState, transitionSymbol);
    for (const auto &state: nextStates) {
        if (std::find(tempNextStates.begin(), tempNextStates.end(), state) == tempNextStates.end()) {
            tempNextStates.push_back(state);
        }
    }
    this->transitions[transitionKey] = tempNextStates;
}

void Automaton::addTransitions(
        const std::map<std::pair<std::shared_ptr<State>, std::string>, std::vector<std::shared_ptr<State>>> &extraTransitions) {
    for (auto &entry: extraTransitions) {
        this->addTransitions(entry.first.first, entry.first.second, entry.second);
    }
}

std::vector<std::shared_ptr<State>>
Automaton::getNextStates(const std::shared_ptr<State> &currentState, const std::string &transitionSymbol) {
    auto iterator = this->transitions.find(std::make_pair(currentState, transitionSymbol));
    if (iterator != this->transitions.end()) {
        return iterator->second;
    } else {
        return {};
    }
}

void Automaton::giveNewIdsAll() {
    this->giveNewIdsAll(0, true);
}

void Automaton::giveNewIdsAll(int fromId, bool positive) {
    int i = fromId;
    for (auto &state: this->states) {
        state->setId(i);
        i = (positive) ? (i + 1) : (i - 1);
    }
}

void Automaton::setToken(const std::string &tokenName) {
    for (auto &state: this->accepting) {
        state->setAccepting(true);
        state->setToken(tokenName);
    }
}

std::map<std::pair<std::shared_ptr<State>, std::string>, std::shared_ptr<State>> Automaton::getTransitionsDFAFormat() {
    std::map<std::pair<std::shared_ptr<State>, std::string>, std::shared_ptr<State>> dfaTransitions;
    for (auto &entry: this->transitions) {
        if (!entry.second.empty()) {
            std::shared_ptr<State> nextState = *entry.second.begin();
            dfaTransitions[entry.first] = nextState;
        }
    }

    return dfaTransitions;
}

void Automaton::setTransitionsDFAFormat(
        const std::map<std::pair<std::shared_ptr<State>, std::string>, std::shared_ptr<State>> &newTransitions) {
    std::map<std::pair<std::shared_ptr<State>, std::string>, std::vector<std::shared_ptr<State>>> tempTransitions;
    for (auto &entry: newTransitions) {
        tempTransitions[entry.first] = {entry.second};
    }
    this->transitions = tempTransitions;
}

std::string Automaton::getTokens() {
    std::string tokens = "{";
    for (const auto &finalState: this->accepting) {
        tokens += "\"" + finalState->getToken() + "\", ";
    }

    // Remove the trailing comma and space, then add the closing brace
    if (tokens.length() > 1) {
        tokens = tokens.substr(0, tokens.length() - 2);
    }
    tokens += "}";
    return tokens;
}

std::string Automaton::getToken() {
    return this->accepting[0]->getToken();
}


std::shared_ptr<State> Automaton::getStateById(int id) {
    for (auto &state: this->states) {
        if (state->getId() == id) {
            return state;
        }
    }
    // If the state with the given ID is not found, throw an exception.
    throw std::runtime_error("State with given ID not found");
}

void Automaton::addState(const std::shared_ptr<State> &state) {
    if (std::find(this->states.begin(), this->states.end(), state) == this->states.end()) {
        this->states.push_back(state);
    }
}

void Automaton::addStates(const std::vector<std::shared_ptr<State>> &extraStates) {
    for (const auto &state: extraStates) {
        if (std::find(this->states.begin(), this->states.end(), state) == this->states.end()) {
            this->states.push_back(state);
        }
    }
}

void Automaton::addFinal(const std::shared_ptr<State> &state) {
    if (std::find(this->accepting.begin(), this->accepting.end(), state) == this->accepting.end()) {
        this->accepting.push_back(state);
    }
}

[[maybe_unused]] void Automaton::addFinals(const std::vector<std::shared_ptr<State>> &extraStates) {
    for (const auto &state: extraStates) {
        if (std::find(this->accepting.begin(), this->accepting.end(), state) == this->accepting.end()) {
            this->accepting.push_back(state);
        }
    }
}

std::vector<std::shared_ptr<State>> &Automaton::getStates() {
    return this->states;
}

std::vector<std::string> &Automaton::getAlphabets() {
    return this->alphabets;
}

void Automaton::addAlphabets(const std::vector<std::string> &extraAlphabets) {
    for (const std::string &alphabet: extraAlphabets) {
        if (std::find(this->alphabets.begin(), this->alphabets.end(), alphabet) == this->alphabets.end()) {
            this->alphabets.push_back(alphabet);
        }
    }
}

void Automaton::addAlphabet(const std::string &alphabet) {
    if (std::find(this->alphabets.begin(), this->alphabets.end(), alphabet) == this->alphabets.end()) {
        this->alphabets.push_back(alphabet);
    }
}

std::shared_ptr<State> &Automaton::getStart() {
    return this->start;
}

void Automaton::setStart(const std::shared_ptr<State> &state) {
    this->start = state;
}


std::map<std::pair<std::shared_ptr<State>, std::string>, std::vector<std::shared_ptr<State>>> &
Automaton::getTransitions() {
    return this->transitions;
}

std::vector<std::shared_ptr<State>> &Automaton::getAccepting() {
    return this->accepting;
}

bool Automaton::isAcceptingState(const std::shared_ptr<State> &state) {
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
        sb << "\"" << entry.first.first->getId() << "\":{\""
           << entry.first.second << "\":\"";
        for (const auto &state: entry.second) {
            sb << state->getId();
        }
        sb << "\"},";
    }
    // Remove trailing comma
    if (sb.tellp() > 1) {
        sb.seekp(-1, std::ios_base::end);
    }
    sb << "},";

    // Start State
    sb << R"("startState":")" << start->getId() << "\",";

    // Accept States
    sb << "\"acceptStates\":[";
    for (const auto &state: accepting) {
        sb << "\"" << state->getId() << "\",";
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
        ss << state->getId() << " ";
    }
    ss << "\n";

    ss << "Input Symbols: ";
    for (const auto &symbol: this->alphabets) {
        ss << symbol << " ";
    }
    ss << "\n";

    ss << "Start State: " << this->start->getId() << "\n";

    ss << "Final States: ";
    for (const auto &state: this->accepting) {
        ss << state->getId() << " ";
    }
    ss << "\n";

    ss << "Transition Function: \n";
    // Create a vector of the map's pairs
    std::vector<std::pair<std::pair<std::shared_ptr<State>, std::string>, std::vector<std::shared_ptr<State>>>> sortedTransitions(
            this->transitions.begin(),
            this->transitions.end());
    // Sort the vector
    std::sort(sortedTransitions.begin(), sortedTransitions.end());

    for (const auto &entry: sortedTransitions) {
        ss << "f(" << entry.first.first->getId() << ", " << entry.first.second << ") = ";
        for (const auto &state: entry.second) {
            ss << state->getId() << " ";
        }
        ss << "\n";
    }

    return ss.str();
}
