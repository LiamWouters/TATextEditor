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

    EXPECT_FALSE(sentence2->getWord(0)->isSpecialChar());
    EXPECT_TRUE(sentence2->getWord(sentence2->size()-1)->isSpecialChar());

    delete text;
}

TEST(Tokenization, RandgevalAbbreviations) {
    Text text;
    text.Tokenize("TokenizationRandgeval1.txt");

    EXPECT_EQ(6, text.getSentences().size());
    //shouldn't be abbreviations:
    EXPECT_FALSE(text.getSentences()[0]->getWord(text.getSentences()[0]->size()-1)->isAbbreviation());
    EXPECT_EQ(".", text.getSentences()[0]->getWord(text.getSentences()[0]->size()-1)->getString());
    EXPECT_EQ("dot", text.getSentences()[0]->getWord(text.getSentences()[0]->size()-2)->getString());

    EXPECT_FALSE(text.getSentences()[1]->getWord(text.getSentences()[1]->size()-1)->isAbbreviation());
    EXPECT_EQ(".", text.getSentences()[1]->getWord(text.getSentences()[1]->size()-1)->getString());
    EXPECT_EQ("sentence", text.getSentences()[1]->getWord(text.getSentences()[1]->size()-2)->getString());

    EXPECT_FALSE(text.getSentences()[2]->getWord(text.getSentences()[2]->size()-1)->isAbbreviation());
    EXPECT_EQ(".", text.getSentences()[2]->getWord(text.getSentences()[2]->size()-1)->getString());
    EXPECT_EQ("efficiency)", text.getSentences()[2]->getWord(text.getSentences()[2]->size()-2)->getString());

    EXPECT_FALSE(text.getSentences()[3]->getWord(text.getSentences()[3]->size()-1)->isAbbreviation());
    EXPECT_EQ(".", text.getSentences()[3]->getWord(text.getSentences()[3]->size()-1)->getString());
    EXPECT_EQ("abbreviation", text.getSentences()[3]->getWord(text.getSentences()[3]->size()-2)->getString());

    EXPECT_FALSE(text.getSentences()[4]->getWord(text.getSentences()[4]->size()-1)->isAbbreviation());
    EXPECT_EQ(".", text.getSentences()[4]->getWord(text.getSentences()[4]->size()-1)->getString());
    EXPECT_EQ("youtuber", text.getSentences()[4]->getWord(text.getSentences()[4]->size()-2)->getString());

    //should be abbreviations:
    EXPECT_TRUE(text.getSentences()[4]->getWord(2)->isAbbreviation());
    EXPECT_EQ("dr.", text.getSentences()[4]->getWord(2)->getString());

    EXPECT_TRUE(text.getSentences()[5]->getWord(text.getSentences()[5]->size()-1)->isAbbreviation());
    EXPECT_EQ("vs.", text.getSentences()[5]->getWord(text.getSentences()[5]->size()-1)->getString());

    EXPECT_TRUE(text.getSentences()[5]->getWord(text.getSentences()[5]->size()-2)->isAbbreviation());
    EXPECT_EQ("mrs.", text.getSentences()[5]->getWord(text.getSentences()[5]->size()-2)->getString());

    EXPECT_TRUE(text.getSentences()[5]->getWord(text.getSentences()[5]->size()-3)->isAbbreviation());
    EXPECT_EQ("mr.", text.getSentences()[5]->getWord(text.getSentences()[5]->size()-3)->getString());

    EXPECT_TRUE(text.getSentences()[5]->getWord(text.getSentences()[5]->size()-4)->isAbbreviation());
    EXPECT_EQ("approx.", text.getSentences()[5]->getWord(text.getSentences()[5]->size()-4)->getString());

    EXPECT_TRUE(text.getSentences()[5]->getWord(text.getSentences()[5]->size()-5)->isAbbreviation());
    EXPECT_EQ("e.g.", text.getSentences()[5]->getWord(text.getSentences()[5]->size()-5)->getString());

    EXPECT_FALSE(text.getSentences()[5]->getWord(text.getSentences()[5]->size()-6)->isAbbreviation());
    EXPECT_EQ("abbreviations:", text.getSentences()[5]->getWord(text.getSentences()[5]->size()-6)->getString());
}

TEST(Tokenization, RandgevalTripleDot) {
    Text text;
    text.Tokenize("TokenizationRandgeval2.txt");

    EXPECT_EQ(4, text.getSentences().size());

    EXPECT_TRUE(text.getSentences()[0]->getWord(11)->isSpecialChar());
    EXPECT_EQ("...", text.getSentences()[0]->getWord(11)->getString());

    EXPECT_FALSE(text.getSentences()[2]->getWord(14)->isSpecialChar());
    EXPECT_EQ("this", text.getSentences()[2]->getWord(14)->getString());
    EXPECT_TRUE(text.getSentences()[2]->getWord(15)->isSpecialChar());
    EXPECT_EQ("...", text.getSentences()[2]->getWord(15)->getString());

    EXPECT_TRUE(text.getSentences()[2]->getWord(26)->isSpecialChar());
    EXPECT_EQ("...", text.getSentences()[2]->getWord(26)->getString()); // not caught as triple dot but caught the ',' after it
    EXPECT_TRUE(text.getSentences()[2]->getWord(27)->isSpecialChar());
    EXPECT_EQ(",", text.getSentences()[2]->getWord(27)->getString());
}
////////////////////
