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
#include "NFA.h"
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

void Text::makeAbbreviationsAutomata() {
    ifstream abbreviationDFA;
    abbreviationDFA.open("../SavedAutomata/AbbreviationsDFA.json");
    if (!abbreviationDFA) {
        // create NFA per word
        NFA nfa = NFA();
        nfa.fileToNFA("../SavedAutomata/Abbreviations.txt");
        DFA finalDFA = nfa.toDFA();
        finalDFA.printToFile("AbbreviationsDFA");
    }
    abbreviationDFA.close();
}

////////////////////////
/// Public functions ///

void Text::Tokenize(string filename) {
    ifstream file;
    file.open(filename);

    Sentence* sentence = new Sentence();

    ifstream abbreviation;
    abbreviation.open("../SavedAutomata/AbbreviationsDFA.json");
    if (!abbreviation) {makeAbbreviationsAutomata();}
    abbreviation.close();
    DFA* abbreviationDFA = new DFA ("../SavedAutomata/AbbreviationsDFA.json");
    /*
    cout << boolalpha << abbreviationDFA->accepts("abbrev.") << endl; // should all be true
    cout << boolalpha << abbreviationDFA->accepts("abd.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("capt.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("cartogr.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("catal.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("catech.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("cath.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("cent.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("cent.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("ceram.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("cert.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("certif.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("cf.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("ch.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("chamb.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("char.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("charac.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("chas.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("chem.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("chesh.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("ch.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("chr.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("chr.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("chron.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("chron.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("chronol.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("chrons.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("cinematogr.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("circ.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("civ.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("cl.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("cl.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("class.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("class.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("classif.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("climatol.") << endl;
    cout << boolalpha << abbreviationDFA->accepts("climatl.") << endl; // from here should be false
    cout << boolalpha << abbreviationDFA->accepts(".") << endl;
    cout << boolalpha << abbreviationDFA->accepts("") << endl;
    */

    while (!file.eof()) {
        string token;
        file >> token;

        // turn all characters to lowercase
        for (char& c : token) {
            c = tolower(c);
        }
        //

        /// (special) Functionality:
        // check voor "..." (niet persee einde zin)
        // check voor vb: "Dr."
        ///

        if (token.back() == '.' or token.back() == '!' or token.back() == '?') {
            if (token.back() == '.' && abbreviationDFA->accepts(token)) {
                Word *w = new Word(token);
                w->setAbbreviation();
                sentence->addWord(w);
            } else {
                // check for "..."
                if (token.find("...") != string::npos) {
                    // split triple dot from string (if there is more to the string)
                    if (token != "...") {
                        Word *w = new Word(token.substr(0, token.size()-3));
                        sentence->addWord(w);
                    }
                    Word *tripleDot = new Word(token.substr(token.size()-3, token.size()));
                    tripleDot->setSpecialChar();
                    sentence->addWord(tripleDot);
                    continue;
                }
                Word *punctMark = new Word(token.substr(token.size() - 1));
                punctMark->setSpecialChar();
                token = token.substr(0, token.size() - 1);
                if (token != "") {
                    Word *w = new Word(token);
                    sentence->addWord(w);
                }
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
            comma->setSpecialChar();

            // check for abbreviation and ... again
            if (token.find("...") != string::npos) {
                // split triple dot from string (if there is more to the string)
                if (token != "...") {
                    Word *w = new Word(token.substr(0, token.size()-3));
                    sentence->addWord(w);
                }
                Word *tripleDot = new Word(token.substr(token.size()-3, token.size()));
                tripleDot->setSpecialChar();
                sentence->addWord(tripleDot); sentence->addWord(comma);
                continue;
            }
            if (token != "") {
                Word *w = new Word(token);
                if (abbreviationDFA->accepts(token)) {
                    w->setAbbreviation();
                }
                sentence->addWord(w);
            }
            sentence->addWord(comma);
        }
        //else if (token.back() == '"') {

        //}
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
    delete abbreviationDFA;
    file.close();
}

const vector<Sentence *> &Text::getSentences() const {
    return sentences;
}

void Text::addSentence(Sentence* s) {
    sentences.push_back(s);
}

vector<pair<string, int>> Text::createNgram(int n, string word) {
    // We will check all encounters in the text for the given word and check which words come before and after it most often
    // the number n(-1) represents the amount of words saved and checked next to the word
    // example: n == 2, word = "quick":
    // the QUICK BROWN fox  -> Brown is added to the dictionary with value 1 (amount of times found next to word)
    // THE QUICK brown fox  -> The is added to the dictionary with value 1 (amount of times found next to word)

    /// Return a vector containing a pair of the biggest word before and after,
    /// example: {("biggestBefore", amount), ("biggestAfter", amount)}

    map<string, int> wordsBefore = {};
    map<string, int> wordsAfter = {};

    /// check for all encounters of the word
    for (Sentence* sentence : sentences) {
        for (int i = 0; i < sentence->getWords().size(); i++) {
            Word* w = sentence->getWords()[i];
            if (w->getString() == word) {
                vector<Word*> bWords = {};
                vector<Word*> aWords = {};
                for (int j = 1; j < n; j++) {
                    // gather before words
                    if ((i-j) >= 0) {
                        if (!sentence->getWords()[i-j]->isSpecialChar()) { // if its a ',' you can ignore it (also the words before it because it should have no relevance to the word)
                            //cout << "Before: " << sentence->getWords()[i - j]->getString() << endl;
                            bWords.push_back(sentence->getWords()[i - j]);
                        }
                    }

                    // gather after words;
                    if ((i+j) < sentence->getWords().size()) {
                        if (!sentence->getWords()[i+j]->isSpecialChar()) {
                            //cout << "After: " << sentence->getWords()[i + j]->getString() << endl;
                            aWords.push_back(sentence->getWords()[i + j]);
                        }
                    }
                }
                // if awords, bwords do not contain n-1 words, don't add
                if (bWords.size() == n-1) {
                    string key = "";
                    for (int i = 0; i < bWords.size(); i++) {key += bWords[i]->getString(); if (i != bWords.size()-1) {key += " ";}} // i use this key because for some reason when using the vector<Word*> as key it doesn't work
                    auto it = wordsBefore.find(key);
                    if (it != wordsBefore.end()) {
                        // word already exists in map
                        it->second += 1;
                    } else {
                        pair<string, int> p = {key, 1};
                        wordsBefore.emplace(p);
                    }
                }
                if (aWords.size() == n-1) {
                    string key = "";
                    for (int i = 0; i < aWords.size(); i++) {key += aWords[i]->getString(); if (i != aWords.size()-1) {key += " ";}} // i use this key because for some reason when using the vector<Word*> as key it doesn't work
                    auto it = wordsAfter.find(key);
                    if (it != wordsAfter.end()) {
                        // word already exists in map
                        it->second += 1;
                    } else {
                        pair<string, int> p = {key, 1};
                        wordsAfter.emplace(p);
                    }
                }
            }
        }
    }
    if (true == true) {
        /// print for debug ///
        for (auto it = wordsBefore.begin(); it != wordsBefore.end(); it++) {
            cout << "Before: " << it->first << " | " << it->second << endl;
        }

        for (auto it = wordsAfter.begin(); it != wordsAfter.end(); it++) {
            cout << "After: " << it->first << " | " << it->second << endl;
        }
        //////////////////////
    }

    pair<string, int> biggestBefore = {"", 0};
    pair<string, int> biggestAfter = {"", 0};
    for (auto it = wordsBefore.begin(); it != wordsBefore.end(); it++) {
        if (it->second > biggestBefore.second) {
            biggestBefore = *it;
        }
    }
    for (auto it = wordsAfter.begin(); it != wordsAfter.end(); it++) {
        if (it->second > biggestAfter.second) {
            biggestAfter = *it;
        }
    }
    vector<pair<string, int>> biggest = {biggestBefore, biggestAfter};
    return biggest;
}

Text::~Text() {
    for (Sentence* s : sentences) {
        delete s;
    }
}
