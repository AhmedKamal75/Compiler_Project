package LexicalAnalysisGenerator.Automaton;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class AutomatonTest {
    private static final Utilities utilities = new Utilities();
    private static final Conversion conversion = new Conversion();

    public static void main(String[] args) throws Exception {
        test2();
    }

    private static HashMap<String, Automaton> getAutomata() {
        List<String> alphabets = new ArrayList<>();
        for (char letter = 'a'; letter <= 'z'; letter++) {
            alphabets.add(String.valueOf(letter));
        }
        HashMap<String, Automaton> automatas = new HashMap<>();
        for (String alphabet : alphabets) {
            automatas.put(alphabet, new Automaton(alphabet, null, null));
        }
        return automatas;
    }

    public static void test1() {
        HashMap<String, Automaton> automata = getAutomata();

        // Construct the regular expression (a+b)*c+(d|e)*
        Automaton a = automata.get("a");
        Automaton b = automata.get("b");
        Automaton c = automata.get("c");
        Automaton d = automata.get("d");
        Automaton e = automata.get("e");

        Automaton aUb = utilities.union(a, b, null);
        Automaton aUbK = utilities.kleeneClosure(aUb, null);
        Automaton cP = utilities.positiveClosure(c, null);
        Automaton dUe = utilities.union(d, e, null);
        Automaton dUeK = utilities.kleeneClosure(dUe, null);

        Automaton aUbKcP = utilities.concatenate(aUbK, cP, null);
        Automaton regex = utilities.concatenate(aUbKcP, dUeK, null);

        System.out.println(regex.toString());
        System.out.println(regex.getTokens());


        Automaton dfa = conversion.convertToDFA(regex);
        System.out.println(dfa.toString());
        System.out.println(dfa.getTokens());

        Automaton mDFA = conversion.minimizeDFA(dfa);
        System.out.println(mDFA.toString());
        System.out.println(mDFA.getTokens());
    }

    private static void test2() {
        HashMap<String, Automaton> automata = getAutomata();
        Automaton aK = utilities.positiveClosure(automata.get("a"), null);
        Automaton bK = utilities.positiveClosure(automata.get("b"), null);
        Automaton aUbK = utilities.kleeneClosure(utilities.union(automata.get("a"),
                automata.get("b"), null), null);
        Automaton aKbKaUbK = utilities.concatenate(utilities.concatenate(aK, bK,
                null), aUbK, null);
        System.out.println(aKbKaUbK.toString());
        System.out.println(aKbKaUbK.getTokens());
        Automaton dfa = conversion.convertToDFA(aKbKaUbK);
        System.out.println(dfa.toString());
        System.out.println(dfa.getTokens());
        Automaton mDFA = conversion.minimizeDFA(dfa);
        System.out.println(mDFA.toString());
        System.out.println(mDFA.getTokens());

    }
}
