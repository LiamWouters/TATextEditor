//
// Created by liam on 4/06/2022.
//

#ifndef TATEXTEDITOR_SPECIALCHARACTER_H
#define TATEXTEDITOR_SPECIALCHARACTER_H

#include "Word.h"

class SpecialCharacter : public Word {
private:
    SpecialCharacter* _initCheck;
public:
    /*
     * ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
     */
    SpecialCharacter(const string &token);

    /***/
    bool properlyInitialized();

    /*
     * REQUIRE(properlyInitialized(), "SpecialCharacter wasn't initialized when calling isSpecialChar()");
     */
    bool isSpecialChar();

    /*
     * REQUIRE(properlyInitialized(), "SpecialCharacter wasn't initialized when calling destructor");
     */
    virtual ~SpecialCharacter();
};

#endif //TATEXTEDITOR_SPECIALCHARACTER_H
