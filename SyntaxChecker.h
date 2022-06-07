//
// Created by axeld on 22/05/2022.
//

#ifndef TA_SYNTAXCHECKER_H
#define TA_SYNTAXCHECKER_H
#include "string"
#include <vector>
using namespace std;


class SyntaxChecker {

    vector<string> lowerCase = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q",
                                  "r", "s", "t", "u", "v", "w", "x", "y", "z"};

    vector<string> upperCase = lowerCase;

    vector<string> otherSymbols = { "²", "&", "\'", "\"", "(", "§", "!", ")", "-", "<", ",", ";", ":", "="};

    vector<string> numbersString = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

    vector<char> numbersChar = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    vector<string> remainingShift = {"³", "_", "*", "%", ">", "?", ".", "/", "\\", "+"};

    vector<string> altGR = {"|", "@", "#", "^", "{", "}", "[", "]", "~", "`", "´", "€"};

    vector<string> htmlIllegalBegin = {",", ".", "=", "&", "@", "\"", "\'", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
                                  "²", "³", "(", ")", "[", "]", "{", "}", "!", "+", "-", "*", "$", "£", "´", "`", "|", "~",
                                  ";", "§", "<", ">", "\\", "°", "#", "^", " "};


    vector<string> htmlLegal2 = {",", "&", "@",
                                 "²", "³", "(", ")", "[", "]", "{", "}", "!", "+", "-", "*", "$", "£", "´", "`", "|", "~",
                                 ";", "§", "<", ">", "\\", "°", "#", "^",};

    vector<string> accentVowelsLC = {"à", "á", "â", "ã", "ä", "é", "è", "ê", "ë", "ù", "ú", "û", "ü",
                                       "ò", "ó", "ô", "õ", "ö", "ì", "í", "ï", "î"};

    vector<string> accentVowelsUC = {"À", "Á", "Â", "Ã", "Ä", "É", "È", "Ê", "Ë", "Ù", "Ú", "Û", "Ü",
                                       "Ò", "Ó", "Ô", "Õ", "Ö", "Ì", "Í", "Ï", "Î"};

    vector<string> everything;

    vector<char> ascii;

    string endLine = "\n";

public:

    const vector<string> &getHtmlIllegalBegin() const;

    bool validJson(string Jsonfile);

    bool validHTML(const string &htmlFile);

    vector<string> getEverything();

    const vector<string> &getNumbersString() const;

    SyntaxChecker();

};


#endif //TA_SYNTAXCHECKER_H
