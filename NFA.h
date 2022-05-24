//
// Created by youssef on 31.03.22.
//
#ifndef NFA_NFA_H
#define NFA_NFA_H
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <tuple>
#include <vector>
#include "json.hpp"
class DFA;
using namespace std;
using json = nlohmann::json;

class NFA {
private:
    vector<tuple<string,bool,bool>> states;
    vector<tuple<string,string,string>> transitions;
    vector<string> alphabet;
public:
    NFA();

    NFA(string);

    void fileToNFA(string filename);

    DFA toDFA();
};


#endif //NFA_NFA_H
