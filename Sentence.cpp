//
// Created by liaml on 18/05/2022.
//

#include "Sentence.h"
#include "Word.h"
#include "SpecialCharacter.h"

Sentence::Sentence() {
    _initCheck = this;
    ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
}

bool Sentence::properlyInitialized() {
    return _initCheck == this;
}

vector<Word *> Sentence::getWords() {
    REQUIRE(properlyInitialized(), "Sentence wasn't initialized when calling getWords()");
    return words;
}

Word *Sentence::getWord(int index) {
    REQUIRE(properlyInitialized(), "Sentence wasn't initialized when calling getWord()");
    REQUIRE(index < words.size(), "Sentence wasn't initialized when calling getWord()");
    return words[index];
}

void Sentence::addWord(Word* token) {
    REQUIRE(properlyInitialized(), "Sentence wasn't initialized when calling addWord()");
    words.push_back(token);
    ENSURE(words[words.size()-1] == token, "Last word in sentence is now token");
}

int Sentence::size() {
    REQUIRE(properlyInitialized(), "Sentence wasn't initialized when calling size()");
    return words.size();
}

int Sentence::getWordCount() {
    REQUIRE(properlyInitialized(), "Sentence wasn't initialized when calling getWordCount()");
    // size without punctuation marks
    int wCount = 0;
    for (Word* w : words) {
        if (!w->isSpecialChar()) {
            wCount += 1;
        }
    }
    return wCount;
    ENSURE(wCount < size(), "Word count is smaller or equal to the amount of instances in sentence");
}


Sentence::~Sentence() {
    REQUIRE(properlyInitialized(), "Sentence wasn't initialized when calling destructor");
    for (Word *w: words) {
        delete w;
    }
    words.clear();
    ENSURE(words.empty(), "No words remain in the sentence");
}
