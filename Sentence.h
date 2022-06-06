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
    Sentence* _initCheck;
    vector<Word*> words;
public:
    /*
     * ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
     */
    Sentence();

    /***/
    bool properlyInitialized();

    /*
     * REQUIRE(properlyInitialized(), "Sentence wasn't initialized when calling getWords()");
     */
    vector<Word*> getWords();

    /*
     * REQUIRE(properlyInitialized(), "Sentence wasn't initialized when calling getWord()");
     * REQUIRE(index < words.size(), "Sentence wasn't initialized when calling getWord()");
     */
    Word* getWord(int index);

    /*
     * REQUIRE(properlyInitialized(), "Sentence wasn't initialized when calling addWord()");
     * ENSURE(words[words.size()-1] == token, "Last word in sentence is now token");
     */
    void addWord(Word* token);

    /*
     * REQUIRE(properlyInitialized(), "Sentence wasn't initialized when calling size()");
     */
    int size();

    /*
     * REQUIRE(properlyInitialized(), "Sentence wasn't initialized when calling getWordCount()");
     * ENSURE(wCount < size(), "Word count is smaller or equal to the amount of instances in sentence");
     */
    int getWordCount();

    /*
     * REQUIRE(properlyInitialized(), "Sentence wasn't initialized when calling destructor");
     * ENSURE(words.empty(), "No words remain in the sentence");
     */
    virtual ~Sentence();
};

#endif //TATEXTEDITOR_SENTENCE_H
