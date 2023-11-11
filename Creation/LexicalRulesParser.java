package LexicalAnalysisGenerator.Creation;


import LexicalAnalysisGenerator.Automaton.Automaton;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;


public class LexicalRulesParser {
    private final String epsilonSymbol = "\\L";
    private final Parsing parsing;

    public LexicalRulesParser() {
        this.parsing = new Parsing();
    }

    public Map<String, Automaton> parseFile(String filename) throws IOException {
        Map<String, Automaton> automata = new HashMap<>();
        BufferedReader reader = new BufferedReader(new FileReader(filename));
        String line;
        while ((line = reader.readLine()) != null) {
            line = line.trim();
            String non_terminal = line.split("\\s+")[0];
            boolean isRegularDefinition = non_terminal.toCharArray()[non_terminal.length() - 1] == ':';

            String s = line.substring(1, line.length() - 1).trim();
            if (line.startsWith("{")) { // done
                // These are keywords
                for (String keyword : s.split("\\s+")) {
                    Automaton a = parsing.regexToMinimizedDFA(keyword, epsilonSymbol);
                    a.setRegex(a.getToken());
                    a.setTokenAll(keyword);
                    automata.put(keyword, a);
                }
            } else if (line.startsWith("[")) {
                // These are punctuation
                for (String punctuation : s.split("\\s+")) {
                    Automaton a = parsing.regexToMinimizedDFA(punctuation, epsilonSymbol);
                    a.setRegex(a.getToken());
                    a.setTokenAll(punctuation);
                    automata.put(punctuation, a);
                }
            } else if (isRegularDefinition) {
                // This is a regular definition
                String[] parts = line.split(":");
                String name = parts[0].trim();
                String rd = parts[1].trim().replaceAll("\\s+", "");
                Automaton a = parsing.parseRegularDefinition(rd, automata, epsilonSymbol);
                a.setRegex(a.getToken());
                a.setTokenAll(name);
                automata.put(name, a);
            } else if (line.contains("=")) {
                // This is a regular definition
                String[] parts = line.split("=");
                String name = parts[0].trim();
                String regex = parts[1].trim().replaceAll("\\s+", "");
                Automaton a = parsing.regexToMinimizedDFA(regex, epsilonSymbol);
                a.setRegex(a.getToken());
                a.setToken(name);
                automata.put(name, a);
            }
        }
        reader.close();
        return automata;
    }
}
