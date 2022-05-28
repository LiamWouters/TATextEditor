#include <iostream>
#include "Text.h"
#include "Sentence.h"
#include "Word.h"
using namespace std;

int main() {
    string filename = "stemming.txt";

    Text text;
    text.Tokenize(filename);

    return 0;
};