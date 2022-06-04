//
// Created by liam on 4/06/2022.
//

#ifndef TATEXTEDITOR_SPECIALCHARACTER_H
#define TATEXTEDITOR_SPECIALCHARACTER_H

#include "Word.h"

class SpecialCharacter : public Word {
private:

public:
    SpecialCharacter(const string &token);
    bool isSpecialChar();
};

#endif //TATEXTEDITOR_SPECIALCHARACTER_H
