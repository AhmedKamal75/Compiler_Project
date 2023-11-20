#include "LexicalRulesHandler.h"
#include <fstream>
#include <sstream>
#include <algorithm>


LexicalRulesHandler::LexicalRulesHandler() = default;

[[maybe_unused]] std::unordered_map<std::string, std::shared_ptr<Automaton>>
LexicalRulesHandler::handleFile(const std::string &filename) {
    std::unordered_map<std::string, std::shared_ptr<Automaton>> automata{};
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
            std::string rd = line.substr(line.find(':') + 1);
            std::shared_ptr<Automaton> a = toAutomaton.regular_definition_to_minimized_dfa(rd, automata, epsilonSymbol);
            a->set_regex(a->get_token());
            a->set_token(name);
            automata[name] = a;
        } else if (line.find('=') != std::string::npos) {
            // This is a regular definition
            std::string name = line.substr(0, line.find('='));
            std::string regex = line.substr(line.find('=') + 1);
            regex.erase(remove_if(regex.begin(), regex.end(), isspace), regex.end());
            std::shared_ptr<Automaton> a = toAutomaton.regex_to_minimized_dfa(regex, epsilonSymbol);
            a->set_regex(a->get_token());
            a->set_token(name);
            automata[name] = a;
        }
    }
    file.close();
    return automata;
}
