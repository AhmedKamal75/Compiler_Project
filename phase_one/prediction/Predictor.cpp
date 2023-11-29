#include <fstream>
#include <utility>
#include <vector>
#include <algorithm>
#include <sstream>
#include <stack>
#include <limits>
#include "Predictor.h"

Predictor::Predictor(std::shared_ptr<Automaton> &a, const std::map<std::string, int> &priorities,
                     const std::string &program_text) {
    this->index = 0;
    this->program = read_file(program_text);
    this->automaton = a;
    this->priorities = priorities;

    this->automaton->give_new_ids_all();
    // Generate the matrix once and store it as a member variable
    this->matrix = this->automaton->matrix_representation();
    //
    find_dead_states();
    // Create a sorted vector of symbols
    std::vector<std::string> sorted_alphabets(this->automaton->get_alphabets().begin(),
                                              this->automaton->get_alphabets().end());
    std::sort(sorted_alphabets.begin(), sorted_alphabets.end());
    this->symbols = sorted_alphabets;
}

// In read_file
std::string Predictor::read_file(const std::string &file_name) {
    std::ifstream inFile(file_name);
    if (!inFile) {
        throw std::runtime_error("Failed to open file: " + file_name);
    }

    // Read the entire file into a string stream
    std::stringstream ss;
    ss << inFile.rdbuf();

    // Convert the string stream to a string
    std::string contents = ss.str();

    inFile.close();
    return contents;
}


std::pair<std::string, std::string> Predictor::get_next_token() {
    std::shared_ptr<State> current_state = this->automaton->get_start();
    std::stack<std::pair<std::string, std::string>> token_stack{};
    std::string token{};
    bool exit = false;
    while (this->index < this->program.size()) {
        char c = this->program[this->index];
        if (std::isspace(static_cast<unsigned char>(c))) {
            index++;
            break;
        }
        // character is appended to the end of the token as we now know that it isn't a space character or end on input.
        token += std::string(1, c);
        std::shared_ptr<State> next_state = this->get_next_state(current_state, std::string(1, c));
        if (next_state == nullptr) {
            index++;
            break;
        }
        if (next_state->getIsAccepting()) {
            Types::string_set_t tokens{};
            for (const std::pair<std::shared_ptr<State>, Types::string_set_t> entry: this->automaton->get_tokens()) {
                if (entry.first->getId() == next_state->getId()) {
                    tokens = entry.second;
                    break;
                }
            }
            int max_priority = std::numeric_limits<int>::min();
            std::string taken_token{};
            for (const std::string &t: tokens) {
                int new_priority = this->priorities.at(t);
                if (max_priority < new_priority) {
                    max_priority = new_priority;
                    taken_token = t;
                }
            }
//            token += std::string(1, c);
            token_stack.emplace(taken_token, token);
        }
        for (const std::shared_ptr<State> &dead_state_ptr: this->dead_states) {
            if (next_state->getId() == dead_state_ptr->getId()) {
                exit = true;
                break;
            }
        }
        if (exit) {
            break;
        }
        current_state = next_state;
        this->index++;
    }
    if (token_stack.empty()) {
        if (this->index < this->program.size()) {
            return this->get_next_token();
        }
        return std::make_pair("", "");
    }
    return token_stack.top();
}

void Predictor::find_dead_states() {
    // Assume transition_matrix is a 2D vector that represents the state transitions
//    std::vector<std::vector<std::shared_ptr<State>>> transition_matrix = this->automaton->matrix_representation();

    // Iterate over all states in the automaton
    for (int i = 0; i < this->automaton->get_states().size(); i++) {
        std::shared_ptr<State> state_ptr = this->automaton->get_state_using_id(i);

        // Check if all outgoing transitions lead to the same state
        bool is_dead_state = true;
        for (int j = 0; j < this->automaton->get_alphabets().size(); j++) {
            std::shared_ptr<State> next_state_ptr = this->matrix[i][j];
            if (next_state_ptr != state_ptr) {
                is_dead_state = false;
                break;
            }
        }

        // If all transitions lead to the same state, it's a dead state
        if (is_dead_state) { // only one dead state can be found
            this->dead_states.insert(state_ptr);
//            break;
        }
    }
}

std::shared_ptr<State>
Predictor::get_next_state(const std::shared_ptr<State> &current_state, const std::string &symbol) {
    // Find the index of the symbol in the sorted vector
    auto it = std::find(this->symbols.begin(), this->symbols.end(), symbol);
    if (it == symbols.end()) {
        return nullptr;  // Return null if the symbol is not found
    }
    int symbol_index = (int) std::distance(symbols.begin(), it);

    // Get the current state id
    int state_id = current_state->getId();

    // Return the next state from the matrix
    return matrix[state_id][symbol_index];
}


