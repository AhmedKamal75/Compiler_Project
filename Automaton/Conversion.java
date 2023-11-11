package LexicalAnalysisGenerator.Automaton;

import java.util.*;

/**
 * This class provides methods for converting automata.
 * It includes methods for converting epsilon-NFAs to NFAs and NFAs to DFAs.
 */
public class Conversion {
    private final HashMap<State, HashSet<State>> epsilonClosures;
    private Integer counter;
    private final Utilities utilities;

    /**
     * Constructs a new Conversion object.
     */
    public Conversion() {
        this.epsilonClosures = new HashMap<>();
        this.counter = 0;
        this.utilities = new Utilities();
    }

    /**
     * Prepares the Conversion object for a new automaton.
     * This method should be called before starting to work with a new automaton.
     *
     * @param a the new automaton
     */
    public void prepareForAutomaton(Automaton a) {
        epsilonClosures.clear();
        this.counter = a.getStates().size() + 1;
    }

    /**
     * Computes the epsilon-closure of a state in an automaton.
     * The epsilon-closure of a state is the set of states that can be reached
     * from the state by following epsilon-transitions.
     * it uses the depth first search algorithm.
     *
     * @param a     the automaton
     * @param state the state
     * @return the epsilon-closure of the state
     */
    private HashSet<State> epsilonClosure(Automaton a, State state) {
        if (epsilonClosures.containsKey(state)) {
            return epsilonClosures.get(state);
        }
        HashSet<State> epsilonClosure = new HashSet<>();
        Stack<State> stack = new Stack<>();
        stack.push(state);

        while (!stack.isEmpty()) {
            State currentState = stack.pop();
            epsilonClosure.add(currentState);

            for (State nextState : a.getNextStates(currentState, a.getEpsilonSymbol())) {
                if (!epsilonClosure.contains(nextState)) {
                    stack.push(nextState);
                }
            }
        }
        epsilonClosures.put(state, epsilonClosure);
        return epsilonClosure;
    }

    /**
     * IMPORTANT NOTE: don't use this method it was tested and didn't work correctly.
     * Transforms an epsilon-NFA to a normal NFA.
     * a function to transform this epsilon automata to normal automata (i.e.
     * from epsilon nfa to normal nfa) using this formula:
     * δ --> epsilon nfa transitions
     * δ' --> normal nfa transitions
     * δ'(state, symbol) = epsilon_closure(δ(epsilon_closure(state),symbol))
     * x = epsilon_closure(state)
     * y = δ(x,symbol)
     * z = epsilon_closure(y)
     * δ'(state, symbol) = [z]
     *
     * @param automaton the epsilon-NFA to transform
     * @return a new automaton that is the normal NFA equivalent of the input
     * epsilon-NFA
     */
    public Automaton removeEpsilonTransitions(Automaton automaton) {
        // create a copy of this
        Automaton a = utilities.copyAutomaton(automaton);

        // Create a new automaton
        Automaton nfa = new Automaton();

        // Copy ε-symbol, the states, alphabets, start state, and accepting states
        nfa.setEpsilonSymbol(a.getEpsilonSymbol());
        nfa.getStates().addAll(a.getStates());
        nfa.getAlphabets().addAll(a.getAlphabets());
        nfa.setStart(a.getStart());
        nfa.getAccepting().addAll(a.getAccepting());

        // now we work on the transitions, and add new accepting states

        this.prepareForAutomaton(a);
        // For each state and each alphabet, compute the set of reachable states
        for (State state : a.getStates()) {
            for (String alphabet : a.getAlphabets()) {
                if (!alphabet.equals(a.getEpsilonSymbol())) {
                    // x = epsilon_closure(state)
                    HashSet<State> x = epsilonClosure(a, state);
                    // update accepting
                    // every state able to reach an accepting state using epsilon transitions, it is
                    // accepting.
                    HashSet<State> common = new HashSet<>(x);
                    common.retainAll(a.getAccepting());
                    if (!common.isEmpty()) {
                        state.setAccepting(true);
                        state.setTokenName(common.iterator().next().getTokenName());
                        nfa.getAccepting().add(state);
                    }
                    // y = δ(x,alphabet)
                    HashSet<State> y = new HashSet<>();
                    for (State s : x) {
                        y.addAll(a.getNextStates(s, alphabet));
                    }
                    // z = epsilon_closure(y)
                    HashSet<State> z = new HashSet<>();
                    for (State s : y) {
                        z.addAll(epsilonClosure(a, s));
                    }
                    // δ'(state, alphabet) = [z]
                    nfa.addTransitions(state, alphabet, z);
                }
            }
        }
        nfa.giveNewIdsAll(nfa.getStates(), true);
        return nfa;
    }

    /**
     * Transforms an NFA to a DFA.
     *
     * @param automaton the NFA to transform
     * @return a new automaton that is the DFA equivalent of the input NFA
     */
    public Automaton convertToDFA(Automaton automaton) {
        // create a copy of this
        Automaton a = utilities.copyAutomaton(automaton);

        // Create a new automaton
        Automaton dfa = new Automaton();

        // Copy the alphabets and epsilon symbol
        dfa.getAlphabets().addAll(a.getAlphabets());
        dfa.setEpsilonSymbol(a.getEpsilonSymbol());

        HashMap<HashSet<State>, State> dfaStates = new HashMap<>();
        Queue<HashSet<State>> queue = new LinkedList<>();

        prepareForAutomaton(a);

        // Compute the epsilon closure of the start state
        HashSet<State> startSet = epsilonClosure(a, a.getStart());
        // System.out.println("start set: " + startSet.toString());
        queue.add(startSet);
        boolean startIsSet = false;

        while (!queue.isEmpty()) {
            HashSet<State> currentSet = queue.remove();
            State dfaState = getDFAState(currentSet, dfaStates);
            if (dfaState == null) {
                dfaState = createDFAState(currentSet, a, dfa);
                dfaStates.put(currentSet, dfaState);
            }
            // make dfaState a part of the dfa states not the automaton
            if (!startIsSet && currentSet.contains(a.getStart())) {
                dfa.setStart(dfaState);
                startIsSet = true;
                // that mean that the dfa will have its start state set one time only, and that
                // is for the correct state
            }
            // now we have a new state that needs to be added to the automaton

            for (String alphabet : a.getAlphabets()) {
                if (!alphabet.equals(a.getEpsilonSymbol())) {
                    // get the set reachable from dfaState(currentState) using alphabet
                    HashSet<State> nextSet = new HashSet<>();
                    for (State state : currentSet) { // currentSet is already an epsilon closure
                        nextSet.addAll(a.getNextStates(state, alphabet));
                    }
                    HashSet<State> epsilonClosureSet = new HashSet<>();// Compute the epsilon closure of the next states
                    for (State state : nextSet) {
                        epsilonClosureSet.addAll(epsilonClosure(a, state));
                    }
                    // compute the next state from the ε-closure set calculated above.
                    State nextState;
                    if (epsilonClosures.isEmpty()) { // meaning that next state is a dead state
                        nextState = getDFAState(epsilonClosureSet, dfaStates);
                        if (nextState == null) {
                            nextState = createDeadState(dfa);
                        }
                    } else {
                        nextState = getDFAState(epsilonClosureSet, dfaStates);
                        if (nextState == null) {
                            nextState = createDFAState(epsilonClosureSet, a, dfa);
                        }
                    }
                    // nextState calculated and dfa adjusted to accommodate it, then add the transition
                    // currentState(dfaState) --alphabet--> nextState
                    dfa.addTransitions(dfaState, alphabet, new HashSet<>(Collections.singletonList(nextState)));
                    // keep the following code in its order
                    if (!dfaStates.containsKey(epsilonClosureSet)) {
                        queue.add(epsilonClosureSet);
                    }
                    dfaStates.put(epsilonClosureSet, nextState);
                }
            }

        }

        dfa.giveNewIdsAll(dfa.getStates(), true);

        return dfa;
    }

    private State createDFAState(HashSet<State> stateSet, Automaton a, Automaton dfa) {
        State newState = new State(++this.counter, false, null);
        dfa.getStates().add(newState);
        if (this.utilities.containsAcceptingState(stateSet, a)) {
            newState.setAccepting(true);
            newState.setTokenName(a.getAccepting().iterator().next().getTokenName());
            dfa.getAccepting().add(newState);
        }
        return newState;
    }

    private State getDFAState(HashSet<State> stateSet, HashMap<HashSet<State>, State> dfaStates) {
        if (!dfaStates.containsKey(stateSet)) {
            return null;
        }
        return dfaStates.get(stateSet);
    }

    private State createDeadState(Automaton dfa) {
        State deadState = new State(++this.counter, false, null);
        dfa.getStates().add(deadState);
        for (String alphabet : dfa.getAlphabets()) {
            dfa.addTransitions(deadState, alphabet, new HashSet<>(Collections.singletonList(deadState)));
        }
        return deadState;
    }


    /**
     * This method minimizes a given DFA (Deterministic Finite LexicalAnalysisGenerator.Automaton) using Hopcroft's algorithm.
     * The algorithm works by partitioning the states of the DFA into groups of indistinguishable states,
     * and then collapsing each group of states into a single state. The resulting minimized DFA has the
     * property that it has the smallest possible number of states and is equivalent to the original DFA.
     *
     * @param automaton The DFA to be minimized.
     * @return The minimized DFA.
     */
    public Automaton minimizeDFA(Automaton automaton) {
        // Step 0: Create a copy of the original automaton so that it is changed.
        Automaton dfa = this.utilities.copyAutomaton(automaton);

        // Step 1: Create a list of groups of states. Initially, there are two groups: accepting states and non-accepting states.
        List<Set<State>> currentGroup = new ArrayList<>();
        currentGroup.add(new HashSet<>(dfa.getAccepting()));
        currentGroup.add(new HashSet<>(dfa.getStates()));
        currentGroup.get(1).removeAll(dfa.getAccepting());


        // Step 2: Refine the groups until no further refinement is possible.
        while (true) {
            List<Set<State>> nextGroup = getNextEquivalence(currentGroup, dfa.getAlphabets(), dfa.getTransitionsDFAFormat());
            if (currentGroup.equals(nextGroup)) {
                break;
            }
            currentGroup = nextGroup;
        }

        // Step 3: Construct the minimized DFA.
        Automaton minDFA = new Automaton();
        // fields that don't need any computations.
        minDFA.setEpsilonSymbol(dfa.getEpsilonSymbol());
        minDFA.getAlphabets().addAll(dfa.getAlphabets());
        // calculating the states.
        Pair<List<State>, Pair<State, Set<State>>> statesData = getNewStatesAndSpecialStates(currentGroup, dfa);
        minDFA.getStates().addAll(statesData.getFirst());
        minDFA.setStart(statesData.getSecond().getFirst());
        minDFA.getAccepting().addAll(statesData.getSecond().getSecond());
        // calculating the transitions.
        minDFA.setTransitionsDFAFormat(getNewTransitions(dfa, currentGroup, statesData.getFirst()));
        minDFA.giveNewIdsAll(minDFA.getStates(), 0, true);
        return minDFA;
    }

    public List<Set<State>> getNextEquivalence(List<Set<State>> prevEquivalence, Set<String> alphabets, Map<Pair<State, String>, State> transitions) {
        List<Set<State>> nextEquivalence = new ArrayList<>();

        for (Set<State> group : prevEquivalence) {
            Map<Set<State>, Set<State>> newGroups = new HashMap<>();
            for (State state : group) {
                Set<State> key = new HashSet<>();
                for (String symbol : alphabets) {
                    State nextState = transitions.get(new Pair<>(state, symbol));
                    for (Set<State> g : prevEquivalence) {
                        if (g.contains(nextState)) {
                            key.add(g.iterator().next());
                            break;
                        }
                    }
                }
                if (!newGroups.containsKey(key)) {
                    newGroups.put(key, new HashSet<>());
                }
                newGroups.get(key).add(state);
            }
            nextEquivalence.addAll(newGroups.values());
        }

        return nextEquivalence;
    }

    public Pair<List<State>, Pair<State, Set<State>>> getNewStatesAndSpecialStates(List<Set<State>> group, Automaton automaton) {
        List<State> newStates = new ArrayList<>(group.size());
        State newStartState = null;
        Set<State> newAcceptingStates = new HashSet<>();

        // Create new states and map old states to new states
        for (Set<State> g : group) {
            State representativeState = g.iterator().next();  // take the first state of the set as the representative of the set
            newStates.add(representativeState);

            if (g.contains(automaton.getStart())) {
                newStartState = representativeState;  // the new start state is the representative of the group containing the old start state
            }

            for (State oldState : g) {
                if (automaton.getAccepting().contains(oldState)) {
                    newAcceptingStates.add(representativeState);  // if the group contains an old accepting state, the representative state is a new accepting state
                    break;
                }
            }
        }
        Pair<State, Set<State>> temp = new Pair<>(newStartState, newAcceptingStates);
        return new Pair<>(newStates, temp);
    }

    public Map<Pair<State, String>, State> getNewTransitions(Automaton oldDFA, List<Set<State>> group, List<State> newStates) {
        Map<Pair<State, String>, State> oldTransitions = oldDFA.getTransitionsDFAFormat();
        Map<State, State> stateToRepresentative = new HashMap<>();

        // Map old states to their respective representative.
        int i = 0;
        for (Set<State> g : group) {
            for (State oldState : g) {
                stateToRepresentative.put(oldState, newStates.get(i));
            }
            i++;
        }

        // Create new transitions
        Map<Pair<State, String>, State> newTransitions = new HashMap<>();

        for (State newState : newStates) {
            for (String symbol : oldDFA.getAlphabets()) {
                Pair<State, String> key = new Pair<>(newState, symbol);
                newTransitions.put(key, stateToRepresentative.get(oldTransitions.get(key)));
            }
        }
        return newTransitions;
    }
}
