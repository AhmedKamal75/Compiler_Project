#include <iostream>
#include <algorithm>
#include <set>
#include <list>
#include <fstream>
#include "phase_one/automaton/Automaton.h"
#include "phase_one/automaton/Conversions.h"
#include "phase_one/creation/InfixToPostfix.h"
#include "phase_one/creation/ToAutomaton.h"
#include "phase_one/creation/LexicalRulesHandler.h"
#include "phase_one/prediction/Predictor.h"

LexicalRulesHandler handler;

void create(const std::string &input_file_path, const std::string &data_directory_path);

void export_token_list_to_file(const std::vector<std::pair<std::string, std::string>> &token_list,
                               const std::string &filename);

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0]
                  << " <output_token_path> <input_program_path> <input_rules_path>\n";// <data_directory_path>\n";
        return 1;
    }
//    std::string output_token_path = R"(../output/token_list.txt)";
//    std::string input_program_path = R"(../inputs/temp_program.txt)";
//    std::string input_rules_path = R"(../inputs/temp_rules.txt)";
    std::string data_directory_path = R"(../data/)";

    std::string output_token_path = argv[1];
    std::string input_program_path = argv[2];
    std::string input_rules_path = argv[3];
//    std::string data_directory_path = argv[4];


    // create the DFA of rules and export its detains and priorities to ../data/final.txt and ../data/priorities.txt
    create(input_rules_path, data_directory_path);

    // import final automaton (NFA form)
    std::shared_ptr<Automaton> loaded_automaton = Automaton::import_from_file(data_directory_path + "final.txt");
    // import tokens priorities
    std::map<std::string, int> priorities = LexicalRulesHandler::import_priorities(
            data_directory_path + "priorities.txt");

    // prediction
    Predictor predictor(loaded_automaton, priorities, input_program_path);
    std::vector<std::pair<std::string, std::string >> token_list{};
    while (true) {
        std::pair<std::string, std::string> entry = predictor.get_next_token();
        if (entry.first.empty() && entry.second.empty()) {
            // if output is ("","") then we reached the end
            break;
        }
        std::cout << entry.first << ": " << entry.second << std::endl;
        token_list.push_back(entry);
    }

    export_token_list_to_file(token_list, output_token_path);
    return 0;
}

void create(const std::string &input_file_path, const std::string &data_directory_path) {
    // generate automata
    // map of a token and the minimized DFA that can define it.
    std::unordered_map<std::string, std::shared_ptr<Automaton>> automata = handler.handleFile(input_file_path);
    // contain those DFAs into a vector
    std::vector<std::shared_ptr<Automaton>> vector_automata{};
    for (const auto &pair: automata) {
        vector_automata.push_back(pair.second);
    }
    // export final automaton (union all: DFAs --union--> NFA --subset construction--> DFA (no minimization))
    handler.export_automata(vector_automata, data_directory_path + "final.txt");
    // export the priorities of tokens
    LexicalRulesHandler::export_priorities(handler.get_priorities(), data_directory_path + "priorities.txt");
}


void export_token_list_to_file(const std::vector<std::pair<std::string, std::string>> &token_list,
                               const std::string &filename) {
    std::ofstream outfile(filename);
    if (!outfile) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    for (const auto &pair: token_list) {
        outfile << pair.first << " " << pair.second << "\n";
    }

    outfile.close();
}

