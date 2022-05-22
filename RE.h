//
// Created by youssef on 02.04.22.
//

#ifndef UNTITLED2_RE_H
#define UNTITLED2_RE_H
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <tuple>
#include <vector>
class ENFA;
using namespace std;

class RE {
private:
    string regex;
    vector<char> alphabet;
    char epsilon;
public:
    RE(const string&, char);

    ENFA toENFA();

    void print();

};

ENFA makeBasicENFA(string ,bool = false);

ENFA Union(const ENFA&,const ENFA&);

ENFA Concatenation(const ENFA&,const ENFA&);

ENFA Star(const ENFA&);

#endif //UNTITLED2_RE_H