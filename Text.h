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

public:
    void Tokenize(string filename);

    const vector<Sentence *> &getSentences() const;
    void addSentence(Sentence* s);

};

#endif //TATEXTEDITOR_TEXT_H
