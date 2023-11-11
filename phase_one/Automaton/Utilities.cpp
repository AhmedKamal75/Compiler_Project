#include "Utilities.h"

bool Utilities::containsAcceptingState(const std::vector<std::shared_ptr<State>> &stateSet, Automaton &a) {
    for (const auto &state: stateSet) {
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
        copy->addState(std::make_shared<State>(*state));
    }

    for (const auto &alphabet: originalAutomaton.getAlphabets()) {
        copy->addAlphabet(alphabet);
    }

    for (const auto &entry: originalAutomaton.getTransitions()) {
        std::shared_ptr<State> fromState = copy->getStateById(entry.first.first->getId());
        std::string transitionSymbol = entry.first.second;
        std::vector<std::shared_ptr<State>> toStates;
        for (const auto &originalToState: entry.second) {
            std::shared_ptr<State> toState = copy->getStateById(originalToState->getId());
            toStates.push_back(toState);
        }

        copy->addTransitions(fromState, transitionSymbol, toStates);
    }

    for (const auto &originalAcceptingState: originalAutomaton.getAccepting()) {
        std::shared_ptr<State> copiedAcceptingState = copy->getStateById(originalAcceptingState->getId());
        copy->addFinal(copiedAcceptingState);
    }

    copy->setStart(copy->getStateById(originalAutomaton.getStart()->getId()));

    copy->setRegex(originalAutomaton.getRegex());

    return copy;
}

Automaton *Utilities::unionAutomata(const Automaton &a1, const Automaton &a2, const std::string &newToken) {
    Automaton tempA1 = a1;
    Automaton tempA2 = a2;
    tempA1.giveNewIdsAll(0, false);
    tempA2.giveNewIdsAll(0, true);

    // Create a new automaton
    auto *unionAutomaton = new Automaton();
    // Add the epsilon symbol
    unionAutomaton->setEpsilonSymbol(tempA1.getEpsilonSymbol());

    // Add the states and transitions from the first automaton
    unionAutomaton->addStates(tempA1.getStates());
    unionAutomaton->addAlphabets(tempA1.getAlphabets());
    unionAutomaton->addTransitions(tempA1.getTransitions());
    // Add the states and transitions from the second automaton
    unionAutomaton->addStates(tempA2.getStates());
    unionAutomaton->addAlphabets(tempA2.getAlphabets());
    unionAutomaton->addTransitions(tempA2.getTransitions());

    // Create a new start state with ε-transitions to the start states of the
    // original automata
    auto newStartState = std::make_shared<State>(static_cast<int>(unionAutomaton->getStates().size()) + 1, false, "");
    unionAutomaton->addTransitions(newStartState, unionAutomaton->getEpsilonSymbol(), {tempA1.getStart()});
    unionAutomaton->addTransitions(newStartState, unionAutomaton->getEpsilonSymbol(), {tempA2.getStart()});
    unionAutomaton->setStart(newStartState);
    unionAutomaton->addState(newStartState);

    // Add the accepting states from both automata
    unionAutomaton->addFinals(tempA1.getAccepting());
    unionAutomaton->addFinals(tempA2.getAccepting());

    // If a new token name is not provided, construct it from the old token names
    std::string tempToken = (newToken.empty()) ? ("(" + tempA1.getToken() + "|" + tempA2.getToken() + ")") : (newToken);

    // Update the token names of the accepting states
    unionAutomaton->setToken(newToken);
    unionAutomaton->giveNewIdsAll();

    return unionAutomaton;
}

Automaton *Utilities::concatAutomaton(const Automaton &a1, const Automaton &a2, const std::string &newToken) {
    Automaton tempA1 = a1;
    Automaton tempA2 = a2;
    tempA1.giveNewIdsAll(0, false);
    tempA2.giveNewIdsAll(0, true);

    // Create a new automaton
    auto *concatAutomaton = new Automaton();

    // Add the epsilon symbol
    concatAutomaton->setEpsilonSymbol(tempA1.getEpsilonSymbol());

    // Add the states and transitions from the first automaton
    concatAutomaton->addStates(tempA1.getStates());
    concatAutomaton->addAlphabets(tempA1.getAlphabets());
    concatAutomaton->addTransitions(tempA1.getTransitions());
    // Add the states and transitions from the second automaton
    concatAutomaton->addStates(tempA2.getStates());
    concatAutomaton->addAlphabets(tempA2.getAlphabets());
    concatAutomaton->addTransitions(tempA2.getTransitions());

    // Set the start state to the start state of the first automaton
    concatAutomaton->setStart(concatAutomaton->getStateById(tempA1.getStart()->getId()));

    // Add ε-transitions from the accepting states of the first automaton to the
    // start state of the second automaton
    std::shared_ptr<State> a2Start = concatAutomaton->getStateById(tempA2.getStart()->getId());
    for (const auto &s: tempA1.getAccepting()) {
        std::shared_ptr<State> state = concatAutomaton->getStateById(s->getId());
        state->setAccepting(false);
        concatAutomaton->addTransitions(state, concatAutomaton->getEpsilonSymbol(), {a2Start});
    }

    // If a new token name is not provided, construct it from the old token names
    std::string tempToken = (newToken.empty()) ? ("(" + tempA1.getToken() + tempA2.getToken() + ")") : (newToken);

    // Update the token names of the accepting states
    concatAutomaton->setToken(tempToken);
    concatAutomaton->giveNewIdsAll();

    return concatAutomaton;
}

Automaton *Utilities::kleeneClosure(const Automaton &a, const std::string &newToken) {
    Automaton tempA = a;
    tempA.giveNewIdsAll(0, false);

    // Create a new automaton
    auto *kleeneAutomaton = new Automaton();

    // Add the epsilon symbol
    kleeneAutomaton->setEpsilonSymbol(tempA.getEpsilonSymbol());

    // Add the states and transitions from the original automaton
    kleeneAutomaton->addStates(tempA.getStates());
    kleeneAutomaton->addAlphabets(tempA.getAlphabets());
    kleeneAutomaton->addTransitions(tempA.getTransitions());

    // Create a new start state and a new accepting state
    auto newStartState = std::make_shared<State>(static_cast<int>(kleeneAutomaton->getStates().size()) + 1, false, "");
    auto newAcceptingState = std::make_shared<State>(static_cast<int>(kleeneAutomaton->getStates().size()) + 2, true, newToken);
    kleeneAutomaton->addStates({newStartState, newAcceptingState});

    // Set the start state and the accepting states
    kleeneAutomaton->setStart(newStartState);
    kleeneAutomaton->addFinal(newAcceptingState);

    // Add ε-transitions from the new start state to the new accepting state
    kleeneAutomaton->addTransitions(newStartState, kleeneAutomaton->getEpsilonSymbol(), {newAcceptingState});

    // Add ε-transitions from the new accepting state to the new start state
    kleeneAutomaton->addTransitions(newAcceptingState, kleeneAutomaton->getEpsilonSymbol(), {newStartState});

    // Add ε-transitions from the new start state to the start state of the original automaton
    kleeneAutomaton->addTransitions(newStartState, kleeneAutomaton->getEpsilonSymbol(),
                                    {kleeneAutomaton->getStateById(tempA.getStart()->getId())});

    // Add ε-transitions from the accepting states of the original automaton to the new accepting state
    for (const auto &s: tempA.getAccepting()) {
        std::shared_ptr<State> state = kleeneAutomaton->getStateById(s->getId());
        state->setAccepting(false);
        kleeneAutomaton->addTransitions(state, kleeneAutomaton->getEpsilonSymbol(), {newAcceptingState});
    }

    // If a new token name is not provided, construct it from the old token names
    std::string tempToken = (newToken.empty()) ? ("(" + tempA.getToken() + ")*") : (newToken);

    // Update the token names of the accepting states
    kleeneAutomaton->setToken(tempToken);
    kleeneAutomaton->giveNewIdsAll();

    return kleeneAutomaton;
}

Automaton *Utilities::positiveClosure(const Automaton &a, const std::string &newToken) {
    Automaton tempA = a;
    tempA.giveNewIdsAll(0, false);

    // Create a new automaton
    auto *positiveAutomaton = new Automaton();

    // Add the epsilon symbol
    positiveAutomaton->setEpsilonSymbol(tempA.getEpsilonSymbol());

    // Add the states and transitions from the original automaton
    positiveAutomaton->addStates(tempA.getStates());
    positiveAutomaton->addAlphabets(tempA.getAlphabets());
    positiveAutomaton->addTransitions(tempA.getTransitions());

    // Create a new start state and a new accepting state
    auto newStartState = std::make_shared<State>(static_cast<int>(positiveAutomaton->getStates().size()) + 1, false, "");
    auto newAcceptingState = std::make_shared<State>(static_cast<int>(positiveAutomaton->getStates().size()) + 2, true, newToken);
    positiveAutomaton->addStates({newStartState, newAcceptingState});

    // Set the start state and the accepting states
    positiveAutomaton->setStart(newStartState);
    positiveAutomaton->addFinal(newAcceptingState);

    // Add ε-transitions from the new accepting state to the new start state
    positiveAutomaton->addTransitions(newAcceptingState, positiveAutomaton->getEpsilonSymbol(), {newStartState});

    // Add ε-transitions from the new start state to the start state of the original automaton
    positiveAutomaton->addTransitions(newStartState, positiveAutomaton->getEpsilonSymbol(),
                                      {positiveAutomaton->getStateById(tempA.getStart()->getId())});

    // Add ε-transitions from the accepting states of the original automaton to the new accepting state
    for (const auto &s: tempA.getAccepting()) {
        std::shared_ptr<State> state = positiveAutomaton->getStateById(s->getId());
        state->setAccepting(false);
        positiveAutomaton->addTransitions(state, positiveAutomaton->getEpsilonSymbol(), {newAcceptingState});
    }

    // If a new token name is not provided, construct it from the old token names
    std::string tempToken = (newToken.empty()) ? ("(" + tempA.getToken() + ")+") : (newToken);

    // Update the token names of the accepting states
    positiveAutomaton->setToken(tempToken);
    positiveAutomaton->giveNewIdsAll();

    return positiveAutomaton;
}

Automaton *Utilities::unionAutomataSet(std::vector<Automaton> &automata) {
    // Create a new automaton
    auto *unionAutomaton = new Automaton();

    // If the vector is empty, return the new (empty) automaton
    if (automata.empty()) {
        return unionAutomaton;
    }

    // Set the epsilon symbol to that of the first automaton in the vector
    unionAutomaton->setEpsilonSymbol(automata[0].getEpsilonSymbol());

    // Create a new start state
    auto newStartState = std::make_shared<State>(0, false, "");

    // Set the start state
    unionAutomaton->setStart(newStartState);

    // Add the new start state to the set of states
    unionAutomaton->addState(newStartState);

    // Iterate over the automata in the vector
    for (Automaton &a: automata) {
        // Give new IDs to the states in the current automaton
        a.giveNewIdsAll(static_cast<int>(unionAutomaton->getStates().size()) + 1, true);

        // Add the states and transitions from the current automaton
        unionAutomaton->addStates(a.getStates());
        unionAutomaton->addAlphabets(a.getAlphabets());
        unionAutomaton->addTransitions(a.getTransitions());

        // Add ε-transitions from the new start state to the start state of the current automaton
        std::shared_ptr<State> aStart = unionAutomaton->getStateById(a.getStart()->getId());
        unionAutomaton->addTransitions(newStartState, unionAutomaton->getEpsilonSymbol(), {aStart});

        // Add the accepting states from the current automaton
        unionAutomaton->addFinals(a.getAccepting());
    }

    // Give new IDs to all states in the union automaton
    unionAutomaton->giveNewIdsAll();

    return unionAutomaton;
}
