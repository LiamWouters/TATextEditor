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
///// Stemming /////
TEST(Stemming, happy_day) {
    /*
     * BUNCH OF EXAMPLE WORDS
     */
    Word w = Word("trees");
    EXPECT_EQ("tree", w.getRoot());
    Word w0 = Word("by");
    EXPECT_EQ("by", w0.getRoot());
    Word w1 = Word("trouble");
    EXPECT_EQ("troubl", w1.getRoot());
    Word w2 = Word("oats");
    EXPECT_EQ("oat", w2.getRoot());
    Word w3 = Word("ivy");
    EXPECT_EQ("ivi", w3.getRoot());
    Word w4 = Word("troubles");
    EXPECT_EQ("troubl", w4.getRoot());
    Word w5 = Word("private");
    EXPECT_EQ("privat", w5.getRoot());
    Word w6 = Word("oaten");
    EXPECT_EQ("oaten", w6.getRoot());
    Word w7 = Word("orrery");
    EXPECT_EQ("orreri", w7.getRoot());
    Word w8 = Word("caresses");
    EXPECT_EQ("caress", w8.getRoot());
    Word w9 = Word("caress");
    EXPECT_EQ("caress", w9.getRoot());
    Word w10 = Word("ties");
    EXPECT_EQ("ti", w10.getRoot());
    Word w11 = Word("ponies");
    EXPECT_EQ("poni", w11.getRoot());
    Word w12 = Word("cats");
    EXPECT_EQ("cat", w12.getRoot());
    Word w13 = Word("cares");
    EXPECT_EQ("care", w13.getRoot());
    Word w14 = Word("feed");
    EXPECT_EQ("feed", w14.getRoot());
    Word w15 = Word("agreed");
    EXPECT_EQ("agre", w15.getRoot());
    Word w16 = Word("plastered");
    EXPECT_EQ("plaster", w16.getRoot());
    Word w17 = Word("bled");
    EXPECT_EQ("bled", w17.getRoot());
    Word w18 = Word("motoring");
    EXPECT_EQ("motor", w18.getRoot());
    Word w19 = Word("sing");
    EXPECT_EQ("sing", w19.getRoot());
    Word w20 = Word("conflated");
    EXPECT_EQ("conflat", w20.getRoot());
    Word w21 = Word("troubled");
    EXPECT_EQ("troubl", w21.getRoot());
    Word w22 = Word("sized");
    EXPECT_EQ("size", w22.getRoot());
    Word w23 = Word("hopped");
    EXPECT_EQ("hop", w23.getRoot());
    Word w24 = Word("tanned");
    EXPECT_EQ("tan", w24.getRoot());
    Word w25 = Word("falling");
    EXPECT_EQ("fall", w25.getRoot());
    Word w26 = Word("hissing");
    EXPECT_EQ("hiss", w26.getRoot());
    Word w27 = Word("failing");
    EXPECT_EQ("fail", w27.getRoot());
    Word w28 = Word("filing");
    EXPECT_EQ("file", w28.getRoot());
    Word w29 = Word("happy");
    EXPECT_EQ("happi", w29.getRoot());
    Word w30 = Word("sky");
    EXPECT_EQ("sky", w30.getRoot());
    Word w31 = Word("relational");
    EXPECT_EQ("relat", w31.getRoot());
    Word w32 = Word("conditional");
    EXPECT_EQ("condit", w32.getRoot());
    Word w33 = Word("rational");
    EXPECT_EQ("ration", w33.getRoot());
    Word w34 = Word("valenci");
    EXPECT_EQ("valenc", w34.getRoot());
    Word w35 = Word("hesitanci");
    EXPECT_EQ("hesit", w35.getRoot());
    Word w36 = Word("digitizer");
    EXPECT_EQ("digit", w36.getRoot());
    Word w37 = Word("conformabli");
    EXPECT_EQ("conform", w37.getRoot());
    Word w38 = Word("radicalli");
    EXPECT_EQ("radic", w38.getRoot());
    Word w39 = Word("differently");
    EXPECT_EQ("differ", w39.getRoot());
    Word w40 = Word("vileli");
    EXPECT_EQ("vile", w40.getRoot());
    Word w41 = Word("analogousli");
    EXPECT_EQ("analog", w41.getRoot());
    Word w42 = Word("vietnamization");
    EXPECT_EQ("vietnam", w42.getRoot());
    Word w43 = Word("predication");
    EXPECT_EQ("predic", w43.getRoot());
    Word w44 = Word("operator");
    EXPECT_EQ("oper", w44.getRoot());
    Word w45 = Word("feudalism");
    EXPECT_EQ("feudal", w45.getRoot());
    Word w46 = Word("decisiveness");
    EXPECT_EQ("decis", w46.getRoot());
    Word w47 = Word("hopefulness");
    EXPECT_EQ("hope", w47.getRoot());
    Word w48 = Word("callousness");
    EXPECT_EQ("callous", w48.getRoot());
    Word w49 = Word("formaliti");
    EXPECT_EQ("formal", w49.getRoot());
    Word w50 = Word("sensitiviti");
    EXPECT_EQ("sensit", w50.getRoot());
    Word w51 = Word("sensibiliti");
    EXPECT_EQ("sensibl", w51.getRoot());
    Word w52 = Word("triplicate");
    EXPECT_EQ("triplic", w52.getRoot());
    Word w53 = Word("formative");
    EXPECT_EQ("form", w53.getRoot());
    Word w54 = Word("formalize");
    EXPECT_EQ("formal", w54.getRoot());
    Word w55 = Word("electriciti");
    EXPECT_EQ("electr", w55.getRoot());
    Word w56 = Word("electrical");
    EXPECT_EQ("electr", w56.getRoot());
    Word w57 = Word("hopeful");
    EXPECT_EQ("hope", w57.getRoot());
    Word w58 = Word("goodness");
    EXPECT_EQ("good", w58.getRoot());
    Word w59 = Word("revival");
    EXPECT_EQ("reviv", w59.getRoot());
    Word w60 = Word("allowance");
    EXPECT_EQ("allow", w60.getRoot());
    Word w61 = Word("inference");
    EXPECT_EQ("infer", w61.getRoot());
    Word w62 = Word("airliner");
    EXPECT_EQ("airlin", w62.getRoot());
    Word w63 = Word("gyroscopic");
    EXPECT_EQ("gyroscop", w63.getRoot());
    Word w64 = Word("adjustable");
    EXPECT_EQ("adjust", w64.getRoot());
    Word w65 = Word("defensible");
    EXPECT_EQ("defens", w65.getRoot());
    Word w66 = Word("irritant");
    EXPECT_EQ("irrit", w66.getRoot());
    Word w67 = Word("replacement");
    EXPECT_EQ("replac", w67.getRoot());
    Word w68 = Word("adjustment");
    EXPECT_EQ("adjust", w68.getRoot());
    Word w69 = Word("dependent");
    EXPECT_EQ("depend", w69.getRoot());
    Word w70 = Word("adoption");
    EXPECT_EQ("adopt", w70.getRoot());
    Word w71 = Word("homologou");
    EXPECT_EQ("homolog", w71.getRoot());
    Word w72 = Word("communism");
    EXPECT_EQ("commun", w72.getRoot());
    Word w73 = Word("activate");
    EXPECT_EQ("activ", w73.getRoot());
    Word w74 = Word("angulariti");
    EXPECT_EQ("angular", w74.getRoot());
    Word w75 = Word("homologous");
    EXPECT_EQ("homolog", w75.getRoot());
    Word w76 = Word("effective");
    EXPECT_EQ("effect", w76.getRoot());
    Word w77 = Word("bowdlerize");
    EXPECT_EQ("bowdler", w77.getRoot());
    Word w78 = Word("probate");
    EXPECT_EQ("probat", w78.getRoot());
    Word w79 = Word("rate");
    EXPECT_EQ("rate", w79.getRoot());
    Word w80 = Word("cease");
    EXPECT_EQ("ceas", w80.getRoot());
    Word w81 = Word("controll");
    EXPECT_EQ("control", w81.getRoot());
    Word w82 = Word("roll");
    EXPECT_EQ("roll", w82.getRoot());
    Word w83 = Word("relate");
    EXPECT_EQ("relat", w83.getRoot());
    Word w84 = Word("derivate");
    EXPECT_EQ("deriv", w84.getRoot());
    Word w85 = Word("probate");
    EXPECT_EQ("probat", w85.getRoot());
    Word w86 = Word("activate");
    EXPECT_EQ("activ", w86.getRoot());
    Word w87 = Word("conflate");
    EXPECT_EQ("conflat", w87.getRoot());
    Word w88 = Word("demonstrate");
    EXPECT_EQ("demonstr", w88.getRoot());
    Word w89 = Word("pirate");
    EXPECT_EQ("pirat", w89.getRoot());
    Word w90 = Word("necessitate");
    EXPECT_EQ("necessit", w90.getRoot());
    Word w91 = Word("prelate");
    EXPECT_EQ("prelat", w91.getRoot());
    Word w92 = Word("renovate");
    EXPECT_EQ("renov", w92.getRoot());
    Word w93 = Word("derivate");
    EXPECT_EQ("deriv", w93.getRoot());
    Word w94 = Word("derive");
    EXPECT_EQ("deriv", w94.getRoot());
    Word w95 = Word("activate");
    EXPECT_EQ("activ", w95.getRoot());
    Word w96 = Word("active");
    EXPECT_EQ("activ", w96.getRoot());
    Word w97 = Word("necessitate");
    EXPECT_EQ("necessit", w97.getRoot());
    Word w98 = Word("necessitous");
    EXPECT_EQ("necessit", w98.getRoot());
    Word w99 = Word("multidimensional");
    EXPECT_EQ("multidimension", w99.getRoot());
    Word w100 = Word("characterization");
    EXPECT_EQ("character", w100.getRoot());

    /*
     * EXAMPLE WORDS WITH SAME STEM (like)
     */
    Word w101 = Word("likes");
    EXPECT_EQ("like", w101.getRoot());
    Word w102 = Word("liked");
    EXPECT_EQ("like", w102.getRoot());
    Word w103 = Word("likely");
    EXPECT_EQ("like", w103.getRoot());
    Word w104 = Word("liking");
    EXPECT_EQ("like", w104.getRoot());

    /*
     * EXAMPLE SENTENCE
     */
    Word w105 = Word("this");
    EXPECT_EQ("thi", w105.getRoot());
    Word w106 = Word("is");
    EXPECT_EQ("i", w106.getRoot());
    Word w107 = Word("definitely");
    EXPECT_EQ("definit", w107.getRoot());
    Word w108 = Word("a");
    EXPECT_EQ("a", w108.getRoot());
    Word w109 = Word("controversy");
    EXPECT_EQ("controversi", w109.getRoot());
    Word w110 = Word("as");
    EXPECT_EQ("a", w110.getRoot());
    Word w111 = Word("the");
    EXPECT_EQ("the", w111.getRoot());
    Word w112 = Word("attorney");
    EXPECT_EQ("attornei", w112.getRoot());
    Word w113 = Word("labeled");
    EXPECT_EQ("label", w113.getRoot());
    Word w114 = Word("the");
    EXPECT_EQ("the", w114.getRoot());
    Word w115 = Word("case");
    EXPECT_EQ("case", w115.getRoot());
    Word w116 = Word("extremely");
    EXPECT_EQ("extrem", w116.getRoot());
    Word w117 = Word("controversial");
    EXPECT_EQ("controversi", w117.getRoot());
}

TEST(Stemming, sad_day) {
    /// Overstemming examples
    Word wo = Word("generous");
    EXPECT_EQ("gener", wo.getRoot());
    Word wo1 = Word("general");
    EXPECT_EQ("gener", wo1.getRoot());
        // they both stem to "gener" even though they mean completely different things

    Word wo2 = Word("university");
    EXPECT_EQ("univers", wo2.getRoot());
    Word wo3 = Word("universe");
    EXPECT_EQ("univers", wo3.getRoot());
    Word wo4 = Word("universal");
    EXPECT_EQ("univers", wo4.getRoot());


    /// Understemming examples
    Word wu = Word("bought");
    EXPECT_EQ("bought", wu.getRoot());
    Word wu1 = Word("buy");
    EXPECT_EQ("bui", wu1.getRoot());

    Word wu2 = Word("does");
    EXPECT_EQ("doe", wu2.getRoot());
    Word wu3 = Word("do");
    EXPECT_EQ("do", wu3.getRoot());

    Word wu4 = Word("data"); // information
    EXPECT_EQ("data", wu4.getRoot());
    Word wu5 = Word("datum"); // in english datum = single piece of information
    EXPECT_EQ("datum", wu5.getRoot());
    // they should both be stemmed to "dat" because they mean the same thing
}
////////////////////
///// N-grams //////
TEST(NGrams, happy_day) {
    Text t;
    t.Tokenize("GlobalWarming-vs-ClimateChange-NASA.txt");
    // de expected values zijn geteld
    vector<pair<vector<string>,int>> biggest = t.createNgram(0, "global");
    pair<vector<string>, int> empty = {{""},0};
    EXPECT_EQ(empty, biggest[0]); EXPECT_EQ(empty, biggest[1]);

    biggest = t.createNgram(1, "global");
    EXPECT_EQ(empty, biggest[0]); EXPECT_EQ(empty, biggest[1]);

    biggest = t.createNgram(2, "global");
    pair<vector<string>, int> before2 = {{"and", "as", "earth's"}, 2};
    pair<vector<string>, int> b2 = {{"and", "as", "earth\xE2\x80\x99s"}, 2};
    pair<vector<string>, int> after2 = {{"warming"}, 4};
    EXPECT_TRUE((before2 == biggest[0]) or (b2 == biggest[0]));
    EXPECT_EQ(after2, biggest[1]);

    biggest = t.createNgram(3, "global");
    pair<vector<string>, int> before3 = {{"change and", "in earth's", "increased earth's", "or even", "regional and", "such as", "to as", "what is"}, 1};
    pair<vector<string>, int> b3 = {{"change and", "in earth\xE2\x80\x99s", "increased earth\xE2\x80\x99s", "or even", "regional and", "such as", "to as", "what is"}, 1};
    pair<vector<string>, int> after3 = {{"average of", "average temperature", "land and", "surface temperature", "warming are", "warming is"}, 1};
    EXPECT_TRUE((before3 == biggest[0]) or (b3 == biggest[0]));
    EXPECT_EQ(after3, biggest[1]);
}

TEST(NGrams, Randgeval) {
    // woord dat niet bestaat
    Text t;
    t.Tokenize("GlobalWarming-vs-ClimateChange-NASA.txt");
    vector<pair<vector<string>,int>> biggest = t.createNgram(0, "globale");
    pair<vector<string>, int> empty = {{""},0};
    EXPECT_EQ(empty, biggest[0]); EXPECT_EQ(empty, biggest[1]);
}
////////////////////