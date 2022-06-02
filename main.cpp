#include <iostream>
#include "Text.h"
#include "Sentence.h"
#include "Word.h"
using namespace std;

int main() {
    string filename = "GlobalWarming-vs-ClimateChange-NASA.txt";

    Text text;
    text.Tokenize(filename);
    vector<pair<string,int>> biggest = text.createNgram(2, "climate");

    cout << biggest[0].first << " -> " << biggest[0].second << endl;
    cout << biggest[1].first << " -> " << biggest[1].second << endl;

    return 0;
};