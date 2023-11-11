package LexicalAnalysisGenerator.Automaton;

import java.util.*;

public class Utilities {
    public Utilities() {}

    /**
     * Combines two automata using the union operation.
     *
     * @param a1           the first automaton
     * @param a2           the second automaton
     * @param newTokenName the token name for the new automaton
     * @return a new automaton that is the union of a1 and a2
     */
    public Automaton union(Automaton a1, Automaton a2, String newTokenName) {
        // make a copy of the parameters automatas so that we can use them in another
        // operations, and
        // the copy will be used here.
        Automaton copyA1 = copyAutomaton(a1);
        Automaton copyA2 = copyAutomaton(a2);
        copyA1.giveNewIdsAll(copyA1.getStates(), false);
        copyA2.giveNewIdsAll(copyA2.getStates(), true);

        // Create a new automaton
        Automaton unionAutomaton = new Automaton();
        unionAutomaton.setEpsilonSymbol(copyA1.getEpsilonSymbol());

        // Add the states and transitions from the first automaton
        unionAutomaton.getStates().addAll(copyA1.getStates());
        unionAutomaton.getAlphabets().addAll(copyA1.getAlphabets());
        unionAutomaton.getTransitions().putAll(copyA1.getTransitions());

        // Add the states and transitions from the second automaton
        unionAutomaton.getStates().addAll(copyA2.getStates());
        unionAutomaton.getAlphabets().addAll(copyA2.getAlphabets());
        unionAutomaton.getTransitions().putAll(copyA2.getTransitions());

        // Create a new start state with ε-transitions to the start states of the
        // original automata
        State newStartState = new State(unionAutomaton.getStates().size() + 1, false, null);
        unionAutomaton.addTransitions(newStartState, unionAutomaton.getEpsilonSymbol(),
                new HashSet<>(Collections.singletonList(copyA1.getStart())));
        unionAutomaton.addTransitions(newStartState, unionAutomaton.getEpsilonSymbol(),
                new HashSet<>(Collections.singletonList(copyA2.getStart())));
        unionAutomaton.setStart(newStartState);
        unionAutomaton.getStates().add(newStartState);

        // Add the accepting states from both automata
        unionAutomaton.getAccepting().addAll(copyA1.getAccepting());
        unionAutomaton.getAccepting().addAll(copyA2.getAccepting());

        // If a new token name is not provided, construct it from the old token names
        if (newTokenName == null) {
            String tokenName1 = copyA1.getAccepting().iterator().next().getTokenName();
            String tokenName2 = copyA2.getAccepting().iterator().next().getTokenName();
            newTokenName = "(" + tokenName1 + "|" + tokenName2 + ")";
        }

        // Update the token names of the accepting states
        unionAutomaton.setTokenAll(newTokenName);
        unionAutomaton.giveNewIdsAll(unionAutomaton.getStates(), true);
        return unionAutomaton;
    }

    /**
     * Combines two automata using the concatenation operation.
     *
     * @param a1           the first automaton
     * @param a2           the second automaton
     * @param newTokenName the token name for the new automaton
     * @return a new automaton that is the concatenation of a1 and a2
     */
    public Automaton concatenate(Automaton a1, Automaton a2, String newTokenName) {
        // make a copy of the parameters automatas so that we can use them in another
        // operations, and
        // the copy will be used here.
        Automaton copyA1 = copyAutomaton(a1);
        Automaton copyA2 = copyAutomaton(a2);
        copyA1.giveNewIdsAll(copyA1.getStates(), false);
        copyA2.giveNewIdsAll(copyA2.getStates(), true);

        // Create a new automaton
        Automaton concatAutomaton = new Automaton();
        concatAutomaton.setEpsilonSymbol(copyA1.getEpsilonSymbol());

        // Add the states and transitions from the first automaton
        concatAutomaton.getStates().addAll(copyA1.getStates());
        concatAutomaton.getAlphabets().addAll(copyA1.getAlphabets());
        concatAutomaton.getTransitions().putAll(copyA1.getTransitions());

        // Add the states and transitions from the second automaton
        concatAutomaton.getStates().addAll(copyA2.getStates());
        concatAutomaton.getAlphabets().addAll(copyA2.getAlphabets());
        concatAutomaton.getTransitions().putAll(copyA2.getTransitions());

        // Set the start state to the start state of the first automaton
        concatAutomaton.setStart(copyA1.getStart());

        // Add ε-transitions from the accepting states of the first automaton to the
        // start
        // state of the second automaton
        for (State acceptingState : copyA1.getAccepting()) {
            acceptingState.setAccepting(false);
            concatAutomaton.addTransitions(acceptingState, concatAutomaton.getEpsilonSymbol(),
                    new HashSet<>(Collections.singletonList(copyA2.getStart())));
        }

        // Set the accepting states to the accepting states of the second automaton
        concatAutomaton.getAccepting().addAll(copyA2.getAccepting());

        // If a new token name is not provided, construct it from the old token names
        if (newTokenName == null) {
            String tokenName1 = copyA1.getAccepting().iterator().next().getTokenName();
            String tokenName2 = copyA2.getAccepting().iterator().next().getTokenName();
            newTokenName = "(" + tokenName1 + tokenName2 + ")";
        }

        // Update the token names of the accepting states
        concatAutomaton.setTokenAll(newTokenName);
        concatAutomaton.giveNewIdsAll(concatAutomaton.getStates(), true);

        return concatAutomaton;
    }

    /**
     * Creates the Kleene closure of an automaton.
     *
     * @param a            the automaton
     * @param newTokenName the token name for the new automaton
     * @return a new automaton that is the Kleene closure of a
     */
    public Automaton kleeneClosure(Automaton a, String newTokenName) {
        // make a copy of the parameters automatas so that we can use them in another
        // operations, and
        // the copy will be used here.
        Automaton copyA = copyAutomaton(a);
        copyA.giveNewIdsAll(copyA.getStates(), false);

        // Create a new automaton
        Automaton kleeneAutomaton = new Automaton();
        kleeneAutomaton.setEpsilonSymbol(copyA.getEpsilonSymbol());

        // Add the states and transitions from the original automaton
        kleeneAutomaton.getStates().addAll(copyA.getStates());
        kleeneAutomaton.getAlphabets().addAll(copyA.getAlphabets());
        kleeneAutomaton.getTransitions().putAll(copyA.getTransitions());

        // Create a new start state and a new accepting state
        State newStartState = new State(kleeneAutomaton.getStates().size() + 1, false, null);
        State newAcceptingState = new State(kleeneAutomaton.getStates().size() + 2, true, newTokenName);
        kleeneAutomaton.getStates().addAll(Arrays.asList(newStartState, newAcceptingState));
        // Add ε-transitions from the new start state to the start state of the original
        // automaton
        kleeneAutomaton.addTransitions(newStartState, kleeneAutomaton.getEpsilonSymbol(),
                new HashSet<>(Collections.singletonList(copyA.getStart())));

        // Add ε-transitions from the accepting states of the original automaton to the
        // new accepting state
        for (State acceptingState : copyA.getAccepting()) {
            acceptingState.setAccepting(false);
            kleeneAutomaton.addTransitions(acceptingState, kleeneAutomaton.getEpsilonSymbol(),
                    new HashSet<>(Collections.singletonList(newAcceptingState)));
        }

        // Add ε-transitions from the new start state to the new accepting state
        kleeneAutomaton.addTransitions(newStartState, kleeneAutomaton.getEpsilonSymbol(),
                new HashSet<>(Collections.singletonList(newAcceptingState)));

        // Add ε-transitions from the new accepting state to the new start state
        kleeneAutomaton.addTransitions(newAcceptingState, kleeneAutomaton.getEpsilonSymbol(),
                new HashSet<>(Collections.singletonList(newStartState)));

        // Set the start state and the accepting states
        kleeneAutomaton.setStart(newStartState);
        kleeneAutomaton.getAccepting().add(newAcceptingState);

        // If a new token name is not provided, construct it from the old token names
        if (newTokenName == null) {
            String tokenName1 = copyA.getAccepting().iterator().next().getTokenName();
            newTokenName = "(" + tokenName1 + ")*";
        }

        // Update the token names of the accepting states
        kleeneAutomaton.setTokenAll(newTokenName);
        kleeneAutomaton.giveNewIdsAll(kleeneAutomaton.getStates(), true);

        return kleeneAutomaton;
    }

    /**
     * Creates the positive closure of an automaton.
     *
     * @param a            the automaton
     * @param newTokenName the token name for the new automaton
     * @return a new automaton that is the positive closure of a
     */
    public Automaton positiveClosure(Automaton a, String newTokenName) {
        // make a copy of the parameters automatas so that we can use them in another
        // operations, and
        // the copy will be used here.
        Automaton copyA = copyAutomaton(a);
        copyA.giveNewIdsAll(copyA.getStates(), false);

        // Create a new automaton
        Automaton positiveAutomaton = new Automaton();
        positiveAutomaton.setEpsilonSymbol(copyA.getEpsilonSymbol());

        // Add the states and transitions from the original automaton
        positiveAutomaton.getStates().addAll(copyA.getStates());
        positiveAutomaton.getAlphabets().addAll(copyA.getAlphabets());
        positiveAutomaton.getTransitions().putAll(copyA.getTransitions());

        // Create a new start state and a new accepting state
        State newStartState = new State(positiveAutomaton.getStates().size() + 1, false, null);
        State newAcceptingState = new State(positiveAutomaton.getStates().size() + 2, true, newTokenName);
        positiveAutomaton.getStates().addAll(Arrays.asList(newStartState, newAcceptingState));
        // Add ε-transitions from the new start state to the start state of the original
        // automaton
        positiveAutomaton.addTransitions(newStartState, positiveAutomaton.getEpsilonSymbol(),
                new HashSet<>(Collections.singletonList(copyA.getStart())));

        // Add ε-transitions from the accepting states of the original automaton to the
        // new
        // accepting state
        for (State acceptingState : copyA.getAccepting()) {
            acceptingState.setAccepting(false);
            positiveAutomaton.addTransitions(acceptingState, positiveAutomaton.getEpsilonSymbol(),
                    new HashSet<>(Collections.singletonList(newAcceptingState)));
        }

        // Add ε-transitions from the new accepting state to the new start state
        positiveAutomaton.addTransitions(newAcceptingState, positiveAutomaton.getEpsilonSymbol(),
                new HashSet<>(Collections.singletonList(newStartState)));

        // Set the start state and the accepting states
        positiveAutomaton.setStart(newStartState);
        positiveAutomaton.getAccepting().add(newAcceptingState);

        // If a new token name is not provided, construct it from the old token names
        if (newTokenName == null) {
            String tokenName1 = copyA.getAccepting().iterator().next().getTokenName();
            newTokenName = "(" + tokenName1 + ")+";
        }

        // Update the token names of the accepting states
        positiveAutomaton.setTokenAll(newTokenName);
        positiveAutomaton.giveNewIdsAll(positiveAutomaton.getStates(), true);

        return positiveAutomaton;
    }

    /**
     * Combines a list of automata.
     *
     * @param automataList the list of automata
     * @param tokenNames   the list of token names
     * @return a new automaton that is the combination of the automata in the list
     */
    public Automaton combineAutomataList(List<Automaton> automataList, List<String> tokenNames) {
        if (automataList.isEmpty() || tokenNames.size() != automataList.size()) {
            throw new IllegalArgumentException(
                    "Automata list and token names list must have the same size, and they cannot be empty.");
        }

        // create a copy of automata list
        List<Automaton> copyAutomata = new ArrayList<>(automataList.size());
        int i = 0;
        int idCount = 0;
        for (Automaton automaton : automataList) {
            // Update the token names of the accepting states if a token name is provided
            String tokenName = tokenNames.get(i++);
            if (tokenName != null) {
                automaton.setToken(tokenName);
            }
            Automaton copyA = copyAutomaton(automaton);
            idCount += copyA.getStates().size();
            copyA.giveNewIdsAll(copyA.getStates(), idCount, false);
            copyAutomata.add(copyAutomaton(automaton));
        }

        // Create a new automaton
        Automaton combinedAutomaton = new Automaton();
        combinedAutomaton.setEpsilonSymbol(copyAutomata.get(0).getEpsilonSymbol());

        // Copy the states, transitions, and accepting states of the old automata
        for (Automaton automaton : copyAutomata) {
            combinedAutomaton.getStates().addAll(automaton.getStates());
            combinedAutomaton.getAlphabets().addAll(automaton.getAlphabets());
            combinedAutomaton.getTransitions().putAll(automaton.getTransitions());
            for (State acceptingState : automaton.getAccepting()) {
                combinedAutomaton.getAccepting().add(acceptingState);
            }
        }

        // Create a new start state
        State newStartState = new State(combinedAutomaton.getStates().size() + 1, false, null);
        combinedAutomaton.setStart(newStartState);
        combinedAutomaton.getStates().add(newStartState);

        // Add epsilon transitions from the new start state to the start states of old
        // automata
        for (Automaton a : copyAutomata) {
            combinedAutomaton.addTransitions(newStartState, combinedAutomaton.getEpsilonSymbol(),
                    new HashSet<>(Collections.singletonList(a.getStart())));
        }

        combinedAutomaton.giveNewIdsAll(combinedAutomaton.getStates(), true);

        return combinedAutomaton;
    }

    /**
     * Creates a deep copy of the given automaton.
     *
     * @param originalAutomaton the automaton to copy
     * @return a deep copy of the original automaton
     */
    public Automaton copyAutomaton(Automaton originalAutomaton) {
        // Create a new automaton
        Automaton copiedAutomaton = new Automaton();

        // Copy epsilon symbol and other properties
        copiedAutomaton.setEpsilonSymbol(originalAutomaton.getEpsilonSymbol());

        // Copy states
        for (State originalState : originalAutomaton.getStates()) {
            State copiedState = originalState.copyState();
            copiedAutomaton.addState(copiedState);
        }

        // Copy alphabets
        for (String alphabet : originalAutomaton.getAlphabets()) {
            copiedAutomaton.getAlphabets().add(alphabet);
        }

        // Copy transitions
        for (Map.Entry<Pair<State, String>, HashSet<State>> entry : originalAutomaton.getTransitions().entrySet()) {
            Pair<State, String> transitionKey = entry.getKey();
            State fromState = copiedAutomaton.getStateById(transitionKey.getFirst().getId());
            String transitionSymbol = transitionKey.getSecond();
            HashSet<State> toStates = new HashSet<>();
            for (State originalToState : entry.getValue()) {
                State toState = copiedAutomaton.getStateById(originalToState.getId());
                toStates.add(toState);
            }

            copiedAutomaton.addTransitions(fromState, transitionSymbol, toStates);
        }

        // Copy accepting states
        for (State originalAcceptingState : originalAutomaton.getAccepting()) {
            State copiedAcceptingState = copiedAutomaton.getStateById(originalAcceptingState.getId());
            copiedAutomaton.addFinalState(copiedAcceptingState);
        }

        // Copy start state
        State originalStartState = originalAutomaton.getStart();
        State copiedStartState = copiedAutomaton.getStateById(originalStartState.getId());
        copiedAutomaton.setStart(copiedStartState);

        return copiedAutomaton;
    }

    public boolean containsAcceptingState(HashSet<State> stateSet, Automaton a) {
        for (State state : stateSet) {
            if (a.isAcceptingState(state)) {
                return true;
            }
        }
        return false;
    }
}
