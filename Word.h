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

    bool abbreviation; // is een afkorting vb: "Dr."
    bool startquote; // token bevat eigenlijk een '"' voor de string
    bool endquote; // token bevat eigenlijk een '"' na de string
    bool isVowel(char* c);
    bool isConsonant(string word, int index);
    bool changeSuffix(string& word, string S1, string S2);
    int calculateMeasure(string word);
    bool containsVowel(string word);
    bool endsWithDoubleConsonant(string word);
    bool endsCVC(string word);
public:
    Word(const string &token);

    string getString();
    void setString(string t);
    string getRoot();
    virtual bool isSpecialChar();
    void setAbbreviation();
    bool isAbbreviation();
    void setStartQuote();
    bool isStartQuote();
    void setEndQuote();
    bool isEndQuote();

    string Stem();
};

#endif //TATEXTEDITOR_WORD_H
