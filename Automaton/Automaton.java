package LexicalAnalysisGenerator.Automaton;

import java.util.*;

/**
 * This class represents a finite automaton.
 * An automaton is a finite representation of a formal language.
 */
public class Automaton {
    private HashSet<State> states;
    private HashSet<String> alphabets;
    private State start;
    private HashMap<Pair<State, String>, HashSet<State>> transitions;
    private HashSet<State> accepting;
    private String epsilonSymbol;
    private String regex;

    private final String BUILT_IN_EPSILON_SYMBOL = String.valueOf('\u03B5');
    // Constructors

    public Automaton() {
        this.states = new HashSet<>();
        this.alphabets = new HashSet<>();
        this.accepting = new HashSet<>();
        this.transitions = new HashMap<>();
        this.accepting = new HashSet<>();
        this.epsilonSymbol = this.BUILT_IN_EPSILON_SYMBOL;
    }

    public Automaton(String alphabet, String tokenName, String epsilonSymbol) {
        // Create the states
        State q0 = new State(0, false, null);
        State q1 = new State(1, true, (tokenName == null) ? alphabet : tokenName);

        // Initialize the fields
        this.states = new HashSet<>(Arrays.asList(q0, q1));
//        this.alphabets = (alphabet.equals(epsilonSymbol)) ? (new HashSet<>(Collections.singletonList(alphabet))) : (new HashSet<>());
        this.alphabets = new HashSet<>(Collections.singletonList(alphabet));
        this.start = q0;
        this.accepting = new HashSet<>(Collections.singletonList(q1));
        this.transitions = new HashMap<>();
        this.epsilonSymbol = (epsilonSymbol == null) ? this.BUILT_IN_EPSILON_SYMBOL : epsilonSymbol;

        // Add the transition
        this.addTransitions(q0, alphabet, new HashSet<>(Collections.singletonList(q1)));
    }

    // Getters and Setters

    public void addTransitions(State currentState, String transitionSymbol, HashSet<State> nextStates) {
        Pair<State, String> transitionKey = new Pair<>(currentState, transitionSymbol);
        HashSet<State> tempNextStates = getNextStates(currentState, transitionSymbol);
        tempNextStates.addAll(nextStates);
        this.transitions.put(transitionKey, tempNextStates);
    }

    public HashSet<State> getNextStates(State currentState, String transitionSymbol) {
        return this.transitions.getOrDefault(new Pair<>(currentState, transitionSymbol), new HashSet<>());
    }

    public void giveNewIdsAll(HashSet<State> states, boolean positive) {
        giveNewIdsAll(states, 0, positive);
    }

    public void giveNewIdsAll(HashSet<State> states, int fromId, boolean positive) {
        int i = fromId;
        for (State state : states) {
            state.setId(i);
            i = (positive) ? (i + 1) : (i - 1);
        }
    }

    public void setToken(String tokenName) {
        for (State state : this.accepting) {
            state.setTokenName(tokenName);
            state.setAccepting(true);
        }
    }

    public Map<Pair<State, String>, State> getTransitionsDFAFormat() {
        Map<Pair<State, String>, State> dfaTransitions = new HashMap<>();
        for (Map.Entry<Pair<State, String>, HashSet<State>> entry : this.transitions.entrySet()) {
            // Take the first state from the set of next states.
            State nextState = entry.getValue().iterator().next();
            dfaTransitions.put(entry.getKey(), nextState);
        }
        return dfaTransitions;
    }

    public void setTransitionsDFAFormat(Map<Pair<State, String>, State> newTransitions) {
        HashMap<Pair<State, String>, HashSet<State>> transitions = new HashMap<>();
        for (Map.Entry<Pair<State, String>, State> entry : newTransitions.entrySet()) {
            transitions.put(entry.getKey(), new HashSet<>(Collections.singletonList(entry.getValue())));
        }
        this.transitions = transitions;
    }


    public String getTokens() {
        StringBuilder tokens = new StringBuilder("{");
        for (State finalState : this.getAccepting()) {
            tokens.append("\"").append(finalState.getTokenName()).append("\", ");
        }
        // Remove the trailing comma and space, then add the closing brace
        if (tokens.length() > 1) {
            tokens.setLength(tokens.length() - 2);
        }
        tokens.append("}");
        return tokens.toString();
    }

    public String getToken() {
        return this.accepting.iterator().next().getTokenName();
    }


    public State getStateById(int id) {
        for (State state : states) {
            if (state.getId() == id) {
                return state;
            }
        }
        // If the state with the given ID is not found, you can return null or throw an
        // exception.
        return null;
    }

    public void addState(State state) {
        this.states.add(state);
    }

    public void addFinalState(State state) {
        this.accepting.add(state);
    }

    public HashSet<State> getStates() {
        return states;
    }


    public HashSet<String> getAlphabets() {
        return alphabets;
    }


    public State getStart() {
        return start;
    }

    public void setStart(State start) {
        this.start = start;
    }

    public HashMap<Pair<State, String>, HashSet<State>> getTransitions() {
        return transitions;
    }

    public HashSet<State> getAccepting() {
        return accepting;
    }

    public boolean isAcceptingState(State state) {
        return this.accepting.contains(state);
    }

    public String getEpsilonSymbol() {
        return epsilonSymbol;
    }

    public void setEpsilonSymbol(String epsilonSymbol) {
        this.epsilonSymbol = epsilonSymbol;
    }

    public void setRegex(String regex) {
        this.regex = regex;
    }

    public String getRegex() {
        return this.regex;
    }

    public String toJson() {
        StringBuilder sb = new StringBuilder();
        sb.append("{\"type\":\"DFA\",\"dfa\":{\"transitions\":{");

        // Transitions
        List<Map.Entry<Pair<State, String>, HashSet<State>>> sortedTransitions = new ArrayList<>(this.transitions.entrySet());
        sortedTransitions.sort(Comparator.comparing(entry -> entry.getKey().getFirst().getId()));
        for (Map.Entry<Pair<State, String>, HashSet<State>> entry : sortedTransitions) {
            sb.append("\"").append(entry.getKey().getFirst().getId()).append("\":{\"")
                    .append(entry.getKey().getSecond()).append("\":\"");
            for (State state : entry.getValue()) {
                sb.append(state.getId());
            }
            sb.append("\"},");
        }
        sb.deleteCharAt(sb.length() - 1); // Remove trailing comma
        sb.append("},");

        // Start State
        sb.append("\"startState\":\"").append(this.start.getId()).append("\",");

        // Accept States
        sb.append("\"acceptStates\":[");
        for (State state : this.accepting) {
            sb.append("\"").append(state.getId()).append("\",");
        }
        sb.deleteCharAt(sb.length() - 1); // Remove trailing comma
        sb.append("]}}");

        return sb.toString();
    }


    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();

        sb.append("States: ");
        for (State state : this.states) {
            sb.append(state.getId()).append(" ");
        }
        sb.append("\n");

        sb.append("Input Symbols: ");
        for (String symbol : this.alphabets) {
            sb.append(symbol).append(" ");
        }
        sb.append("\n");

        sb.append("Start State: ").append(this.start.getId()).append("\n");

        sb.append("Final States: ");
        for (State state : this.accepting) {
            sb.append(state.getId()).append(" ");
        }
        sb.append("\n");

        sb.append("Transition Function: \n");
        List<Map.Entry<Pair<State, String>, HashSet<State>>> sortedTransitions = new ArrayList<>(this.transitions.entrySet());
        sortedTransitions.sort(Comparator.comparing(entry -> entry.getKey().getFirst().getId()));
        for (Map.Entry<Pair<State, String>, HashSet<State>> entry : sortedTransitions) {
            sb.append("δ(").append(entry.getKey().getFirst().getId()).append(", ").append(entry.getKey().getSecond())
                    .append(") = ");
            for (State state : entry.getValue()) {
                sb.append(state.getId()).append(" ");
            }
            sb.append("\n");
        }

        return sb.toString();
    }

    @Override
    public int hashCode() {
        return Objects.hash(states, alphabets, start, accepting, transitions, epsilonSymbol);
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null || getClass() != obj.getClass()) {
            return false;
        }
        Automaton automaton = (Automaton) obj;
        return Objects.equals(states, automaton.states) &&
                Objects.equals(alphabets, automaton.alphabets) &&
                Objects.equals(start, automaton.start) &&
                Objects.equals(accepting, automaton.accepting) &&
                Objects.equals(transitions, automaton.transitions) &&
                Objects.equals(epsilonSymbol, automaton.epsilonSymbol);
    }

}
