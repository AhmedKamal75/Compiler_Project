#ifndef COMPILER_PROJECT_LEXICALRULESHANDLER_H
#define COMPILER_PROJECT_LEXICALRULESHANDLER_H


#include <unordered_map>
#include <memory>
#include <string>
#include <queue>
#include <map>
#include "../automaton/Automaton.h"
#include "ToAutomaton.h"

class LexicalRulesHandler {
public:
    LexicalRulesHandler();

    static void export_priorities(const std::map<std::string, int> &p, const std::string &filename);

    static std::map<std::string, int> import_priorities(const std::string &filename);

    // call this method only after you have called handleFile
    std::map<std::string, int> get_priorities();

    // will make a union on the automata and then output them to the output file path
    std::shared_ptr<Automaton>
    export_automata(std::vector<std::shared_ptr<Automaton>> &automata, const std::string &output_file_path);

    [[maybe_unused]] std::unordered_map<std::string, std::shared_ptr<Automaton>>
    handleFile(const std::string &filename);


private:
    std::string epsilonSymbol = "\\L";
    ToAutomaton toAutomaton;
    Conversions conversions;
    std::vector<std::string> priorities{};
    std::unordered_map<std::string, int> attempts{};
    const int MAX_ATTEMPTS = 100;


    void handle_backlog(std::unordered_map<std::string, std::shared_ptr<Automaton>> &automata,
                        std::queue<std::pair<std::string, std::string>> &backlog,
                        const std::vector<std::string> &regex_tokens);

    // trim from start (in place)
    void ltrim(std::string &s);

    // trim from end (in place)
    void rtrim(std::string &s);

    // trim from both ends (in place)
    void trim(std::string &s);

};


#endif