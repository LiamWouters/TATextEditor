//
// Created by liaml on 18/05/2022.
//

#include "Sentence.h"
#include "Word.h"

Word *Sentence::getWord(int index) {
    return words[index];
}

void Sentence::addWord(Word* token) {
    words.push_back(token);
}

int Sentence::size() {
    return words.size();
}

Sentence::~Sentence() {
    for (Word* w : words) {
        delete w;
    }
}
