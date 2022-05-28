//
// Created by liam on 18/05/2022.
//

#include "Word.h"

/// Private Functions ///
bool Word::isVowel(char* c) {
    // its better to use !isConsonant(), for if Y is counted as a vowel
    return (*c == 'a' || *c == 'e' || *c == 'i' || *c == 'o' || *c == 'u');
}

bool Word::isConsonant(string word, int index) {
    // a consonant is a letter that is not a vowel, and other than Y preceded by a consonant
    bool b = isVowel(&word[index]);
    if (b == false) {
        // is not vowel, check for Y
        if (word[index] == 'y') {
            if (index != 0) {
                return !isConsonant(word, index-1); // check if letter before Y is not a consonant;
            }
            else {return true;}
        }
        else {return true;}
    }
    else {return false;}
}

bool Word::changeSuffix(string& word, string S1, string S2) {
    if (word.length() < S1.length()) {return false;}
    if (word.substr(word.length()-S1.length(), word.length()) == S1) {
        word = word.erase(word.length()-S1.length(), word.length());
        word += S2;
        return true;
    }
    return false;
}

int Word::calculateMeasure(string word) {
    // the measure of a word is the amount of times a vowel is followed by a consonant in a word
    // every word can be represented with the following formula:
    // [C] (VC)^m [V]
    // (the square brackets mean it can be present or missing)
    int measure = 0;
    bool previousIsVowel = false;
    for (int i = 0; i<word.length(); i++) {
        if (previousIsVowel) {
            // check if this letter is consonant
            if (isConsonant(word, i)) {
                measure += 1;
                previousIsVowel = false;
            }
            else {
                previousIsVowel = true;
            }
        }
        else {
            previousIsVowel = !(isConsonant(word, i));
        }
    }
    return measure;
}

bool Word::containsVowel(string word) {
    for (int i = 0; i < word.length(); i++) {
        if (!(isConsonant(word, i))) {
            return true;
        }
    }
    return false;
}

bool Word::endsWithDoubleConsonant(string word) {
    if (word[word.length()-1] == word[word.length()-2]) {
        if (isConsonant(word, word.length()-1)) {
            return true;
        }
    }
    return false;
}

bool Word::endsCVC(string word) {
    // returns true if string ends with Consonant Vowel Consonant
    // (second consonant is not W, X or Y)
    if (word.length() >= 3) {
        if (isConsonant(word, word.length()-3) && !isConsonant(word, word.length() - 2)) {
            if (isConsonant(word, word.length() - 1) && word[word.length() - 1] != 'w' &&
                word[word.length() - 1] != 'x' && word[word.length() - 1] != 'y') {
                return true;
            }
        }
    }
    return false;
}

/////////////////////////
/// Public Functions ///

Word::Word(const string &token) : token(token) {
    specialChar = false;
    abbreviation = false;
    root = Stem();
}

string Word::getString() {
    return token;
}

void Word::setSpecialChar() {
    specialChar = true;
}

bool Word::isSpecialChar() {
    return specialChar;
}

void Word::setAbbreviation() {
    abbreviation = true;
}

bool Word::isAbbreviation() {
    return abbreviation;
}

string Word::Stem() {
    /*
     * The porter algorithm for suffix trimming (stemming)
     * source: http://facweb.cs.depaul.edu/mobasher/classes/csc575/papers/porter-algorithm.html
     */
    string rootForm;
    bool skip = false; // bool used to skip parts if necessary
    bool skip2 = false; // another bool used for deciding if certain parts need to be skipped or not

    //cout << "word: "<< token << ", measure: " << calculateMeasure(token) << endl;

    if (token == "bled") {
        cout << "FOR DEBUG" << endl;
    }

    cout << token << " -> ";
    ///
    // SSES -> SS
    if (!skip) {
        skip = changeSuffix(token, "sses", "ss");
    }
    // IES -> I
    if (!skip) {
        skip = changeSuffix(token, "ies", "i");
    }
    // SS -> SS
    if (!skip) {
        skip = changeSuffix(token, "ss", "ss");
    }
    // S ->
    if (!skip) {
        skip = changeSuffix(token, "s", "");
    }

    ///
    skip = false; // reset bool
    skip2 = true;
    ///

    // (m>0) EED -> EE (ALL CONDITIONS APPLY TO ROOT)
    if (!skip) {
        skip = changeSuffix(token, "eed", "");
        if (!(calculateMeasure(token) > 0) && skip == true) { // check root for m > 0
            token += "eed"; // put suffix back to how it was
        } else if (calculateMeasure(token) > 0 && skip == true){
            changeSuffix(token, "", "ee");
        }
    }
    // (*v*) ED ->
    if (!skip) {
        skip = changeSuffix(token, "ed", "");
        if (!containsVowel(token) && skip == true) {
            token += "ed"; // put suffix back to how it was
        }
        if (containsVowel(token) && skip) {skip2 = false;}
    }
    // (*v*) ING ->
    if (!skip) {
        skip = changeSuffix(token, "ing", "");
        if (!containsVowel(token) && skip == true) {
            token += "ing"; // put suffix back to how it was
        }
        if (containsVowel(token) && skip) {skip2 = false;}
    }

    // extra rule if 2nd or 3rd rules applied
    skip = false;
    if (!skip2) {
        // AT -> ATE
        if (!skip) {
            skip = changeSuffix(token, "at", "ate");
        }
        // BL -> BLE
        if (!skip) {
            skip = changeSuffix(token, "bl", "ble");
        }
        // IZ -> IZE
        if (!skip) {
            skip = changeSuffix(token, "iz", "ize");
        }
        // double consonants (not L, S or Z)
        if (!skip) {
            if (endsWithDoubleConsonant(token) && token[token.length()-1] != 'l' && token[token.length()-1] != 's' && token[token.length()-1] != 'z') {
                token = token.substr(0, token.length()-1);
                skip = true;
            }
        }

        if (!skip) {
            if (calculateMeasure(token) == 1 && endsCVC(token)) {
                token += "e";
                skip = true;
            }
        }
    }

    ///
    skip = false; // reset bool
    skip2 = false;
    ///

    // (*v*) Y -> I
    if (!skip) {
        skip = changeSuffix(token, "y", "i");
        if (!containsVowel(token) && skip == true) {
            token += "ing"; // put suffix back to how it was
        }
        if (containsVowel(token) && skip) {skip2 = false;}
    }

    ///
    skip = false; // reset bool
    skip2 = false;
    ///

    // (m>0) ATIONAL -> ATE
    if (!skip) {
        skip = changeSuffix(token, "ational", "");
        if (!(calculateMeasure(token) > 0) && skip == true) {
            changeSuffix(token, "", "ational"); // put suffix back to how it was
        } else if (calculateMeasure(token) > 0 && skip == true){
            changeSuffix(token, "", "ate");
        }
    }

    // (m>0) TIONAL -> TION
    if (!skip) {
        skip = changeSuffix(token, "tional", "");
        if (!(calculateMeasure(token) > 0) && skip == true) {
            changeSuffix(token, "", "tional"); // put suffix back to how it was
        } else if (calculateMeasure(token) > 0 && skip == true){
            changeSuffix(token, "", "tion");
        }
    }

    // (m>0) ENCI -> ENCE
    if (!skip) {
        skip = changeSuffix(token, "enci", "");
        if (!(calculateMeasure(token) > 0) && skip == true) {
            changeSuffix(token, "", "enci"); // put suffix back to how it was
        } else if (calculateMeasure(token) > 0 && skip == true){
            changeSuffix(token, "", "ence");
        }
    }

    // (m>0) ANCI -> ANCE
    if (!skip) {
        skip = changeSuffix(token, "anci", "");
        if (!(calculateMeasure(token) > 0) && skip == true) {
            changeSuffix(token, "", "anci"); // put suffix back to how it was
        } else if (calculateMeasure(token) > 0 && skip == true){
            changeSuffix(token, "", "ance");
        }
    }

    // (m>0) IZER -> IZE
    if (!skip) {
        skip = changeSuffix(token, "izer", "");
        if (!(calculateMeasure(token) > 0) && skip == true) {
            changeSuffix(token, "", "izer"); // put suffix back to how it was
        } else if (calculateMeasure(token) > 0 && skip == true){
            changeSuffix(token, "", "ize");
        }
    }

    // (m>0) ABLI -> ABLE
    if (!skip) {
        skip = changeSuffix(token, "abli", "");
        if (!(calculateMeasure(token) > 0) && skip == true) {
            changeSuffix(token, "", "abli"); // put suffix back to how it was
        } else if (calculateMeasure(token) > 0 && skip == true){
            changeSuffix(token, "", "able");
        }
    }

    // (m>0) ALLI -> AL
    if (!skip) {
        skip = changeSuffix(token, "alli", "");
        if (!(calculateMeasure(token) > 0) && skip == true) {
            changeSuffix(token, "", "alli"); // put suffix back to how it was
        } else if (calculateMeasure(token) > 0 && skip == true){
            changeSuffix(token, "", "al");
        }
    }

    // (m>0) ENTLI -> ENT
    if (!skip) {
        skip = changeSuffix(token, "entli", "");
        if (!(calculateMeasure(token) > 0) && skip == true) {
            changeSuffix(token, "", "entli"); // put suffix back to how it was
        } else if (calculateMeasure(token) > 0 && skip == true){
            changeSuffix(token, "", "ent");
        }
    }

    // (m>0) ELI -> E
    if (!skip) {
        skip = changeSuffix(token, "eli", "");
        if (!(calculateMeasure(token) > 0) && skip == true) {
            changeSuffix(token, "", "eli"); // put suffix back to how it was
        } else if (calculateMeasure(token) > 0 && skip == true){
            changeSuffix(token, "", "e");
        }
    }

    // (m>0) OUSLI -> OUS
    if (!skip) {
        skip = changeSuffix(token, "ousli", "");
        if (!(calculateMeasure(token) > 0) && skip == true) {
            changeSuffix(token, "", "ousli"); // put suffix back to how it was
        } else if (calculateMeasure(token) > 0 && skip == true){
            changeSuffix(token, "", "ous");
        }
    }

    // (m>0) IZATION -> IZE
    if (!skip) {
        skip = changeSuffix(token, "ization", "");
        if (!(calculateMeasure(token) > 0) && skip == true) {
            changeSuffix(token, "", "ization"); // put suffix back to how it was
        } else if (calculateMeasure(token) > 0 && skip == true){
            changeSuffix(token, "", "ize");
        }
    }

    // (m>0) ATION -> ATE
    if (!skip) {
        skip = changeSuffix(token, "ation", "");
        if (!(calculateMeasure(token) > 0) && skip == true) {
            changeSuffix(token, "", "ation"); // put suffix back to how it was
        } else if (calculateMeasure(token) > 0 && skip == true){
            changeSuffix(token, "", "ate");
        }
    }

    // (m>0) ATOR -> ATE
    if (!skip) {
        skip = changeSuffix(token, "ator", "");
        if (!(calculateMeasure(token) > 0) && skip == true) {
            changeSuffix(token, "", "ator"); // put suffix back to how it was
        } else if (calculateMeasure(token) > 0 && skip == true){
            changeSuffix(token, "", "ate");
        }
    }

    // (m>0) ALISM -> AL
    if (!skip) {
        skip = changeSuffix(token, "alism", "");
        if (!(calculateMeasure(token) > 0) && skip == true) {
            changeSuffix(token, "", "alism"); // put suffix back to how it was
        } else if (calculateMeasure(token) > 0 && skip == true){
            changeSuffix(token, "", "al");
        }
    }

    // (m>0) IVENESS -> IVE
    if (!skip) {
        skip = changeSuffix(token, "iveness", "");
        if (!(calculateMeasure(token) > 0) && skip == true) {
            changeSuffix(token, "", "iveness"); // put suffix back to how it was
        } else if (calculateMeasure(token) > 0 && skip == true){
            changeSuffix(token, "", "ive");
        }
    }

    // (m>0) FULNESS -> FUL
    if (!skip) {
        skip = changeSuffix(token, "fulness", "");
        if (!(calculateMeasure(token) > 0) && skip == true) {
            changeSuffix(token, "", "fulness"); // put suffix back to how it was
        } else if (calculateMeasure(token) > 0 && skip == true){
            changeSuffix(token, "", "ful");
        }
    }

    // (m>0) OUSNESS -> OUS
    if (!skip) {
        skip = changeSuffix(token, "ousness", "");
        if (!(calculateMeasure(token) > 0) && skip == true) {
            changeSuffix(token, "", "ousness"); // put suffix back to how it was
        } else if (calculateMeasure(token) > 0 && skip == true){
            changeSuffix(token, "", "ous");
        }
    }

    // (m>0) ALITI -> AL
    if (!skip) {
        skip = changeSuffix(token, "aliti", "");
        if (!(calculateMeasure(token) > 0) && skip == true) {
            changeSuffix(token, "", "aliti"); // put suffix back to how it was
        } else if (calculateMeasure(token) > 0 && skip == true){
            changeSuffix(token, "", "al");
        }
    }

    // (m>0) IVITI -> IVE
    if (!skip) {
        skip = changeSuffix(token, "iviti", "");
        if (!(calculateMeasure(token) > 0) && skip == true) {
            changeSuffix(token, "", "iviti"); // put suffix back to how it was
        } else if (calculateMeasure(token) > 0 && skip == true){
            changeSuffix(token, "", "ive");
        }
    }

    // (m>0) BILITI -> BLE
    if (!skip) {
        skip = changeSuffix(token, "biliti", "");
        if (!(calculateMeasure(token) > 0) && skip == true) {
            changeSuffix(token, "", "biliti"); // put suffix back to how it was
        } else if (calculateMeasure(token) > 0 && skip == true){
            changeSuffix(token, "", "ble");
        }
    }

    ///
    skip = false; // reset bool
    skip2 = false;
    ///

    // (m>0) ICATE -> IC
    if (!skip) {
        skip = changeSuffix(token, "icate", "");
        if (!(calculateMeasure(token) > 0) && skip == true) {
            changeSuffix(token, "", "icate"); // put suffix back to how it was
        } else if (calculateMeasure(token) > 0 && skip == true){
            changeSuffix(token, "", "ic");
        }
    }

    // (m>0) ATIVE ->
    if (!skip) {
        skip = changeSuffix(token, "ative", "");
        if (!(calculateMeasure(token) > 0) && skip == true) {
            changeSuffix(token, "", "ative"); // put suffix back to how it was
        }
    }

    // (m>0) ALIZE -> AL
    if (!skip) {
        skip = changeSuffix(token, "alize", "");
        if (!(calculateMeasure(token) > 0) && skip == true) {
            changeSuffix(token, "", "alize"); // put suffix back to how it was
        } else if (calculateMeasure(token) > 0 && skip == true){
            changeSuffix(token, "", "al");
        }
    }

    // (m>0) ICITI -> IC
    if (!skip) {
        skip = changeSuffix(token, "iciti", "");
        if (!(calculateMeasure(token) > 0) && skip == true) {
            changeSuffix(token, "", "iciti"); // put suffix back to how it was
        } else if (calculateMeasure(token) > 0 && skip == true){
            changeSuffix(token, "", "ic");
        }
    }

    // (m>0) ICAL -> IC
    if (!skip) {
        skip = changeSuffix(token, "ical", "");
        if (!(calculateMeasure(token) > 0) && skip == true) {
            changeSuffix(token, "", "ical"); // put suffix back to how it was
        } else if (calculateMeasure(token) > 0 && skip == true){
            changeSuffix(token, "", "ic");
        }
    }

    // (m>0) FUL ->
    if (!skip) {
        skip = changeSuffix(token, "ful", "");
        if (!(calculateMeasure(token) > 0) && skip == true) {
            changeSuffix(token, "", "ful"); // put suffix back to how it was
        }
    }

    // (m>0) NESS ->
    if (!skip) {
        skip = changeSuffix(token, "ness", "");
        if (!(calculateMeasure(token) > 0) && skip == true) {
            changeSuffix(token, "", "ness"); // put suffix back to how it was
        }
    }

    ///
    cout << token << endl;
    return rootForm;
}
