//
// Created by liam on 18/05/2022.
//

#include "Word.h"

Word::Word(const string &token) : token(token) {
    specialChar = false;
    abbreviation = false;
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
