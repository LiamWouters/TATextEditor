//
// Created by youssef on 16.02.22.
//

#include "DFA.h"

DFA::DFA() {

}

DFA::DFA(string s) {
    ifstream input(s);
    json j;
    input >> j;
    for (auto i:j["alphabet"]) alphabet.push_back(i);
    for (auto i:j["states"])
        states.emplace_back(i["name"],i["starting"],i["accepting"]);
    for (auto i:j["transitions"])
        transitions.emplace_back(i["from"],i["to"],i["input"]);
}

bool DFA::accepts(string s) {
    tuple<string,bool,bool> state;
    bool b = true;
    for (auto j:s){
        if (!b) {
            //cout << "fout alfabet" << endl;
            return false;
        }
        for(auto i:alphabet) {
            b = false;
            if (i == string(1,j)) {
                b = true;
                break;
            }
        }
    }
    for(auto i:states) if (get<1>(i)) state = i;
    for (auto i:s){
        string a(1,i);
        for (auto j:transitions){
            if (a == "stop") break;
            if (get<0>(j) == get<0>(state)){
                for (auto z:states) {
                    if (get<1>(j) == get<0>(z) and a == get<2>(j)) {
                        state = z;
                        a = "stop";
                    }
                }
            }
        }
    }
    return get<2>(state);
}

void DFA::print() {
    json j;
    json type;
    json alph;
    json sts;
    json trans;
    type = "DFA";
    for (auto i:alphabet) {
        alph.push_back(i);
    }
    for (auto i:states) {
        json a;
        a["name"] = get<0>(i);
        a["starting"] = get<1>(i);
        a["accepting"] = get<2>(i);
        sts.push_back(a);
    }
    for (auto i:transitions) {
        json a;
        a["from"] = get<0>(i);
        a["to"] = get<1>(i);
        a["input"] = get<2>(i);
        trans.push_back(a);
    }
    j["type"] = type;
    j["alphabet"] = alph;
    j["states"] = sts;
    j["transitions"] = trans;
    cout << setw(4) << j << endl;
}

void DFA::printToFile(string filename) {
    json j;
    json type;
    json alph;
    json sts;
    json trans;
    type = "DFA";
    for (auto i:alphabet) {
        alph.push_back(i);
    }
    for (auto i:states) {
        json a;
        a["name"] = get<0>(i);
        a["starting"] = get<1>(i);
        a["accepting"] = get<2>(i);
        sts.push_back(a);
    }
    for (auto i:transitions) {
        json a;
        a["from"] = get<0>(i);
        a["to"] = get<1>(i);
        a["input"] = get<2>(i);
        trans.push_back(a);
    }
    j["type"] = type;
    j["alphabet"] = alph;
    j["states"] = sts;
    j["transitions"] = trans;

    ofstream outputFile;
    outputFile.open("../SavedAutomata/"+ filename +".json");
    outputFile << setw(4) << j;
    outputFile.close();
}

RE DFA::toRE() {
    string re;
    string tre;
    vector<tuple<string, string, string>> reTransitions = transToRe();
    vector<tuple<string, string, string>> newTransitions;
    string snaam;
    string expression;
    for (auto state:states) {
        if (get<1>(state)) {
            snaam = get<0>(state);
        }
    }
    for (auto state: states) {
        string naam = get<0>(state);
        bool Fstate = get<2>(state);
        if (Fstate) {
            vector<string> toElim;
            string start;
            for (auto elim:states) {
                string enaam = get<0>(elim);
                bool starting = get<1>(elim);
                if (starting) {
                    start = get<0>(elim);
                }
                if (enaam != naam and !starting) {
                    toElim.push_back(enaam);
                }
            }
            sort(toElim.begin(), toElim.end());
            while (!toElim.empty()) {
                vector<string> elimcopy = toElim;
                elimcopy.push_back(naam);
                elimcopy.push_back(start);
                vector<tuple<string,string>> elimTo;
                vector<tuple<string,string>> elimFrom;
                for (auto trans:reTransitions) {
                    string from = get<0>(trans);
                    string to = get<1>(trans);
                    string symbol = get<2>(trans);
                    if (from == toElim[0]) {
                        elimTo.emplace_back(to, symbol);
                    }
                    if (to == toElim[0] and to != from) {
                        elimFrom.emplace_back(from, symbol);
                    }
                }
                string star;
                for (auto trans:reTransitions) {
                    if (get<0>(trans) == toElim[0] and get<1>(trans) == toElim[0]) {
                        star = get<2>(trans);
                    }
                }
                for (auto from:elimFrom) {
                    for (auto to:elimTo) {
                        string fr = get<0>(from);
                        string t = get<0>(to);
                        string regex1;
                        bool first = true;
                        if (t != toElim[0] and fr != toElim[0]) {
                            for (auto trans:reTransitions) {
                                if (get<0>(trans) == get<0>(from) and get<0>(to) == get<1>(trans)) {
                                    if (!first) {
                                        regex1 += "+" + get<2>(trans);
                                    }
                                    else {
                                        regex1 = get<2>(trans);
                                        first = false;
                                    }
                                }
                            }
                            string regex = get<1>(from) + star;
                            bool plus = false;
                            for (auto i: get<1>(to)) {
                                if (i == '+') {
                                    plus = true;
                                }
                            }
                            if (plus) {
                                regex += "(" + get<1>(to) + ")";
                            }
                            else {
                                regex += get<1>(to);
                            }
                            string newReg;
                            if (regex1.empty()) {
                                newReg = regex;
                            }
                            else {
                                newReg = regex1 +"+"+regex;
                            }
                            newTransitions.emplace_back(fr,t,newReg);
                        }
                    }
                }
                for (auto trans:reTransitions) {
                    bool insert = false;
                    for (const auto& item:elimcopy) {
                        if (item == get<0>(trans)) {
                            insert = true;
                            break;
                        }
                    }
                    for (auto t:newTransitions) {
                        if (get<0>(t) == get<0>(trans) and get<1>(t) == get<1>(trans)) {
                            insert = false;
                            break;
                        }
                    }

                    if (get<0>(trans) == toElim[0] or get<1>(trans) == toElim[0]) {
                        insert = false;
                    }
                    if (insert) {
                        string from = get<0>(trans);
                        string to = get<1>(trans);
                        string symb = get<2>(trans);
                        newTransitions.emplace_back(trans);
                    }
                }
                reTransitions = newTransitions;
                newTransitions.clear();
                toElim.erase(toElim.begin());
            }
            string R;
            string S;
            string T;
            string U;
            string reg;
            for (auto trans:reTransitions) {
                string from = get<0>(trans);
                string to = get<1>(trans);
                string r = get<2>(trans);
                if (from == to and from == snaam) {
                    R = r;
                }
                if (from == snaam and from != to) {
                    S = r;
                }
                if (to == snaam  and from != to) {
                    U = r;
                }
                if (from == to and to != snaam) {
                    U = r;
                }
            }
            if (R.empty() and (!S.empty() and !U.empty() and T.empty())) {
                reg = "("+S+")" + U;
            }
            else if (!R.empty() and (!S.empty() and !U.empty() and T.empty())) {
                reg = "("+R+")*"+S+U;
            }
            else if (S.empty() or U.empty()) {
                reg = R+"*";
            }
            else if (!R.empty() and (!S.empty() and !U.empty() and !T.empty())) {
                reg = "("+R+"+"+"("+S+")"+U+"*"+T+")"+"*"+S+U+"*";
            }
            if (expression.empty()) {
                expression += reg;
            }
            else {
                expression += "+" + reg;
            }
        }
    }
    RE finalre(expression,'e');
    return finalre;
}

DFA DFA::minimize() {
    DFA mindfa;
    mindfa.alphabet = alphabet;
    vector<tuple<string,string,bool>> table;
    vector<string> statenames;
    for (auto state:states) {
        statenames.push_back(get<0>(state));
    }
    int kolom = 1;
    sort(statenames.begin(),statenames.end());
    for (int i = 1; i < statenames.size(); ++i) {
        for (int j = 0; j < kolom; ++j) {
            table.emplace_back(statenames[i], statenames[j],false);
        }
        kolom++;
    }
    for (int i = 0; i < table.size(); ++i) {
        if (!get<2>(table[i])) {
            bool state1;
            bool state2;
            for (auto state:states) {
                if (get<0>(state) == get<0>(table[i])) {
                    state1 = get<2>(state);
                }
                if (get<0>(state) ==  get<1>(table[i])) {
                    state2 = get<2>(state);
                }
            }
            if ((!state1 and state2) or (state1 and !state2)) {
                table[i] = tuple<string,string,bool> (get<0>(table[i]), get<1>(table[i]),true);
            }
        }
    }
    bool change = true;
    while (change) {
        change = false;
        for (const auto& letter:alphabet) {
            for (int i = 0; i < table.size(); ++i) {
                string state1 = get<0>(table[i]);
                string state2 = get<1>(table[i]);
                if (!get<2>(table[i])) {
                    string s1;
                    string s2;
                    for (auto trans:transitions) {
                        if (get<2>(trans) == letter and get<0>(table[i]) == get<0>(trans)) {
                            s1 = get<1>(trans);
                        }
                        if (get<2>(trans) == letter and get<1>(table[i]) == get<0>(trans)) {
                            s2 = get<1>(trans);
                        }
                        if (!s1.empty() and !s2.empty()) {
                            break;
                        }
                    }
                    bool mark = false;
                    for (auto cell:table) {
                        if ((s1 == get<0>(cell) and s2 == get<1>(cell)) or (s2 == get<0>(cell) and s1 == get<1>(cell))) {
                            mark = get<2>(cell);
                        }
                    }
                    if (mark) {
                        table[i] = tuple<string,string,bool> (get<0>(table[i]), get<1>(table[i]),true);
                        change = true;
                    }
                }
            }
        }
    }
    vector<pair<string,string>> findnewstates;
    for (auto cell:table) {
        if (!get<2>(cell)) {
            findnewstates.emplace_back(get<0>(cell), get<1>(cell));
        }
    }
    vector<string> newstate;
    vector<vector<string>> newstates;
    while (!findnewstates.empty()) {
        newstate = {findnewstates[0].first,findnewstates[0].second};
        findnewstates.erase(findnewstates.begin());
        int j = 0;
        int k = findnewstates.size();
        for (int i = 0; i < k; ++i) {
            bool exists1 = false;
            string name1;
            bool exists2 = false;
            string name2;
            for (auto item:newstate) {
                if (item == findnewstates[i-j].first) {
                    exists1 = true;
                }
                if (item == findnewstates[i-j].second) {
                    exists2 = true;
                }
            }
            if (exists1 and !exists2) {
                newstate.push_back(findnewstates[i-j].second);
            }
            if (!exists1 and exists2) {
                newstate.push_back(findnewstates[i-j].first);
            }
            if (exists1 or exists2) {
                findnewstates.erase(findnewstates.begin()+i-j);
                ++j;
            }
        }
        newstates.push_back(newstate);
    }
    vector<tuple<string,bool,bool>> newStates;
    vector<string> unmerged;
    bool merged = false;
    for (auto state:states) {
        merged = false;
        for (auto item:newstates) {
            for (auto i:item) {
                if (i == get<0>(state)) {
                    merged = true;
                }
            }
        }
        if (!merged) {
            newStates.push_back(tuple<string,bool,bool> ("{"+get<0>(state)+"}", get<1>(state), get<2>(state)));
            unmerged.push_back(get<0>(state));
        }
    }
    for (int i = 0; i < newstates.size(); ++i) {
        sort(newstates[i].begin(),newstates[i].end());
    }
    vector<tuple<string,string,string>> newTransitions;
    for (auto letter:alphabet) {
        for (auto state:newstates) {
            bool in = false;
            string from = "{";
            string to;
            for (auto trans:transitions) {
                for (auto item:state) {
                    if (item == get<0>(trans)) {
                        in = true;
                    }
                }
                if (get<2>(trans) == letter and in) {
                    from = "{";
                    bool first = true;
                    for (auto letter:state) {
                        if (first) {
                            from += letter;
                            first = false;
                        }
                        else {
                            from += ", " + letter;
                        }
                    }
                    from += "}";
                    in = false;
                    for (auto i:newstates) {
                        for (auto j:i) {
                            if (get<1>(trans) == j) {
                                in = true;
                            }
                        }
                        if (in) {
                            to = "{";
                            first = true;
                            for (auto l:i) {
                                if (first) {
                                    to += l;
                                    first = false;
                                }
                                else {
                                    to += ", " + l;
                                }
                            }
                            to += "}";
                            break;
                        }
                    }
                    if (!in) {
                        to = "{"+get<1>(trans)+"}";
                    }
                    break;
                }
            }
            newTransitions.emplace_back(from,to,letter);
        }
    }
    for (auto letter:alphabet) {
        for (auto state:unmerged) {
            string newTo;
            for (auto trans:transitions) {
                if (letter == get<2>(trans) and state == get<0>(trans)) {
                    bool in = false;
                    vector<string> to;
                    for (auto i:newstates) {
                        for (auto item:i) {
                            if (item == get<1>(trans)) {
                                in = true;
                                to = i;
                            }
                        }
                    }

                    if (in) {
                        newTo = "{";
                        bool first = true;
                        for (auto l:to) {
                            if (first) {
                                newTo += l;
                                first = false;
                            }
                            else {
                                newTo += ", " + l;
                            }
                        }
                        newTo += "}";
                    }
                    else {
                        newTo = "{"+get<1>(trans)+"}";
                    }
                    break;
                }
            }
            newTransitions.emplace_back("{"+state+"}",newTo,letter);
        }
    }
    for (auto state:newstates) {
        bool starting = false;
        bool accepting = false;
        for (auto item:state) {
            for (auto i:states) {
                if (item == get<0>(i)) {
                    if (get<1>(i)) {
                        starting = true;
                    }
                    if (get<2>(i)) {
                        accepting = true;
                    }
                }
            }
        }
        string newname = "{";
        bool first = true;
        for (auto letter:state) {
            if (first) {
                newname += letter;
                first = false;
            }
            else {
                newname += ", " + letter;
            }
        }
        newname += "}";
        newStates.emplace_back(newname,starting,accepting);
    }
    mindfa.states = newStates;
    mindfa.transitions = newTransitions;
    return mindfa;
}

const vector<tuple<string, bool, bool>> &DFA::getStates() const {
    return states;
}

void DFA::setStates(const vector<tuple<string, bool, bool>> &states) {
    DFA::states = states;
}

const vector<tuple<string, string, string>> &DFA::getTransitions() const {
    return transitions;
}

void DFA::setTransitions(const vector<tuple<string, string, string>> &transitions) {
    DFA::transitions = transitions;
}

const vector<string> &DFA::getAlphabet() const {
    return alphabet;
}

void DFA::setAlphabet(const vector<string> &alphabet) {
    DFA::alphabet = alphabet;
}

DFA::DFA(const DFA& dfa1,const DFA& dfa2, bool product) {
    alphabet = dfa1.getAlphabet();
    tuple<string,bool,bool> currentState;
    vector<tuple<string,bool,bool>> newStates;
    vector<tuple<string,bool,bool>> statesToCheck;
    vector<tuple<string,string>> names;
    vector<tuple<string,string,string>> newTransitions;
    string name1;
    string name2;
    bool accepting1;
    bool accepting2;
    for (auto state:dfa1.getStates()) {
        if (get<1>(state)) {
            name1 = get<0>(state);
            accepting1 = get<2>(state);
        }
    }
    for (auto state:dfa2.getStates()) {
        if (get<1>(state)) {
            name2 = get<0>(state);
            accepting2 = get<2>(state);
        }
    }
    string naam = "(" + name1 + "," + name2 + ")";
    if (!product and (accepting1 or accepting2)) {
        currentState = tuple<string,bool,bool> (naam, true,true);
    }
    else if(product and accepting1 and accepting2) {
        currentState = tuple<string,bool,bool> (naam, true,true);
    }
    else {
        currentState = tuple<string,bool,bool> (naam, true,false);
    }
    newStates.push_back(currentState);
    statesToCheck.push_back(currentState);
    names.emplace_back(name1,name2);
    while (!statesToCheck.empty()) {
        currentState = statesToCheck[0];
        string cname1 = get<0>(names[0]);
        string cname2 = get<1>(names[0]);
        for (const auto& letter:dfa1.getAlphabet()) {
            name1 = cname1;
            name2 = cname2;
            for (auto transition:dfa1.getTransitions()) {
                string from = get<0>(transition);
                string to = get<1>(transition);
                string symbol = get<2>(transition);
                if (from == name1 and symbol == letter) {
                    name1 = to;
                    for (auto st:dfa1.getStates()) {
                        if (name1 == get<0>(st)) {
                            accepting1 = get<2>(st);
                        }
                    }
                    break;
                }
            }
            for (auto transition:dfa2.getTransitions()) {
                string from = get<0>(transition);
                string to = get<1>(transition);
                string symbol = get<2>(transition);
                if (from == name2 and symbol == letter) {
                    name2 = to;
                    for (auto st:dfa2.getStates()) {
                        if (name2 == get<0>(st)) {
                            accepting2 = get<2>(st);
                        }
                    }
                    break;
                }
            }
            naam = "(" + name1 + "," + name2 + ")";
            bool CheckCopy = false;
            for (const auto& st:newStates) {
                if (naam == get<0>(st)) {
                    CheckCopy = true;
                }
            }
            tuple<string,bool,bool> newState;
            if (!CheckCopy) {
                if (!product and (accepting1 or accepting2)) {
                    newState = tuple<string,bool,bool> (naam, false,true);
                }
                else if(product and accepting1 and accepting2) {
                    newState = tuple<string,bool,bool> (naam, false,true);
                }
                else {
                    newState = tuple<string,bool,bool> (naam, false,false);
                }
                newStates.push_back(newState);
                statesToCheck.push_back(newState);
                names.emplace_back(name1,name2);
            }
            newTransitions.emplace_back(get<0>(currentState),naam,letter);
        }
        statesToCheck.erase(statesToCheck.begin());
        names.erase(names.begin());
    }
    states = newStates;
    transitions = newTransitions;
}

vector<tuple<string,string,string>> DFA::transToRe() {
    string tre;
    vector<string> tres;
    vector<tuple<string,string,string>> reTransitions;
    for (auto state:states) {
        string naam = get<0>(state);
        string retrans;
        for (auto st:states) {
            string stnaam = get<0>(st);
            for (auto trans:transitions) {
                string from = get<0>(trans);
                string to = get<1>(trans);
                string symbol = get<2>(trans);
//                eigenlijk ook nog rekening houden of st == state
                if (from == naam and to == stnaam) {
                    tres.push_back(symbol);
                }
            }
            sort(tres.begin(),tres.end());
            if (!tres.empty()) {
                tre = tres[0];
            }
            for (int i = 1; i < tres.size(); ++i) {
                tre += "+" + tres[i];
            }
            if (stnaam == naam and !tres.empty() and tres.size() != 1) {
                tre = "(" + tre + ")*";
            }
            else if (stnaam == naam and tres.size() == 1) {
                tre += '*';
            }
            if (!tres.empty()) {
                reTransitions.emplace_back(naam,stnaam,tre);
                tre = "";
                tres.clear();
            }
        }
    }
    return reTransitions;
}

void DFA::printTable() {
    vector<tuple<string,string,bool>> table;
    vector<string> statenames;
    for (auto state:states) {
        statenames.push_back(get<0>(state));
    }
    int kolom = 1;
    sort(statenames.begin(),statenames.end());
    for (int i = 1; i < statenames.size(); ++i) {
        for (int j = 0; j < kolom; ++j) {
            table.emplace_back(statenames[i], statenames[j],false);
        }
        kolom++;
    }
    bool change = true;
    for (int i = 0; i < table.size(); ++i) {
        if (!get<2>(table[i])) {
            bool state1;
            bool state2;
            for (auto state:states) {
                if (get<0>(state) == get<0>(table[i])) {
                    state1 = get<2>(state);
                }
                if (get<0>(state) ==  get<1>(table[i])) {
                    state2 = get<2>(state);
                }
            }
            if ((!state1 and state2) or (state1 and !state2)) {
                table[i] = tuple<string,string,bool> (get<0>(table[i]), get<1>(table[i]),true);
            }
        }
    }
    while (change) {
        change = false;
        for (const auto& letter:alphabet) {
            for (int i = 0; i < table.size(); ++i) {
                if (!get<2>(table[i])) {
                    string s1;
                    string s2;
                    for (auto trans:transitions) {
                        if (get<2>(trans) == letter and get<0>(table[i]) == get<0>(trans)) {
                            s1 = get<1>(trans);
                        }
                        if (get<2>(trans) == letter and get<1>(table[i]) == get<0>(trans)) {
                            s2 = get<1>(trans);
                        }
                    }
                    bool mark = false;
                    for (auto cell:table) {
                        if ((s1 == get<0>(cell) and s2 == get<1>(cell)) or (s2 == get<0>(cell) and s1 == get<1>(cell))) {
                            mark = get<2>(cell);
                        }
                    }
                    if (mark) {
                        table[i] = tuple<string,string,bool> (get<0>(table[i]), get<1>(table[i]),true);
                        change = true;
                    }
                }
            }
        }
    }
    int cell = 0;
    int rij = 1;
    for (int i = 1; i < statenames.size()+1; ++i) {
        if (i == statenames.size()) {
            cout << "   ";
            for (int j = 0; j < statenames.size()-1; ++j) {
                cout << statenames[j] << "  ";
            }
            cout << endl;
        }
        else {
            cout << statenames[i] << "  ";
            for (int j = 0; j < rij; ++j) {
                if (get<2>(table[cell])) {
                    cout << "x  ";
                }
                else {
                    cout << "-  ";
                }
                ++cell;
            }
            cout << endl;
            ++rij;
        }
    }
}

bool DFA::operator==(DFA dfa) {
    DFA Unie;
    Unie.states = this->states;
    Unie.alphabet = this->alphabet;
    Unie.transitions = this->transitions;
    string state1;
    string state2;
    for (auto state:states) {
        if (get<1>(state)) {
            state1 = get<0>(state);
        }
    }
    for (auto state:dfa.getStates()) {
        if (get<1>(state)) {
            state2 = get<0>(state);
        }
        Unie.states.push_back(state);
    }
    for (const auto& trans:dfa.getTransitions()) {
        Unie.transitions.emplace_back(trans);
    }
    vector<tuple<string,string,bool>> table = Unie.getTable();
    Unie.printTable();
    for (auto cell:table) {
        if ((get<0>(cell) == state1 and get<1>(cell) == state2) or (get<0>(cell) == state2 and get<1>(cell) == state1)) {
            if (!get<2>(cell)) {
                return true;
            }
            else {
                return false;
            }
        }
    }
    return false;
}

vector<tuple<string, string, bool>> DFA::getTable() {
    vector<tuple<string,string,bool>> table;
    vector<string> statenames;
    for (auto state:states) {
        statenames.push_back(get<0>(state));
    }
    int kolom = 1;
    sort(statenames.begin(),statenames.end());
    for (int i = 1; i < statenames.size(); ++i) {
        for (int j = 0; j < kolom; ++j) {
            table.emplace_back(statenames[i], statenames[j],false);
        }
        kolom++;
    }
    bool change = true;
    for (int i = 0; i < table.size(); ++i) {
        if (!get<2>(table[i])) {
            bool state1;
            bool state2;
            for (auto state:states) {
                if (get<0>(state) == get<0>(table[i])) {
                    state1 = get<2>(state);
                }
                if (get<0>(state) ==  get<1>(table[i])) {
                    state2 = get<2>(state);
                }
            }
            if ((!state1 and state2) or (state1 and !state2)) {
                table[i] = tuple<string,string,bool> (get<0>(table[i]), get<1>(table[i]),true);
            }
        }
    }
    while (change) {
        change = false;
        for (const auto& letter:alphabet) {
            for (int i = 0; i < table.size(); ++i) {
                if (!get<2>(table[i])) {
                    string s1;
                    string s2;
                    for (auto trans:transitions) {
                        if (get<2>(trans) == letter and get<0>(table[i]) == get<0>(trans)) {
                            s1 = get<1>(trans);
                        }
                        if (get<2>(trans) == letter and get<1>(table[i]) == get<0>(trans)) {
                            s2 = get<1>(trans);
                        }
                    }
                    bool mark = false;
                    for (auto cell:table) {
                        if ((s1 == get<0>(cell) and s2 == get<1>(cell)) or (s2 == get<0>(cell) and s1 == get<1>(cell))) {
                            mark = get<2>(cell);
                        }
                    }
                    if (mark) {
                        table[i] = tuple<string,string,bool> (get<0>(table[i]), get<1>(table[i]),true);
                        change = true;
                    }
                }
            }
        }
    }
    return table;
}
