#include <iostream>
#include "fstream"
#include "json.hpp"
#include "Text.h"
#include "Sentence.h"
#include "Word.h"
#include "SyntaxChecker.h"
#include <algorithm>
#include "RE.h"
#include "ENFA.h"
#include "DFA.h"
#include <map>
#include "DesignByContract.h"

using namespace std;
using json = nlohmann::json;

int main() {

    /*
     * THE RULES:
     *
     * JSON: Contains ONE 'Text' object, (if there's multiple, it will only check the last version).
     * The text object is either a string or array of strings.
     * May have a Regex object (epsilon character is optional) and Replacement string object on the first JSON level.
     *
     * HTML: May contain a Regex and Replacement tag (these are not allowed to have other tags inside them).
     * Anything else that is not a tag nor inside the 2 previously mentioned tags will be considered as text.
     *
     * TXT: As these files don't really have a syntax, there aren't as many rules here.
     * Again, a Regex and a Replacement rule can be added on a separate line like this:
     * Regex: ...
     * Replacement: ...
     * Epsilon: ...
     *
     * The words Regex, Replacement and Epsilon are not case-sensitive except for JSON files.
     */

    Text text;

    // DO NOT name your file buffer.txt
    string filename = "demo.json";
    SyntaxChecker s;
    string regex;
    int foundRegex = 0;
    string replacement;
    char epsilon = '\0';
    int foundEpsilon = 0;
    int foundReplacement = 0;

    // HTML & JSON case
    ifstream i(filename);
    ofstream o;
    // JSON case
    json j;

    // Check every key, only use information from ones with a correct name.
    // If ones with a correct name are incorrectly used, don't use their contents.
    if(filename.find(".json") == filename.size()-5){
        if(!s.validJson(filename)){
            return 1;
        }
        else{
            vector<string> lines;
            i >> j;
            if(!j.contains("Text")){
                cout << "Invalid: Json file has no \'Text\' key" << endl;
                return 1;
            }
            else{
                bool message = false;
                if(j["Text"].is_array()){
                    for(auto &k: j["Text"]){
                        if(k.is_string()){
                            lines.push_back(k);
                        }
                        else if (!message){
                            cout << "Warning: non-string element detected in array \'Text\', this will not get used." << endl;
                            message = true;
                        }
                    }
                }
                else if(j["Text"].is_string()){
                    lines.push_back(j["Text"]);
                }
                else{
                    cout << "Invalid value for key \'Text\': expected a string or array." << endl;
                    return 1;
                }
            }
            if(j.contains("Regex")){
                if(j["Regex"].is_string()){
                    regex = j["Regex"];
                    epsilon = '\0';
                }
                else if(j["Regex"].is_array()){
                    if(j["Regex"].contains("Expression")){
                        if(j["Regex"]["Expression"].is_string()) {
                            regex = j["Regex"]["Expression"];
                        }
                        else{
                            cout << "Invalid value for \'expression\', it won't be used." << endl;
                        }
                    }
                    if(j["Regex"].contains("Epsilon")){
                        if(j["Regex"]["Epsilon"].is_string()){
                            if(j["Regex"]["Epsilon"].size() == 1){
                                string eps = j["Regex"]["Epsilon"];
                                epsilon = eps[0];
                            }
                            else{
                                cout << "Invalid: Given epsilon is more than 1 character long, it won't be used." << endl;
                                epsilon = '\0';
                            }
                        }
                        else{
                            cout << "Invalid value for \'epsilon\', it won't be used.";
                            epsilon = '\0';
                        }
                    }
                    if(!j["Regex"].contains("Expression") && j["Regex"].contains("Epsilon")){
                        cout << "Invalid: only an epsilon was given, regex and epsilon won't be used.";
                        epsilon = '\0';
                    }
                }
                else{
                    cout << "Invalid value for key \'Regex\', it won't be used." << endl;
                    regex = "";
                }
            }
            if(j.contains("Replacement")){
                if(j["Replacement"].is_string()){
                    replacement = j["Replacement"];
                }
                else{
                    cout << "Invalid value for key \'Replacement\', it won't be used." << endl;
                }
            }
            o.open("buffer.txt");
            for(int p = 0; p < lines.size(); p++){
                o << lines[p];
                if(p != lines.size()-1){
                    o << "\n";
                }
            }
            o.close();
            text.Tokenize("buffer.txt");
        }
    }

    else if(filename.find(".html") == filename.size()-5){

        vector<string> illegal = s.getHtmlIllegalBegin();

        // Copy all information in one giant string and look for regex and replacement rule first.
        vector<string> lines;
        if(!s.validHTML(filename)){
            return 1;
        }
        string allLines;
        string dummyLine;
        while(getline(i, dummyLine)){
            allLines += dummyLine;
        }

        for(auto a: allLines){
            a = tolower(a);
        }

        bool regWarning = false;
        while(allLines.find("<regex") != string::npos){
            string inputRegex;
            if(foundRegex < 1){
                int begin = allLines.find("<regex")+6;
                int end = allLines.find("</regex")-1;
                while(allLines[begin] != '>'){
                    begin += 1;
                }
                begin += 1;
                inputRegex = allLines.substr(begin, end-begin+1);
                int k = 0;
                while(inputRegex[k] == ' '){
                    k += 1;
                }
                int l = inputRegex.size()-1;
                while(inputRegex[l] == ' '){
                    l -= 1;
                }
                inputRegex = inputRegex.substr(k, l - k + 1);
                bool valid = true;
                for(int g = 0; g < inputRegex.size(); g++) {
                    string nextChar(1, inputRegex[g+1]);
                    if(inputRegex[g] == '<' && find(illegal.begin(), illegal.end(), nextChar) == illegal.end()){
                        cout << "Invalid regex detected with tags inside of it, it won't be used." << endl;
                        valid = false;
                        break;
                    }
                }
                if(valid) {
                    regex = inputRegex;
                    foundRegex += 1;
                }
            }
            else{
                if(!regWarning) {
                    cout << "Warning: multiple regex detected, only the first valid one will be used." << endl;
                    regWarning = true;
                }
            }
            int u = allLines.find("</regex")+6;
            while(allLines[u] != '>'){
                u += 1;
            }
            u += 1;
            string allLinesLeft = allLines.substr(0, allLines.find("<regex"));
            string allLinesRight = " " + allLines.substr(u, allLines.size()-u);
            allLines = allLinesLeft + allLinesRight;
        }

        bool replaceWarning = false;
        while(allLines.find("<replacement") != string::npos){
            string inputReplacement;
            if(foundReplacement < 1){
                int begin = allLines.find("<replacement")+12;
                int end = allLines.find("</replacement")-1;
                while(allLines[begin] != '>'){
                    begin += 1;
                }
                begin += 1;
                inputReplacement = allLines.substr(begin, end-begin+1);
                int k = 0;
                while(inputReplacement[k] == ' '){
                    k += 1;
                }
                int l = inputReplacement.size()-1;
                while(inputReplacement[l] == ' '){
                    l -= 1;
                }
                inputReplacement = inputReplacement.substr(k, l - k + 1);
                bool valid = true;
                for(int g = 0; g < inputReplacement.size(); g++) {
                    string nextChar(1, inputReplacement[g+1]);
                    if(inputReplacement[g] == '<' && find(illegal.begin(), illegal.end(), nextChar) == illegal.end()){
                        cout << "Invalid regex detected with tags inside of it, it won't be used." << endl;
                        valid = false;
                        break;
                    }
                }
                if(valid){
                    replacement = inputReplacement;
                    foundReplacement += 1;
                }
            }
            else{
                if(!replaceWarning) {
                    cout << "Warning: multiple replacements detected, only the first valid one will be used." << endl;
                    replaceWarning = true;
                }
            }
            int u = allLines.find("</replacement")+12;
            while(allLines[u] != '>'){
                u += 1;
            }
            u += 1;
            string allLinesLeft = allLines.substr(0, allLines.find("<replacement"));
            string allLinesRight = " " + allLines.substr(u, allLines.size()-u);
            allLines = allLinesLeft + allLinesRight;
        }

        bool epsWarning = false;
        while(allLines.find("<epsilon") != string::npos){
            string epsChar;
            if(foundRegex < 1){
                int begin = allLines.find("<epsilon")+8;
                int end = allLines.find("</epsilon")-1;
                while(allLines[begin] != '>'){
                    begin += 1;
                }
                begin += 1;
                epsChar = allLines.substr(begin, end-begin+1);
                int k = 0;
                while(epsChar[k] == ' '){
                    k += 1;
                }
                int l = epsChar.size()-1;
                while(epsChar[l] == ' '){
                    l -= 1;
                }
                epsChar = epsChar.substr(k, l - k + 1);
                bool valid = true;
                if(epsChar.size() == 1){
                    epsilon = epsChar[0];
                    foundEpsilon += 1;
                }
                else{
                    cout << "Invalid: Epsilon with more than 1 character given, it won't be used." << endl;
                }
            }
            else{
                if(!epsWarning) {
                    cout << "Warning: multiple epsilon detected, only the first valid one will be used." << endl;
                    epsWarning = true;
                }
            }
            int u = allLines.find("</regex")+6;
            while(allLines[u] != '>'){
                u += 1;
            }
            u += 1;
            string allLinesLeft = allLines.substr(0, allLines.find("<regex"));
            string allLinesRight = " " + allLines.substr(u, allLines.size()-u);
            allLines = allLinesLeft + allLinesRight;
        }

        // Grab all text (stuff not between <aValidTag>)
        string realText;
        bool tag = false;
        for(int p = 0; p < allLines.size(); p++){
            string nextChar(1, allLines[p+1]);
            if(allLines[p] == '<' && find(illegal.begin(), illegal.end(), nextChar) == illegal.end()){
                tag = true;
                realText += " ";
            }
            else if(allLines[p] == '>' && tag){
                tag = false;
                realText += " ";
            }
            else if(!tag){
                realText += allLines[p];
            }
        }

        string word;
        vector<string> words;
        for(int q = 0; q < realText.size(); q++){
            if(realText[q] != ' '){
                int r = q;
                while(realText[r] != ' '){
                    word += realText[r];
                    r += 1;
                }
                words.push_back(word);
                word = "";
                q = r;
            }
        }

        // Save the text to a buffer txt file.
        o.open("buffer.txt");
        int charactersOnline = 0;
        for(auto &t: words){
            charactersOnline += t.size()+1;
            o << t;
            o << " ";
            if(charactersOnline > 120){
                o << "\n";
            }
        }
        o.close();
        text.Tokenize("buffer.txt");
    }

    else if(filename.find(".txt") == filename.size()-4){
        o.open("buffer.txt");
        // Look for Regex and Replacement.
        string currLine;
        while(getline(i, currLine)){
            for(int l = 0; l < currLine.size(); l++){
                currLine[l] = tolower(currLine[l]);
            }
            int begin = 0;
            while(currLine[begin] == ' '){
                begin += 1;
            }
            currLine = currLine.substr(begin, currLine.size()-begin);
            if(currLine.find("regex:") == 0 && foundRegex < 1){
                int regBegin = 6;
                int regEnd = currLine.size()-1;
                while(currLine[regBegin] == ' '){
                    regBegin += 1;
                }
                while(currLine[regEnd] == ' '){
                    regEnd -= 1;
                }
                regex = currLine.substr(regBegin, regEnd+1-regBegin);
                foundRegex += 1;
            }
            else if(currLine.find("regex:") == 0 && foundRegex == 1){
                cout << "Warning: More than one regex detected, only the first one will be used." << endl;
            }
            else if(currLine.find("epsilon:") == 0 && foundEpsilon < 1){
                int epsBegin = 8;
                int epsEnd = currLine.size()-1;
                while(currLine[epsBegin] == ' '){
                    epsBegin += 1;
                }
                while(currLine[epsEnd] == ' '){
                    epsEnd -= 1;
                }
                string epsChar = currLine.substr(epsBegin, epsEnd+1-epsBegin);
                if(epsChar.size() == 1) {
                    epsilon = epsChar[0];
                    foundEpsilon += 1;
                }
                else{
                    cout << "Invalid epsilon value: more than 1 characters given, expected only 1." << endl;
                }
            }
            else if(currLine.find("epsilon:") == 0 && foundEpsilon == 1){
                cout << "Warning: More than one epsilon detected, only the first one will be used." << endl;
            }
            else if(currLine.find("replacement:") == 0 && foundReplacement < 1){
                int replBegin = 12;
                int replEnd = currLine.size()-1;
                while(currLine[replBegin] == ' '){
                    replBegin += 1;
                }
                while(currLine[replEnd] == ' '){
                    replEnd -= 1;
                }
                string change = currLine.substr(replBegin, replEnd+1-replBegin);
                replacement = change;
                foundReplacement = 1;
            }
            else if(currLine.find("replacement:") == 0 && foundReplacement == 1) {
                cout << "Warning: More than one replacement detected, only the first one will be used." << endl;
            }
            else{
                o << currLine << "\n";
            }
        }
        o.close();
        text.Tokenize("buffer.txt");
    }

    else{
        cout << "Invalid input file: expected a txt, json or html file." << endl;
        return 1;
    }

    ofstream o2;
    o2.open("result.html");

    bool mark = false;
    bool replace = false;
    if(!regex.empty()){
        mark = true;
    }
    if(!replacement.empty()){
        replace = true;
    }

    RE r(regex, epsilon);
    DFA d1;
    DFA d2("../SavedAutomata/AbbreviationsDFA.json");
    DFA d3("../SavedAutomata/EnglishWordsDFA.json");
    if(mark) {
        d1 = r.toENFA().toDFA();
    }
    int misspell = 0;
    int noWords = 0;
    int noUniqueWords = 0;
    vector<Sentence*> sentences = text.getSentences();
    int noSentences = sentences.size();
    vector<string> passedWords;
    vector<int> averageWordLength;
    vector<int> averageWordsPerSentence;
    map<string, int> mostUsed;
    map<string, vector<string>> correctionList;
    vector<string> corrections;

    o2 << "<big><b><u>Notes:</u></b></big>" << "<br>" << endl;
    o2 << "-Words affected by the search and replace function are bold" << "<br>" << endl;
    o2 << "-Incorrectly spelled words are marked in <mark>yellow</mark>" << "<br>" << "<br>" << endl << endl;
    o2 << "<b><u><big>Your Text:</big></u></b><br>" << endl;

    vector<string> numbers = s.getNumbersString();
    for(int x = 0; x < sentences.size(); x++){
        averageWordsPerSentence.push_back(sentences[x]->getWordCount());
        vector<Word*> sentenceWords = sentences[x]->getWords();
        for(int w = 0; w < sentenceWords.size(); w++){
            // Actual words.
            if(!sentenceWords[w]->isSpecialChar()) {
                string word = sentenceWords[w]->getString();
                if(word[word.size()-2] == '\'' && word[word.size()-1] == 's'){
                    word = word.substr(0, word.size()-2);
                }
                bool isNumber = true;
                for(auto l: word){
                    string letter(1, l);
                    if(find(numbers.begin(), numbers.end(), letter) == numbers.end() && letter != "."){
                        isNumber = false;
                        break;
                    }
                    if(l == word[word.size()-3] && word[word.size()-2] == 't' && word[word.size()-1] == 'h'){
                        break;
                    }
                }
                // Variable that will be useful later if the replacement is also a non-existing word.
                bool bold = false;
                // Add 1 to wordAmount.
                noWords += 1;
                // Get the root.
                string root = sentenceWords[w]->getRoot();
                // If searched, highlight if no replacement, replace if replacement given.
                if(mark){
                    if(d1.accepts(sentenceWords[w]->getString())){
                        bold = true;
                        if(replace) {
                            sentenceWords[w]->setString(replacement);
                        }
                    }
                }
                // If word has not been encountered before, add 1 to noUniqueWords and add it to passedWords
                if(find(passedWords.begin(), passedWords.end(), sentenceWords[w]->getString()) == passedWords.end()){
                    noUniqueWords += 1;
                    passedWords.push_back(sentenceWords[w]->getString());
                }
                // Keep the length in averageWordLength.
                averageWordLength.push_back(sentenceWords[w]->getString().size());
                // Check if the word exists, if not set highlight to true and add 1 to misspell.
                // Also put the word in corrections.
                if(!d2.accepts(word) && !d3.accepts(word) && word != "a" && !isNumber && word != "i"){
                    misspell += 1;
                    if(find(corrections.begin(), corrections.end(), sentenceWords[w]->getString()) == corrections.end()) {
                        corrections.push_back(sentenceWords[w]->getString());
                        if(w == 0 && !sentenceWords[w+1]->isSpecialChar()){
                            correctionList[word] = d3.spellingCheck(word, "", sentenceWords[w+1]->getString(), text);
                        }
                        else if(w == sentenceWords.size()-2 && !sentenceWords[w-1]->isSpecialChar()){
                            correctionList[word] = d3.spellingCheck(word, sentenceWords[w-1]->getString(), "", text);
                        }
                        else if(w > 0 && w < sentenceWords.size()-2){
                            string left;
                            string right;
                            if(!sentenceWords[w-1]->isSpecialChar() && d3.accepts(sentenceWords[w-1]->getString())){
                                left = sentenceWords[w-1]->getString();
                            }
                            if(!sentenceWords[w+1]->isSpecialChar() && d3.accepts(sentenceWords[w+1]->getString())){
                                right = sentenceWords[w+1]->getString();
                            }
                            correctionList[word] = d3.spellingCheck(word, left, right, text);
                        }
                    }
                    sentenceWords[w]->setHighlight(true);
                }
                else{
                    if (mostUsed.find(root) == mostUsed.end()) {
                        mostUsed[root] = 1;
                    }
                    else {
                        mostUsed[root] += 1;
                    }
                }
                // If first word of sentence, capitalize.
                if(w == 0){
                    string capitalize = sentenceWords[w]->getString();
                    capitalize[0] = toupper(capitalize[0]);
                    sentenceWords[w]->setString(capitalize);
                }
                if(sentenceWords[w]->isHighlight()){
                    o2 << "<mark>";
                }
                if(bold){
                    o2 << "<b>";
                }
                if(sentenceWords[w]->isStartQuote()){
                    o2 << "\"";
                }
                o2 << sentenceWords[w]->getString();
                if(sentenceWords[w]->isEndQuote()){
                    o2 << "\"";
                }
                if(bold){
                    o2 << "</b>";
                }
                if(sentenceWords[w]->isHighlight()){
                    o2 << "</mark>";
                }
                if(w < sentenceWords.size()-2){
                    if(!sentenceWords[w+1]->isSpecialChar() || sentenceWords[w+1]->getString() == "("){
                        o2 << " ";
                    }
                }
                else if(w == sentenceWords.size()-1){
                    o2 << " ";
                }
            }
            else{
                o2 << sentenceWords[w]->getString();
                if(w == sentenceWords.size()-1 || sentenceWords[w]->getString() == "," || sentenceWords[w]->getString() == ")" || sentenceWords[w]->getString() == ";"){
                    o2 << " ";
                }
            }
        }
    }

    double averageWordLen = 0.0;
    for(auto k: averageWordLength){
        averageWordLen += k;
    }
    averageWordLen = averageWordLen/averageWordLength.size();

    double averageWordsPerS = 0.0;
    for(auto k: averageWordsPerSentence){
        averageWordsPerS += k;
    }
    averageWordsPerS = averageWordsPerS/averageWordsPerSentence.size();

    multimap<int, string> roots;
    for(auto &k: mostUsed){
        roots.emplace(k.second, k.first);
    }
    vector<pair<int,string>> pairs;

    if(roots.size() < 10){
        pairs = vector<pair<int,string>>(roots.size());
    }
    else{
        pairs = vector<pair<int,string>>(10);
    }

    for(int v = 0; v < pairs.size(); v++){
        auto iter = prev(roots.end());
        pairs[v] = make_pair(iter->first, iter->second);
        roots.erase(iter);
    }

    o2 << "<br><br>" << endl;
    o2 << "<u><b><big>Text Data:</big></b></u><br>" << endl;
    o2 << "<table border=\"1\">";
    o2 << "<tr><td>#Words:</td><td>" + to_string(noWords) + "</td></tr>";
    o2 << "<tr><td>#UniqueWords:</td><td>" + to_string(noUniqueWords) + "</td></tr>";
    o2 << "<tr><td>#Sentences:</td><td>" + to_string(noSentences) + "</td></tr>";
    o2 << "<tr><td>#Misspelled words:</td><td>" + to_string(misspell) + "</td></tr>";
    o2 << "<tr><td>Average word length:</td><td>" + to_string(averageWordLen) + "</td></tr>";
    o2 << "<tr><td>Average words per sentence:</td><td>" + to_string(averageWordsPerS) + "</td></tr>";
    o2 << "</table>" << "<br>" << endl;

    o2 << "<u><b><big>Top 10 most used word roots:</big></b></u><br>" << endl;
    o2 << "<table border=\"1\">";
    o2 << "<tr><td>Rank:</td><td>Root:</td><td>Times used:</td></tr>";
    for(int k = 0; k < pairs.size(); k++){
        o2 << "<tr><td>" + to_string(k+1) + "</td><td>" + pairs[k].second + "</td><td>" + to_string(pairs[k].first) + "</td></tr>";
    }
    o2 << "</table>" << "<br>" << endl;
    if(!corrections.empty()) {
        o2 << "<u><b><big>Correction suggestions:</big></b></u><br>" << endl;
        o2 << "<table border=\"1\">";
        for (auto &l: correctionList) {
            o2 << "<tr><td>\'" + l.first + "\'" + ":" + "</td><td>";
            vector<string> passed;
            if(l.second.empty()){
                o2 << "</td>";
                o2 << "</tr>";
                continue;
            }
            string added;
            for (auto &m: l.second) {
                int suggestions;
                if (l.second.size() < 5) {
                    suggestions = l.second.size();
                }
                else {
                    suggestions = 5;
                }
                for (int n = 0; n < suggestions; n++) {
                    if(m == l.first){
                        continue;
                    }
                    if(find(passed.begin(), passed.end(), m) == passed.end()) {
                        added += m;
                        passed.push_back(m);
                        if (n != suggestions - 1) {
                            added += ", ";
                        }
                    }
                }
            }
            if(added[added.size()-2] == ',' && added[added.size()-1] == ' '){
                added.pop_back();
                added.pop_back();
            }
            o2 << added;
            o2 << "</td>";
            o2 << "</tr>";
        }
        o2 << "</table>";
    }
    o2.close();
}