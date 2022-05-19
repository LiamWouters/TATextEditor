//
// Created by youssef on 31.03.22.
//

#include "NFA.h"
#include "DFA.h"

NFA::NFA(string Input) {
    ifstream input(Input);
    json j;
    input >> j;
    for (auto i:j["alphabet"]) alphabet.push_back(i);
    for (auto i:j["states"])
        states.emplace_back(i["name"],i["starting"],i["accepting"]);
    for (auto i:j["transitions"])
        transitions.emplace_back(i["from"],i["to"],i["input"]);
}

DFA NFA::toDFA() {
    DFA dfa;
    dfa.setAlphabet(alphabet);
    tuple<string,bool,bool> currentState;
    vector<tuple<string,bool,bool>> newStates;
    vector<tuple<string,bool,bool>> statesToCheck;
    vector<tuple<string,string,string>> newTransitions;
    for (auto i:states) {
        if(get<1>(i)) {
            currentState = tuple<string,bool,bool>("{" + get<0>(i) + "}", get<1>(i), get<2>(i));
            break;
        }
    }
    newStates.push_back(currentState);
    statesToCheck.push_back(currentState);
    while (!statesToCheck.empty()) {
        currentState =  statesToCheck[0];
        for (const auto& j:alphabet) {
            string names;
            for (auto i: get<0>(currentState)) {
                for (auto k:transitions) {
                    if (get<0>(k) == string(1,i) and get<2>(k) == j) {
                        int check = 0;
                        for (auto i:names) {
                            if (string(1,i) == get<1>(k)) {
                                check = 1;
                            }
                        }
                        if (check == 0) {
                            names += get<1>(k);
                        }
                    }
                }
            }
    //        get new state name
            int komma = 0;
            string ns = "{";
            sort(names.begin(),names.end());
            for (auto i:names) {
                if (komma == 0) {
                    ns += i;
                    komma = 1;
                }
                else {
                    ns += ",";
                    ns += i;
                }
            }
            ns += "}";
    //        get newstate accepting
            bool accepting = false;
            for (auto i:states) {
                for (auto k:ns) {
                    if (string(1,k) == get<0>(i) and get<2>(i)) {
                        accepting = true;
                        break;
                    }
                }
            }
            tuple<string,bool,bool> newstate (ns,false,accepting);
            bool CheckCopy = false;
            for (const auto& i:newStates) {
                if (i == newstate) {
                    CheckCopy = true;
                }
            }
            if (!CheckCopy) {
                newStates.push_back(newstate);
                statesToCheck.push_back(newstate);
            }
            newTransitions.emplace_back(get<0>(currentState), ns,j);
        }
        statesToCheck.erase(statesToCheck.begin());
    }
    dfa.setStates(newStates);
    dfa.setTransitions(newTransitions);
    return dfa;
}