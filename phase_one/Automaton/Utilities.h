#ifndef COMPILER_PROJECT_UTILITIES_H
#define COMPILER_PROJECT_UTILITIES_H


#include <set>
#include <map>
#include <string>
#include "State.h"
#include "Automaton.h"

class Utilities {
public:
    /**
     * Creates a deep copy of the given automaton.
     *
     * @param originalAutomaton the automaton to copy
     * @return a pointer to a deep copy of the original automaton
     *
     * remember to:
     * Automaton* automaton = utilities.copyAutomaton(originalAutomaton);
     * // Do something with automaton...
     * delete automaton;
     */
    static std::shared_ptr<Automaton> copyAutomaton(std::shared_ptr<Automaton> &originalAutomaton);


    /**
     * Combines two automata using the union operation.
     *
     * @param a1           the first automaton
     * @param a2           the second automaton
     * @param newTokenName the token name for the new automaton
     * @return a new automaton that is the union of a1 and a2
     *
     * remember to:
     * Automaton* automaton = utilities.unionAutomata(a1, a2, newToken);
     * // Do something with automaton...
     * delete automaton;
     */
    static std::shared_ptr<Automaton> unionAutomata(std::shared_ptr<Automaton> &a1, std::shared_ptr<Automaton> &a2, const std::string &newToken);

    /**
     * Combines two automata using the concatenation operation.
     *
     * @param a1           the first automaton
     * @param a2           the second automaton
     * @param newTokenName the token name for the new automaton
     * @return a new automaton that is the concatenation of a1 and a2
     *
     * remember to:
     * Automaton* automaton = utilities.concatenateAutomata(a1, a2, newToken);
     * // Do something with automaton...
     * delete automaton;
     * */
    static std::shared_ptr<Automaton> concatAutomaton(std::shared_ptr<Automaton> &a1, std::shared_ptr<Automaton> &a2, const std::string &newToken);

    /**
     * Creates the Kleene closure of an automaton.
     *
     * @param a            the automaton
     * @param newTokenName the token name for the new automaton
     * @return a new automaton that is the Kleene closure of a
     *
     * remember to:
     * Automaton* automaton = utilities.kleeneClosure(a, newToken);
     * // Do something with automaton...
     * delete automaton;
     * */
    static std::shared_ptr<Automaton> kleeneClosure(std::shared_ptr<Automaton> &a, const std::string &newToken);

    /**
     * Creates the positive closure of an automaton.
     *
     * @param a            the automaton
     * @param newTokenName the token name for the new automaton
     * @return a new automaton that is the positive closure of a
     *
     * remember to:
     * Automaton* automaton = utilities.positiveClosure(a1, newToken);
     * // Do something with automaton...
     * delete automaton;
     */
    static std::shared_ptr<Automaton> positiveClosure(std::shared_ptr<Automaton> &a, const std::string &newToken);

    /**
     * Creates a new automaton that represents the union of a set of automata.
     *
     * @param automata: A vector of Automaton objects. Each Automaton in the vector retains its own token.
     * @return A pointer to a new Automaton object that represents the union of all the Automaton objects in the input vector.
     *
     * This function creates a new Automaton object that represents the union of all the Automaton objects in the input vector.
     * Each Automaton in the vector retains its own token. The function first creates a new Automaton object and sets its epsilon symbol
     * to that of the first Automaton in the vector. It then iterates over the Automaton objects in the vector, giving new IDs to their states,
     * and adding their states, alphabets, transitions, and accepting states to the new Automaton. It also adds ε-transitions from the new start
     * state to the start state of each Automaton in the vector. Finally, it sets the start state of the new Automaton and gives new IDs to all
     * its states. The function returns a pointer to the new Automaton.
     *
     * Note: Memory management is crucial when using raw pointers in C++. Consider using smart pointers to automatically manage the memory of these objects.
     */
    static std::shared_ptr<Automaton> unionAutomataSet(std::vector<std::shared_ptr<Automaton>> &automata);

    /**
     * @brief Checks if two vectors of shared pointers to State objects are equal.
     *
     * This function checks if every State in vector y is also present in vector x.
     * Two States are considered equal if their contents are equal, not if they are the same object.
     * The function uses the State class's equality operator (operator==) to compare States.
     *
     * @param x A reference to the first vector of shared pointers to State objects.
     * @param y A reference to the second vector of shared pointers to State objects.
     * @return true if every State in y is also present in x, false otherwise.
     */
    static bool vector_equal(std::vector<std::shared_ptr<State>> &x, std::vector<std::shared_ptr<State>> &y);

    /**
     * @brief Adds unique states from one vector to another.
     *
     * This function iterates over each shared_ptr<State> in the 'from' vector. For each state in 'from',
     * it checks if there is an equivalent state in the 'to' vector. Two states are considered equivalent
     * if their contents are equal, not if they are the same object. The function uses the State class's
     * equality operator (operator==) to compare states.
     *
     * If no equivalent state is found in the 'to' vector, the state from the 'from' vector is added to 'to'.
     *
     * @param to A reference to the vector of shared pointers to State objects to add to.
     * @param from A const reference to the vector of shared pointers to State objects to add from.
     */
    static void addAll(std::vector<std::shared_ptr<State>> &to, std::vector<std::shared_ptr<State>> &from);

};


#endif //COMPILER_PROJECT_UTILITIES_H
