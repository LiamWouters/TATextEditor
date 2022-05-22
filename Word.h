//
// Created by liam on 18/05/2022.
//

#ifndef TATEXTEDITOR_WORD_H
#define TATEXTEDITOR_WORD_H

#include <iostream>
using namespace std;

class Word {
private:
    string word;

    bool punctuation; // is een punt, uitroepteken, vraagteken, komma
    bool abbreviation; // is een afkorting vb: "Dr."
public:
    Word(const string &word);

    string getString();
    void setPunctuationMark();
    bool isPunctuationMark();
    void setAbbreviation();
    bool isAbbreviation();
};

#endif //TATEXTEDITOR_WORD_H
