//
// Created by liam on 18/05/2022.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <utility>
#include "Text.h"
#include "Sentence.h"
#include "Word.h"
#include "SpecialCharacter.h"

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
        // check voor quotationmarks
        ///

        Word* newWord = new Word(token);
        if (token.back() == '\"' or token.back() == '\235') {
            token = token.substr(0, token.size()-1);
            newWord->setString(token);
            newWord->setEndQuote();
        }
        if (token.front() == '\"' or token.front() == '\342') {
            token = token.substr(1, token.size());
            newWord->setString(token);
            newWord->setStartQuote();
        }

        if (token.back() == '.' or token.back() == '!' or token.back() == '?') {
            if (token.back() == '.' && abbreviationDFA->accepts(token)) {
                newWord->setAbbreviation();
                sentence->addWord(newWord);
            } else {
                // check for "..."
                if (token.find("...") != string::npos) {
                    // split triple dot from string (if there is more to the string)
                    if (token != "...") {
                        newWord->setString(token.substr(0, token.size()-3));
                        sentence->addWord(newWord);
                    }
                    SpecialCharacter *tripleDot = new SpecialCharacter(token.substr(token.size()-3, token.size()));
                    sentence->addWord(tripleDot);
                    continue;
                }
                SpecialCharacter *punctMark = new SpecialCharacter(token.substr(token.size() - 1));
                token = token.substr(0, token.size() - 1);
                if (token != "") {
                    if (token.back() == '\"' or token.back() == '\235') {
                        token = token.substr(0, token.size()-1);
                        newWord->setEndQuote();
                    }
                    if (token.front() == '\"' or token.front() == '\342') {
                        token = token.substr(1, token.size()-1);
                        newWord->setStartQuote();
                    }
                    newWord->setString(token);
                    sentence->addWord(newWord);
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
            SpecialCharacter* comma = new SpecialCharacter(",");

            // check for abbreviation and ... again
            if (token.find("...") != string::npos) {
                // split triple dot from string (if there is more to the string)
                if (token != "...") {
                    newWord->setString(token.substr(0, token.size()-3));
                    sentence->addWord(newWord);
                }
                SpecialCharacter *tripleDot = new SpecialCharacter(token.substr(token.size()-3, token.size()));
                sentence->addWord(tripleDot); sentence->addWord(comma);
                continue;
            }
            // check for quotes again
            if (token.back() == '\"' or token.back() == '\235') {
                token = token.substr(0, token.size()-1);
                newWord->setString(token);
                newWord->setEndQuote();
            }
            if (token.front() == '\"' or token.front() == '\342') {
                token = token.substr(1, token.size()-1);
                newWord->setString(token);
                newWord->setStartQuote();
            }
            if (token != "") {
                newWord->setString(token);
                if (abbreviationDFA->accepts(token)) {
                    newWord->setAbbreviation();
                }
                sentence->addWord(newWord);
            }
            sentence->addWord(comma);
        }
        else {
            newWord->setString(token);
            sentence->addWord(newWord);
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

vector<pair<vector<string>, int>> Text::createNgram(int n, string word) {
    // We will check all encounters in the text for the given word and check which words come before and after it most often
    // the number n(-1) represents the amount of words saved and checked next to the word
    // example: n == 2, word = "quick":
    // the QUICK BROWN fox  -> Brown is added to the dictionary with value 1 (amount of times found next to word)
    // THE QUICK brown fox  -> The is added to the dictionary with value 1 (amount of times found next to word)

    /// Return a vector containing a pair of the biggest words before and after,
    /// example: {({"biggestBefore"}, amount), ({"biggestAfter"}, amount)}
    /// example: if there are 2 or more "biggestBefores" with the same amount
    ///         -> {({"biggestBefore1", "biggestBefore2"}, amount), ({"biggestAfter"}, amount)}

    if (n < 2) {return {{{""}, 0},{{""}, 0}};}

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
                            bWords.insert(bWords.begin(), sentence->getWords()[i - j]);
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
    if (true == false) {
        /// print for debug ///
        for (auto it = wordsBefore.begin(); it != wordsBefore.end(); it++) {
            cout << "Before: " << it->first << " | " << it->second << endl;
        }

        for (auto it = wordsAfter.begin(); it != wordsAfter.end(); it++) {
            cout << "After: " << it->first << " | " << it->second << endl;
        }
        //////////////////////
    }

    pair<vector<string>, int> biggestBefore = {{""}, 0};
    pair<vector<string>, int> biggestAfter = {{""}, 0};
    for (auto it = wordsBefore.begin(); it != wordsBefore.end(); it++) {
        if (it->second > biggestBefore.second) {
            biggestBefore.second = it->second;
            biggestBefore.first = {it->first};
        }
        else if (it->second == biggestBefore.second) {
            biggestBefore.first.push_back(it->first);
        }
    }
    for (auto it = wordsAfter.begin(); it != wordsAfter.end(); it++) {
        if (it->second > biggestAfter.second) {
            biggestAfter.second = it->second;
            biggestAfter.first = {it->first};
        }
        else if (it->second == biggestAfter.second) {
            biggestAfter.first.push_back(it->first);
        }
    }
    vector<pair<vector<string>, int>> biggest = {biggestBefore, biggestAfter};
    return biggest;
}

Text::~Text() {
    for (Sentence* s : sentences) {
        delete s;
    }
}
void Text::Find(string filename, const string& search) {
    Text text;
    text.Tokenize(std::move(filename));
    for (auto zin:text.sentences) {
        for(auto woord:zin->getWords()){
            if (woord->getString()==search){
                woord->setHighlight(true);
            }
            else{
                woord->setHighlight(false);
            }
        }
    }
}

void Text::Replace(string filename,const  string& search, const string& replace) {
    Text text;
    text.Tokenize(std::move(filename));
    for (auto zin:text.sentences) {
        for(auto woord:zin->getWords()){
            if (woord->getString()==search){
                woord->setReplace(replace);
            }
        }
    }
}
