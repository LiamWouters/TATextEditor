//
// Created by axeld on 22/05/2022.
//

#include "SyntaxChecker.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <fstream>

using namespace std;

vector<string> SyntaxChecker::getEverything(){
    return everything;
}

SyntaxChecker::SyntaxChecker() {

    for(auto &i: upperCase){
        i[0] = toupper(i[0]);
    }
    for(auto &i : remainingShift){
        everything.push_back(i);
    }
    for(auto &i : altGR){
        everything.push_back(i);
    }
    for(auto &i : accentVowelsUC){
        everything.push_back(i);
    }
    for(auto &i :accentVowelsLC){
        everything.push_back(i);
    }
    for(auto &i : otherSymbols){
        everything.push_back(i);
    }
    for(auto &i : numbersString){
        everything.push_back(i);
    }
    for(char i = ' '; i <= '~'; i++){
        ascii.push_back(i);
    }

}

bool SyntaxChecker::validJson(string jsonFileString){

    // Read the input, endlines between lines is not necessary since they're irrelevant.
    string readInput;
    string line;
    ifstream f(jsonFileString);
    while(getline(f, line)){
        readInput += line;
        readInput += '\n';
    }
    int index = 0;
    int currentLine = 1;
    int accoladeStack = 0;
    int squareBracketStack = 0;
    vector<string> addedOpenings;
    vector<string> jsonValues;
    vector<string> arguments;
    string newValue;
    string onlySyntaxSymbols;
    bool firstNumber = true;
    bool betweenQuotations = false;
    bool startedWithSquareBracket = false;
    bool closedJson = false;
    pair<char, char> beforeAndAfterQuotes;
    pair<char, char> beforeAndAfterNumber;
    for(auto j: readInput){

        // Endlines aren't allowed in between quotations
        // Note that "\n" in a string gets seen as a separate '\\' and 'n'
        if(j == '\n'){
            if(!betweenQuotations) {
                currentLine += 1;
                index += 1;
                continue;
            }
            else{
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << "Error: endline between quotations." << endl;
                return false;
            }
        }

        // Spaces are mostly ignored.
        else if(j == ' '){
            if(!betweenQuotations){
                index += 1;
                continue;
            }
            else{
                newValue += j;
            }
        }

        // Comma, should be preceded by: "", e, ], } or l
        // Should be followed by: "", t, f, n, [, {
        else if(j == ',' && !betweenQuotations){
            int l = index-1;
            int r = index+1;
            while(readInput[l] == ' ' || readInput[l] == '\n'){
                l -= 1;
            }
            while(readInput[r] == ' ' || readInput[r] == '\n'){
                r += 1;
            }
            if(readInput[l] != '}' && readInput[l] != '\"' && readInput[l] != ']' && readInput[l] != 'l' && readInput[l] != 'e'
            && find(numbersChar.begin(), numbersChar.end(), readInput[l]) == numbersChar.end()){
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << "Invalid character before \',\', ";
                cout << R"(expected '}', ", 'l', 'e', ']' or a number.)" << endl;
                return false;
            }
            if(readInput[r] != '\"' && readInput[r] != '{' && readInput[r] != '[' && readInput[r] != 'n' && readInput[r] != 't'
            && find(numbersChar.begin(), numbersChar.end(), readInput[r]) == numbersChar.end() && readInput[r] != 'f'){
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << "Invalid character after \',\', ";
                cout << R"(expected "", 'n', 't', 'f' or a number.)" << endl;
                return false;
            }
            onlySyntaxSymbols += j;
        }

        // Quotation mark, different depending on if it's opening or closing.
        // Opening quotation mark should be preceded by ',', :, {, [
        // Closing quotation mark should be followed by :, ',', }, ]
        // What's in between is mostly irrelevant (except for endlines).
        else if(j == '\"'){
            if(readInput[index-1] == '\\'){
                index += 1;
                newValue += '\"';
                continue;
            }
            if(!betweenQuotations){
                int k = index-1;
                while(readInput[k] == ' ' || readInput[k] == '\n'){
                    k -= 1;
                }
                if(readInput[k] != ',' && readInput[k] != ':' && readInput[k] != '{' && readInput[k] != '['){
                    cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                    cout << R"(Invalid, string not preceded by ':', ',', '{' or '['')" << endl;
                    return false;
                }
                betweenQuotations = true;
                onlySyntaxSymbols += j;
                newValue += j;
                beforeAndAfterQuotes.first = readInput[k];
            }
            else{
                int k = index+1;
                while(readInput[k] == ' ' || readInput[k] == '\n'){
                    k += 1;
                }
                if(readInput[k] != ',' && readInput[k] != ':' && readInput[k] != '}' && readInput[k] != ']'){
                    cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                    cout << R"(Invalid, string not followed by ':', ',', '}' or ']')" << endl;
                    return false;
                }
                betweenQuotations = false;
                onlySyntaxSymbols += j;
                newValue += j;
                jsonValues.push_back(newValue);
                beforeAndAfterQuotes.second = readInput[k];
                if(beforeAndAfterQuotes.first == '{' && beforeAndAfterQuotes.second == '}'){
                    cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                    cout << "Invalid: String preceded by { and followed by }, this is not valid." << endl;
                    return false;
                }
                if(beforeAndAfterQuotes.first == ':' && beforeAndAfterQuotes.second == ']'){
                    cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                    cout << "Invalid: String preceded by : and followed by ], this is not valid." << endl;
                    return false;
                }
                newValue = "";
                beforeAndAfterQuotes.first = '\0';
                beforeAndAfterQuotes.second = '\0';
            }
        }

        // Left accolade, should be preceded by: nothing (if first accolade), [, or a comma.
        // It should be followed by a quotation mark or '}'.
        else if(j == '{' && !betweenQuotations){
            if(closedJson){
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << "Invalid: Json was already fully closed, check your accolades." << endl;
                return false;
            }
            onlySyntaxSymbols += j;
            accoladeStack += 1;
            addedOpenings.emplace_back("{");
            int k = index-1;
            int l = index+1;
            if(k != -1){
                while(readInput[k] == ' ' || readInput[k] == '\n'){
                    k -= 1;
                }
                if(readInput[k] != '[' && readInput[k] != ','){
                    cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                    cout << R"(Invalid: '{' did not get preceded by a ',' or '[')" << endl;
                    return false;
                }
            }
            while(readInput[l] == ' ' || readInput[l] == '\n'){
                l += 1;
            }
            if(readInput[l] != '\"' && readInput[l] != '}'){
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << R"(Invalid: '{' did not get followed by "" or '}')" << endl;
                return false;
            }
        }

        // Right accolade, should be preceded by {, ], e, l, "" or a number.
        // Should be followed by: nothing (if final accolade) or comma
        else if(j == '}' && !betweenQuotations){
            onlySyntaxSymbols += j;
            accoladeStack -= 1;
            addedOpenings.pop_back();
            if(accoladeStack == 0){
                closedJson = true;
            }
            if(accoladeStack < 0){
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << R"(Error: '}' doesn't have a corresponding '{')" << endl;
                return false;
            }
            int k = index-1;
            int l = index+1;
            while(readInput[k] == ' ' || readInput[k] == '\n'){
                k -= 1;
            }
            if(readInput[k] != ']' && readInput[k] != '{' && readInput[k] != '\"' && readInput[k] != 'l' && readInput[k] != 'e' && find(numbersChar.begin(), numbersChar.end(), readInput[k]) == numbersChar.end()){
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << R"(Invalid: '}' did not get preceded by "", e, l, or a number)" << endl;
                return false;
            }
            if(index == readInput.size()-1){
                break;
            }
            while((readInput[l] == ' ' || readInput[l] == '\n') && l < readInput.size()-1){
                l += 1;
            }
            if(l == readInput.size()-1){
                break;
            }
            if(readInput[l] != ']' && readInput[l] != ','){
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << R"(Invalid: '}' did not get followed by a ']' or a ',')" << endl;
                return false;
            }
        }

        // Left square bracket, should be preceded by : or ,
        // Should be followed by {, "", n, t, f or numbers
        else if(j == '[' && !betweenQuotations){
            onlySyntaxSymbols += j;
            squareBracketStack += 1;
            addedOpenings.emplace_back("[");
            if(addedOpenings.size() == 1){
                startedWithSquareBracket = true;
            }
            if(startedWithSquareBracket && closedJson){
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << "Invalid: \'[\' found but Json was already closed, check your brackets." << endl;
                return false;
            }
            int k = index-1;
            int l = index+1;
            while(readInput[k] == ' ' || readInput[k] == '\n'){
                k -= 1;
            }
            while(readInput[l] == ' ' || readInput[l] == '\n'){
                l += 1;
            }
            if(k == -1){
                index += 1;
                continue;
            }
            if(readInput[k] != ':' && readInput[k] != ','){
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << R"(Invalid: '[' did not get preceded by ':' or ',')" << endl;
                return false;
            }
            if(readInput[l] != ']' && readInput[l] != '{' && readInput[l] != '\"' && readInput[l] != 'n' && readInput[l] != 't' && readInput[l] != 'f' && find(numbersChar.begin(), numbersChar.end(), readInput[l]) == numbersChar.end()){
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << R"(Invalid: '[' did not get followed by '{', "", 'n', 't', 'f' or a number)" << endl;
                return false;
            }
        }

        // Right square bracket, should be preceded by "", e, l, }, or a number
        // Should be followed by a comma, ']' or '}'
        else if(j == ']' && !betweenQuotations){
            onlySyntaxSymbols += j;
            squareBracketStack -= 1;
            addedOpenings.pop_back();
            if(squareBracketStack == 0 && startedWithSquareBracket){
                closedJson = true;
            }
            if(squareBracketStack < 0){
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << R"(Error: ']' doesn't have a corresponding '[')" << endl;
                return false;
            }
            int k = index-1;
            int l = index+1;
            while(readInput[k] == ' ' || readInput[k] == '\n'){
                k -= 1;
            }
            while(readInput[l] == ' ' || readInput[l] == '\n'){
                l += 1;
            }
            if(l == readInput.size()){
                index += 1;
                continue;
            }
            if(readInput[k] != '[' && readInput[k] != ']' && readInput[k] != '}' && readInput[k] != 'e' && readInput[k] != 'l' && readInput[k] != '\"' && find(numbersChar.begin(), numbersChar.end(), readInput[k]) == numbersChar.end()){
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << R"(Invalid: ']' did not get preceded by '}', "", 'e', 'l' or a number)" << endl;
                return false;
            }
            if(readInput[l] != '}' && readInput[l] != ',' && readInput[l] != ']'){
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << R"(Invalid: ']' did not get followed by '}', ',' or ']')" << endl;
                return false;
            }
        }

        else if(j == ':'){
            if(!betweenQuotations){
                int k = index-1;
                while(readInput[k] == ' ' || readInput[k] == '\n'){
                    k -= 1;
                }
                if(readInput[k] != '\"'){
                    cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                    cout << R"(Invalid: expected "" before ':', but could not be found.)" << endl;
                    return false;
                }
                if(addedOpenings[addedOpenings.size()-1] != "{"){
                    cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                    cout << "Invalid: ':' between square brackets." << endl;
                    return false;
                }
                onlySyntaxSymbols += j;
                arguments.push_back(jsonValues[jsonValues.size()-1]);
                jsonValues.pop_back();
            }
            else{
                newValue += j;
            }
        }

        else if((j == 'n' || j == 't') && !betweenQuotations){
            string valueQuestionMark(1, j);
            for(int p = 1; p <= 3; p++){
                valueQuestionMark += readInput[index+p];
            }
            if(valueQuestionMark == "null" || valueQuestionMark == "true"){
                int k = index + 4;
                while(readInput[k] == ' ' || readInput[k] == '\n'){
                    k += 1;
                }
                if(readInput[k] != ',' && readInput[k] != '}' && readInput[k] != ']'){
                    cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                    cout << R"(Error: No ',', '}' or ']' after valid value.)" << endl;
                    return false;
                }
                jsonValues.push_back(valueQuestionMark);
                index += 1;
                continue;
            }
            else{
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << "Invalid value \'" + valueQuestionMark + "\': value should be an integer, double, bool or null." << endl;
                return false;
            }
        }

        else if(j == 'f' && !betweenQuotations){
            string falseQuestionMark(1, j);
            for(int p = 1; p <= 4; p++){
                falseQuestionMark += readInput[index+p];
            }
            if(falseQuestionMark == "false"){
                int k = index + 5;
                while(readInput[k] == ' ' || readInput[k] == '\n'){
                    k += 1;
                }
                if(readInput[k] != ',' && readInput[k] != '}' && readInput[k] != ']'){
                    cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                    cout << R"(Error: No ',', '}' or ']' after valid value.)" << endl;
                    return false;
                }
                jsonValues.push_back(falseQuestionMark);
                index += 1;
                continue;
            }
            else{
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << "Invalid value \'" + falseQuestionMark + "\': value should be an integer, double, bool or null." << endl;
                return false;
            }
        }

        else if(j == 'u' && !betweenQuotations){
            if(readInput[index-1] != 'r' && readInput[index-1] != 'n'){
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << "Invalid character \'u\'." << endl;
                return false;
            }
            if(readInput[index+1] != 'e' && readInput[index+1] != 'l'){
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << "Invalid character \'u\'." << endl;
                return false;
            }
        }

        else if(j == 'l' && !betweenQuotations){
            if(readInput[index-1] != 'l' && readInput[index-1] != 'u' && readInput[index-1] != 'a'){
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << "Invalid character \'l\'." << endl;
                return false;
            }
            if(readInput[index+1] == 'l' || readInput[index+1] == 's'){
                index += 1;
                continue;
            }
            int k = index + 1;
            if(readInput[k] != 'l' && readInput[index+1] == 's') {
                while (readInput[k] == ' ' || readInput[k] == '\n') {
                    k += 1;
                }
                if (readInput[k] != ',' && readInput[k] != '}' && readInput[k] != ']') {
                    cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                    cout << "Invalid character \'l\'." << endl;
                    return false;
                }
            }
        }

        else if(j == 'r' && !betweenQuotations){
            if(readInput[index-1] != 't'){
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << "Invalid character \'r\'." << endl;
                return false;
            }
            if(readInput[index+1] != 'u'){
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << "Invalid character \'r\'." << endl;
                return false;
            }
        }

        else if(j == 'a' && !betweenQuotations){
            if(readInput[index-1] != 'f'){
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << "Invalid character \'a\'." << endl;
                return false;
            }
            if(readInput[index+1] != 'l'){
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << "Invalid character \'a\'." << endl;
                return false;
            }
        }

        else if(j == 's' && !betweenQuotations){
            if(readInput[index-1] != 'l'){
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << "Invalid character \'s\'." << endl;
                return false;
            }
            if(readInput[index+1] != 'e'){
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << "Invalid character \'s\'." << endl;
                return false;
            }
        }

        else if(j == 'e' && !betweenQuotations){
            if(readInput[index-1] != 'u' && readInput[index-1] != 's'){
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << "Invalid character \'e\'." << endl;
                return false;
            }
            int k = index + 1;
            while(readInput[k] == ' ' || readInput[k] == '\n') {
                k += 1;
            }
            if (readInput[k] != ',' && readInput[k] != '}' && readInput[k] != ']') {
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << "Invalid character \'e\'." << endl;
                return false;
            }
        }

        else if(find(numbersChar.begin(), numbersChar.end(), j) != numbersChar.end() && !betweenQuotations){
            if(firstNumber) {
                beforeAndAfterNumber.first = readInput[index-1];
                firstNumber = false;
            }
            newValue += j;
            int k = index+1;
            int l = k;
            int periods = 0;
            while(find(numbersChar.begin(), numbersChar.end(), readInput[k]) != numbersChar.end() || readInput[k] == '.'){
                if(readInput[k] == '.'){
                    periods += 1;
                }
                k += 1;
            }
            int p = k;
            while(readInput[p] == ' ' || readInput[p] == '\n'){
                p += 1;
            }
            bool passedEndline = false;
            while(readInput[l] != '}' && readInput[l] != ',' && readInput[l] != ']'){
                if(readInput[l] == '\n'){
                    if(!passedEndline){
                        passedEndline = true;
                    }
                    else if(readInput[l] != '\n') {
                        cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                        cout << "Invalid: Endline between number." << endl;
                        return false;
                    }
                }
                l += 1;
            }
            if(p != l || periods > 1){
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << "Invalid number value." << endl;
                return false;
            }
            string end = readInput.substr(index+1, l-index-1);
            bool noMoreNumbers = true;
            for(auto m: end){
                if(find(numbersChar.begin(), numbersChar.end(), m) != numbersChar.end()){
                    noMoreNumbers = false;
                }
            }
            if(noMoreNumbers){
                beforeAndAfterNumber.second = readInput[l];
                if(beforeAndAfterNumber.first == '{' && beforeAndAfterNumber.second == '}'){
                    cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                    cout << "Invalid: Number preceded by { and followed by }, this is not valid." << endl;
                    return false;
                }
                if(beforeAndAfterNumber.first == ':' && beforeAndAfterNumber.second == ']'){
                    cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                    cout << "Invalid: Number preceded by : and followed by ], this is not valid." << endl;
                    return false;
                }
                jsonValues.push_back(newValue);
                newValue = "";
                beforeAndAfterNumber.first = '\0';
                beforeAndAfterNumber.second = '\0';
                firstNumber = true;
            }
        }

        else if(j == '.' && !betweenQuotations){
            if(find(numbersChar.begin(), numbersChar.end(), readInput[index-1]) == numbersChar.end()){
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << "Invalid \'.\', these are only allowed for adding doubles." << endl;
                return false;
            }
            if(find(numbersChar.begin(), numbersChar.end(), readInput[index+1]) == numbersChar.end()){
                cout << "Error found on line " + to_string(currentLine) + ":" << endl;
                cout << "Invalid \'.\', these are only allowed for adding doubles." << endl;
                return false;
            }
            newValue += j;
        }

        else if(betweenQuotations){
            newValue += j;
        }

        else{
            string invalid(1, j);
            cout << "Error found on line " + to_string(currentLine) + ":" << endl;
            cout << "Invalid character: \'" + invalid + "\'" << endl;
            return false;
        }

        index += 1;

    }

    if(betweenQuotations){
        cout << "Invalid: Odd amount of quotation marks, expected an even amount." << endl;
        return false;
    }

    if(accoladeStack != 0){
        cout << "Error: At least one '{' has no corresponding '}'" << endl;
        return false;
    }

    if(squareBracketStack != 0){
        cout << "Error: At least one '[' has no corresponding ']'" << endl;
        return false;
    }

    for(auto &m: arguments){
        for(int n = 0; n < m.size(); n++){
            if(m[n] == '\\'){
                int o = n+1;
                if(m[o] == 'n'){
                    continue;
                }
                if(m[o] == '\"'){
                    continue;
                }
                int backslash = 1;
                while(m[o] == '\\'){
                    o += 1;
                    backslash += 1;
                    n += 1;
                }
                if(backslash%2 != 0){
                    cout << "Error with string value: \'" + m + "\':" << endl;
                    cout << "Invalid: Argument has odd amount of consecutive backslashes inside of it" << endl;
                    return false;
                }
            }
        }
    }

    for(auto &m: jsonValues){
        if(m == "null" || m == "true" || m == "false"){
            continue;
        }
        if(m[0] == '\"'){
            for(int n = 0; n < m.size(); n++){
                if(m[n] == '\\'){
                    int o = n+1;
                    if(m[o] == 'n'){
                        continue;
                    }
                    if(m[o] == '\"'){
                        continue;
                    }
                    int backslash = 1;
                    while(m[o] == '\\'){
                        o += 1;
                        backslash += 1;
                        n += 1;
                    }
                    if(backslash%2 != 0){
                        cout << "Error with string value: \'" + m + "\':" << endl;
                        cout << "Invalid: String value has odd amount of consecutive backslashes inside of it" << endl;
                        return false;
                    }
                }
            }
            if(m[m.size()-1] != '\"'){
                cout << "Error with string value: \'" + m + "\':" << endl;
                cout << "Invalid: missing string end quote." << endl;
                return false;
            }
        }
        if(find(numbersChar.begin(), numbersChar.end(), m[0]) != numbersChar.end()){
            if(std::count(m.begin(), m.end(), '.') > 1){
                cout << "Error with double value: \'" + m + "\'" << endl;
                cout << "Invalid: Double has more than one period." << endl;
                return false;
            }
        }
    }

    return true;

}

bool SyntaxChecker::validHTML(const string &htmlFile) {

    /*
     * THIS SYNTAX CHECKER FULLY WORKS NOW!!
     *
     * RULES OF THE HTML SYNTAX:
     *
     * Correct use of tags (label only has letters, correct use of attributes).
     *
     * Every tag must be closed.
     *
     * Tags must also be correctly positioned.
     *
     * Tags can have the same name, but need to be properly closed in between.
     *
     * Attributes belong in the opening tag.
     */

    // Read HTML file and save all contents in a string, line per line.
    ifstream i(htmlFile);
    string line;
    string html;
    while(getline(i, line)){
        html += line;
        html += "\n";
    }
    html.pop_back();

    // Save all tags to check if they're correctly positioned, one list contains all tags, another one only the start tags.
    // A stack-implementation will be used to check on correct use of end-tags.
    vector<pair<int, string>> allTags;
    vector<pair<int, string>> beginTags;

    // Keep track of the current line.
    int currentLine = 1;

    // Look out for quotations, they're only allowed under certain conditions.
    bool insideQuotations = false;
    multimap<pair<int, string>, pair<int, int>> TagQuotes;

    // Iterate over every character of string html.
    for(int j = 0; j < html.size(); j++){

        string newTag;

        // If the current character is '<', this possibly indicates a tag.
        if(html[j] == '<'){

            newTag += html[j];

            // No tag if the first character is not allowed, like '%', ',' or something else in htmlIllegal
            string test;
            test += html[j+1];
            if(find(htmlIllegalBegin.begin(), htmlIllegalBegin.end(), test) != htmlIllegalBegin.end() || test == " "){
                continue;
            }

            // Else, keep iterating until the matching '>'.
            int k = j+1;
            int beginQuote = 0;
            int beginQuoteIndex;
            char beginQuotation = '\0';
            int endQuote = 0;
            char endQuotation = '\0';
            vector<pair<int, int>> quotationIndexPairs;

            // Bool to keep track of when a double break is needed.
            bool doubleBreak = false;

            while(html[k] != '>' || insideQuotations){
                // Check every character on the path.
                string currentChar(1, html[k]);
                // Reached end of string? No tag.
                if(k == html.size()-1){
                    doubleBreak = true;
                    break;
                }
                // : case, these are not allowed in combination with other characters
                if(html[k] == ':'){
                    int p = k;
                    while(p > j+2){
                        if(p > j+1 && html[p] != ':'){
                            cout << "Error in tag on line " + to_string(currentLine) + " \':\' cannot be combined with other characters, consider renaming the tag." << endl;
                            return false;
                        }
                        p -= 1;
                    }
                    if(html[p] != ':' && html[p] != '/'){
                        cout << "Error in tag on line " + to_string(currentLine) + " \':\' cannot be combined with other characters, consider renaming the tag." << endl;
                        return false;
                    }
                }
                // Space case, these are only allowed under certain conditions.
                if(html[k] == ' '){
                    if(insideQuotations){
                        newTag += ' ';
                        k += 1;
                    }
                    int l = k+1;
                    int n = l;
                    bool textBefore = false;
                    bool textAfter = false;
                    for(auto m: newTag){
                        if(m != ' ' && m != '<'){
                            textBefore = true;
                            break;
                        }
                    }
                    while(html[l] != '>'){
                        if(html[l] != ' '){
                            textAfter = true;
                            break;
                        }
                        l += 1;
                    }
                    while(html[n] != '>'){
                        n += 1;
                    }
                    if(!textBefore){
                        break;
                    }
                    if(textAfter){
                        string equationAndQuotationCheck = html.substr(k, n-k);
                        if(equationAndQuotationCheck.find('=') == string::npos){
                            cout << "Invalid space usage in tag on line " + to_string(currentLine) + ": Remove the space or add a proper attribute." << endl;
                            return false;
                        }
                        if(equationAndQuotationCheck.find('\'') == string::npos && equationAndQuotationCheck.find('\"') == string::npos){
                            cout << "Invalid space usage in tag on line " + to_string(currentLine) + ": Remove the space or add a proper attribute." << endl;
                            return false;
                        }
                    }
                }
                // In the '=' case, check if it isn't after the tag name.
                if(html[k] == '='){
                    int l = k-1;
                    while(html[l] == ' '){
                        l -= 1;
                    }
                    // A space between the tag name and an attribute is required.
                    string p = html.substr(j, l-k);
                    if(p.find(' ') == string::npos){
                        cout << "Error: tag at line " + to_string(currentLine) + " has an attribute connected to tag name." << endl;
                        return false;
                    }
                    while(html[l] != ' '){
                        l -= 1;
                        if(html[l] == '\'' || html[l] == '\"'){
                            cout << "Error: No space between 2 attributes in tag on line " + to_string(currentLine) + "." << endl;
                            return false;
                        }
                    }
                }
                // In case of quotations, if it's the first, a '=' should be before it, the second is nothing special.
                // More than 2 quotation marks are not allowed, both should be the same too.
                if(html[k] == '\"' || html[k] == '\''){
                    if(!insideQuotations) {
                        int l = k - 1;
                        while (html[l] == ' ') {
                            l -= 1;
                        }
                        if (html[l] != '=') {
                            cout << "Error: On line " + to_string(currentLine) +
                                    " expected \'=\' before opening quotation mark." << endl;
                            return false;
                        }
                        insideQuotations = true;
                        beginQuote += 1;
                        beginQuotation = html[k];
                        beginQuoteIndex = k;
                    }
                    else{
                        insideQuotations = false;
                        endQuote += 1;
                        endQuotation = html[k];
                        quotationIndexPairs.emplace_back(beginQuoteIndex-j, k);
                        if(html[k+1] != ' ' && html[k+1] != '>'){
                            cout << "Invalid: No space after attribute in tag on line " + to_string(currentLine) + "." << endl;
                            return false;
                        }
                    }
                }
                // Check on invalid characters inside the tag.
                else if(find(htmlIllegalBegin.begin(), htmlIllegalBegin.end(), test) != htmlIllegalBegin.end() && !insideQuotations){
                    cout << "Invalid character \'" + currentChar + "\' in tag " +  " at line " + to_string(currentLine) + "." << endl;
                    return false;
                }
                newTag += html[k];
                k += 1;
            }
            if(doubleBreak){
                break;
            }
            if(endQuote != beginQuote){
                cout << "Error: On line " + to_string(currentLine) + " odd amount of quotation marks found." << endl;
                return false;
            }
            if(endQuotation != beginQuotation){
                cout << "Error: On Line " + to_string(currentLine) + " different quotation marks were used." << endl;
                return false;
            }
            newTag += html[k];
            for(auto m: quotationIndexPairs) {
                TagQuotes.insert({make_pair(currentLine, newTag), m});
            }
            allTags.emplace_back(currentLine, newTag);
            insideQuotations = false;
            beginQuoteIndex = 0;
        }
        else if(html[j] == '\n'){
            currentLine += 1;
        }

    }

    for(auto j: allTags){

        if(j.second == "</>"){
            cout << "Invalid: tag at line " + to_string(j.first) + "has no label.";
            return false;
        }

        if(j.second[1] != '/'){
            vector<int> left;
            int currentLeft;
            vector<int> right;
            int currentRight;
            int currentIndex;
            bool insideMap = false;
            for(auto &m: TagQuotes){
                if(m.first == j){
                    insideMap = true;
                    left.push_back(m.second.first);
                    right.push_back(m.second.second);
                }
            }
            if(insideMap){
                currentLeft = left[0];
                currentRight = right[0];
                currentIndex = 0;
            }
            for(int k = 2; k < j.second.size()-1; k++){
                string s;
                s += j.second[k];
                if(insideMap && k <= currentRight && k >= currentLeft){
                    if(k == currentRight && currentIndex < right.size()-1){
                        currentIndex += 1;
                        currentLeft = left[currentIndex];
                        currentRight = right[currentIndex];
                    }
                    continue;
                }
                if(find(htmlLegal2.begin(), htmlLegal2.end(), s) != htmlLegal2.end()){
                    cout << "Invalid character \'" + s + "\' in tag at line " + to_string(j.first) << endl;
                    return false;
                }
            }
            if(j.second[j.second.size()-2] == '\\'){
                cout << "Warning: Self-closing tag detected, this might not work on your browser.";
            }
            beginTags.push_back(j);
        }

        else{
            int m = 2;
            string withoutSlash = "<";
            while(j.second[m] != '>'){
                if(j.second[m] == ' '){
                    break;
                }
                withoutSlash += j.second[m];
                m += 1;
            }
            withoutSlash += '>';
            string accordingTagWithoutAttributes;
            int begin = 0;
            while(beginTags[beginTags.size()-1].second[begin] != ' ' && begin < beginTags[beginTags.size()-1].second.size()){
                accordingTagWithoutAttributes += beginTags[beginTags.size()-1].second[begin];
                begin += 1;
            }
            if(accordingTagWithoutAttributes[accordingTagWithoutAttributes.size()-1] != '>') {
                accordingTagWithoutAttributes += ">";
            }
            if(beginTags.empty() || withoutSlash != accordingTagWithoutAttributes){
                if(beginTags.empty()){
                    cout << "No beginning tag found to match endtag " + j.second + " at line " + to_string(j.first) << endl;
                    return false;
                }
                cout << "Invalid end-tag match, \'" + j.second + "\' on line " + to_string(j.first) + " got compared with \'" +
                        beginTags[beginTags.size()-1].second + "\' on line " + to_string(beginTags[beginTags.size()-1].first) + "." << endl;
                return false;
            }
            else{
                beginTags.pop_back();
            }
        }
    }

    for(auto j: allTags){
        if(j.second[1] == '/'){
            for(int k = 2; k < j.second.size(); k++){
                if(j.second[k] == ' '){
                    int l = k+1;
                    while(j.second[l] == ' '){
                        l += 1;
                    }
                    if(l != j.second.size()-1){
                        cout << "Invalid: Endtag at line " + to_string(j.first) + " contains potential attribute, put it in the opening tag." << endl;
                        return false;
                    }
                }
                else{
                    continue;
                }
            }
        }
    }

    if(!beginTags.empty()){
        for(auto &m: beginTags){
            cout << "Invalid: Endtag missing for: " + m.second << endl;
        }
        return false;
    }

    cout << "Valid HTML file!" << endl;
    return true;

}