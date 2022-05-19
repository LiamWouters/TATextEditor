//
// Created by liam on 18/05/2022.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Text.h"
#include "Sentence.h"
#include "Word.h"

/// Private functions ///
void Text::print() {
    for (int j = 0; j < getSentences().size(); j++) {
        Sentence* s = getSentences()[j];
        for (int i = 0; i < s->size(); i++) {
            cout << s->getWord(i)->getString() << " ";
        }
        cout << endl;
    }
}

////////////////////////
/// Public functions ///

void Text::Tokenize(string filename) {
    ifstream file;
    file.open(filename);

    Sentence* sentence = new Sentence();
    while (!file.eof()) {
        string token;
        file >> token;

        if (token.back() == '.' or token.back() == '!' or token.back() == '?') {
            /// TO DO:
            // check voor "..." (niet persee einde zin)
            // check voor vb: "Dr."
            ///
            Word* punctMark = new Word(token.substr(token.size()-1));
            punctMark->setPunctuationMark();
            token = token.substr(0, token.size()-1);
            Word* w = new Word(token);
            sentence->addWord(w); sentence->addWord(punctMark);

            // end sentence and start a new one.
            addSentence(sentence);
            sentence = new Sentence();

        }
        else if (token.back() == ',') {
            // split off comma as a seperate word (punctuation = true).
            token = token.substr(0, token.size()-1);
            Word* comma = new Word(",");
            comma->setPunctuationMark();
            Word* w = new Word(token);
            sentence->addWord(w); sentence->addWord(comma);
        }
        else {
            Word* w = new Word(token);
            sentence->addWord(w);
        }
    }
    // if the text doesn't end with a '.'
    // we will still add the sentence to the text
    auto it = find(sentences.begin(), sentences.end(), sentence);
    if (it == sentences.end()) {
        sentences.push_back(sentence);
    }

    print();
}

const vector<Sentence *> &Text::getSentences() const {
    return sentences;
}

void Text::addSentence(Sentence* s) {
    sentences.push_back(s);
}

Text::~Text() {
    for (Sentence* s : sentences) {
        delete s;
    }
}
