#ifndef COMPILER_PROJECT_STATE_H
#define COMPILER_PROJECT_STATE_H


#include <string>
#include <functional>

class State {
public:
    State();

    State(int id, bool isAccepting, const std::string &token);

    [[nodiscard]] State copy() const;

    [[nodiscard]] std::string getToken() const;

    void setToken(const std::string &t);

    [[nodiscard]] int getId() const;

    void setId(int value);

    [[nodiscard]] bool getIsAccepting() const;

    void setAccepting(bool value);

    [[nodiscard]] std::string toString() const;

    [[nodiscard]] std::string toStringFull() const;

    bool operator==(State &other);

    bool operator!=(State &other);

    bool operator<(const State &other) const;

    bool operator>(const State &other) const;

    [[nodiscard]] std::size_t hash() const;

private:
    int id{};
    bool isAccepting{};
    std::string token{};
};

namespace std {
    template<>
    struct hash<State> {
        std::size_t operator()(State &state) {
            return state.hash();
        }
    };
}


#endif //COMPILER_PROJECT_STATE_H
