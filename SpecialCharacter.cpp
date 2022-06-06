//
// Created by liam on 4/06/2022.
//

#include "SpecialCharacter.h"

SpecialCharacter::SpecialCharacter(const string &token) : Word(token) {
    _initCheck = this;
    ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
}

bool SpecialCharacter::properlyInitialized() {
    return _initCheck == this;
}

bool SpecialCharacter::isSpecialChar() {
    REQUIRE(properlyInitialized(), "SpecialCharacter wasn't initialized when calling isSpecialChar()");
    return true;
}

SpecialCharacter::~SpecialCharacter() {
    REQUIRE(properlyInitialized(), "SpecialCharacter wasn't initialized when calling destructor");
}
