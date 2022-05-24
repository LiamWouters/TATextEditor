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
    if (true) {
        // create NFA per word
        NFA nfa = NFA();
        nfa.fileToNFA("../SavedAutomata/Abbreviations.txt");
        cout << "toNFA done" << endl << endl;
        DFA finalDFA = nfa.toDFA();
        cout << "toDFA done" << endl << endl;
        finalDFA.printToFile("AbbreviationsDFA");
    }
    abbreviationDFA.close();
    /*
    ifstream abbreviationDFA;
    abbreviationDFA.open("../SavedAutomata/AbbreviationsDFA.json");
    if (true) {
        // if not create the abbreviation.json
        ifstream abbreviations;
        abbreviations.open("../SavedAutomata/Abbreviations.txt");
        string line;
        getline(abbreviations, line);
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
        for (char& c : line) {
            c = tolower(c);
        }
        RE regex = RE(line, ':');
        DFA bigDFA = regex.toENFA().toDFA().minimize();
        bigDFA.setAlphabet({".", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"});

        while (getline(abbreviations, line)) {
            for (char& c : line) {
                c = tolower(c);
            }
            line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
            RE regex = RE(line, ':');
            DFA addedDFA = regex.toENFA().toDFA().minimize();
            cout << "addedDFA (should be true): "<< boolalpha << addedDFA.accepts(line) << endl;
            cout << "bigDFA before new product: "<< boolalpha << bigDFA.accepts(line) << endl;
            bigDFA = DFA(bigDFA, addedDFA, false);
            cout << "bigDFA after new product: "<< boolalpha << bigDFA.accepts(line) << endl << endl;
        }
        bigDFA.printToFile("AbbreviationsDFA");
    }
    abbreviationDFA.close();
    */
    // check if the DFA already exists
    /*
    int linesPerDFA = 10; // set to a number like 10, 20, 30, ...
    ifstream abbreviationENFA;
    abbreviationENFA.open("../SavedAutomata/Abbreviations.json");
    if (!abbreviationENFA) {
        // if not create the abbreviation.json
        ifstream abbreviations;
        abbreviations.open("../SavedAutomata/Abbreviations.txt");
        stringstream abbrevs;
        string line;
        int lineNum = 0;
        string fileName;
        while (getline(abbreviations, line)) {
            lineNum += 1;
            if (lineNum%linesPerDFA == 0) {
                string filename = "AbbreviationsDFA" + to_string((lineNum/linesPerDFA));
                string result = abbrevs.str();
                abbrevs.str("");
                abbrevs.clear();
                result.pop_back();
                result.erase( std::remove(result.begin(), result.end(), '\r'), result.end() );
                RE *regex = new RE(result, ':'); // this takes a long while
                ENFA enfa = regex->toENFA();
                DFA dfa = enfa.toDFA();
                dfa.minimize();
                dfa.printToFile(filename);
                delete regex;
            }
            for (char& c : line) {
                c = tolower(c);
            }
            abbrevs << line << "+";
        }
        // nu de overgebleven woorden
        if (lineNum%linesPerDFA != 0) {
            lineNum = ((lineNum/linesPerDFA)+1)*linesPerDFA;
            string filename = "AbbreviationsDFA" + to_string(lineNum/linesPerDFA);
            string result = abbrevs.str();
            result.pop_back();
            result.erase(std::remove(result.begin(), result.end(), '\r'), result.end());
            RE *regex = new RE(result, ':'); // this takes a long while
            ENFA enfa = regex->toENFA();
            DFA dfa = enfa.toDFA();
            dfa.minimize();
            dfa.printToFile(filename);
            delete regex;
        }

        if (lineNum != 0) {
            DFA bigDFA = DFA("../SavedAutomata/AbbreviationsDFA1.json");
            for (int i = 2; i <= (lineNum / linesPerDFA); i++) {
                // create product
                DFA addedDFA = DFA("../SavedAutomata/AbbreviationsDFA" + to_string(i) + ".json");
                bigDFA = DFA(bigDFA, addedDFA, true);
            }
            bigDFA.setAlphabet({".", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"});
            bigDFA.printToFile("AbbreviationsDFA");
        }
        */
        /*
        string result = abbrevs.str();
        result.pop_back();
        result.erase( std::remove(result.begin(), result.end(), '\r'), result.end() );

        RE *regex = new RE(result, ':'); // this takes a long while
        ENFA enfa = regex->toENFA();
        DFA dfa = enfa.toDFA();
        dfa.minimize();
        dfa.printToFile("abbreviationsDFA");
         */
        /*
        abbreviations.close();
    }
    abbreviationENFA.close();
*/
}

////////////////////////
/// Public functions ///

void Text::Tokenize(string filename) {
    ifstream file;
    file.open(filename);

    Sentence* sentence = new Sentence();

    ifstream abbreviation;
    abbreviation.open("../SavedAutomata/AbbreviatiosDFA.json");
    if (!abbreviation) {makeAbbreviationsAutomata();}
    abbreviation.close();
    DFA abbreviationDFA = DFA ("../SavedAutomata/AbbreviationsDFA.json");

    cout << boolalpha << abbreviationDFA.accepts("abbrev.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("abd.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("capt.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("cartogr.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("catal.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("catech.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("cath.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("cent.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("cent.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("ceram.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("cert.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("certif.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("cf.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("ch.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("chamb.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("char.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("charac.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("chas.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("chem.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("chesh.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("ch.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("chr.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("chr.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("chron.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("chron.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("chronol.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("chrons.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("cinematogr.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("circ.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("civ.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("cl.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("cl.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("class.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("class.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("classif.") << endl;
    cout << boolalpha << abbreviationDFA.accepts("climatol.") << endl;

     /*
    ENFA* abbreviationsENFA = new ENFA("../SavedAutomata/AbbreviationsDFA.json");

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

            if (token.back() == '.' && abbreviationsENFA->accepts(token)) {
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
    delete abbreviationsENFA;
    */
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
