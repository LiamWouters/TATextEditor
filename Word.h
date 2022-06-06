//
// Created by liam on 18/05/2022.
//

#ifndef TATEXTEDITOR_WORD_H
#define TATEXTEDITOR_WORD_H

#include <iostream>
#include "DesignByContract.h"
using namespace std;

class Word {
private:
    Word* _initCheck;
    string token;
    string root;
    bool abbreviation; // is een afkorting vb: "Dr."
    bool startquote; // token bevat eigenlijk een '"' voor de string
    bool endquote; // token bevat eigenlijk een '"' na de string
    bool highlight; // als het true is moet het gefluoriseerd worden in de html.
    string replace;

    bool isVowel(char* c);
    bool isConsonant(string word, int index);
    bool changeSuffix(string& word, string S1, string S2);
    int calculateMeasure(string word);
    bool containsVowel(string word);
    bool endsWithDoubleConsonant(string word);
    bool endsCVC(string word);

    void Stem();
public:
    /*
     * ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
     */
    Word(const string &token);

    /***/
    bool properlyInitialized();

    /*
     * REQUIRE(properlyInitialized(), "Word wasn't initialized when calling getString()");
     */
    string getString();

    /*
     * REQUIRE(properlyInitialized(), "Word wasn't initialized when calling setString()");
     * ENSURE(token == t, "token changed to new value");
     */
    void setString(string t);

    /*
     * REQUIRE(properlyInitialized(), "Word wasn't initialized when calling getRoot()");
     */
    string getRoot();

    /*
     * REQUIRE(properlyInitialized(), "Word wasn't initialized when calling isSpecialChar()");
     */
    virtual bool isSpecialChar();

    /*
     * REQUIRE(properlyInitialized(), "Word wasn't initialized when calling setAbbreviation()");
     * ENSURE(abbreviation == true, "Word is an abbreviation");
     */
    void setAbbreviation();

    /*
     * REQUIRE(properlyInitialized(), "Word wasn't initialized when calling isAbbreviation()");
     */
    bool isAbbreviation();

    /*
     * REQUIRE(properlyInitialized(), "Word wasn't initialized when calling setStartQuote()");
     * ENSURE(startQuote == true, "value of startquote equals given value");
     */
    void setStartQuote(bool b);

    /*
     * REQUIRE(properlyInitialized(), "Word wasn't initialized when calling isStartQuote()");
     */
    bool isStartQuote();

    /*
     * REQUIRE(properlyInitialized(), "Word wasn't initialized when calling setEndQuote()");
     * ENSURE(endquote == true, "value of endquote equals given value");
     */
    void setEndQuote(bool b);

    /*
     * REQUIRE(properlyInitialized(), "Word wasn't initialized when calling isEndQuote()");
     */
    bool isEndQuote();

    /*
     * REQUIRE(properlyInitialized(), "Word wasn't initialized when calling isHighlight()");
     */
    bool isHighlight();

    /*
     * REQUIRE(properlyInitialized(), "Word wasn't initialized when calling setHighlight()");
     * ENSURE(highlight == h, "Highlight now contains the correct value");
     */
    void setHighlight(bool h);

    /*
     * REQUIRE(properlyInitialized(), "Word wasn't initialized when calling getReplace()");
     */
    const string &getReplace();

    /*
     * REQUIRE(properlyInitialized(), "Word wasn't initialized when calling setReplace()");
     * ENSURE(replace == r, "Replace now contains the correct value");
     */
    void setReplace(const string &r);

    /*
     * REQUIRE(properlyInitialized(), "Word wasn't initialized when calling destructor");
     */
    virtual ~Word();
};

#endif //TATEXTEDITOR_WORD_H
