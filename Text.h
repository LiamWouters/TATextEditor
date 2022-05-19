//
// Created by liam on 18/05/2022.
//

#ifndef TATEXTEDITOR_TEXT_H
#define TATEXTEDITOR_TEXT_H

#include <vector>
#include <string>
using namespace std;

class Sentence;

class Text {
private:
    vector<Sentence*> sentences;

    /*
     * function used for debugging
     * prints all sentences to cout
     * with spaces between all words and enters between all sentences
     */
    void print();
public:
    void Tokenize(string filename);

    const vector<Sentence *> &getSentences() const;
    void addSentence(Sentence* s);

    virtual ~Text();
};

#endif //TATEXTEDITOR_TEXT_H
