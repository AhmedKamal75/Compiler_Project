package LexicalAnalysisGenerator.Creation;

import java.util.HashMap;

public class Constants {
    public final char ESCAPE = '\\';
    public final char KLEENE_CLOSURE = '*';
    public final char POSITIVE_CLOSURE = '+';
    public final char RANGE = '-';
    public final char CONCATENATION = '.';
    public final char UNION = '|';
    public final char OPEN_PARENTHESIS = '(';
    public final char CLOSE_PARENTHESIS = ')';
    private final HashMap<Character, Integer> priorities;

    public Constants() {
        this.priorities = new HashMap<>();
        this.priorities.put(ESCAPE, 5);
        this.priorities.put(KLEENE_CLOSURE, 4);
        this.priorities.put(POSITIVE_CLOSURE, 4);
        this.priorities.put(RANGE, 3);
        this.priorities.put(CONCATENATION, 2);
        this.priorities.put(UNION, 1);
        this.priorities.put(OPEN_PARENTHESIS, 0);
        this.priorities.put(CLOSE_PARENTHESIS, 0);
    }

    public int priority(char operator) {
        if (isOperator(operator)) {
            return this.priorities.get(operator);
        }
        return -1;
    }

    public boolean isOperator(char c) {
        return this.priorities.containsKey(c);
    }
}

