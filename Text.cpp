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
        DFA dfa = DFA();
        dfa.fileToDFA("../SavedAutomata/Abbreviations.txt");
        dfa.printToFile("AbbreviationsDFA");
    }
    /*
     * OUDE MANIER
    if (!abbreviationDFA) {
        // create NFA per word
        NFA nfa = NFA();
        nfa.fileToNFA("../SavedAutomata/Abbreviations.txt");
        DFA finalDFA = nfa.toDFA();
        finalDFA.printToFile("AbbreviationsDFA");
    }*/
    abbreviationDFA.close();
}

////////////////////////
/// Public functions ///

Text::Text() {
    _initCheck = this;
    ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
}

bool Text::properlyInitialized() {
    return _initCheck == this;
}


void Text::Tokenize(string filename) {
    REQUIRE(properlyInitialized(), "Text wasn't initialized when calling Tokenize()");
    ifstream file;
    file.open(filename);

    Sentence* sentence = new Sentence();

    ifstream abbreviation;
    abbreviation.open("../SavedAutomata/AbbreviationsDFA.json");
    if (!abbreviation) {makeAbbreviationsAutomata();}
    abbreviation.close();
    DFA* abbreviationDFA = new DFA ("../SavedAutomata/AbbreviationsDFA.json");

    while (!file.eof()) {
        string token;
        file >> token;
        int insertSkip = 0; // insert at position sentence[sentence.size() - insertSkip];
        bool sentenceOver = false;

        // turn all characters to lowercase
        for (char& c : token) {
            c = tolower(c);
        }
        if (token.find("\r") != string::npos) {
            token = token.substr(0,token.size()-1);
        }
        //

        /// (special) Functionality:
        // check voor "..." (niet persee einde zin)
        // check voor vb: "Dr."
        // check voor quotationmarks
        // split off special characters: ".", "!", "?", ",", ";", ":", "()", "[]", "{}"
        ///

        Word* newWord = new Word(token);
        if (token.back() == '\"' or token.back() == '\235') {
            token = token.substr(0, token.size()-1);
            newWord->setString(token);
            newWord->setEndQuote(true);
        }
        if (token.front() == '\"' or token.front() == '\342') {
            token = token.substr(1, token.size());
            newWord->setString(token);
            newWord->setStartQuote(true);
        }

        while (token.front() == '(' or token.front() == '[' or token.front() == '{') {
            string specC = "";
            specC += token[0];
            SpecialCharacter* SC = new SpecialCharacter(specC);
            sentence->addWord(SC);
            token = token.substr(1,token.size());
        }

        while (token.back() == '.' or token.back() == '!' or token.back() == '?' or token.back() == ',' or token.back() == ';' or token.back() == ':' or token.back() == ')' or token.back() == ']' or token.back() == '}') {
            if (token.back() == '.' && abbreviationDFA->accepts(token)) {
                newWord->setAbbreviation();
                sentence->addWord(newWord);
                insertSkip = -1;
                break;
            } else {
                insertSkip += 1;
                // check for "..."
                if (token.find("...") != string::npos) {
                    if (token.substr(token.size()-3, token.size()) == "...") { // specifically at the end
                        // split triple dot from string (if there is more to the string)
                        SpecialCharacter *tripleDot = new SpecialCharacter(
                                token.substr(token.size() - 3, token.size()));
                        token = (token.substr(0, token.size() - 3));
                        if (token == "") {
                            if (newWord->isEndQuote()) {
                                tripleDot->setEndQuote(true);
                            }
                            if (newWord->isStartQuote()) {
                                tripleDot->setStartQuote(true);
                            }
                        }
                        sentence->insertWord(tripleDot, sentence->size() - insertSkip + 1);
                        continue;
                    }
                }
                // create punctMark and shorten token
                SpecialCharacter *punctMark = new SpecialCharacter(token.substr(token.size() - 1));
                if (newWord->isEndQuote()) {
                    punctMark->setEndQuote(true);
                    newWord->setEndQuote(false);
                }
                token = token.substr(0, token.size() - 1);
                if (token == "") {
                    if (newWord->isEndQuote()) {
                        punctMark->setEndQuote(true);
                    }
                    if (newWord->isStartQuote()) {
                        punctMark->setStartQuote(true);
                    }
                }
                // don't add token (we use insertSkip and add it later)
                sentence->insertWord(punctMark, sentence->size()-insertSkip+1);
                if (punctMark->getString() == "." or punctMark->getString() == "!" or punctMark->getString() == "?") {
                    sentenceOver = true;
                }
            }
        }
        // now finalize word:
        if (token.back() == '\"' or token.back() == '\235') {
            token = token.substr(0, token.size()-1);
            newWord->setEndQuote(true);
        }
        if (token.front() == '\"' or token.front() == '\342') {
            token = token.substr(1, token.size()-1);
            newWord->setStartQuote(true);
        }
        newWord->setString(token);
        // add word
        if (insertSkip >= 0 && newWord->getString() != "") {
            sentence->insertWord(newWord, sentence->size() - insertSkip);
        }
        else if (insertSkip >= 0 && newWord->getString() == "") {delete newWord;}
        if (sentenceOver) {
            addSentence(sentence);
            sentence = new Sentence();
        }
    }
    // if the text doesn't end with a '.'
    // we will still add the sentence to the text
    auto it = find(sentences.begin(), sentences.end(), sentence);
    if (it == sentences.end() && sentence->size() != 0) {
        sentences.push_back(sentence);
    } else if (it == sentences.end() && sentence->size() == 0) {
        delete sentence;
    }
    delete abbreviationDFA;
    file.close();
}

const vector<Sentence *> &Text::getSentences() {
    REQUIRE(properlyInitialized(), "Text wasn't initialized when calling getSentences()");
    return sentences;
}

void Text::addSentence(Sentence* s) {
    REQUIRE(properlyInitialized(), "Text wasn't initialized when calling addSentence()");
    sentences.push_back(s);
    ENSURE(sentences[sentences.size()-1] == s, "The last sentence in the vector is s");
}

vector<pair<vector<string>, int>> Text::createNgram(int n, string word) {
    REQUIRE(properlyInitialized(), "Text wasn't initialized when calling createNgram()");
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

void Text::Find(string filename, const string& search) {
    REQUIRE(properlyInitialized(), "Text wasn't initialized when calling find()");
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
    REQUIRE(properlyInitialized(), "Text wasn't initialized when calling replace()");
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

Text::~Text() {
    REQUIRE(properlyInitialized(), "Text wasn't initialized when calling Destructor");
    for (Sentence* s : sentences) {
        delete s;
    }
    sentences.clear();
    ENSURE(sentences.empty(), "No remaining sentences");
}