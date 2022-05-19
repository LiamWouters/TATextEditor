#ifndef UNTITLED2_ENFA_H
#define UNTITLED2_ENFA_H

#include <string>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <tuple>
#include <vector>
#include "json.hpp"
class DFA;
using namespace std;
using json = nlohmann::json;

class ENFA {
private:
    vector<tuple<string, bool, bool>> states;
    vector<tuple<string, string, string>> transitions;
    vector<string> alphabet;
    string epsilon;
    vector<tuple<string, vector<string>>> Eclose;
public:
    ENFA();

    ENFA(string);

    bool accepts(string);

    DFA toDFA();

    vector<string> getEclose(vector<string> &, const vector<string> &);

    void printStats();

    const vector<tuple<string, bool, bool>> &getStates() const;

    void setStates(const vector<tuple<string, bool, bool>> &states);

    const vector<tuple<string, string, string>> &getTransitions() const;

    void setTransitions(const vector<tuple<string, string, string>> &transitions);

    const vector<string> &getAlphabet() const;

    void setAlphabet(const vector<string> &alphabet);

    const string &getEpsilon() const;

    void setEpsilon(const string &epsilon);

    const vector<tuple<string, vector<string>>> &getEclose1() const;

    void setEclose(const vector<tuple<string, vector<string>>> &eclose);

};

#endif