//
// Created by youssef on 16.02.22.
//

#ifndef UNTITLED2_DFA_H
#define UNTITLED2_DFA_H
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <tuple>
#include <vector>
#include "RE.h"
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

class DFA {
private:
    vector<tuple<string,bool,bool>> states;
    vector<tuple<string,string,string>> transitions;
    vector<string> alphabet;
public:
    DFA();

    DFA(const DFA&,const DFA&,bool);

    DFA(string);

    bool accepts(string);

    void print();

    void printToFile(string filename);

    RE toRE();

    DFA minimize();

    void printTable();

    vector<tuple<string,string,string>> transToRe();

    bool operator==(DFA);

    vector<tuple<string,string,bool>> getTable();

    const vector<tuple<string, bool, bool>> &getStates() const;

    void setStates(const vector<tuple<string, bool, bool>> &states);

    const vector<tuple<string, string, string>> &getTransitions() const;

    void setTransitions(const vector<tuple<string, string, string>> &transitions);

    const vector<string> &getAlphabet() const;

    void setAlphabet(const vector<string> &alphabet);

    void fileToDFA(string);

    vector<string> spellingCheck(string);

    void spellingCheckRecursion(string, vector<pair<string, int>>&, const tuple<string,
            bool, bool>& = tuple<string, bool, bool> ("False", false, false), int = 0, int = 0, int = 0);
};




#endif //UNTITLED2_DFA_H
