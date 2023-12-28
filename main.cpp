#include <iostream>
#include <algorithm>
#include <list>
#include <fstream>
#include "phase_one/automaton/Automaton.h"
#include "phase_one/automaton/Conversions.h"
#include "phase_one/creation/InfixToPostfix.h"
#include "phase_one/creation/ToAutomaton.h"
#include "phase_one/creation/LexicalRulesHandler.h"
#include "phase_one/prediction/Predictor.h"
#include "phase_two/Table.h"
#include "phase_two/Parser.h"

LexicalRulesHandler handler;

std::string final_dfa_file_name = "final_dfa.txt";
std::string tokens_priorities_name = "tokens_priorities.txt";
std::string parsing_tree_name = "parsing_tree.txt";
std::string parsing_table_name = "parsing_table.txt";
std::string parsing_output_name = "parsing_output.txt";

std::shared_ptr<Automaton>
init(const std::string &input_file_path, const std::string &final_dfa_path, const std::string &tokens_priorities);

void export_token_list_to_file(const std::vector<std::pair<std::string, std::string>> &token_list,
                               const std::string &filename);

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0]
                  << " <output_token_path> <input_program_path> <input_rules_path> <input_cfg_path>\n";// <data_directory_path>\n";
        return 1;
    }
    // ############################## create export lexical data ##############################

    // files
    std::string data_directory_path = R"(../data/)";
    std::string output_token_path = argv[1];
    std::string input_program_path = argv[2];
    std::string input_rules_path = argv[3];
    std::string input_cfg_path = argv[4];
    std::string final_dfa_path = data_directory_path + final_dfa_file_name;
    std::string tokens_priorities_path = data_directory_path + tokens_priorities_name;
    std::string parsing_tree_path = data_directory_path + parsing_tree_name;
    std::string parsing_table_path = data_directory_path + parsing_table_name;
    std::string parsing_output_path = data_directory_path + parsing_output_name;



    // init the DFA of rules and export its detains and priorities to ../data/final_dfa.txt and ../data/tokens_priorities.txt
    std::shared_ptr<Automaton> final_dfa = init(input_rules_path, final_dfa_path, tokens_priorities_path);

    // ############################## load lexical data ##############################

    // import final automaton (NFA form)
    std::shared_ptr<Automaton> loaded_automaton = Automaton::import_from_file(final_dfa_path);
    // import tokens priorities
    std::map<std::string, int> priorities = LexicalRulesHandler::import_priorities(tokens_priorities_path);

    // ############################## predicting tokens and parsing ##############################
    if (true) {
        std::shared_ptr<Predictor> tokenizer = std::make_shared<Predictor>(loaded_automaton, priorities,
                                                                           input_program_path);
        std::shared_ptr<Table> parsing_table = std::make_shared<Table>(input_cfg_path, parsing_table_path);
        std::shared_ptr<Parser> parser = std::make_shared<Parser>(parsing_table);
        parser->parse(tokenizer, parsing_tree_path, parsing_output_path);
    }
    else {
        // prediction
        Predictor predictor(loaded_automaton, priorities, input_program_path);

        std::vector<std::pair<std::string, std::string >> token_list{};
        std::vector<std::string> tokens{};
        // ############################## predict tokens ##############################
        std::cout << "############################ Tokens ############################" << '\n';
        while (true) {
            std::pair<std::string, std::string> entry = predictor.next_token();
            if (entry.first.empty() && entry.second.empty()) {
                // if output is ("","") then we reached the end
                break;
            }
            std::cout << entry.first << ": " << entry.second << std::endl;
            token_list.push_back(entry);
            tokens.push_back(entry.first);
        }
        export_token_list_to_file(token_list, output_token_path);
        std::cout << "########################################################" << '\n';

        // ############################## load parser data ##############################
        std::shared_ptr<Table> table = std::make_shared<Table>(input_cfg_path, parsing_table_path);
        std::shared_ptr<Parser> parser = std::make_shared<Parser>(table);
        parser->parse(tokens);

        // ############################## end ##############################
    }


    return 0;
}

std::shared_ptr<Automaton>
init(const std::string &input_file_path, const std::string &final_dfa_path, const std::string &tokens_priorities) {
    // ############################## create export automata data ##############################
    // map of a token and the minimized DFA that can define it.
    std::unordered_map<std::string, std::shared_ptr<Automaton>> automata = handler.handleFile(input_file_path);
    // contain those DFAs into a vector
    std::vector<std::shared_ptr<Automaton>> vector_automata{};
    for (const auto &pair: automata) {
        vector_automata.push_back(pair.second);
    }
    // export final automaton (union all: DFAs --union--> NFA --subset construction--> DFA (no minimization))
    std::shared_ptr<Automaton> final_dfa = handler.export_automata(vector_automata, final_dfa_path);
    // export the priorities of tokens
    LexicalRulesHandler::export_priorities(handler.get_priorities(), tokens_priorities);
    return final_dfa;
}


void export_token_list_to_file(const std::vector<std::pair<std::string, std::string>> &token_list,
                               const std::string &filename) {
    std::ofstream outfile(filename);
    if (!outfile) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    for (const auto &pair: token_list) {
        outfile << pair.first << '\n';
    }

    outfile.close();
}

