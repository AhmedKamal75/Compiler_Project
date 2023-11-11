package LexicalAnalysisGenerator.Automaton;

import guru.nidi.graphviz.attribute.Color;
import guru.nidi.graphviz.attribute.Label;
import guru.nidi.graphviz.engine.Format;
import guru.nidi.graphviz.engine.Graphviz;
import guru.nidi.graphviz.model.Link;
import guru.nidi.graphviz.model.MutableGraph;
import guru.nidi.graphviz.model.Node;

import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;

import static guru.nidi.graphviz.model.Factory.*;

public class AutomatonVisualizer {
    public void visualize(Automaton automaton, String outputPath) throws IOException {
        MutableGraph g = mutGraph("automaton").setDirected(true);
        HashMap<State, Node> nodes = new HashMap<>();

        // Create nodes for all states
        for (State state : automaton.getStates()) {
            Node node = node(String.valueOf(state.getId()));
            if (state.isAccepting()) {
                node = node.with(Color.RED);
            }
            if (state.equals(automaton.getStart())) {
                node = node.with(Color.GREEN);
            }
            nodes.put(state, node);
            g.add(node);
        }

        // Create edges for all transitions
        for (Map.Entry<Pair<State, String>, HashSet<State>> entry : automaton.getTransitions().entrySet()) {
            Pair<State, String> pair = entry.getKey();
            State state = pair.getFirst();
            String alphabet = pair.getSecond();
            HashSet<State> nextStates = entry.getValue();
            for (State nextState : nextStates) {
                Link link = nodes.get(state).linkTo(nodes.get(nextState)).with(Label.of(alphabet));
                g.add(link.asLinkSource());
            }
        }

        // Render the graph to a file
        Graphviz.fromGraph(g).width(800).render(Format.PNG).toFile(new File(outputPath));
    }
}
