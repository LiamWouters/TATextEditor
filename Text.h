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
    Text* _initCheck;
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
     * ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
     */
    Text();

    /***/
    bool properlyInitialized();

    /*
     * REQUIRE(properlyInitialized(), "Text wasn't initialized when calling Tokenize()");
     */
    void Tokenize(string filename);

    /*
     * REQUIRE(properlyInitialized(), "Text wasn't initialized when calling getSentences()");
     */
    const vector<Sentence *> &getSentences();

    /*
     * REQUIRE(properlyInitialized(), "Text wasn't initialized when calling addSentence()");
     * ENSURE(sentences[sentences.size()-1] == s, "The last sentence in the vector is s");
     */
    void addSentence(Sentence* s);

    /*
     * REQUIRE(properlyInitialized(), "Text wasn't initialized when calling find()");
     */
    void Find(string filename, const string& search);

    /*
     * REQUIRE(properlyInitialized(), "Text wasn't initialized when calling replace()");
     */
    void Replace(string filename,const  string& search, const string& replace);

    /*
     * REQUIRE(properlyInitialized(), "Text wasn't initialized when calling createNgram()");
     */
    vector<pair<vector<string>, int>> createNgram(int n, string word);

    /*
     *
     */
    virtual ~Text();
};

#endif //TATEXTEDITOR_TEXT_H
