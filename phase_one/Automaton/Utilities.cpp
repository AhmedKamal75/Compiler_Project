#include "Utilities.h"

Automaton *Utilities::copyAutomaton(Automaton &originalAutomaton) {
    auto *copy = new Automaton();

    copy->setEpsilonSymbol(originalAutomaton.getEpsilonSymbol());

    for (const std::shared_ptr<State> &state_ptr: originalAutomaton.getStates()) {
        copy->addState(std::make_shared<State>(state_ptr->copy()));
    }

    for (const auto &alphabet: originalAutomaton.getAlphabets()) {
        copy->addAlphabet(alphabet);
    }

    for (const auto &entry: originalAutomaton.getTransitions()) {
        std::shared_ptr<State> from_state_ptr = copy->getStateById(entry.first.first->getId());
        std::string transitionSymbol = entry.first.second;
        std::vector<std::shared_ptr<State>> to_states;
        for (const auto &originalToState: entry.second) {
            std::shared_ptr<State> to_state_ptr = copy->getStateById(originalToState->getId());
            to_states.push_back(to_state_ptr);
        }
        copy->addTransitions(from_state_ptr, transitionSymbol, to_states);
    }

    for (const std::shared_ptr<State> &originalAcceptingState: originalAutomaton.getAccepting()) {
        copy->addFinal(copy->getStateById(originalAcceptingState->getId()));
    }

    copy->setStart(copy->getStateById(originalAutomaton.getStart()->getId()));

    copy->setRegex(originalAutomaton.getRegex());

    return copy;
}

Automaton *Utilities::unionAutomata(Automaton &a1, Automaton &a2, const std::string &newToken) {
    Automaton tempA1 = *copyAutomaton(a1);
    Automaton tempA2 = *copyAutomaton(a2);
    tempA1.giveNewIdsAll(-1, false);
    tempA2.giveNewIdsAll(1, true);

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
    unionAutomaton->addTransitions(newStartState, unionAutomaton->getEpsilonSymbol(),
                                   {tempA1.getStart(), tempA2.getStart()});
    unionAutomaton->setStart(newStartState);
    unionAutomaton->addState(newStartState);

    // Add the accepting states from both automata
    unionAutomaton->addFinals(tempA1.getAccepting());
    unionAutomaton->addFinals(tempA2.getAccepting());

    // If a new token name is not provided, construct it from the old token names
    std::string tempToken = (newToken.empty()) ? ("(" + tempA1.getToken() + "|" + tempA2.getToken() + ")") : (newToken);

    // Update the token names of the accepting states
    unionAutomaton->setToken(tempToken);
    unionAutomaton->giveNewIdsAll();

    return unionAutomaton;
}

Automaton *Utilities::concatAutomaton(Automaton &a1, Automaton &a2, const std::string &newToken) {
    Automaton tempA1 = *copyAutomaton(a1);
    Automaton tempA2 = *copyAutomaton(a2);
    tempA1.giveNewIdsAll(-1, false);
    tempA2.giveNewIdsAll(1, true);

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
    // Set the accepting states to the accepting states of the second automaton
    concatAutomaton->addFinals(tempA2.getAccepting());

    // If a new token name is not provided, construct it from the old token names
    std::string tempToken = (newToken.empty()) ? ("(" + tempA1.getToken() + tempA2.getToken() + ")") : (newToken);

    // Update the token names of the accepting states
    concatAutomaton->setToken(tempToken);
    concatAutomaton->giveNewIdsAll();

    return concatAutomaton;
}

Automaton *Utilities::kleeneClosure(Automaton &a, const std::string &newToken) {
    Automaton tempA = *copyAutomaton(a);
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
    auto newAcceptingState = std::make_shared<State>(static_cast<int>(kleeneAutomaton->getStates().size()) + 2, true,
                                                     newToken);
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

Automaton *Utilities::positiveClosure(Automaton &a, const std::string &newToken) {
    Automaton tempA = *copyAutomaton(a);
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
    auto newStartState = std::make_shared<State>(static_cast<int>(positiveAutomaton->getStates().size()) + 1, false,
                                                 "");
    auto newAcceptingState = std::make_shared<State>(static_cast<int>(positiveAutomaton->getStates().size()) + 2, true,
                                                     newToken);
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
    auto newStartState = std::make_shared<State>(-1, false, "");

    // Set the start state
    unionAutomaton->setStart(newStartState);

    // Add the new start state to the set of states
    unionAutomaton->addState(newStartState);

    std::string newToken;

    // Iterate over the automata in the vector
    for (Automaton &tempA: automata) {
        Automaton a = *copyAutomaton(tempA);
        // Give new IDs to the states in the current automaton
        a.giveNewIdsAll(static_cast<int>(unionAutomaton->getStates().size()) + 1, true);

        // Add the states and transitions from the current automaton
        unionAutomaton->addStates(a.getStates());
        unionAutomaton->addAlphabets(a.getAlphabets());
        unionAutomaton->addTransitions(a.getTransitions());

        // Add ε-transitions from the new start state to the start state of the current automaton
        unionAutomaton->addTransitions(newStartState, unionAutomaton->getEpsilonSymbol(),
                                       {unionAutomaton->getStateById(a.getStart()->getId())});

        // Add the accepting states from the current automaton
        unionAutomaton->addFinals(a.getAccepting());

        // Append the token of the current automaton to the new token
        if (!newToken.empty()) {
            newToken += "|";
        }
        newToken += "(" + a.getToken() + ")";
    }

    unionAutomaton->setToken(newToken);

    // Give new IDs to all states in the union automaton
    unionAutomaton->giveNewIdsAll();

    return unionAutomaton;
}
