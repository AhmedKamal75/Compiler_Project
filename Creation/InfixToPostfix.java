package LexicalAnalysisGenerator.Creation;

import java.util.Stack;

public class InfixToPostfix {
    private final Constants constants = new Constants();

    // This method adds explicit concatenation operators to the regular expression
    private String concatExplicit(String regex) {
        StringBuilder newRegex = new StringBuilder();
        for (int i = 0; i < regex.length() - 1; i++) {
            char c1 = regex.charAt(i);
            char c2 = regex.charAt(i + 1);

            newRegex.append(c1);
            // If the current character is not an operator or is a closing parenthesis or a Kleene closure or a positive closure
            // and the next character is not an operator or is an opening parenthesis or an escape character, then add a concatenation operator
            if ((!constants.isOperator(c1) || c1 == constants.CLOSE_PARENTHESIS ||
                    c1 == constants.KLEENE_CLOSURE || c1 == constants.POSITIVE_CLOSURE ||
                    (constants.isOperator(c1) && i != 0 && regex.charAt(i - 1) == constants.ESCAPE)) &&
                    (!constants.isOperator(c2) || c2 == constants.OPEN_PARENTHESIS || c2 == constants.ESCAPE)) {
                newRegex.append(constants.CONCATENATION);
            }
        }
        newRegex.append(regex.charAt(regex.length() - 1));
        return newRegex.toString();
    }

    /**
     * Convert regular expression from infix to postfix notation using
     * Shunting-yard algorithm.
     *
     * @param regex infix notation
     * @return postfix notation
     */
    public String infixToPostfix(String regex) {
        StringBuilder postfix = new StringBuilder();
        Stack<Character> stack = new Stack<>();
        String formattedRegex = concatExplicit(regex);
        for (int i = 0; i < formattedRegex.length(); i++) {
            Character c = formattedRegex.charAt(i);
            if (!constants.isOperator(c)) {
                postfix.append(c);
            } else if (c == constants.OPEN_PARENTHESIS) {
                stack.push(c);
            } else if (c == constants.CLOSE_PARENTHESIS) {
                while (!stack.peek().equals('(')) {
                    postfix.append(stack.pop());
                }
                stack.pop();
            } else if (c == constants.ESCAPE) {
                char c1 = formattedRegex.charAt(i + 1);
                postfix.append(c1);
                postfix.append(formattedRegex.charAt(i));
                i++;
            } else { //  it is an operator
                while (!stack.isEmpty() && stack.peek() != constants.OPEN_PARENTHESIS) {
                    if (constants.priority(stack.peek()) >= constants.priority(c)) {
                        postfix.append(stack.pop());
                    } else {
                        break;
                    }
                }
                stack.push(c); // Push the current operator onto the stack
            }
        }

        while (!stack.isEmpty())
            postfix.append(stack.pop());

        return postfix.toString();
    }

    // This method evaluates the postfix expression, i.e. from postfix to infix
    public String evaluatePostfix(String postfix) {
        Stack<String> stack = new Stack<>();
        for (int i = 0; i < postfix.length(); i++) {
            char c = postfix.charAt(i);
            if (!constants.isOperator(c)) {
                stack.push(String.valueOf(c));
            } else {
                if (i < postfix.length() - 1 && constants.ESCAPE == postfix.charAt(i + 1)) {
                    char c1 = postfix.charAt(i + 1);
                    stack.push("(" + c1 + c + ")");
                    i++;
                } else if (c == constants.KLEENE_CLOSURE) {
                    stack.push("(" + stack.pop() + ")*"); // replace with your own operation
                } else if (c == constants.POSITIVE_CLOSURE) {
                    stack.push("(" + stack.pop() + ")+"); // replace with your own operation
                } else if (c == constants.RANGE) {
                    String operand2 = stack.pop();
                    String operand1 = stack.pop();
                    stack.push("(" + operand1 + "-" + operand2 + ")"); // replace with your own operation
                } else if (c == constants.CONCATENATION) {
                    String operand2 = stack.pop();
                    String operand1 = stack.pop();
                    stack.push("(" + operand1 + "." + operand2 + ")"); // replace with your own operation
                } else if (c == constants.UNION) {
                    String operand2 = stack.pop();
                    String operand1 = stack.pop();
                    stack.push("(" + operand1 + "|" + operand2 + ")"); // replace with your own operation
                } else if (c == constants.ESCAPE) {
                    char c1 = postfix.charAt(i - 1);
                    stack.pop();
                    stack.push("(" + c + c1 + ")");
                }
            }
        }

        return stack.pop();
    }
}
