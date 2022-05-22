//
// Created by liam on 18/05/2022.
//

#include "Word.h"

Word::Word(const string &word) : word(word) {
    punctuation = false;
}

string Word::getString() {
    return word;
}

void Word::setPunctuationMark() {
    punctuation = true;
}

bool Word::isPunctuationMark() {
    return punctuation;
}

void Word::setAbbreviation() {
    abbreviation = true;
}

bool Word::isAbbreviation() {
    return abbreviation;
}
