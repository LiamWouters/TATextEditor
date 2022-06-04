//
// Created by liam on 18/05/2022.
//

#ifndef TATEXTEDITOR_TEXT_H
#define TATEXTEDITOR_TEXT_H

#include <vector>
#include <string>
using namespace std;

class DFA;
class Sentence;
class Word;

class Text {
private:
    vector<Sentence*> sentences;

    /*
     * function used for debugging
     * prints all sentences to cout
     * with spaces between all words and enters between all sentences
     */
    void print();

    /// Private Functions ///
    void makeAbbreviationsAutomata();
    void checkToken(string token, DFA* abbreviationsDFA);
public:
    /*
     *
     */
    void Tokenize(string filename);

    const vector<Sentence *> &getSentences() const;

    void addSentence(Sentence* s);

    void Find(string filename, const string& search);

    void Replace(string filename,const  string& search, const string& replace);

    vector<pair<vector<string>, int>> createNgram(int n, string word);

    virtual ~Text();
};

#endif //TATEXTEDITOR_TEXT_H
