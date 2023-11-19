#ifndef COMPILER_PROJECT_STATE_H
#define COMPILER_PROJECT_STATE_H


#include <string>

/**
 * This class represents a state in an automaton.
 * A state is a basic unit in an automaton and can be accepting or non-accepting.
 */
class State {
public:
    /**
     * Default constructor.
     */
    State();

    /**
     * Constructor that initializes the state with an id, a boolean indicating if it's accepting, and a token.
     */
    State(int id, bool isAccepting, const std::string &token);

    /**
     * Returns a copy of the state.
     */
    [[nodiscard]] State copy() const;

    /**
     * Returns the token associated with the state.
     */
    [[nodiscard]] std::string getToken() const;

    /**
     * Sets the token associated with the state.
     */
    void setToken(const std::string &t);

    /**
     * Returns the id of the state.
     */
    [[nodiscard]] int getId() const;

    /**
     * Sets the id of the state.
     */
    void setId(int value);

    /**
     * Returns whether the state is accepting.
     */
    [[nodiscard]] bool getIsAccepting() const;

    /**
     * Sets whether the state is accepting.
     */
    void setAccepting(bool value);

    /**
     * Returns a string representation of the state.
     */
    [[nodiscard]] std::string toString() const;

    /**
     * Returns a full string representation of the state.
     */
    [[nodiscard]] std::string toStringFull() const;

    /**
     * Overloads the equality operator.
     */
    bool operator==(const State &other) const;

    /**
     * Overloads the inequality operator.
     */
    bool operator!=(const State &other) const;

    /**
     * Overloads the less than operator.
     */
    bool operator<(const State &other) const;

    /**
     * Overloads the greater than operator.
     */
    bool operator>(const State &other) const;

    /**
     * Returns the hash of the state.
     */
    [[nodiscard]] std::size_t hash() const;

private:
    int id{};
    bool isAccepting{};
    std::string token{};
};


#endif //COMPILER_PROJECT_STATE_H
