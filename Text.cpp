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

#include "DFA.h"
#include "ENFA.h"
#include "RE.h"

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

bool Text::checkAbbreviation(string token) {
    // check if the DFA already exists
    ifstream abbreviationDFA;
    abbreviationDFA.open("../SavedAutomata/Abbreviations.json");
    if (!abbreviationDFA) {
        // if not create the abbreviation.json
        ifstream abbreviations;
        abbreviations.open("../SavedAutomata/Abbreviations.txt");
        stringstream abbrevs;
        string line;
        while (getline(abbreviations, line)) {
            for (char& c : line) {
                c = tolower(c);
            }
            abbrevs << line << "+";
        }
        string result = abbrevs.str();
        result.pop_back();
        result.erase( std::remove(result.begin(), result.end(), '\r'), result.end() );

        RE* regex = new RE(result, ':'); // this takes a long while
        ENFA enfa = regex->toENFA();
        DFA dfa = enfa.toDFA();
        dfa.minimize();
        dfa.printToFile("Abbreviations");
        abbreviationDFA.open("../SavedAutomata/Abbreviations.json");
        if (!abbreviationDFA) {
            cerr << "fout bij aanmaken van Abbreviations.json" << endl;
        }
    }
    DFA abbreviationsDFA = DFA("../SavedAutomata/Abbreviations.json");
    abbreviationsDFA.setAlphabet({"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"}); // delete this line
    return abbreviationsDFA.accepts(token);
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

        // turn all characters to lowercase
        for (char& c : token) {
            c = tolower(c);
        }
        //

        if (token.back() == '.' or token.back() == '!' or token.back() == '?') {
            /// TO DO:
            // check voor "..." (niet persee einde zin)
            // check voor vb: "Dr."
            ///

            if (checkAbbreviation(token)) {
                Word *w = new Word(token);
                sentence->addWord(w);
            } else {
                Word *punctMark = new Word(token.substr(token.size() - 1));
                punctMark->setPunctuationMark();
                token = token.substr(0, token.size() - 1);
                Word *w = new Word(token);
                sentence->addWord(w);
                sentence->addWord(punctMark);

                // end sentence and start a new one.
                addSentence(sentence);
                sentence = new Sentence();
            }
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
    if (it == sentences.end() && sentence->size() != 0) {
        sentences.push_back(sentence);
    }

    file.close();
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
