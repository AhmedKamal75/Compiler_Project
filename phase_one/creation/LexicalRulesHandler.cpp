#include "LexicalRulesHandler.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>


LexicalRulesHandler::LexicalRulesHandler() = default;

[[maybe_unused]] std::unordered_map<std::string, std::shared_ptr<Automaton>>
LexicalRulesHandler::handleFile(const std::string &filename) {
    std::unordered_map<std::string, std::shared_ptr<Automaton>> automata{};
    std::queue<std::pair<std::string, std::string>> backlog;
    std::ifstream file(filename);
    std::string line{};
    while (std::getline(file, line)) {
        line = line.substr(line.find_first_not_of(" \n\r\t"), std::string::npos);
        std::string non_terminal = line.substr(0, line.find_first_of(" \n\r\t"));
        bool is_regular_definition = non_terminal.back() == ':';

        std::string s = line.substr(1, line.length() - 2);
        if (line.front() == '{') { // done
            // These are keywords
            std::istringstream ss(s);
            std::string keyword;
            while (ss >> keyword) {
                std::shared_ptr<Automaton> a = toAutomaton.regex_to_minimized_dfa(keyword, epsilonSymbol);
                a->set_regex(a->get_token());
                a->set_token(keyword);
                automata[keyword] = a;
            }
        } else if (line.front() == '[') {
            // These are punctuation
            std::istringstream ss(s);
            std::string punctuation;
            while (ss >> punctuation) {
                std::shared_ptr<Automaton> a = toAutomaton.regex_to_minimized_dfa(punctuation, epsilonSymbol);
                a->set_regex(a->get_token());
                a->set_token(punctuation);
                automata[punctuation] = a;
            }
        } else if (is_regular_definition) {
            // This is a regular definition
            std::string name = line.substr(0, line.find(':'));
            this->trim(name);
            std::string rd = line.substr(line.find(':') + 1);
            this->trim(rd);
            std::shared_ptr<Automaton> a = toAutomaton.regular_definition_to_minimized_dfa(rd, automata, epsilonSymbol);
            if (a == nullptr) {
                backlog.emplace(name, rd);
            } else
//            if (a != nullptr)
            {
                a->set_regex(a->get_token());
                a->set_token(name);
                automata[name] = a;
            }
        } else if (line.find('=') != std::string::npos) {
            // This is a regular definition
            std::string name = line.substr(0, line.find('='));
            this->trim(name);
            std::string regex = line.substr(line.find('=') + 1);
            this->trim(regex);
            regex.erase(remove_if(regex.begin(), regex.end(), isspace), regex.end());
            std::shared_ptr<Automaton> a = toAutomaton.regex_to_minimized_dfa(regex, epsilonSymbol);
            a->set_regex(a->get_token());
            a->set_token(name);


            automata[name] = a;
        }
    }
    file.close();

    // After the while loop, process the pairs in the backlog
    this->handle_backlog(automata, backlog);


    return automata;
}

void LexicalRulesHandler::handle_backlog(std::unordered_map<std::string, std::shared_ptr<Automaton>> &automata,
                                         std::queue<std::pair<std::string, std::string>> &backlog) {
    while (!backlog.empty()) {
        std::pair<std::string, std::string> pair = backlog.front();
        backlog.pop();

        // This is a regular definition
        std::string name = pair.first;
        std::string rd = pair.second;
        if (automata.find(name) != automata.end()) {
            continue;
        }
        std::shared_ptr<Automaton> a = toAutomaton.regular_definition_to_minimized_dfa(rd, automata, epsilonSymbol);
        if (a == nullptr) {
            if (attempts[name]++ < MAX_ATTEMPTS) {
                backlog.emplace(name, rd);
            }
        } else {
            a->set_regex(a->get_token());
            a->set_token(name);
            automata[name] = a;
        }
    }
}

// trim from start (in place)
void LexicalRulesHandler::ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](char &ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
void LexicalRulesHandler::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](char &ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
void LexicalRulesHandler::trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}
