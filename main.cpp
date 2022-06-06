#include <iostream>
#include "fstream"
#include "json.hpp"
#include "Text.h"
#include "Sentence.h"
#include "Word.h"
#include "SyntaxChecker.h"
#include <algorithm>
#include <iomanip>

using namespace std;
using json = nlohmann::json;

int main() {

    /*
     * THE RULES:
     *
     * JSON: Contains at least one TEXT object which is one big string or an array of substrings.
     * May have a Regex object and Replacement object on the first JSON level.
     *
     * HTML: May contain a Regex and Replacement tag (these are not allowed to have other tags inside them).
     * Anything else that is not a tag nor inside the 2 previously mentioned tags will be considered as text.
     *
     * TXT: As these files don't really have a syntax, there aren't as many rules here.
     * Again, a Regex and a Replacement rule can be added on a separate line like this:
     * Regex: ...
     * Replacement: ...
     *
     * The words Regex and Replacement are not case-sensitive in any of these cases.
     */

    Text text;

    // DO NOT name your file buffer.txt
    string filename = "justAList.json";
    SyntaxChecker s;
    string regex;
    int foundRegex = 0;
    string replacement;
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
            cout << setw(4) << j << endl;
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
                    regex = inputReplacement;
                    foundRegex += 1;
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
        o.open("buffer.txt");
        o.close();
    }

    else if(filename.find(".txt") == filename.size()-4){
        text.Tokenize(filename);
    }

    else{
        cout << "Invalid input file: expected a txt, json or html file." << endl;
        return 1;
    }

    //vector<pair<string,int>> biggest = text.createNgram(2, "climate");

    //cout << biggest[0].first << " -> " << biggest[0].second << endl;
    //cout << biggest[1].first << " -> " << biggest[1].second << endl;

    o.clear();

    return 0;

};