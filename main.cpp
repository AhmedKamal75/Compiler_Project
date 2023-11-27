#include <iostream>
#include <set>
#include <list>
#include <fstream>
#include "phase_one/automaton/Automaton.h"
#include "phase_one/automaton/Utilities.h"
#include "phase_one/automaton/Conversions.h"
#include "phase_one/creation/InfixToPostfix.h"
#include "phase_one/creation/ToAutomaton.h"
#include "phase_one/creation/LexicalRulesHandler.h"

Conversions conversions;
InfixToPostfix infixToPostfix;
ToAutomaton toAutomaton;
LexicalRulesHandler handler;

// all these functions are for testing and debugging the code while I writing it.

void input_handler();

void to_automaton_test();

void infix_to_postfix_test();

void automaton_test();


std::unordered_map<std::string, std::shared_ptr<Automaton>> getAutomata();


int main() {
//    input_handler();
    std::string input_file_path = R"(/home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/inputs/temp_file.txt)";
    std::string data_directory_path = R"(/home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/data/)";
    std::unordered_map<std::string, std::shared_ptr<Automaton>> automata = handler.handleFile(input_file_path);
    std::vector<std::shared_ptr<Automaton>> vector_automata{};
    for (const auto& pair:automata){
        vector_automata.push_back(pair.second);
    }
    handler.export_automata(vector_automata,data_directory_path);

    return 0;
}

void input_handler() {
    std::string filename = R"(/home/ahmed-kamal/Projects/cpp_projects/Compiler_Project/inputs/temp_file.txt)";
    std::unordered_map<std::string, std::shared_ptr<Automaton>> automata = handler.handleFile(filename);
    std::cout << "################# after handing #################" << std::endl;
    std::cout << filename << std::endl;
    for (const auto &pair: automata) {
        std::cout << "token: " << pair.first << std::endl;
        std::cout << "Automaton: " << pair.second->to_string_transition_table() << std::endl;
    }
    std::cout << "################# file is read #################" << std::endl;
}

void to_automaton_test() {
    std::string regex0 = R"((a*)(a|b)abc)";
    std::string regex1 = R"(hello)";
    // take too much time to compute
    std::string regex2 = R"(((a-z)|(A-Z))((a-z)|(A-Z)|(0-9)|(\.))*@((a-z)|(A-Z)|(0-9))+\.((a-z)|(A-Z))+)";
    const std::string &regex = regex2;
    std::shared_ptr<Automaton> automaton = toAutomaton.regex_to_minimized_dfa(regex, "\\L");

    std::cout << "regex: " << regex << std::endl;
    std::cout << automaton->to_string() << std::endl;
}


void infix_to_postfix_test() {
    std::string regex1 = "(hello|hi)+(and|or)(welcome|out!)(infinite_loop)*";
    std::string regex2 = R"(^(int|float|boolean|if|else|while)?\s*(([a-zA-Z][a-zA-Z0-9]*)\s*(=)\s*)?(([0-9]+(\.[0-9]+)?(E[0-9]+)?)|([a-zA-Z][a-zA-Z0-9]*))\s*([\+\-\*\/])?\s*(([0-9]+(\.[0-9]+)?(E[0-9]+)?)|([a-zA-Z][a-zA-Z0-9]*))?\s*((==|!=|>|>=|<|<=)\s*(([0-9]+(\.[0-9]+)?(E[0-9]+)?)|([a-zA-Z][a-zA-Z0-9]*)))?\s*(;)?$)";
    std::string regex3 = R"(((a-z)|(A-Z))((a-z)|(A-Z)|(0-9)|(\.))*@((a-z)|(A-Z)|(0-9))+\.((a-z)|(A-Z))+)";
    std::string concat_explicit = infixToPostfix.concat_explicit(regex3);
    std::string postfix = infixToPostfix.regex_infix_to_postfix(regex3);
    std::string evaluated_regex = infixToPostfix.regex_evaluate_postfix(postfix);
    std::cout << "regex    : " << regex3 << std::endl;
    std::cout << "concat   : " << concat_explicit << std::endl;
    std::cout << "postfix  : " << postfix << std::endl;
    std::cout << "evaluated: " << evaluated_regex << std::endl;

}


void automaton_test() {
    std::unordered_map<std::string, std::shared_ptr<Automaton>> automata = getAutomata();
    std::shared_ptr<Automaton> aP = Utilities::positiveClosure(automata["a"]);
    std::shared_ptr<Automaton> bK = Utilities::kleeneClosure(automata["b"]);
    std::shared_ptr<Automaton> aUb = Utilities::unionAutomata(automata["a"], automata["b"]);
    std::shared_ptr<Automaton> aPUbK = Utilities::unionAutomata(aP, bK);
    std::shared_ptr<Automaton> aPUbKK = Utilities::kleeneClosure(aPUbK);
    std::shared_ptr<Automaton> aPUbKKa = Utilities::concatAutomaton(aPUbKK, automata["a"]);
    std::shared_ptr<Automaton> aPbK = Utilities::concatAutomaton(aP, bK);
    std::shared_ptr<Automaton> aPbKaPUbK = Utilities::concatAutomaton(aPbK, aPUbK);

    std::shared_ptr<Automaton> nfa = Utilities::copyAutomaton(aPbKaPUbK);
    std::cout << "nfa:" << std::endl;
    std::cout << nfa->to_string() << std::endl;
    std::shared_ptr<Automaton> dfa = conversions.convertToDFA(nfa);
    std::cout << "dfa:" << std::endl;
    std::cout << dfa->to_string() << std::endl;
    std::shared_ptr<Automaton> minDFA = conversions.minimizeDFA(dfa);
    std::cout << "min dfa:" << std::endl;
    std::cout << minDFA->to_string() << std::endl;

}


std::unordered_map<std::string, std::shared_ptr<Automaton>> getAutomata() {
    std::list<std::string> alphabets;
    for (char letter = 'a'; letter <= 'z'; letter++) {
        alphabets.emplace_back(1, letter);
    }
    std::unordered_map<std::string, std::shared_ptr<Automaton>> automatas;
    for (const std::string &alphabet: alphabets) {
        automatas[alphabet] = std::make_shared<Automaton>(alphabet, "", "");
    }
    return automatas;
}