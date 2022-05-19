#include "ENFA.h"
#include "DFA.h"

ENFA::ENFA(string Input) {
    ifstream input(Input);
    json j;
    input >> j;
    for (auto i:j["alphabet"]) alphabet.push_back(i);
    for (auto i:j["states"])
        states.emplace_back(i["name"],i["starting"],i["accepting"]);
    for (auto i:j["transitions"])
        transitions.emplace_back(i["from"],i["to"],i["input"]);
    epsilon = j["eps"];
    for (auto i:states) {
        vector<string> statename;
        statename.push_back(get<0>(i));
        vector<string> eclose = getEclose(statename, statename);
        sort(eclose.begin(), eclose.end());
        tuple<string, vector<string>> e(get<0>(i), eclose);
        Eclose.push_back(e);
    }
}

DFA ENFA::toDFA() {
    DFA dfa;
    dfa.setAlphabet(alphabet);
    tuple<string,bool,bool> currentState;
    vector<tuple<string,bool,bool>> newStates;
    vector<tuple<string,bool,bool>> statesToCheck;
    vector<tuple<string,string,string>> newTransitions;
//    Get first/starting state
    for (auto i:states) {
        if(get<1>(i)) {
            string fs = "{";
            vector<string> cfs;
//            Hier veranderen naar komma
            for (auto l:Eclose) {
                if (get<0>(l) == get<0>(i)) {
                    cfs = get<1>(l);
                }
            }
            int komma = 0;
            sort(cfs.begin(),cfs.end());
            for (auto i:cfs) {
                if (komma == 0) {
                    fs += i;
                    komma = 1;
                }
                else {
                    fs += ",";
                    fs += i;
                }
            }
            fs += "}";
            bool accepting = false;
            for (auto i:cfs) {
                for (auto state:states) {
                    if (get<0>(state) == i and get<2>(state)){
                        accepting = true;
                    }
                }
            }
            currentState = tuple<string,bool,bool>(fs, get<1>(i), accepting);
            break;
        }
    }
    newStates.push_back(currentState);
    statesToCheck.push_back(currentState);
    while (!statesToCheck.empty()) {
        currentState =  statesToCheck[0];
        for (const auto& j:alphabet) {
            string subname;
            vector<string> subStates;
            for (auto letter: get<0>(currentState)) {
                if (letter == ',' or letter == '{' or letter == '}') {
                    if (!subname.empty()) {
                        subStates.push_back(subname);
                    }
                    subname = "";
                }
                else {
                    subname += letter;
                }
            }
            vector<string> transities;
            for (auto substate:subStates) {
                for (auto trans:transitions) {
                    if (get<0>(trans) == substate and get<2>(trans) == j) {
                        int check = 0;
                        for (auto name:transities) {
                            if (name == get<1>(trans)) {
                                check = 1;
                            }
                        }
                        if (check == 0) {
                            transities.push_back(get<1>(trans));
                        }
                    }
                }
            }
            vector<string> copyname;
            for (auto closer:transities) {
                for (auto c:Eclose) {
                    if (closer == get<0>(c)) {
                        for (auto n: get<1>(c)) {
                            int check = 0;
                            for (auto z:copyname) {
                                if (z == n) {
                                    check = 1;
                                }
                            }
                            if (check == 0) {
                                copyname.push_back(n);
                            }
                        }
                        break;
                    }
                }
            }
//            make new state name
            int komma = 0;
            string ns = "{";
//            Hier ook veranderen naar komma
            sort(copyname.begin(),copyname.end());
            for (auto i:copyname) {
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
//            Check if accepting
            bool accepting = false;
            for (auto i:states) {
                for (auto k:copyname) {
                    if (k == get<0>(i) and get<2>(i)) {
                        accepting = true;
                        break;
                    }
                }
            }
//            Make the new state and check if it already exists,
//                if it does, only add transition
//                else, also add the state
            tuple<string,bool,bool> newstate (ns,false,accepting);
            bool CheckCopy = false;
            for (const auto& i:newStates) {
                if (get<0>(i) == get<0>(newstate)) {
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

vector<string> ENFA::getEclose(vector<string> &e,const vector<string> &tocheck) {
    vector<string> recursie;
    for (auto i:tocheck) {
        for (auto j:transitions) {
            string from = get<0>(j);
            string to = get<1>(j);
            if(i == from and epsilon == get<2>(j)) {
                int check = 0;
                for (auto k:e) {
                    if (k == to) {
                        check = 1;
                    }
                }
                if (check == 0) {
                    e.push_back(to);
                    recursie.push_back(to);
                }
            }
        }
    }
    if (recursie.empty()) {
        return e;
    }
    else {
        return getEclose(e,recursie);
    }
    return e;
}

ENFA::ENFA() {

}

const vector<tuple<string, bool, bool>> &ENFA::getStates() const {
    return states;
}

void ENFA::setStates(const vector<tuple<string, bool, bool>> &states) {
    ENFA::states = states;
}

const vector<tuple<string, string, string>> &ENFA::getTransitions() const {
    return transitions;
}

void ENFA::setTransitions(const vector<tuple<string, string, string>> &transitions) {
    ENFA::transitions = transitions;
}

const vector<string> &ENFA::getAlphabet() const {
    return alphabet;
}

void ENFA::setAlphabet(const vector<string> &alphabet) {
    ENFA::alphabet = alphabet;
}

const string &ENFA::getEpsilon() const {
    return epsilon;
}

void ENFA::setEpsilon(const string &epsilon) {
    ENFA::epsilon = epsilon;
}

const vector<tuple<string, vector<string>>> &ENFA::getEclose1() const {
    return Eclose;
}

void ENFA::setEclose(const vector<tuple<string, vector<string>>> &eclose) {
    Eclose = eclose;
}

void ENFA::printStats() {
    cout << "no_of_states=" << states.size() << endl;
    sort(alphabet.begin(),alphabet.end());
    int teller = 0;
    for (auto trans:transitions) {
        if (get<2>(trans) == epsilon) {
            teller += 1;
        }
    }
    cout << "no_of_transitions[" << epsilon << "]=" << teller << endl;
    for (auto symb:alphabet) {
        teller = 0;
        for (auto trans:transitions) {
            if (get<2>(trans) == symb) {
                teller += 1;
            }
        }
        cout << "no_of_transitions[" << symb << "]=" << teller << endl;
    }
    vector<int> degrees;
    for (auto state:states) {
        int teller = 0;
        for (auto trans:transitions) {
            if (get<0>(state) == get<0>(trans)) {
                teller += 1;
            }
        }
        degrees.push_back(teller);
    }
    teller = 0;
    while (!degrees.empty()) {
        int aantal = 0;
        for (auto deg:degrees) {
            if (deg == teller) {
                aantal += 1;
            }
        }
        cout << "degree[" << teller << "]=" << aantal << endl;
        vector<int> newdeg;
        for (auto i:degrees) {
            if (i != teller) {
                newdeg.push_back(i);
            }
        }
        degrees = newdeg;
        teller += 1;
    }
}

bool ENFA::accepts(string input) {
    return this->toDFA().accepts(input);
}
