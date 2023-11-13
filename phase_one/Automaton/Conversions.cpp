//#include "Conversions.h"
//
//Conversions::Conversions() : counter(0), utilities() {}
//
//void Conversions::prepareForAutomaton(Automaton &a) {
//    epsilonClosures.clear();
//    this->counter = static_cast<int>(a.getStates().size()) + 1;
//}
//
//std::unordered_set<std::shared_ptr<State>> Conversions::epsilonClosure(Automaton &a, std::shared_ptr<State> state) {
//    if (epsilonClosures.find(state) != epsilonClosures.end()) {
//        return epsilonClosures[state];
//    }
//    std::unordered_set<std::shared_ptr<State>> epsilonClosure;
//    std::stack<std::shared_ptr<State>> stack;
//    stack.push(state);
//
//    while (!stack.empty()) {
//        std::shared_ptr<State> currentState = stack.top();
//        stack.pop();
//        epsilonClosure.insert(currentState);
//
//        for (auto nextState: a.getNextStates(currentState, a.getEpsilonSymbol())) {
//            if (epsilonClosure.find(nextState) == epsilonClosure.end()) {
//                stack.push(nextState);
//            }
//        }
//    }
//    epsilonClosures[state] = epsilonClosure;
//    return epsilonClosure;
//}
//
//Automaton* Conversions::removeEpsilonTransitions(Automaton &automaton) {
//    Automaton a = utilities.copyAutomaton(automaton);
//    Automaton* nfa = new Automaton();
//
//    nfa->setEpsilonSymbol(a.getEpsilonSymbol());
//    nfa->getStates().insert(a.getStates().begin(), a.getStates().end());
//    nfa->getAlphabets().insert(a.getAlphabets().begin(), a.getAlphabets().end());
//    nfa->setStart(a.getStart());
//    nfa->getAccepting().insert(a.getAccepting().begin(), a.getAccepting().end());
//
//    this->prepareForAutomaton(a);
//
//    for (auto &state : a.getStates()) {
//        for (auto &alphabet : a.getAlphabets()) {
//            if (!alphabet.equals(a.getEpsilonSymbol())) {
//                std::unordered_set<std::shared_ptr<State>> x = epsilonClosure(a, state);
//                std::unordered_set<std::shared_ptr<State>> y;
//                for (auto &s : x) {
//                    y.insert(a.getNextStates(s, alphabet).begin(), a.getNextStates(s, alphabet).end());
//                }
//                std::unordered_set<std::shared_ptr<State>> z;
//                for (auto &s : y) {
//                    z.insert(epsilonClosure(a, s).begin(), epsilonClosure(a, s).end());
//                }
//                nfa->addTransitions(state, alphabet, z);
//            }
//        }
//    }
//
//    nfa->giveNewIdsAll(nfa->getStates(), true);
//
//    return nfa;
//}
//
//Automaton* Conversions::convertToDFA(Automaton &automaton) {
//    Automaton a = utilities.copyAutomaton(automaton);
//    Automaton* dfa = new Automaton();
//
//    dfa->getAlphabets().insert(a.getAlphabets().begin(), a.getAlphabets().end());
//    dfa->setEpsilonSymbol(a.getEpsilonSymbol());
//
//    std::unordered_map<std::unordered_set<std::shared_ptr<State>>, std::shared_ptr<State>> dfaStates;
//    std::queue<std::unordered_set<std::shared_ptr<State>>> queue;
//
//    this->prepareForAutomaton(a);
//
//    std::unordered_set<std::shared_ptr<State>> startSet = epsilonClosure(a, a.getStart());
//    queue.push(startSet);
//
//    bool startIsSet = false;
//
//    while (!queue.empty()) {
//        std::unordered_set<std::shared_ptr<State>> currentSet = queue.front();
//        queue.pop();
//        std::shared_ptr<State> dfaState = getDFAState(currentSet, dfaStates);
//        if (dfaState == nullptr) {
//            dfaState = createDFAState(currentSet, a, dfa);
//            dfaStates[currentSet] = dfaState;
//        }
//
//        if (!startIsSet && currentSet.find(a.getStart()) != currentSet.end()) {
//            dfa->setStart(dfaState);
//            startIsSet = true;
//        }
//
//        for (auto &alphabet : a.getAlphabets()) {
//            if (!alphabet.equals(a.getEpsilonSymbol())) {
//                std::unordered_set<std::shared_ptr<State>> nextSet;
//                for (auto &s : currentSet) {
//                    nextSet.insert(a.getNextStates(s, alphabet).begin(), a.getNextStates(s, alphabet).end());
//                }
//                std::unordered_set<std::shared_ptr<State>> epsilonClosureSet;
//                for (auto &s : nextSet) {
//                    epsilonClosureSet.insert(epsilonClosure(a, s).begin(), epsilonClosure(a, s).end());
//                }
//                std::shared_ptr<State> nextState;
//                if (epsilonClosures.empty()) {
//                    nextState = getDFAState(epsilonClosureSet, dfaStates);
//                    if (nextState == nullptr) {
//                        nextState = createDeadState(dfa);
//                    }
//                } else {
//                    nextState = getDFAState(epsilonClosureSet, dfaStates);
//                    if (nextState == nullptr) {
//                        nextState = createDFAState(epsilonClosureSet, a, dfa);
//                    }
//                }
//                dfa->addTransitions(dfaState, alphabet, {nextState});
//                if (dfaStates.find(epsilonClosureSet) == dfaStates.end()) {
//                    queue.push(epsilonClosureSet);
//                }
//                dfaStates[epsilonClosureSet] = nextState;
//            }
//        }
//    }
//
//    dfa->giveNewIdsAll(dfa->getStates(), true);
//
//    return dfa;
//}
//
//
//Automaton* Conversions::minimizeDFA(Automaton &automaton) {
//    Automaton dfa = utilities.copyAutomaton(automaton);
//
//    std::vector<std::unordered_set<std::shared_ptr<State>>> currentGroup;
//    currentGroup.push_back(dfa.getAccepting());
//    std::unordered_set<std::shared_ptr<State>> nonAcceptingStates(dfa.getStates());
//    for (auto &state : dfa.getAccepting()) {
//        nonAcceptingStates.erase(state);
//    }
//    currentGroup.push_back(nonAcceptingStates);
//
//    while (true) {
//        std::vector<std::unordered_set<std::shared_ptr<State>>> nextGroup = getNextEquivalence(currentGroup, dfa.getAlphabets(), dfa.getTransitionsDFAFormat());
//        if (currentGroup == nextGroup) {
//            break;
//        }
//        currentGroup = nextGroup;
//    }
//
//    Automaton* minDFA = new Automaton();
//    minDFA->setEpsilonSymbol(dfa.getEpsilonSymbol());
//    minDFA->getAlphabets().insert(dfa.getAlphabets().begin(), dfa.getAlphabets().end());
//
//    auto statesData = getNewStatesAndSpecialStates(currentGroup, dfa);
//    minDFA->getStates().insert(statesData.first.begin(), statesData.first.end());
//    minDFA->setStart(statesData.second.first);
//    minDFA->getAccepting().insert(statesData.second.second.begin(), statesData.second.second.end());
//
//    minDFA->setTransitionsDFAFormat(getNewTransitions(dfa, currentGroup, statesData.first));
//
//    minDFA->giveNewIdsAll(minDFA->getStates(), true);
//
//    return minDFA;
//}
//
//std::shared_ptr<State> Conversions::createDFAState(std::unordered_set<std::shared_ptr<State>> &stateSet, Automaton &a, Automaton &dfa) {
//    auto representativeState = *stateSet.begin();
//    dfa.getStates().insert(representativeState);
//
//    if (utilities.containsAcceptingState(stateSet, a)) {
//        representativeState->setAccepting(true);
//        representativeState->setTokenName(a.getAccepting().begin()->get()->getTokenName());
//        dfa.getAccepting().insert(representativeState);
//    }
//
//    return representativeState;
//}
//
//std::shared_ptr<State> Conversions::getDFAState(std::unordered_set<std::shared_ptr<State>> &stateSet, std::unordered_map<std::unordered_set<std::shared_ptr<State>>, std::shared_ptr<State>> &dfaStates) {
//    if (dfaStates.find(stateSet) == dfaStates.end()) {
//        return nullptr;
//    }
//    return dfaStates[stateSet];
//}
//
//
//std::vector<std::unordered_set<std::shared_ptr<State>>>
//Conversions::getNextEquivalence(std::vector<std::unordered_set<std::shared_ptr<State>>> &prevEquivalence,
//                                std::unordered_set<std::string> &alphabets,
//                                std::unordered_map<std::pair<std::shared_ptr<State>, std::string>, std::shared_ptr<State>> &transitions) {
//    std::vector<std::unordered_set<std::shared_ptr<State>>> nextEquivalence;
//
//    for (auto &group: prevEquivalence) {
//        std::unordered_map<std::unordered_set<std::shared_ptr<State>>, std::unordered_set<std::shared_ptr<State>>> newGroups;
//        for (auto &state: group) {
//            std::unordered_set<std::shared_ptr<State>> key;
//            for (auto &symbol: alphabets) {
//                if (!symbol.equals(a.getEpsilonSymbol())) {
//                    auto nextState = transitions[std::make_pair(state, symbol)];
//                    for (auto &g: prevEquivalence) {
//                        if (g.find(nextState) != g.end()) {
//                            key.insert(g.begin()->get());
//                            break;
//                        }
//                    }
//                }
//            }
//            if (newGroups.find(key) == newGroups.end()) {
//                newGroups[key] = std::unordered_set<std::shared_ptr<State>>();
//            }
//            newGroups[key].insert(state);
//        }
//        for (auto &newGroup: newGroups) {
//            nextEquivalence.push_back(newGroup.second);
//        }
//    }
//
//    return nextEquivalence;
//}
//
//std::shared_ptr<State> Conversions::createDeadState(Automaton &dfa) {
//    auto deadState = std::make_shared<State>(++this->counter, false, "");
//    dfa.getStates().insert(deadState);
//    for (auto &alphabet: dfa.getAlphabets()) {
//        dfa.addTransitions(deadState, alphabet, {deadState});
//    }
//    return deadState;
//}
//
//std::pair<std::vector<std::shared_ptr<State>>, std::pair<std::shared_ptr<State>, std::unordered_set<std::shared_ptr<State>>>>
//Conversions::getNewStatesAndSpecialStates(std::vector<std::unordered_set<std::shared_ptr<State>>> &group,
//                                          Automaton &automaton) {
//    std::vector<std::shared_ptr<State>> newStates;
//    std::shared_ptr<State> newStartState = nullptr;
//    std::unordered_set<std::shared_ptr<State>> newAcceptingStates;
//
//    int i = 0;
//    for (auto &g: group) {
//        auto representativeState = g.begin()->get();
//        newStates.push_back(representativeState);
//
//        if (g.find(automaton.getStart()) != g.end()) {
//            newStartState = representativeState;
//        }
//
//        for (auto &oldState: g) {
//            if (automaton.getAccepting().find(oldState) != automaton.getAccepting().end()) {
//                newAcceptingStates.insert(representativeState);
//                break;
//            }
//        }
//        i++;
//    }
//
//    return std::make_pair(newStates, std::make_pair(newStartState, newAcceptingStates));
//}
//
//std::unordered_map<std::pair<std::shared_ptr<State>, std::string>, std::shared_ptr<State>>
//Conversions::getNewTransitions(Automaton &oldDFA, std::vector<std::unordered_set<std::shared_ptr<State>>> &group,
//                               std::vector<std::shared_ptr<State>> &newStates) {
//    std::unordered_map<std::pair<std::shared_ptr<State>, std::string>, std::shared_ptr<State>> newTransitions;
//    std::unordered_map<std::shared_ptr<State>, std::shared_ptr<State>> stateToRepresentative;
//
//    int i = 0;
//    for (auto &g: group) {
//        for (auto &oldState: g) {
//            stateToRepresentative[oldState] = newStates[i];
//        }
//        i++;
//    }
//
//    for (auto &newState: newStates) {
//        for (auto &symbol: oldDFA.getAlphabets()) {
//            auto key = std::make_pair(newState, symbol);
//            newTransitions[key] = stateToRepresentative[oldDFA.getTransitionsDFAFormat()[key]];
//        }
//    }
//
//    return newTransitions;
//}
//
