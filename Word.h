//
// Created by liam on 18/05/2022.
//

#ifndef TATEXTEDITOR_WORD_H
#define TATEXTEDITOR_WORD_H

#include <iostream>
using namespace std;

class Word {
private:
    string token;
    string root;

    bool specialChar; // is een special character: punt, komma, ...
    bool abbreviation; // is een afkorting vb: "Dr."
    bool isVowel(char* c);
    bool isConsonant(string word, int index);
    bool changeSuffix(string& word, string S1, string S2);
public:
    Word(const string &token);

    string getString();
    void setSpecialChar();
    bool isSpecialChar();
    void setAbbreviation();
    bool isAbbreviation();

    string Stem();
};

#endif //TATEXTEDITOR_WORD_H
