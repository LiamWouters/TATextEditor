//
// Created by liam on 21/05/2022.
//

#include <iostream>
#include "gtest.h"
#include "Text.h"
#include "Sentence.h"
#include "Word.h"
using namespace std;

int main(int argc, char** argv) {
    clock_t start = clock();

    ::testing::InitGoogleTest(&argc, argv);
    int returnv = RUN_ALL_TESTS();

    clock_t stop = clock();
    double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    printf("\nTime elapsed: %.5f\n", elapsed);
    return returnv;
}

/// Tokenization ///
TEST(Tokenization, happyDay) {
    Text* text = new Text();
    text->Tokenize("TokenizationHappyDay.txt");

    EXPECT_EQ(2, text->getSentences().size());
    /*
    Sentence* sentence1 = text->getSentences()[0];
    Sentence* sentence2 = text->getSentences()[1];

    EXPECT_EQ(10, sentence1->getWordCount());
    EXPECT_EQ(10, sentence2->getWordCount());

    EXPECT_EQ(12, sentence1->size());
    EXPECT_EQ(11, sentence2->size());

    EXPECT_EQ("this", sentence1->getWord(0)->getString()); // all words transformed to lower case
    EXPECT_EQ("is", sentence1->getWord(1)->getString());
    EXPECT_EQ(".", sentence1->getWord(sentence1->size()-1)->getString());
    EXPECT_EQ("tokenization", sentence1->getWord(sentence1->size()-2)->getString());

    EXPECT_EQ("sentence", sentence2->getWord(0)->getString()); // all words transformed to lower case
    EXPECT_EQ("chaining", sentence2->getWord(1)->getString());
    EXPECT_EQ("!", sentence2->getWord(sentence2->size()-1)->getString());
    EXPECT_EQ("datastructure", sentence2->getWord(sentence2->size()-2)->getString());

    EXPECT_FALSE(sentence2->getWord(0)->isPunctuationMark());
    EXPECT_TRUE(sentence2->getWord(sentence2->size()-1)->isPunctuationMark());
    */
    delete text;
}
/*
TEST(Tokenization, RandgevalAbbreviations) {
    Text text;
    text.Tokenize("TokenizationRandgeval1.txt");
}

TEST(Tokenization, RandgevalTripleDot) {
    Text text;
    text.Tokenize("TokenizationRandgeval2.txt");
}
*/
////////////////////
