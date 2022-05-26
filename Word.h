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

    bool specialChar; // is een special character: punt, komma, ...
    bool abbreviation; // is een afkorting vb: "Dr."
public:
    Word(const string &token);

    string getString();
    void setSpecialChar();
    bool isSpecialChar();
    void setAbbreviation();
    bool isAbbreviation();
};

#endif //TATEXTEDITOR_WORD_H
