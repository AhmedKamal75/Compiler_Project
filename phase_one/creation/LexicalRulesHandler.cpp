#include "LexicalRulesHandler.h"
#include "../automaton/Utilities.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>


LexicalRulesHandler::LexicalRulesHandler() = default;

void LexicalRulesHandler::export_priorities(const std::map<std::string, int> &p, const std::string &filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto &pair: p) {
            file << pair.first << " " << pair.second << "\n";
        }
        file.close();
    } else {
        std::cout << "Unable to open file for writing.\n";
    }
}

std::map<std::string, int> LexicalRulesHandler::import_priorities(const std::string &filename) {
    std::map<std::string, int> p;
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string key;
        int value;
        while (file >> key >> value) {
            p[key] = value;
        }
        file.close();
    } else {
        std::cout << "Unable to open file for reading.\n";
    }
    return p;
}

std::map<std::string, int> LexicalRulesHandler::get_priorities() {
    std::map<std::string, int> priorities_map{};
    int size = (int) this->priorities.size();
    for (const std::string &token: this->priorities) {
        priorities_map[token] = size--;
    }
    return priorities_map;
}

std::shared_ptr<Automaton> LexicalRulesHandler::export_automata(std::vector<std::shared_ptr<Automaton>> &automata,
                                                                const std::string &output_file_path) {
    std::shared_ptr<Automaton> nfa = Utilities::unionAutomataSet(automata);
    std::shared_ptr<Automaton> dfa = conversions.convertToDFA(nfa, true);
    dfa->export_to_file(output_file_path);
    /*TODO: i don't know why yet, but you shouldn't minimize the dfa as it will lose details about the
     * tokens identification */
//    std::shared_ptr<Automaton> minimized_dfa = conversions.minimizeDFA(dfa);
//    minimized_dfa->export_to_file(output_file_path);
    return dfa;
}

[[maybe_unused]] std::unordered_map<std::string, std::shared_ptr<Automaton>>
LexicalRulesHandler::handleFile(const std::string &filename) {
    this->priorities = {};
    std::unordered_map<std::string, std::shared_ptr<Automaton>> automata{};
    std::vector<std::string> regex_tokens{};
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
                a->set_token(keyword);
                automata[keyword] = a;
                this->priorities.push_back(keyword);
            }
        } else if (line.front() == '[') {
            // These are punctuation
            std::istringstream ss(s);
            std::string punctuation;
            while (ss >> punctuation) {
                std::shared_ptr<Automaton> a = toAutomaton.regex_to_minimized_dfa(punctuation, epsilonSymbol);
                // TODO: remove the backslash of the punctuation tokens
                if (punctuation.size() > 1) {
                    if (punctuation.at(0) == '\\') {
                        punctuation = punctuation.substr(1, punctuation.size());
                    }
                }
                a->set_token(punctuation);
                // TODO: see if you want to replace the next line with {automata["punctuation"].append(a);}
                automata[punctuation] = a;
                this->priorities.push_back(punctuation);
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
            } else {
                a->set_token(name);
                automata[name] = a;
            }
            this->priorities.push_back(name);
        } else if (line.find('=') != std::string::npos) {
            // This is a regular definition
            std::string name = line.substr(0, line.find('='));
            this->trim(name);
            std::string regex = line.substr(line.find('=') + 1);
            this->trim(regex);
            regex.erase(remove_if(regex.begin(), regex.end(), isspace), regex.end());
            std::shared_ptr<Automaton> a = toAutomaton.regex_to_minimized_dfa(regex, epsilonSymbol);
            a->set_token(name);
            automata[name] = a;
            this->priorities.push_back(name);
            regex_tokens.push_back(name);
        }
    }
    file.close();

    // After the while loop, process the pairs in the backlog
    this->handle_backlog(automata, backlog, regex_tokens);


    return automata;
}

void LexicalRulesHandler::handle_backlog(std::unordered_map<std::string, std::shared_ptr<Automaton>> &automata,
                                         std::queue<std::pair<std::string, std::string>> &backlog,
                                         const std::vector<std::string> &regex_tokens) {
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
            a->set_token(name);
            automata[name] = a;
        }
    }
    // remove \ at the start of every token

    // After processing the backlog, remove the automata for the regex tokens
    for (const std::string &token: regex_tokens) {
        automata.erase(token);
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
