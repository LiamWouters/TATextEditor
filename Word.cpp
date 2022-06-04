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
    abbreviation = false;
    startquote = false;
    endquote = false;
    root = Stem();
}

string Word::getString() {
    return token;
}

void Word::setString(string t) {
    token = t;
    root = Stem();
}

string Word::getRoot() {
    return root;
}

bool Word::isSpecialChar() {
    return false;
}

void Word::setAbbreviation() {
    abbreviation = true;
}

bool Word::isAbbreviation() {
    return abbreviation;
}

void Word::setStartQuote() {
    startquote = true;
}

bool Word::isStartQuote() {
    return startquote;
}

void Word::setEndQuote() {
    endquote = true;
}

bool Word::isEndQuote() {
    return endquote;
}

string Word::Stem() {
    /*
     * The porter algorithm for suffix trimming (stemming)
     * source: http://facweb.cs.depaul.edu/mobasher/classes/csc575/papers/porter-algorithm.html
     */
    string rootForm = token;
    bool skip = false; // bool used to skip parts if necessary
    bool skip2 = false; // another bool used for deciding if certain parts need to be skipped or not

    ///
    // SSES -> SS
    if (!skip) {
        skip = changeSuffix(rootForm, "sses", "ss");
    }
    // IES -> I
    if (!skip) {
        skip = changeSuffix(rootForm, "ies", "i");
    }
    // SS -> SS
    if (!skip) {
        skip = changeSuffix(rootForm, "ss", "ss");
    }
    // S ->
    if (!skip) {
        skip = changeSuffix(rootForm, "s", "");
    }

    ///
    skip = false; // reset bool
    skip2 = true;
    ///

    // (m>0) EED -> EE (ALL CONDITIONS APPLY TO ROOT)
    if (!skip) {
        skip = changeSuffix(rootForm, "eed", "");
        if (!(calculateMeasure(rootForm) > 0) && skip == true) { // check root for m > 0
            rootForm += "eed"; // put suffix back to how it was
        } else if (calculateMeasure(rootForm) > 0 && skip == true){
            changeSuffix(rootForm, "", "ee");
        }
    }
    // (*v*) ED ->
    if (!skip) {
        skip = changeSuffix(rootForm, "ed", "");
        if (!containsVowel(rootForm) && skip == true) {
            rootForm += "ed"; // put suffix back to how it was
        } else if (containsVowel(rootForm) && skip) {skip2 = false;}
    }
    // (*v*) ING ->
    if (!skip) {
        skip = changeSuffix(rootForm, "ing", "");
        if (!containsVowel(rootForm) && skip == true) {
            rootForm += "ing"; // put suffix back to how it was
        }
        if (containsVowel(rootForm) && skip) {skip2 = false;}
    }

    // extra rule if 2nd or 3rd rules applied
    skip = false;
    if (!skip2) {
        // AT -> ATE
        if (!skip) {
            skip = changeSuffix(rootForm, "at", "ate");
        }
        // BL -> BLE
        if (!skip) {
            skip = changeSuffix(rootForm, "bl", "ble");
        }
        // IZ -> IZE
        if (!skip) {
            skip = changeSuffix(rootForm, "iz", "ize");
        }
        // double consonants (not L, S or Z)
        if (!skip) {
            if (endsWithDoubleConsonant(rootForm) && rootForm[rootForm.length()-1] != 'l' && rootForm[rootForm.length()-1] != 's' && rootForm[rootForm.length()-1] != 'z') {
                rootForm = rootForm.substr(0, rootForm.length()-1);
                skip = true;
            }
        }

        // (m=1 and *o) -> E
        if (!skip) {
            if (calculateMeasure(rootForm) == 1 && endsCVC(rootForm)) {
                rootForm += "e";
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
        skip = changeSuffix(rootForm, "y", "");
        if (!containsVowel(rootForm) && skip == true) {
            changeSuffix(rootForm, "", "y"); // put suffix back to how it was
        } else if (containsVowel(rootForm) && skip) {
            changeSuffix(rootForm, "", "i");
        }
    }

    ///
    skip = false; // reset bool
    skip2 = false;
    ///

    // (m>0) ATIONAL -> ATE
    if (!skip) {
        skip = changeSuffix(rootForm, "ational", "");
        if (!(calculateMeasure(rootForm) > 0) && skip == true) {
            changeSuffix(rootForm, "", "ational"); // put suffix back to how it was
        } else if (calculateMeasure(rootForm) > 0 && skip == true){
            changeSuffix(rootForm, "", "ate");
        }
    }

    // (m>0) TIONAL -> TION
    if (!skip) {
        skip = changeSuffix(rootForm, "tional", "");
        if (!(calculateMeasure(rootForm) > 0) && skip == true) {
            changeSuffix(rootForm, "", "tional"); // put suffix back to how it was
        } else if (calculateMeasure(rootForm) > 0 && skip == true){
            changeSuffix(rootForm, "", "tion");
        }
    }

    // (m>0) ENCI -> ENCE
    if (!skip) {
        skip = changeSuffix(rootForm, "enci", "");
        if (!(calculateMeasure(rootForm) > 0) && skip == true) {
            changeSuffix(rootForm, "", "enci"); // put suffix back to how it was
        } else if (calculateMeasure(rootForm) > 0 && skip == true){
            changeSuffix(rootForm, "", "ence");
        }
    }

    // (m>0) ANCI -> ANCE
    if (!skip) {
        skip = changeSuffix(rootForm, "anci", "");
        if (!(calculateMeasure(rootForm) > 0) && skip == true) {
            changeSuffix(rootForm, "", "anci"); // put suffix back to how it was
        } else if (calculateMeasure(rootForm) > 0 && skip == true){
            changeSuffix(rootForm, "", "ance");
        }
    }

    // (m>0) IZER -> IZE
    if (!skip) {
        skip = changeSuffix(rootForm, "izer", "");
        if (!(calculateMeasure(rootForm) > 0) && skip == true) {
            changeSuffix(rootForm, "", "izer"); // put suffix back to how it was
        } else if (calculateMeasure(rootForm) > 0 && skip == true){
            changeSuffix(rootForm, "", "ize");
        }
    }

    // (m>0) ABLI -> ABLE
    if (!skip) {
        skip = changeSuffix(rootForm, "abli", "");
        if (!(calculateMeasure(rootForm) > 0) && skip == true) {
            changeSuffix(rootForm, "", "abli"); // put suffix back to how it was
        } else if (calculateMeasure(rootForm) > 0 && skip == true){
            changeSuffix(rootForm, "", "able");
        }
    }

    // (m>0) ALLI -> AL
    if (!skip) {
        skip = changeSuffix(rootForm, "alli", "");
        if (!(calculateMeasure(rootForm) > 0) && skip == true) {
            changeSuffix(rootForm, "", "alli"); // put suffix back to how it was
        } else if (calculateMeasure(rootForm) > 0 && skip == true){
            changeSuffix(rootForm, "", "al");
        }
    }

    // (m>0) ENTLI -> ENT
    if (!skip) {
        skip = changeSuffix(rootForm, "entli", "");
        if (!(calculateMeasure(rootForm) > 0) && skip == true) {
            changeSuffix(rootForm, "", "entli"); // put suffix back to how it was
        } else if (calculateMeasure(rootForm) > 0 && skip == true){
            changeSuffix(rootForm, "", "ent");
        }
    }

    // (m>0) ELI -> E
    if (!skip) {
        skip = changeSuffix(rootForm, "eli", "");
        if (!(calculateMeasure(rootForm) > 0) && skip == true) {
            changeSuffix(rootForm, "", "eli"); // put suffix back to how it was
        } else if (calculateMeasure(rootForm) > 0 && skip == true){
            changeSuffix(rootForm, "", "e");
        }
    }

    // (m>0) OUSLI -> OUS
    if (!skip) {
        skip = changeSuffix(rootForm, "ousli", "");
        if (!(calculateMeasure(rootForm) > 0) && skip == true) {
            changeSuffix(rootForm, "", "ousli"); // put suffix back to how it was
        } else if (calculateMeasure(rootForm) > 0 && skip == true){
            changeSuffix(rootForm, "", "ous");
        }
    }

    // (m>0) IZATION -> IZE
    if (!skip) {
        skip = changeSuffix(rootForm, "ization", "");
        if (!(calculateMeasure(rootForm) > 0) && skip == true) {
            changeSuffix(rootForm, "", "ization"); // put suffix back to how it was
        } else if (calculateMeasure(rootForm) > 0 && skip == true){
            changeSuffix(rootForm, "", "ize");
        }
    }

    // (m>0) ATION -> ATE
    if (!skip) {
        skip = changeSuffix(rootForm, "ation", "");
        if (!(calculateMeasure(rootForm) > 0) && skip == true) {
            changeSuffix(rootForm, "", "ation"); // put suffix back to how it was
        } else if (calculateMeasure(rootForm) > 0 && skip == true){
            changeSuffix(rootForm, "", "ate");
        }
    }

    // (m>0) ATOR -> ATE
    if (!skip) {
        skip = changeSuffix(rootForm, "ator", "");
        if (!(calculateMeasure(rootForm) > 0) && skip == true) {
            changeSuffix(rootForm, "", "ator"); // put suffix back to how it was
        } else if (calculateMeasure(rootForm) > 0 && skip == true){
            changeSuffix(rootForm, "", "ate");
        }
    }

    // (m>0) ALISM -> AL
    if (!skip) {
        skip = changeSuffix(rootForm, "alism", "");
        if (!(calculateMeasure(rootForm) > 0) && skip == true) {
            changeSuffix(rootForm, "", "alism"); // put suffix back to how it was
        } else if (calculateMeasure(rootForm) > 0 && skip == true){
            changeSuffix(rootForm, "", "al");
        }
    }

    // (m>0) IVENESS -> IVE
    if (!skip) {
        skip = changeSuffix(rootForm, "iveness", "");
        if (!(calculateMeasure(rootForm) > 0) && skip == true) {
            changeSuffix(rootForm, "", "iveness"); // put suffix back to how it was
        } else if (calculateMeasure(rootForm) > 0 && skip == true){
            changeSuffix(rootForm, "", "ive");
        }
    }

    // (m>0) FULNESS -> FUL
    if (!skip) {
        skip = changeSuffix(rootForm, "fulness", "");
        if (!(calculateMeasure(rootForm) > 0) && skip == true) {
            changeSuffix(rootForm, "", "fulness"); // put suffix back to how it was
        } else if (calculateMeasure(rootForm) > 0 && skip == true){
            changeSuffix(rootForm, "", "ful");
        }
    }

    // (m>0) OUSNESS -> OUS
    if (!skip) {
        skip = changeSuffix(rootForm, "ousness", "");
        if (!(calculateMeasure(rootForm) > 0) && skip == true) {
            changeSuffix(rootForm, "", "ousness"); // put suffix back to how it was
        } else if (calculateMeasure(rootForm) > 0 && skip == true){
            changeSuffix(rootForm, "", "ous");
        }
    }

    // (m>0) ALITI -> AL
    if (!skip) {
        skip = changeSuffix(rootForm, "aliti", "");
        if (!(calculateMeasure(rootForm) > 0) && skip == true) {
            changeSuffix(rootForm, "", "aliti"); // put suffix back to how it was
        } else if (calculateMeasure(rootForm) > 0 && skip == true){
            changeSuffix(rootForm, "", "al");
        }
    }

    // (m>0) IVITI -> IVE
    if (!skip) {
        skip = changeSuffix(rootForm, "iviti", "");
        if (!(calculateMeasure(rootForm) > 0) && skip == true) {
            changeSuffix(rootForm, "", "iviti"); // put suffix back to how it was
        } else if (calculateMeasure(rootForm) > 0 && skip == true){
            changeSuffix(rootForm, "", "ive");
        }
    }

    // (m>0) BILITI -> BLE
    if (!skip) {
        skip = changeSuffix(rootForm, "biliti", "");
        if (!(calculateMeasure(rootForm) > 0) && skip == true) {
            changeSuffix(rootForm, "", "biliti"); // put suffix back to how it was
        } else if (calculateMeasure(rootForm) > 0 && skip == true){
            changeSuffix(rootForm, "", "ble");
        }
    }

    ///
    skip = false; // reset bool
    skip2 = false;
    ///

    // (m>0) ICATE -> IC
    if (!skip) {
        skip = changeSuffix(rootForm, "icate", "");
        if (!(calculateMeasure(rootForm) > 0) && skip == true) {
            changeSuffix(rootForm, "", "icate"); // put suffix back to how it was
        } else if (calculateMeasure(rootForm) > 0 && skip == true){
            changeSuffix(rootForm, "", "ic");
        }
    }

    // (m>0) ATIVE ->
    if (!skip) {
        skip = changeSuffix(rootForm, "ative", "");
        if (!(calculateMeasure(rootForm) > 0) && skip == true) {
            changeSuffix(rootForm, "", "ative"); // put suffix back to how it was
        }
    }

    // (m>0) ALIZE -> AL
    if (!skip) {
        skip = changeSuffix(rootForm, "alize", "");
        if (!(calculateMeasure(rootForm) > 0) && skip == true) {
            changeSuffix(rootForm, "", "alize"); // put suffix back to how it was
        } else if (calculateMeasure(rootForm) > 0 && skip == true){
            changeSuffix(rootForm, "", "al");
        }
    }

    // (m>0) ICITI -> IC
    if (!skip) {
        skip = changeSuffix(rootForm, "iciti", "");
        if (!(calculateMeasure(rootForm) > 0) && skip == true) {
            changeSuffix(rootForm, "", "iciti"); // put suffix back to how it was
        } else if (calculateMeasure(rootForm) > 0 && skip == true){
            changeSuffix(rootForm, "", "ic");
        }
    }

    // (m>0) ICAL -> IC
    if (!skip) {
        skip = changeSuffix(rootForm, "ical", "");
        if (!(calculateMeasure(rootForm) > 0) && skip == true) {
            changeSuffix(rootForm, "", "ical"); // put suffix back to how it was
        } else if (calculateMeasure(rootForm) > 0 && skip == true){
            changeSuffix(rootForm, "", "ic");
        }
    }

    // (m>0) FUL ->
    if (!skip) {
        skip = changeSuffix(rootForm, "ful", "");
        if (!(calculateMeasure(rootForm) > 0) && skip == true) {
            changeSuffix(rootForm, "", "ful"); // put suffix back to how it was
        }
    }

    // (m>0) NESS ->
    if (!skip) {
        skip = changeSuffix(rootForm, "ness", "");
        if (!(calculateMeasure(rootForm) > 0) && skip == true) {
            changeSuffix(rootForm, "", "ness"); // put suffix back to how it was
        }
    }

    ///
    skip = false; // reset bool
    skip2 = false;
    ///

    // (m>1) AL ->
    if (!skip) {
        skip = changeSuffix(rootForm, "al", "");
        if (!(calculateMeasure(rootForm) > 1) && skip == true) {
            changeSuffix(rootForm, "", "al"); // put suffix back to how it was
        }
    }

    // (m>1) ANCE ->
    if (!skip) {
        skip = changeSuffix(rootForm, "ance", "");
        if (!(calculateMeasure(rootForm) > 1) && skip == true) {
            changeSuffix(rootForm, "", "ance"); // put suffix back to how it was
        }
    }

    // (m>1) ENCE ->
    if (!skip) {
        skip = changeSuffix(rootForm, "ence", "");
        if (!(calculateMeasure(rootForm) > 1) && skip == true) {
            changeSuffix(rootForm, "", "ence"); // put suffix back to how it was
        }
    }

    // (m>1) ENCE ->
    if (!skip) {
        skip = changeSuffix(rootForm, "ence", "");
        if (!(calculateMeasure(rootForm) > 1) && skip == true) {
            changeSuffix(rootForm, "", "ence"); // put suffix back to how it was
        }
    }

    // (m>1) ER ->
    if (!skip) {
        skip = changeSuffix(rootForm, "er", "");
        if (!(calculateMeasure(rootForm) > 1) && skip == true) {
            changeSuffix(rootForm, "", "er"); // put suffix back to how it was
        }
    }

    // (m>1) IC ->
    if (!skip) {
        skip = changeSuffix(rootForm, "ic", "");
        if (!(calculateMeasure(rootForm) > 1) && skip == true) {
            changeSuffix(rootForm, "", "ic"); // put suffix back to how it was
        }
    }

    // (m>1) ABLE ->
    if (!skip) {
        skip = changeSuffix(rootForm, "able", "");
        if (!(calculateMeasure(rootForm) > 1) && skip == true) {
            changeSuffix(rootForm, "", "able"); // put suffix back to how it was
        }
    }

    // (m>1) IBLE ->
    if (!skip) {
        skip = changeSuffix(rootForm, "ible", "");
        if (!(calculateMeasure(rootForm) > 1) && skip == true) {
            changeSuffix(rootForm, "", "ible"); // put suffix back to how it was
        }
    }

    // (m>1) ANT ->
    if (!skip) {
        skip = changeSuffix(rootForm, "ant", "");
        if (!(calculateMeasure(rootForm) > 1) && skip == true) {
            changeSuffix(rootForm, "", "ant"); // put suffix back to how it was
        }
    }

    // (m>1) EMENT ->
    if (!skip) {
        skip = changeSuffix(rootForm, "ement", "");
        if (!(calculateMeasure(rootForm) > 1) && skip == true) {
            changeSuffix(rootForm, "", "ement"); // put suffix back to how it was
        }
    }

    // (m>1) MENT ->
    if (!skip) {
        skip = changeSuffix(rootForm, "ment", "");
        if (!(calculateMeasure(rootForm) > 1) && skip == true) {
            changeSuffix(rootForm, "", "ment"); // put suffix back to how it was
        }
    }

    // (m>1) ENT ->
    if (!skip) {
        skip = changeSuffix(rootForm, "ent", "");
        if (!(calculateMeasure(rootForm) > 1) && skip == true) {
            changeSuffix(rootForm, "", "ent"); // put suffix back to how it was
        }
    }

    // (m>1 and (*S or *T)) ION ->
    if (!skip) {
        skip = changeSuffix(rootForm, "ion", "");
        if (!(calculateMeasure(rootForm) > 1) && skip == true && !(rootForm[rootForm.length()-1] == 's' or rootForm[rootForm.length()-1] == 't')) {
            changeSuffix(rootForm, "", "ion"); // put suffix back to how it was
        }
    }

    // (m>1) OU ->
    if (!skip) {
        skip = changeSuffix(rootForm, "ou", "");
        if (!(calculateMeasure(rootForm) > 1) && skip == true) {
            changeSuffix(rootForm, "", "ou"); // put suffix back to how it was
        }
    }

    // (m>1) ISM ->
    if (!skip) {
        skip = changeSuffix(rootForm, "ism", "");
        if (!(calculateMeasure(rootForm) > 1) && skip == true) {
            changeSuffix(rootForm, "", "ism"); // put suffix back to how it was
        }
    }

    // (m>1) ATE ->
    if (!skip) {
        skip = changeSuffix(rootForm, "ate", "");
        if (!(calculateMeasure(rootForm) > 1) && skip == true) {
            changeSuffix(rootForm, "", "ate"); // put suffix back to how it was
        }
    }

    // (m>1) ITI ->
    if (!skip) {
        skip = changeSuffix(rootForm, "iti", "");
        if (!(calculateMeasure(rootForm) > 1) && skip == true) {
            changeSuffix(rootForm, "", "iti"); // put suffix back to how it was
        }
    }

    // (m>1) OUS ->
    if (!skip) {
        skip = changeSuffix(rootForm, "ous", "");
        if (!(calculateMeasure(rootForm) > 1) && skip == true) {
            changeSuffix(rootForm, "", "ous"); // put suffix back to how it was
        }
    }

    // (m>1) IVE ->
    if (!skip) {
        skip = changeSuffix(rootForm, "ive", "");
        if (!(calculateMeasure(rootForm) > 1) && skip == true) {
            changeSuffix(rootForm, "", "ive"); // put suffix back to how it was
        }
    }

    // (m>1) IZE ->
    if (!skip) {
        skip = changeSuffix(rootForm, "ize", "");
        if (!(calculateMeasure(rootForm) > 1) && skip == true) {
            changeSuffix(rootForm, "", "ize"); // put suffix back to how it was
        }
    }

    ///
    skip = false; // reset bool
    skip2 = false;
    ///

    // (m>1) E ->
    if (!skip) {
        skip = changeSuffix(rootForm, "e", "");
        if (!(calculateMeasure(rootForm) > 1) && skip == true) {
            changeSuffix(rootForm, "", "e"); // put suffix back to how it was
            skip = false;
        }
    }

    // (m=1 and not *o) E ->
    if (!skip) {
        skip = changeSuffix(rootForm, "e", "");
        if (!(calculateMeasure(rootForm) == 1 && !endsCVC(rootForm)) && skip == true) {
            changeSuffix(rootForm, "", "e"); // put suffix back to how it was
        }
    }

    ///
    ///

    // m > 1 and double consonants and L
    if (calculateMeasure(rootForm) > 1 && endsWithDoubleConsonant(rootForm) && rootForm[rootForm.length()-1] == 'l') {
        rootForm = rootForm.substr(0, rootForm.length()-1);
    }
    return rootForm;
}

bool Word::isHighlight() const {
    return highlight;
}

void Word::setHighlight(bool highlight) {
    Word::highlight = highlight;
}

const string &Word::getReplace() const {
    return replace;
}

void Word::setReplace(const string &replace) {
    Word::replace = replace;
}
