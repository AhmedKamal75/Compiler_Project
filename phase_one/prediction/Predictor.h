#ifndef COMPILER_PROJECT_PREDICTOR_H
#define COMPILER_PROJECT_PREDICTOR_H


#include <map>
#include "../automaton/Automaton.h"

class Predictor {
public:
    Predictor(std::shared_ptr<Automaton> &a, const std::map<std::string, int> &priorities,
              const std::string &program_path);

    std::pair<std::string, std::string> next_token();

    static std::string read_file(const std::string &file_name);

    void find_dead_states();



private:
    std::shared_ptr<Automaton> automaton{};
    std::vector<std::vector<std::shared_ptr<State>>> matrix{};
    std::map<std::string, int> priorities{};
    std::vector<std::string> symbols{};
    Types::state_set_t dead_states{};
    std::string program{};
    int index{};
};


#endif
