//
// Created by youssef on 31.03.22.
//

#include "NFA.h"
#include "DFA.h"
#include <fstream>

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

void NFA::fileToNFA(string filename) {
    // accepted file: list of words (one word per line) (look at Abbreviations.txt)
    ifstream file;
    file.open(filename);

    tuple<string, bool, bool> startState = tuple<string,bool,bool> ("starting", true, false);
    states.push_back(startState);
    string line;
    int lettercount = 0;
    alphabet = {".", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};
    while (getline(file, line)) {
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
        cout << line << endl;
        for (int i = 0; i < line.size(); i++) {
            line[i] = tolower(line[i]);
            lettercount += 1;
            if (i == 0) {
                auto beginState = tuple<string,bool,bool> (to_string(lettercount), false, false);
                //auto beginState = tuple<string,bool,bool> (string(1,line[i]), false, false);
                states.push_back(beginState);
                auto transition = tuple<string,string,string> ("starting",to_string(lettercount), string(1,line[0]));
                //auto transition = tuple<string,string,string> ("starting",string(1,line[i]), string(1,line[i]));
                transitions.push_back(transition);
            }
            else if (i == line.size()-1) {
                auto finalState = tuple<string,bool,bool> (to_string(lettercount), false, true);
                //auto finalState = tuple<string,bool,bool> (string(1,line[i]), false, true);
                states.push_back(finalState);
                auto transition = tuple<string,string,string> (to_string(lettercount-1),to_string(lettercount), string(1,line[i]));
                //auto transition = tuple<string,string,string> (string(1,line[i-1]),string(1,line[i]), string(1,line[i]));
                transitions.push_back(transition);
            }
            else {
                // middle states
                auto state = tuple<string,bool,bool> (to_string(lettercount), false, false);
                states.push_back(state);
                auto transition = tuple<string,string,string> (to_string(lettercount-1),to_string(lettercount), string(1,line[i]));
                transitions.push_back(transition);
            }
        }
    }
    cout << endl;
}

DFA NFA::toDFA() {
    DFA dfa;
    dfa.setAlphabet(alphabet);
    tuple<string,bool,bool> currentState;
    vector<tuple<string,bool,bool>> newStates;
    vector<tuple<string,bool,bool>> statesToCheck;
    vector<tuple<string,string,string>> newTransitions;
    for (auto i:states) {
        if(get<1>(i)) { // get start state
            currentState = tuple<string,bool,bool>("{" + get<0>(i) + "}", get<1>(i), get<2>(i));
            break;
        }
    }
    newStates.push_back(currentState);
    statesToCheck.push_back(currentState);
    while (!statesToCheck.empty()) {
        currentState = statesToCheck[0];
        for (const auto& j:alphabet) {
            vector<string> names;
            string fullName = get<0>(currentState).substr(1, get<0>(currentState).size()-2);
            vector<string> stateNames = {};
            while (fullName.find(',') != string::npos) {
                auto pos = fullName.find(',');
                string name = fullName.substr(0, pos);
                stateNames.push_back(name);
                fullName = fullName.substr(pos+1, fullName.size()-1);
            }
            stateNames.push_back(fullName);
            for (auto i:stateNames) {
                for (auto k: transitions) {
                    if (get<0>(k) == i and get<2>(k) == j) {
                        int check = 0;
                        for (auto name: names) {
                            if (name == get<1>(k)) {
                                check = 1;
                            }
                        }
                        if (check == 0) {
                            names.push_back(get<1>(k));
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
                for (string name: names)
                if (name == get<0>(i) and get<2>(i)) {
                    accepting = true;
                    break;
                }
            }
            tuple<string,bool,bool> newstate (ns,false,accepting);
            bool CheckCopy = false;
            for (const auto& i2:newStates) {
                if (i2 == newstate) {
                    CheckCopy = true;
                    break;
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

/*
/*
     * Functie:
     *      -> schrijf naar een json file, creeer DFA met de json
     *
     * Subset Construction:
     *      -> elke state moet een transitie hebben voor elk input symbool uit het alphabet
     *      -> elke state mag maar 1 keer een input symbool gebruiken

/// Write DFA ///
// construction
string newDFA = """{\n\t\"type\": \"DFA\",\n""" ;
/*
 * alphabet
 *
newDFA += """\t\"alphabet\": [\n""";
for (int i = 0; i < alphabet.size(); i++) {
newDFA += "\t\t\"" + alphabet[i] +  "\"";
if (alphabet.size() > 1 && i != alphabet.size() - 1) {
newDFA += ",";
}
newDFA += "\n";
}
newDFA += """\t],\n""";
/*
 * states
 *
newDFA += """\t\"states\": [\n""";

for (tuple<string, bool, bool> s : states) {
bool starting = get<1>(s);

bool accepting = get<2>(s);

string name = get<0>(s);

newDFA += """\t\t{\n""";
newDFA += "\t\t\t\"name\": \"" + name + "\",\n";
if (starting) {newDFA += "\t\t\t\"starting\": true,\n";}
else {newDFA += "\t\t\t\"starting\": false,\n";}
if (accepting) {newDFA += "\t\t\t\"accepting\": true\n";}
else {newDFA += "\t\t\t\"accepting\": false\n";}
newDFA += "\t\t},\n";
}
newDFA.erase(newDFA.size()-2);
newDFA += "\n\t],\n";

/*
 * transitions
 *
newDFA += "\t\"transitions\": [\n";
for (tuple<string,bool,bool> s : states) {
string fromName = get<0>(s);
for (string symbol : alphabet) {
string toName = "";
for( int i = 0; i < transitions.size(); i++) {
if (get<0>(transitions[i]) == fromName && get<2>(transitions[i]) == symbol) {
if (toName != "") {
toName += ", ";
}
toName += get<1>(transitions[i]);
}
}
if (toName == "") continue;
newDFA += """\t\t{\n""";
newDFA += "\t\t\t\"from\": \"" + fromName + "\",\n";
newDFA += "\t\t\t\"to\": \"" + toName + "\",\n";
newDFA += "\t\t\t\"input\": \"" + symbol + "\"\n";
newDFA += """\t\t},\n""";
}
}
newDFA.erase(newDFA.size()-2);
newDFA += "\n\t]\n}";

/// Write to file ///
ofstream newDFAFile("SubsetConstruction.json");

newDFAFile << newDFA;

newDFAFile.close();

/// create DFA ///
return DFA("SubsetConstruction.json");
 */

NFA::NFA() {}
