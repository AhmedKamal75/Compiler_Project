#include <algorithm>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include "Automaton.h"


Automaton::Automaton() {
    this->states = Types::state_set_t();
    this->alphabets = std::unordered_set<std::string>();
    this->transitions = Types::transitions_t();
    this->accepting = Types::state_set_t();
    this->start.reset();
    this->epsilonSymbol = BUILT_IN_EPSILON_SYMBOL;
    this->regex = "";
}

Automaton::Automaton(const std::string &alphabet, const std::string &token, const std::string &epsilonSymbol) {
    // Create the states
    auto q0 = std::make_shared<State>(0, false, "");
    auto q1 = std::make_shared<State>(1, true, (token.empty()) ? alphabet : token);

    // Initialize the fields
    this->states.insert(q0);
    this->states.insert(q1);
    this->alphabets.insert(alphabet);
    this->start = q0;
    this->accepting.insert(q1);
    this->epsilonSymbol = (epsilonSymbol.empty()) ? this->BUILT_IN_EPSILON_SYMBOL : epsilonSymbol;
    this->set_regex("(" + q1->getToken() + ")");

    // Add the transition
    Types::key_t key = std::make_pair(q0, alphabet);
    Types::state_set_t value = {q1};
    this->transitions.insert({key, value});
}

void Automaton::add_transitions(const std::shared_ptr<State> &currentState, const std::string &transitionSymbol,
                                const Types::state_set_t &nextStates) {
    Types::key_t transitionKey = std::make_pair(currentState, transitionSymbol);
    Types::state_set_t &tempNextStates = this->transitions[transitionKey];
    for (const std::shared_ptr<State> &state: nextStates) {
        tempNextStates.insert(state);
    }
}

void Automaton::add_transitions(const Types::transitions_t &extraTransitions) {
    for (auto &entry: extraTransitions) {
        this->add_transitions(entry.first.first, entry.first.second, entry.second);
    }
}

Types::state_set_t
Automaton::get_next_states(const std::shared_ptr<State> &currentState, const std::string &transitionSymbol) {
    Types::key_t transitionKey = std::make_pair(currentState, transitionSymbol);
    auto it = this->transitions.find(transitionKey);
    if (it != this->transitions.end()) {
        return it->second;
    } else {
        return {};
    }
}

std::shared_ptr<State> Automaton::get_state_using_id(int id) {
    auto it = std::find_if(this->states.begin(), this->states.end(),
                           [id](const std::shared_ptr<State> &state_ptr) { return state_ptr->getId() == id; }
    );

    if (it != this->states.end()) {
        return *it;
    }
    // If the state with the given ID is not found, throw an exception.
    throw std::runtime_error("State with given ID not found");
}

Types::transitions_dfa_t Automaton::get_transitions_dfa_format() {
    Types::transitions_dfa_t dfa_transitions;
    for (auto &entry: this->transitions) {
        if (!entry.second.empty()) {
            std::shared_ptr<State> nextState = *entry.second.begin();
            dfa_transitions[entry.first] = nextState;
        }
    }
    return dfa_transitions;
}

void Automaton::set_transitions_dfa_format(const Types::transitions_dfa_t &new_transitions) {
    Types::transitions_t temp_transitions;
    for (auto &entry: new_transitions) {
        temp_transitions[entry.first] = {entry.second};
    }
    this->transitions = temp_transitions;
}

void Automaton::give_new_ids_all() {
    this->give_new_ids_all(0, true);
}

void Automaton::give_new_ids_all(int fromId, bool positive) {
    int i = fromId;
    /*
     * By changing the loop variable from a reference to a copy
     * (std::shared_ptr<State> state_ptr instead of std::shared_ptr<State>& state),
     * I am  creating a copy of the shared pointer, not the State object it points to.
     * This allows me to modify the State object without modifying the shared pointer
     */
    for (std::shared_ptr<State> state_ptr: this->states) {
        state_ptr->setId(i);
        i = (positive) ? (i + 1) : (i - 1);
    }
}

void Automaton::set_token(const std::string &tokenName) {
    // the same happened here like in the give_new_ids_all
    for (std::shared_ptr<State> state: this->accepting) {
        state->setAccepting(true);
        state->setToken(tokenName);
    }
}

std::string Automaton::get_tokens_string() {
    std::unordered_set<std::string> unique_strings;

    // If tokens are not empty, insert all tokens into the unique_strings set
    if (!tokens.empty()) {
        for (const auto &pair: this->tokens) {
            unique_strings.insert(pair.second.begin(), pair.second.end());
        }
    } else {
        // If tokens are empty, insert tokens from accepting states into the unique_strings set
        for (const std::shared_ptr<State> &state_ptr: this->accepting) {
            unique_strings.insert(state_ptr->getToken());
        }
    }

    // Convert the unique_strings set to a comma-separated string
    std::stringstream ss;
    for (const auto &str: unique_strings) {
        if (!ss.str().empty()) {
            ss << ", ";
        }
        ss << str;
    }

    return ss.str();
}

std::string Automaton::get_token() {
    return (*(this->accepting.begin()))->getToken();
}

void Automaton::add_state(const std::shared_ptr<State> &statePtr) {
    this->states.insert(statePtr);
}

void Automaton::add_states(const Types::state_set_t &extraStates) {
    for (const auto &state_ptr: extraStates) {
        this->add_state(state_ptr);
    }
}

void Automaton::add_accepting_state(const std::shared_ptr<State> &state) {
    this->accepting.insert(state);
}

void Automaton::add_accepting_states(const Types::state_set_t &extraStates) {
    for (const auto &state_ptr: extraStates) {
        this->add_accepting_state(state_ptr);
    }
}

Types::state_set_t &Automaton::get_states() {
    return this->states;
}

std::unordered_set<std::string> &Automaton::get_alphabets() {
    return this->alphabets;
}

void Automaton::add_alphabet(const std::string &alphabet) {
    this->alphabets.insert(alphabet);
}

void Automaton::add_alphabets(const std::unordered_set<std::string> &extraAlphabets) {
    for (const std::string &alphabet: extraAlphabets) {
        this->add_alphabet(alphabet);
    }
}

std::shared_ptr<State> &Automaton::get_start() {
    return this->start;
}

void Automaton::set_start(const std::shared_ptr<State> &state) {
    this->start = state;
}

Types::transitions_t &Automaton::get_transitions() {
    return this->transitions;
}

Types::state_set_t &Automaton::get_accepting_states() {
    return this->accepting;
}

bool Automaton::has_accepting_state(Types::state_set_t &states_set) {
    return std::any_of(states_set.begin(), states_set.end(),
                       [this](const std::shared_ptr<State> &state_ptr) {
                           return this->accepting.find(state_ptr) != this->accepting.end();
                       });
}

bool Automaton::is_accepting_state(const std::shared_ptr<State> &state_ptr) {
    return this->accepting.find(state_ptr) != this->accepting.end();
}

std::string Automaton::get_epsilon_symbol() const {
    return this->epsilonSymbol;
}

void Automaton::set_epsilon_symbol(std::string symbol) {
    this->epsilonSymbol = std::move(symbol);
}

void Automaton::set_regex(std::string string) {
    this->regex = std::move(string);
}

std::string Automaton::get_regex() {
    return this->regex;
}


Types::string_set_t Automaton::get_tokens(const std::shared_ptr<State> &state_ptr) {
    auto it = this->tokens.find(state_ptr);
    if (it != this->tokens.end()) {
        return it->second;
    }
    return {};
}

void Automaton::set_tokens(const Types::state_to_string_set_map_t &new_tokens) {
    this->tokens = {};
    for (const auto & pair:new_tokens){
        if (this->accepting.find(pair.first) != this->accepting.end()){
            this->tokens.insert(std::make_pair(pair.first,pair.second));
        }
    }
//    this->tokens = new_tokens;
}

void Automaton::add_tokens(const std::shared_ptr<State> &state_ptr, const Types::string_set_t &token_set) {
    auto it = this->tokens.find(state_ptr);
    if (it != this->tokens.end()) {
        // State exists in the map, insert new tokens into existing set
        it->second.insert(token_set.begin(), token_set.end());
    } else {
        // State doesn't exist in the map, add it with the new token set
        this->tokens.insert(std::make_pair(state_ptr, token_set));
    }
}

Types::state_to_string_set_map_t Automaton::get_tokens() {
    return this->tokens;
}


std::string Automaton::to_json() {
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

std::string Automaton::to_string() {
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
    std::vector<std::pair<Types::key_t, Types::state_set_t>> sorted_transitions(
            this->transitions.begin(),
            this->transitions.end());
    // Sort the vector
    std::sort(sorted_transitions.begin(), sorted_transitions.end(),
              [](std::pair<Types::key_t, Types::state_set_t> &a,
                 std::pair<Types::key_t, Types::state_set_t> &b) {
                  return a.first.first->getId() < b.first.first->getId();
              });


    for (const auto &entry: sorted_transitions) {
        ss << "f(" << entry.first.first->getId() << ", " << entry.first.second << ") = ";
        for (const auto &state: entry.second) {
            ss << state->getId() << " ";
        }
        ss << "\n";
    }

    // add the token
    ss << "Token: " << this->get_token() << "\n";

    // add the token
    ss << "Regex: " << this->get_regex() << "\n";

    return ss.str();
}

bool Automaton::equals(const Automaton &other) const {
    return states == other.states &&
           alphabets == other.alphabets &&
           *start == *(other.start) &&
           accepting == other.accepting &&
           transitions == other.transitions &&
           epsilonSymbol == other.epsilonSymbol;
}

std::string Automaton::to_string_transition_table() {
    std::stringstream ss;

    // Print the start state
    ss << "Start State: " << this->start->getId() << "\n";

    // Print the final states
    ss << "Final States: ";
    for (const auto &state: this->accepting) {
        ss << state->getId() << " ";
    }
    ss << "\n";

    // First, we need to find all unique symbols
    std::set<std::string> unique_symbols;
    for (const auto &entry: this->transitions) {
        unique_symbols.insert(entry.first.second);
    }

    // Then, we create a map to store the transition table
    std::map<int, std::map<std::string, int>> transition_table;
    for (const auto &entry: this->transitions) {
        for (const auto &state: entry.second) {
            transition_table[entry.first.first->getId()][entry.first.second] = state->getId();
        }
    }

    ss << "Transition Table: \n";
    // Now, we can print the transition table
    ss << "\t";
    for (const auto &symbol: unique_symbols) {
        ss << symbol << "\t";
    }
    ss << "\n";

    for (const auto &row: transition_table) {
        ss << row.first << "\t";
        for (const auto &symbol: unique_symbols) {
            if (row.second.count(symbol)) {
                ss << row.second.at(symbol) << "\t";
            } else {
                ss << "-\t";  // Use '-' or any symbol to denote no transition
            }
        }
        ss << "\n";
    }

    // add the token
    ss << "Token: " << this->get_token() << "\n";

    // add the token
    ss << "Regex: " << this->get_regex() << "\n";


    return ss.str();
}
