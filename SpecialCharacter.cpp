//
// Created by liam on 4/06/2022.
//

#include "SpecialCharacter.h"

SpecialCharacter::SpecialCharacter(const string &token) : Word(token) {}

bool SpecialCharacter::isSpecialChar() {
    return true;
}
