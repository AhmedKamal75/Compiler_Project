package LexicalAnalysisGenerator.Creation;

import LexicalAnalysisGenerator.Automaton.Automaton;
import LexicalAnalysisGenerator.Automaton.Conversion;
import LexicalAnalysisGenerator.Automaton.Utilities;

import java.util.Map;
import java.util.Stack;

/**
 * This class is responsible for parsing regular expressions and converting them into minimized DFAs.
 */
public class Parsing {
    private final InfixToPostfix infixToPostfix;
    private final Constants constants;
    private final Utilities utilities;
    private final Conversion conversion;

    /**
     * Constructor for the Parsing class.
     */
    public Parsing() {
        this.infixToPostfix = new InfixToPostfix();
        this.constants = new Constants();
        this.utilities = new Utilities();
        this.conversion = new Conversion();
    }

    /**
     * Converts a regular expression into a minimized DFA.
     *
     * @param regex         The regular expression to be converted.
     * @param epsilonSymbol The symbol representing epsilon transitions.
     * @return The minimized DFA equivalent of the regular expression.
     */
    public Automaton regexToMinimizedDFA(String regex, String epsilonSymbol) {
        // Parse the regex and construct the corresponding automaton
        String postfix = infixToPostfix.infixToPostfix(regex);
        // parse the postfix regex (easier) to an Automaton
        Automaton regexAutomaton = parseRegex(postfix, epsilonSymbol);
        // Convert the regex automaton to a DFA and minimize it
        Automaton dfa = conversion.convertToDFA(regexAutomaton);
        // return the minimized dfa
        return conversion.minimizeDFA(dfa);
    }


    /**
     * Parses a regular expression and constructs the corresponding automaton.
     *
     * @param regex         The regular expression to be parsed.
     * @param epsilonSymbol The symbol representing epsilon transitions.
     * @return The automaton equivalent of the regular expression.
     */
    private Automaton parseRegex(String regex, String epsilonSymbol) {
        Stack<Automaton> stack = new Stack<>();
        for (int i = 0; i < regex.length(); i++) {
            char c = regex.charAt(i);
            if (!constants.isOperator(c)) {
                stack.push(new Automaton(String.valueOf(c), null, epsilonSymbol));
            } else {
                if (i < regex.length() - 1 && constants.ESCAPE == regex.charAt(i + 1)) {
                    stack.push(new Automaton(String.valueOf(c), null, epsilonSymbol));
                    i++;
                } else if (c == constants.KLEENE_CLOSURE) {
                    stack.push(utilities.kleeneClosure(stack.pop(), null)); // replace with your own operation
                } else if (c == constants.POSITIVE_CLOSURE) {
                    stack.push(utilities.positiveClosure(stack.pop(), null)); // replace with your own operation
                } else if (c == constants.RANGE) {
                    char end = stack.pop().getAlphabets().iterator().next().charAt(0);
                    char start = stack.pop().getAlphabets().iterator().next().charAt(0);
                    Automaton unionAll = new Automaton(String.valueOf(start), null, epsilonSymbol);
                    for (char letter = (char) (start + 1); letter <= end; letter++) {
                        unionAll = utilities.union(unionAll, new Automaton(String.valueOf(letter), null, epsilonSymbol), null);
                    }
                    stack.push(unionAll);
                } else if (c == constants.CONCATENATION) {
                    Automaton a2 = stack.pop();
                    Automaton a1 = stack.pop();
                    stack.push(utilities.concatenate(a1, a2, null)); // replace with your own operation
                } else if (c == constants.UNION) {
                    Automaton a2 = stack.pop();
                    Automaton a1 = stack.pop();
                    stack.push(utilities.union(a1, a2, null)); // replace with your own operation
                } else if (c == constants.ESCAPE) {
                    char c1 = regex.charAt(i - 1);
                    Automaton a = stack.pop();
                    stack.push(a);
                }
            }
        }

        return stack.pop();
    }

    public Automaton parseRegularDefinition(String regularDefinition, Map<String, Automaton> map, String epsilonSymbol) {
        Stack<Automaton> stack = new Stack<>();
        String name = "";
        boolean build = true;
        for (int i = 0; i < regularDefinition.length(); i++) {
            char c = regularDefinition.charAt(i);
            if (!constants.isOperator(c)) {
                stack.push(new Automaton(String.valueOf(c), null, epsilonSymbol));
            } else {
                if (i < regularDefinition.length() - 1 && constants.ESCAPE == regularDefinition.charAt(i + 1)) {
                    stack.push(new Automaton(String.valueOf(c), null, epsilonSymbol));
                    i++;
                } else if (c == constants.KLEENE_CLOSURE) {
                    stack.push(utilities.kleeneClosure(stack.pop(), null)); // replace with your own operation
                } else if (c == constants.POSITIVE_CLOSURE) {
                    stack.push(utilities.positiveClosure(stack.pop(), null)); // replace with your own operation
                } else if (c == constants.RANGE) {
                    char end = stack.pop().getAlphabets().iterator().next().charAt(0);
                    char start = stack.pop().getAlphabets().iterator().next().charAt(0);
                    Automaton unionAll = new Automaton(String.valueOf(start), null, epsilonSymbol);
                    for (char letter = (char) (start + 1); letter <= end; letter++) {
                        unionAll = utilities.union(unionAll, new Automaton(String.valueOf(letter), null, epsilonSymbol), null);
                    }
                    stack.push(unionAll);
                } else if (c == constants.CONCATENATION) {
                    Automaton a2 = stack.pop();
                    Automaton a1 = stack.pop();
                    stack.push(utilities.concatenate(a1, a2, null)); // replace with your own operation
                } else if (c == constants.UNION) {
                    Automaton a2 = stack.pop();
                    Automaton a1 = stack.pop();
                    stack.push(utilities.union(a1, a2, null)); // replace with your own operation
                } else if (c == constants.ESCAPE) {
                    char c1 = regularDefinition.charAt(i - 1);
                    Automaton a = stack.pop();
                    stack.push(a);
                }
            }
        }

        return stack.pop();

    }
}
