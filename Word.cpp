//
// Created by liam on 18/05/2022.
//

#include "Word.h"

/// Private Functions ///
bool Word::isVowel(char* c) {
    return (*c == 'a' || *c == 'e' || *c == 'i' || *c == 'o' || *c == 'u');
}

bool Word::isConsonant(string word, int index) {
    // a consonant is a letter that is not a vowel, and other than Y preceded by a consonant
    bool b = isVowel(&word[index]);
    if (b == false) {
        // is not vowel, check for Y
        if (word[index] == 'y') {
            if (index != 0) {
                return !isConsonant(word, index-1); // check if letter before Y is not a consonant;
            }
            else {return true;}
        }
        else {return true;}
    }
    else {return false;}
}

bool Word::changeSuffix(string& word, string S1, string S2) {
    if (word.length() < S1.length()) {return false;}
    if (word.substr(word.length()-S1.length(), word.length()) == S1) {
        word = word.erase(word.length()-S1.length(), word.length());
        word += S2;
        return true;
    }
    return false;
}

/////////////////////////
/// Public Functions ///

Word::Word(const string &token) : token(token) {
    specialChar = false;
    abbreviation = false;
    root = Stem();
}

string Word::getString() {
    return token;
}

void Word::setSpecialChar() {
    specialChar = true;
}

bool Word::isSpecialChar() {
    return specialChar;
}

void Word::setAbbreviation() {
    abbreviation = true;
}

bool Word::isAbbreviation() {
    return abbreviation;
}

string Word::Stem() {
    /*
     * The porter algorithm for suffix trimming (stemming)
     * source: http://facweb.cs.depaul.edu/mobasher/classes/csc575/papers/porter-algorithm.html
     */
    string rootForm;
    bool skip = false; // bool used to skip parts if necessary

    cout << token << " -> ";
    // SSES -> SS
    if (!skip) {
        skip = changeSuffix(token, "sses", "ss");
    }
    // IES -> I
    if (!skip) {
        skip = changeSuffix(token, "ies", "i");
    }
    // SS -> SS
    if (!skip) {
        skip = changeSuffix(token, "ss", "ss");
    }
    // S ->
    if (!skip) {
        skip = changeSuffix(token, "s", "");
    }
    cout << token << endl;

    skip = false; // reset bool

    // (m>0) EED -> EE
    if (true) {
        changeSuffix(token, "eed", "ee");
    }
    // (*v*) ED ->

    // (*v*) ING ->

    return rootForm;
}
