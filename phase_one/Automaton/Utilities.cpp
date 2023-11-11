//
// Created by Delta on 11/11/2023.
//

#include "Utilities.h"

bool Utilities::containsAcceptingState(const std::set<State> &stateSet, Automaton a) {
    for (const State &state: stateSet) {
        if (a.isAcceptingState(state)) {
            return true;
        }
    }
    return false;
}

Automaton *Utilities::copyAutomaton(Automaton &originalAutomaton) {
    auto *copy = new Automaton();

    copy->setEpsilonSymbol(originalAutomaton.getEpsilonSymbol());

    for (const auto &state: originalAutomaton.getStates()) {
        State stateCopy = state.copy();
        copy->addState(stateCopy);
    }

    for (const auto &alphabet: originalAutomaton.getAlphabets()) {
        copy->getAlphabets().insert(alphabet);
    }

    for (const auto &entry: originalAutomaton.getTransitions()) {
        State fromState = copy->getStateById(entry.first.first.getId());
        std::string transitionSymbol = entry.first.second;
        std::set<State> toStates;
        for (const State &originalToState: entry.second) {
            State toState = copy->getStateById(originalToState.getId());
            toStates.insert(toState);
        }

        copy->addTransitions(fromState, transitionSymbol, toStates);
    }

    for (const State &originalAcceptingState: originalAutomaton.getAccepting()) {
        State copiedAcceptingState = copy->getStateById(originalAcceptingState.getId());
        copy->addFinalState(copiedAcceptingState);
    }

    copy->setStart(copy->getStateById(originalAutomaton.getStart().getId()));

    copy->setRegex(originalAutomaton.getRegex());

    return copy;
}

Automaton *Utilities::unionAutomata(Automaton a1, Automaton a2, const std::string &newToken) {
    a1.giveNewIdsAll(0, false);
    a2.giveNewIdsAll(0, true);

    // Create a new automaton
    auto *unionAutomaton = new Automaton();
    // Add the epsilon symbol
    unionAutomaton->setEpsilonSymbol(a1.getEpsilonSymbol());

    // Add the states and transitions from the first automaton
    unionAutomaton->getStates().insert(a1.getStates().begin(), a1.getStates().end());
    unionAutomaton->getAlphabets().insert(a1.getAlphabets().begin(), a1.getAlphabets().end());
    unionAutomaton->getTransitions().insert(a1.getTransitions().begin(), a1.getTransitions().end());
    // Add the states and transitions from the second automaton
    unionAutomaton->getStates().insert(a2.getStates().begin(), a2.getStates().end());
    unionAutomaton->getAlphabets().insert(a2.getAlphabets().begin(), a2.getAlphabets().end());
    unionAutomaton->getTransitions().insert(a2.getTransitions().begin(), a2.getTransitions().end());

    // Create a new start state with ε-transitions to the start states of the
    // original automata
    State newStartState(static_cast<int>(unionAutomaton->getStates().size()) + 1, false, "");
    unionAutomaton->addTransitions(newStartState, unionAutomaton->getEpsilonSymbol(), {a1.getStart()});
    unionAutomaton->addTransitions(newStartState, unionAutomaton->getEpsilonSymbol(), {a2.getStart()});
    unionAutomaton->setStart(newStartState);
    unionAutomaton->getStates().insert(newStartState);

    // Add the accepting states from both automata
    unionAutomaton->getAccepting().insert(a1.getAccepting().begin(), a1.getAccepting().end());
    unionAutomaton->getAccepting().insert(a2.getAccepting().begin(), a2.getAccepting().end());

    // If a new token name is not provided, construct it from the old token names
    std::string tempToken = (newToken.empty()) ? ("(" + a1.getToken() + "|" + a2.getToken() + ")") : (newToken);

    // Update the token names of the accepting states
    unionAutomaton->setToken(newToken);
    unionAutomaton->giveNewIdsAll();

    return unionAutomaton;
}

Automaton *Utilities::concatAutomaton(Automaton a1, Automaton a2, const std::string &newToken) {
    a1.giveNewIdsAll(0, false);
    a2.giveNewIdsAll(0, true);

    // Create a new automaton
    auto *concatAutomaton = new Automaton();

    // Add the epsilon symbol
    concatAutomaton->setEpsilonSymbol(a1.getEpsilonSymbol());

    // Add the states and transitions from the first automaton
    concatAutomaton->getStates().insert(a1.getStates().begin(), a1.getStates().end());
    concatAutomaton->getAlphabets().insert(a1.getAlphabets().begin(), a1.getAlphabets().end());
    concatAutomaton->getTransitions().insert(a1.getTransitions().begin(), a1.getTransitions().end());
    // Add the states and transitions from the second automaton
    concatAutomaton->getStates().insert(a2.getStates().begin(), a2.getStates().end());
    concatAutomaton->getAlphabets().insert(a2.getAlphabets().begin(), a2.getAlphabets().end());
    concatAutomaton->getTransitions().insert(a2.getTransitions().begin(), a2.getTransitions().end());

    // Set the start state to the start state of the first automaton
    concatAutomaton->setStart(concatAutomaton->getStateById(a1.getStart().getId()));

    // Add ε-transitions from the accepting states of the first automaton to the
    // start
    // state of the second automaton
    State &a2Start = concatAutomaton->getStateById(a2.getStart().getId());
    for (const State &s: a1.getAccepting()) {
        State state = concatAutomaton->getStateById(s.getId());
        state.setAccepting(false);
        concatAutomaton->addTransitions(state, concatAutomaton->getEpsilonSymbol(), {a2Start});
    }

    // If a new token name is not provided, construct it from the old token names
    std::string tempToken = (newToken.empty()) ? ("(" + a1.getToken() + a2.getToken() + ")") : (newToken);

    // Update the token names of the accepting states
    concatAutomaton->setToken(tempToken);
    concatAutomaton->giveNewIdsAll();

    return concatAutomaton;
}

Automaton *Utilities::kleeneClosure(Automaton a, const std::string &newToken) {
    a.giveNewIdsAll(0, false);

    // Create a new automaton
    auto *kleeneAutomaton = new Automaton();

    // Add the epsilon symbol
    kleeneAutomaton->setEpsilonSymbol(a.getEpsilonSymbol());

    // Add the states and transitions from the original automaton
    kleeneAutomaton->getStates().insert(a.getStates().begin(), a.getStates().end());
    kleeneAutomaton->getAlphabets().insert(a.getAlphabets().begin(), a.getAlphabets().end());
    kleeneAutomaton->getTransitions().insert(a.getTransitions().begin(), a.getTransitions().end());

    // Create a new start state and a new accepting state
    State newStartState(static_cast<int>(kleeneAutomaton->getStates().size()) + 1, false, "");
    State newAcceptingState(static_cast<int>(kleeneAutomaton->getStates().size()) + 2, true, newToken);
    kleeneAutomaton->getStates().insert({newStartState, newAcceptingState});

    // Set the start state and the accepting states
    kleeneAutomaton->setStart(newStartState);
    kleeneAutomaton->getAccepting().insert(newAcceptingState);

    // Add ε-transitions from the new start state to the new accepting state
    kleeneAutomaton->addTransitions(newStartState, kleeneAutomaton->getEpsilonSymbol(), {newAcceptingState});

    // Add ε-transitions from the new accepting state to the new start state
    kleeneAutomaton->addTransitions(newAcceptingState, kleeneAutomaton->getEpsilonSymbol(), {newStartState});

    // Add ε-transitions from the new start state to the start state of the original automaton
    kleeneAutomaton->addTransitions(newStartState, kleeneAutomaton->getEpsilonSymbol(),
                                    {kleeneAutomaton->getStateById(a.getStart().getId())});

    // Add ε-transitions from the accepting states of the original automaton to the new accepting state
    for (const State &s: a.getAccepting()) {
        State state = kleeneAutomaton->getStateById(s.getId());
        state.setAccepting(false);
        kleeneAutomaton->addTransitions(state, kleeneAutomaton->getEpsilonSymbol(), {newAcceptingState});
    }

    // If a new token name is not provided, construct it from the old token names
    std::string tempToken = (newToken.empty()) ? ("(" + a.getToken() + ")*") : (newToken);

    // Update the token names of the accepting states
    kleeneAutomaton->setToken(tempToken);
    kleeneAutomaton->giveNewIdsAll();

    return kleeneAutomaton;
}

Automaton *Utilities::positiveClosure(Automaton a, const std::string &newToken) {
    a.giveNewIdsAll(0, false);

    // Create a new automaton
    auto *positiveAutomaton = new Automaton();

    // Add the epsilon symbol
    positiveAutomaton->setEpsilonSymbol(a.getEpsilonSymbol());

    // Add the states and transitions from the original automaton
    positiveAutomaton->getStates().insert(a.getStates().begin(), a.getStates().end());
    positiveAutomaton->getAlphabets().insert(a.getAlphabets().begin(), a.getAlphabets().end());
    positiveAutomaton->getTransitions().insert(a.getTransitions().begin(), a.getTransitions().end());

    // Create a new start state and a new accepting state
    State newStartState(static_cast<int>(positiveAutomaton->getStates().size()) + 1, false, "");
    State newAcceptingState(static_cast<int>(positiveAutomaton->getStates().size()) + 2, true, newToken);
    positiveAutomaton->getStates().insert({newStartState, newAcceptingState});

    // Set the start state and the accepting states
    positiveAutomaton->setStart(newStartState);
    positiveAutomaton->getAccepting().insert(newAcceptingState);

    // Add ε-transitions from the new accepting state to the new start state
    positiveAutomaton->addTransitions(newAcceptingState, positiveAutomaton->getEpsilonSymbol(), {newStartState});

    // Add ε-transitions from the new start state to the start state of the original automaton
    positiveAutomaton->addTransitions(newStartState, positiveAutomaton->getEpsilonSymbol(),
                                      {positiveAutomaton->getStateById(a.getStart().getId())});

    // Add ε-transitions from the accepting states of the original automaton to the new accepting state
    for (const State &s: a.getAccepting()) {
        State state = positiveAutomaton->getStateById(s.getId());
        state.setAccepting(false);
        positiveAutomaton->addTransitions(state, positiveAutomaton->getEpsilonSymbol(), {newAcceptingState});
    }


    // If a new token name is not provided, construct it from the old token names
    std::string tempToken = (newToken.empty()) ? ("(" + a.getToken() + ")+") : (newToken);

    // Update the token names of the accepting states
    positiveAutomaton->setToken(tempToken);
    positiveAutomaton->giveNewIdsAll();

    return positiveAutomaton;
}

Automaton *Utilities::unionAutomataSet(std::vector<Automaton> automata) {
    // Create a new automaton
    auto *unionAutomaton = new Automaton();

    // If the vector is empty, return the new (empty) automaton
    if (automata.empty()) {
        return unionAutomaton;
    }

    // Set the epsilon symbol to that of the first automaton in the vector
    unionAutomaton->setEpsilonSymbol(automata[0].getEpsilonSymbol());

    // Create a new start state
    State newStartState(0, false, "");

    // Set the start state
    unionAutomaton->setStart(newStartState);

    // Add the new start state to the set of states
    unionAutomaton->getStates().insert(newStartState);

    // Iterate over the automata in the vector
    for (Automaton a: automata) {
        // Give new IDs to the states in the current automaton
        a.giveNewIdsAll(static_cast<int>(unionAutomaton->getStates().size()) + 1, true);

        // Add the states and transitions from the current automaton
        unionAutomaton->getStates().insert(a.getStates().begin(), a.getStates().end());
        unionAutomaton->getAlphabets().insert(a.getAlphabets().begin(), a.getAlphabets().end());
        unionAutomaton->getTransitions().insert(a.getTransitions().begin(), a.getTransitions().end());

        // Add ε-transitions from the new start state to the start state of the current automaton
        State &aStart = unionAutomaton->getStateById(a.getStart().getId());
        unionAutomaton->addTransitions(newStartState, unionAutomaton->getEpsilonSymbol(), {aStart});

        // Add the accepting states from the current automaton
        unionAutomaton->getAccepting().insert(a.getAccepting().begin(), a.getAccepting().end());
    }

    // Give new IDs to all states in the union automaton
    unionAutomaton->giveNewIdsAll();

    return unionAutomaton;
}


