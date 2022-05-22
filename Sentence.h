//
// Created by liaml on 18/05/2022.
//

#ifndef TATEXTEDITOR_SENTENCE_H
#define TATEXTEDITOR_SENTENCE_H

#include <iostream>
#include <vector>
using namespace std;

class Word;

class Sentence {
private:
    vector<Word*> words;
public:
    vector<Word*> getWords();
    Word* getWord(int index);
    void addWord(Word* token);
    int size();
    int getWordCount();

    virtual ~Sentence();
};

#endif //TATEXTEDITOR_SENTENCE_H
