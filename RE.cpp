//
// Created by youssef on 02.04.22.
//

#include "ENFA.h"
#include "RE.h"

RE::RE(const string& rx, const char eps) {
    regex = rx;
    epsilon = eps;
    for (auto letter:rx) {
        if (letter != '(' and letter != ')' and letter != '+' and letter != '*' and letter != eps) {
            int check = 0;
            for (auto alph:alphabet) {
                if (alph == letter) {
                    check = 1;
                }
            }
            if (check == 0) {
                alphabet.emplace_back(letter);
            }
        }
    }
}

ENFA RE::toENFA() {
    vector<ENFA> enfas;
    vector<char> ops;
    string subre;
    int conc = 0;
    bool haakCheck = false;
    int aantalHaakjesL = 0;
    int aantalHaakjesR = 0;
    for (int i = 0; i < regex.size(); ++i) {
        if (haakCheck and regex[i] != ')') {
            subre += regex[i];
        }
        if (regex[i] == '(') {
            haakCheck = true;
            aantalHaakjesL += 1;
        }
        if (regex[i] == ')' and aantalHaakjesL == aantalHaakjesR+1) {
            enfas.push_back(RE(subre,epsilon).toENFA());
            subre = "";
            haakCheck = false;
            conc = 1;
        }
        if (regex[i] == ')') {
            aantalHaakjesR += 1;
        }
        if((regex[i] == '+' or regex[i] == '*') and !haakCheck) {
            ops.push_back(regex[i]);
            if (regex[i] == '+') {
                conc = 0;
            }
        }
        else if (regex[i] == epsilon and !haakCheck){
            enfas.push_back(makeBasicENFA(string(1,regex[i]),true));
            conc += 1;
        }
        else if (!haakCheck and regex[i] != '(' and regex[i] != ')'){
            enfas.push_back(makeBasicENFA(string(1,regex[i])));
            conc += 1;
        }
        if ((conc > 1 and !haakCheck) or (conc > 0 and regex[i] == '(')) {
            ops.push_back('.');
        }
    }
    for (auto & enfa : enfas) {
        enfa.setEpsilon(string(1,epsilon));
    }
//    star
    vector<char> newops;
    for (int i = 0; i < ops.size(); ++i) {
        if(ops[i] == '*') {
            enfas[i] = Star(enfas[i]);
        }
        else {
            newops.push_back(ops[i]);
        }
    }
    ops = newops;
    newops = {};
//    concatenation
    int j = 0;
    for (int i = 0; i < ops.size(); ++i) {
        if(ops[i] == '.') {
            enfas.insert(enfas.begin()+j,Concatenation(enfas[j],enfas[j+1]));
            enfas.erase(enfas.begin()+j+1);
            enfas.erase(enfas.begin()+j+1);
        }
        else {
            newops.push_back(ops[i]);
            j++;
        }
    }
    ops = newops;
    newops = {};
//    union
    for (int i = 0; i < ops.size(); ++i) {
        if(ops[i] == '+') {
            enfas.insert(enfas.begin()+0,Union(enfas[0],enfas[1]));
            enfas.erase(enfas.begin()+1);
            enfas.erase(enfas.begin()+1);
        }
    }
    return enfas[0];
}

void RE::print() {
    cout << regex;
}

ENFA makeBasicENFA(string letter, bool eps) {
    ENFA enfa;
    if (eps) {
        enfa.setEpsilon(letter);
    }
    else {
        enfa.setAlphabet({letter});
    }
    enfa.setStates({tuple<string,bool,bool>("0",true,false),tuple<string,bool,bool>("1", false,true)});
    enfa.setTransitions({tuple<string,string,string>("0","1",letter)});
    vector<tuple<string,vector<string>>> newEclose;
    newEclose.push_back(tuple<string,vector<string>>("1",{"1"}));
    if (eps) {
        newEclose.push_back(tuple<string,vector<string>>("0",{"0","1"}));
    }
    else {
        newEclose.push_back(tuple<string,vector<string>>("0",{"0"}));
    }
    enfa.setEclose(newEclose);
    return enfa;
}

ENFA Union(const ENFA& enfa1,const ENFA& enfa2) {
    ENFA enfa;
    enfa.setEpsilon(enfa1.getEpsilon());
    vector<string> newalphabet = enfa1.getAlphabet();
    vector<tuple<string,bool,bool>> newStates;
    vector<tuple<string,string,string>> newTransitions;
    vector<tuple<string,vector<string>>> newEclose;
    tuple<string,bool,bool> newStart ("0",true,false);
    int aantalStates = enfa1.getStates().size() + enfa2.getStates().size() + 2;
    tuple<string,bool,bool> newFinish (to_string(aantalStates-1),false,true);
    newStates.emplace_back(newStart);
    newStates.emplace_back(newFinish);
    int offset = enfa1.getStates().size() + 1;
    for (auto state:enfa1.getStates()) {
        if (get<1>(state)) {
            newTransitions.emplace_back("0",to_string(1),enfa.getEpsilon());
        }
        if (get<2>(state)) {
            newTransitions.emplace_back(to_string(stoi(get<0>(state))+1),
                                        to_string(aantalStates-1),enfa.getEpsilon());
        }
    }
    for (auto state:enfa2.getStates()) {
        if (get<1>(state)) {
            newTransitions.emplace_back("0",to_string(offset),enfa.getEpsilon());
        }
        if (get<2>(state)) {
            newTransitions.emplace_back(to_string(stoi(get<0>(state))+offset),
                                        to_string(aantalStates-1),enfa.getEpsilon());
        }
    }
    for (auto state: enfa1.getStates()) {
        tuple<string,bool,bool> newstate (to_string(stoi(get<0>(state))+1),false, false);
        newStates.emplace_back(newstate);
    }
    for (auto state: enfa2.getStates()) {
        tuple<string,bool,bool> newstate (to_string(stoi(get<0>(state))+offset), false, false);
        newStates.emplace_back(newstate);
    }
    for (auto trans:enfa1.getTransitions()) {
        tuple<string,string,string> newTransition (to_string(stoi(get<0>(trans))+1),
                                                   to_string(stoi(get<1>(trans))+1), get<2>(trans));
        newTransitions.emplace_back(newTransition);
    }
    for (auto trans:enfa2.getTransitions()) {
        tuple<string,string,string> newTransition (to_string(stoi(get<0>(trans))+offset),
                                                   to_string(stoi(get<1>(trans))+offset), get<2>(trans));
        newTransitions.emplace_back(newTransition);
    }
    for (auto &symb:enfa2.getAlphabet()) {
        int check = 0;
        for (auto &alph:enfa1.getAlphabet()) {
            if (symb == alph) {
                check = 1;
            }
        }
        if (check == 0) {
            newalphabet.push_back(symb);
        }
    }
    enfa.setStates(newStates);
    enfa.setTransitions(newTransitions);
    enfa.setAlphabet(newalphabet);
    for (auto i:newStates) {
        vector<string> statename;
        statename.push_back(get<0>(i));
        vector<string> eclose = enfa.getEclose(statename, statename);
        sort(eclose.begin(), eclose.end());
        tuple<string, vector<string>> e(get<0>(i), eclose);
        newEclose.push_back(e);
    }
    enfa.setEclose(newEclose);
    return enfa;
}

ENFA Concatenation(const ENFA &enfa1, const ENFA &enfa2) {
    ENFA enfa;
    enfa.setEpsilon(enfa1.getEpsilon());
    vector<string> newalphabet = enfa1.getAlphabet();
    vector<tuple<string,bool,bool>> newStates;
    vector<tuple<string,string,string>> newTransitions = enfa1.getTransitions();
    vector<tuple<string,vector<string>>> newEclose;
    int aantalStates = enfa1.getStates().size() + enfa2.getStates().size();
    int offset = enfa1.getStates().size();
    for (auto state:enfa1.getStates()) {
        if (get<2>(state)) {
            newTransitions.emplace_back(to_string(stoi(get<0>(state))),
                                        to_string(offset),enfa.getEpsilon());
        }
    }
    for (auto state: enfa1.getStates()) {
        tuple<string,bool,bool> newstate (get<0>(state), get<1>(state), false);
        newStates.emplace_back(newstate);
    }
    for (auto state: enfa2.getStates()) {
        tuple<string,bool,bool> newstate (to_string(stoi(get<0>(state))+offset), false, get<2>(state));
        newStates.emplace_back(newstate);
    }
    for (auto trans:enfa2.getTransitions()) {
        tuple<string,string,string> newTransition (to_string(stoi(get<0>(trans))+offset),
                                                   to_string(stoi(get<1>(trans))+offset), get<2>(trans));
        newTransitions.emplace_back(newTransition);
    }
    for (auto &symb:enfa2.getAlphabet()) {
        int check = 0;
        for (auto &alph:enfa1.getAlphabet()) {
            if (symb == alph) {
                check = 1;
            }
        }
        if (check == 0) {
            newalphabet.push_back(symb);
        }
    }
    enfa.setStates(newStates);
    enfa.setTransitions(newTransitions);
    enfa.setAlphabet(newalphabet);
    for (auto i:newStates) {
        vector<string> statename;
        statename.push_back(get<0>(i));
        vector<string> eclose = enfa.getEclose(statename, statename);
        sort(eclose.begin(), eclose.end());
        tuple<string, vector<string>> e(get<0>(i), eclose);
        newEclose.push_back(e);
    }
    enfa.setEclose(newEclose);
    return enfa;
}

ENFA Star(const ENFA & enfa1) {
    ENFA enfa;
    enfa.setEpsilon(enfa1.getEpsilon());
    enfa.setAlphabet(enfa1.getAlphabet());
    vector<tuple<string,bool,bool>> newStates;
    vector<tuple<string,vector<string>>> newEclose;
    vector<tuple<string,string,string>> newTransitions;
    for (auto trans:enfa1.getTransitions()) {
        tuple<string,string,string> newTransition (to_string(stoi(get<0>(trans))+1),
                                                   to_string(stoi(get<1>(trans))+1), get<2>(trans));
        newTransitions.emplace_back(newTransition);
    }
    for (auto state:enfa1.getStates()) {
        newStates.emplace_back(tuple<string,bool,bool> (to_string(stoi(get<0>(state))+1),false,false));
    }
    tuple<string,bool,bool> newstart ("0",true,false);
    int lastNode = enfa1.getStates().size()+1;
    tuple<string,bool,bool> newFinish (to_string(lastNode),false,true);
    newStates.emplace_back(newstart);
    newStates.emplace_back(newFinish);
    tuple<string,string,string> a ("0","1",enfa.getEpsilon());
    tuple<string,string,string> b ("0", to_string(lastNode),enfa.getEpsilon());
    tuple<string,string,string> c (to_string(lastNode-1), to_string(lastNode),enfa.getEpsilon());
    tuple<string,string,string> d (to_string(lastNode-1),"1",enfa.getEpsilon());
    newTransitions.emplace_back(a);
    newTransitions.emplace_back(b);
    newTransitions.emplace_back(c);
    newTransitions.emplace_back(d);
    enfa.setStates(newStates);
    enfa.setTransitions(newTransitions);
    for (auto i:newStates) {
        vector<string> statename;
        statename.push_back(get<0>(i));
        vector<string> eclose = enfa.getEclose(statename, statename);
        sort(eclose.begin(), eclose.end());
        tuple<string, vector<string>> e(get<0>(i), eclose);
        newEclose.push_back(e);
    }
    enfa.setEclose(newEclose);
    return enfa;
}