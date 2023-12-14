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

    find_dead_states();

}

// In read_file. i.e. reading the program
std::string Predictor::read_file(const std::string &file_name) {
    std::ifstream inFile(file_name);
    if (!inFile) {
        throw std::runtime_error("Failed to open file: " + file_name);
    }

    // ReadCFG the entire file into a string stream
    std::stringstream ss;
    ss << inFile.rdbuf();

    // Convert the string stream to a string
    std::string contents = ss.str();

    inFile.close();
    return contents;
}


std::pair<std::string, std::string> Predictor::next_token() {
    std::shared_ptr<State> current_state_ptr = this->automaton->get_start();
    std::stack<std::pair<std::string, std::string>> token_stack{};
    std::string token{};
    while (this->index < this->program.size()) {
        char c = this->program[this->index];
        if (std::isspace(static_cast<unsigned char>(c))) {
            index++;
            break;
        }
        if (this->automaton->get_alphabets().find(std::string(1, c)) == this->automaton->get_alphabets().end()) {
            // this character isn't in the allowed alphabets
            std::cout << "\033[1;31mError: Invalid input\033[0m" << ", ignoring character:'" << c << "'" << std::endl;
            index++;
            continue;
        }
        // character is appended to the end of the token as we now know that it isn't a space character or end on input.
        token += std::string(1, c);
        std::shared_ptr<State> next_state_ptr = *this->automaton->get_next_states(current_state_ptr,
                                                                                  std::string(1, c)).begin();
        // If next state is dead state
        if (this->dead_states.find(next_state_ptr) != this->dead_states.end()) {
            break;
        }

        // If next state is accepting state
        if (this->automaton->is_accepting_state(next_state_ptr)) {
            Types::string_set_t tokens = this->automaton->get_tokens(next_state_ptr);
            int max_priority = std::numeric_limits<int>::min();
            std::string chosen_token{};
            for (const std::string &t: tokens) {
                int new_priority = this->priorities.at(t);
                if (max_priority < new_priority) {
                    max_priority = new_priority;
                    chosen_token = t;
                }
            }
            token_stack.emplace(chosen_token, token);
        }
        current_state_ptr = next_state_ptr;
        this->index++;
    }
    if (token_stack.empty()) {
        if (this->index < this->program.size()) {
            return this->next_token();
        }
        // done with the program
        return std::make_pair("", "");
    }
    return token_stack.top();
}

void Predictor::find_dead_states() {
    // Iterate over all states in the automaton
    for (const std::shared_ptr<State> &state_ptr: this->automaton->get_states()) {
        // Dead state can't be a start state or an accepting state.
        if (!this->automaton->is_accepting_state(state_ptr) && this->automaton->get_start() != state_ptr) {
            // Check if all outgoing transitions lead to the same state
            bool is_dead_state = true;
            for (const std::string &symbol: this->automaton->get_alphabets()) {
                std::shared_ptr<State> next_state_ptr = *this->automaton->get_next_states(state_ptr, symbol).begin();
                if (next_state_ptr != state_ptr) {
                    is_dead_state = false;
                    break;
                }
            }
            // If all transitions lead to the same state, it's a dead state
            if (is_dead_state) {
                this->dead_states.insert(state_ptr);
            }
        }
    }
}