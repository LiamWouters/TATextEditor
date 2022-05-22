//
// Created by liaml on 18/05/2022.
//

#include "Sentence.h"
#include "Word.h"

vector<Word *> Sentence::getWords() {
    return words;
}

Word *Sentence::getWord(int index) {
    return words[index];
}

void Sentence::addWord(Word* token) {
    words.push_back(token);
}

int Sentence::size() {
    return words.size();
}

int Sentence::getWordCount() {
    // size without punctuation marks
    int wCount = 0;
    for (Word* w : words) {
        if (!w->isPunctuationMark()) {
            wCount += 1;
        }
    }
    return wCount;
}


Sentence::~Sentence() {
    for (Word *w: words) {
        delete w;
    }
}
