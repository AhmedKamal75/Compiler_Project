#include <algorithm>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <iterator>
#include <regex>
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
    for (const auto &pair: new_tokens) {
        if (this->accepting.find(pair.first) != this->accepting.end()) {
            this->tokens.insert(std::make_pair(pair.first, pair.second));
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

    // add the regex
    ss << "Regex: " << this->get_regex() << "\n";

    // add the token
    ss << "Tokens: " << this->get_tokens_string() << "\n";
    for (const auto &pair: this->tokens) {
        ss << pair.first->toString() << ": ";
        for (const auto &str: pair.second) {
            ss << str << " ";
        }
        ss << '\n';
    }

    return ss.str();
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

    // Then, we init a map to store the transition table
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

    // add the regex
    ss << "Regex: " << this->get_regex() << "\n";

    // add the token
    ss << "Tokens: " << this->get_tokens_string() << "\n";
    for (const auto &pair: this->tokens) {
        ss << pair.first->toString() << ": ";
        for (const auto &str: pair.second) {
            ss << str << " ";
        }
        ss << '\n';
    }
    return ss.str();
}

void Automaton::export_to_file(const std::string &filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << to_string();
        file.close();
    } else {
        std::cout << "Unable to open file";
    }
}

std::shared_ptr<Automaton> Automaton::import_from_file(const std::string &filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        std::shared_ptr<Automaton> automaton = std::make_shared<Automaton>();
        while (std::getline(file, line)) {
            // erase the space at the end of the line
            line.erase(line.find_last_not_of(" \n\r\t") + 1);
            // Parse the line and update the automaton object

            // Check if the line contains the states
            if (line.substr(0, 7) == "States:") {
                // Remove the "States: " part from the line
                line.erase(0, 8);

                // Split the line into state IDs
                std::istringstream iss(line);
                std::vector<std::string> stateIDs((std::istream_iterator<std::string>(iss)),
                                                  std::istream_iterator<std::string>());

                // Add each state to the automaton
                for (const auto &id: stateIDs) {
                    try {
                        automaton->add_state(std::make_shared<State>(std::stoi(id), false, ""));
                    } catch (const std::invalid_argument &e) {
                        std::cerr << "Invalid state ID: " << id << '\n';
                    } catch (const std::out_of_range &e) {
                        std::cerr << "State ID out of range: " << id << '\n';
                    }
                }
            }

            if (line.substr(0, 14) == "Input Symbols:") {
                // Remove the "Input Symbols: " part from the line
                line.erase(0, 15);

                // Split the line into symbols
                std::istringstream iss(line);
                std::vector<std::string> symbols((std::istream_iterator<std::string>(iss)),
                                                 std::istream_iterator<std::string>());

                // Add each symbol to the automaton
                for (const auto &symbol: symbols) {
                    automaton->add_alphabet(symbol);
                }
            }

            if (line.substr(0, 12) == "Start State:") {
                // Remove the "Start State: " part from the line
                line.erase(0, 13);

                // Parse the start state ID
                int startStateID = std::stoi(line);

                // Set the start state in the automaton
                automaton->set_start(automaton->get_state_using_id(startStateID));
            }

            if (line.substr(0, 13) == "Final States:") {
                // Remove the "Final States: " part from the line
                line.erase(0, 14);

                // Split the line into state IDs
                std::istringstream iss(line);
                std::vector<std::string> stateIDs((std::istream_iterator<std::string>(iss)),
                                                  std::istream_iterator<std::string>());

                // Add each state to the automaton
                for (const auto &id: stateIDs) {
                    try {
                        automaton->add_accepting_state(automaton->get_state_using_id(std::stoi(id)));
                    } catch (const std::invalid_argument &e) {
                        std::cerr << "Invalid state ID: " << id << '\n';
                    } catch (const std::out_of_range &e) {
                        std::cerr << "State ID out of range: " << id << '\n';
                    }
                }
            }

            if (line.substr(0, 20) == "Transition Function:") {
                // ReadCFG the next lines until a line that doesn't match the format "f(fromState, symbol) = toState" is encountered
                while (std::getline(file, line) && !line.empty()) {
                    // Trim the trailing spaces
                    line.erase(line.find_last_not_of(" \n\r\t") + 1);

                    // The line should be in the format "f(fromState, symbol) = toState"
                    // Use a regular expression to parse this format
                    std::regex re(R"(f\((\d+), (.+)\) = (\d+))");
                    std::smatch match;
                    if (std::regex_search(line, match, re) && match.size() > 3) {
                        // Extract the fromState, symbol, and toState
                        int fromStateID = std::stoi(match.str(1));
                        std::string symbol = match.str(2);
                        int toStateID = std::stoi(match.str(3));

                        // Get the fromState and toState pointers
                        std::shared_ptr<State> fromState = automaton->get_state_using_id(fromStateID);
                        std::shared_ptr<State> toState = automaton->get_state_using_id(toStateID);

                        // Add the transition to the automaton
                        Types::state_set_t toStates = {toState};
                        automaton->add_transitions(fromState, symbol, toStates);
                    } else {
                        // If the line doesn't match the format "f(fromState, symbol) = toState", stop reading the transition function
                        break;
                    }
                }
            }

            if (line.substr(0, 6) == "Regex:") {
                // Remove the "Regex: " part from the line
                line.erase(0, 7);

                // Set the regex in the automaton
                automaton->set_regex(line);
            }

            if (line.substr(0, 7) == "Tokens:") {
                // ReadCFG the next lines until an empty line is encountered
                while (std::getline(file, line) && !line.empty()) {
                    // The line should be in the format "[number]: token1 token2 ..."
                    // Use a regular expression to parse this format
                    std::regex re(R"(\[(\d+)\]: (.+))");
                    std::smatch match;
                    if (std::regex_search(line, match, re) && match.size() > 2) {
                        // Extract the id and the tokens
                        int id = std::stoi(match.str(1));
                        std::string tokens_str = match.str(2);

                        // Split the tokens_str into individual tokens
                        std::istringstream iss(tokens_str);
                        std::vector<std::string> vector_tokens((std::istream_iterator<std::string>(iss)),
                                                               std::istream_iterator<std::string>());

                        // Add each token to the automaton
                        std::shared_ptr<State> state_ptr = automaton->get_state_using_id(id);
                        state_ptr->setToken(*vector_tokens.begin());
                        state_ptr->setAccepting(true);
                        Types::string_set_t ts = {};
                        ts.insert(vector_tokens.begin(), vector_tokens.end());
                        automaton->add_tokens(state_ptr, ts);
                    }
                }
            }

        }
        file.close();
//        automaton->give_new_ids_all();
        return automaton;

    } else {
        std::cout << "Unable to open file";
        return nullptr;
    }
}

std::vector<std::vector<std::shared_ptr<State>>> Automaton::matrix_representation() {
    // Get the number of states
    int num_states = (int) this->states.size();

    // Initialize the matrix with null pointers
    std::vector<std::vector<std::shared_ptr<State>>> matrix(num_states,
                                                            std::vector<std::shared_ptr<State>>(
                                                                    this->alphabets.size(), nullptr));

    // Create a sorted vector of symbols
    std::vector<std::string> symbols(this->alphabets.begin(), this->alphabets.end());
    std::sort(symbols.begin(), symbols.end());

    // Fill the matrix with the transitions
    for (const auto &entry: this->transitions) {
        int from_id = entry.first.first->getId();
        std::string symbol = entry.first.second;

        // Find the index of the symbol in the sorted vector
        auto it = std::find(symbols.begin(), symbols.end(), symbol);
        if (it == symbols.end()) {
            continue;  // Skip if the symbol is not found
        }
        int index = (int) std::distance(symbols.begin(), it);

        // For each destination state, add it to the matrix
        for (const auto &state: entry.second) {
            matrix[from_id][index] = state;
            // Assuming deterministic transitions, break after the first state
            break;
        }
    }

    return matrix;
}

