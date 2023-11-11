package LexicalAnalysisGenerator.Automaton;

import java.util.Objects;

public class State {
    private int id;
    private boolean isAccepting;
    private String tokenName;

    public State() {
    }

    public State(int id, boolean isAccepting, String tokenName) { // Modified this line
        this.id = id;
        this.isAccepting = isAccepting;
        this.tokenName = tokenName;
    }

    public State copyState() {
        // Create a new state with the same properties as the original state
        return new State(this.getId(), this.isAccepting(), this.getTokenName());
    }

    public String getTokenName() { // Added this method
        return tokenName;
    }

    public void setTokenName(String tokenName) { // Added this method
        this.tokenName = tokenName;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public boolean isAccepting() {
        return isAccepting;
    }

    public void setAccepting(boolean isAccepting) {
        this.isAccepting = isAccepting;
    }

    @Override
    public String toString() {
        return "[" + id + "]";
    }

    @Override
    public boolean equals(Object o) {
        if (this == o)
            return true;
        if (o == null || getClass() != o.getClass())
            return false;
        State state = (State) o;
        return this.id == state.id &&
                this.isAccepting == state.isAccepting &&
                Objects.equals(this.tokenName, state.tokenName);
    }

    @Override
    public int hashCode() {
        return Objects.hash(this.id, this.isAccepting, this.tokenName);
    }
}
