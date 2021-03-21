#ifndef STANDARD_OUTPUT_H
#define STANDARD_OUTPUT_H

#include <string>
using namespace std;

class Standard_Output
{
public:
    virtual void ShowInfoDialog(const string &) const;
};

enum
{
    GLOBAL_NO_ERROR = 0,
    GLOBAL_ERROR_INVALID_VARIABLE = 1,
    GLOBAL_ERROR_BAD_INPUT,
    GLOBAL_ERROR_NO_CONSTANT_ON_RIGHT,
    GLOBAL_ERROR_INCORRECT_NUMBER_OF_EQUATION,
    GLOBAL_ERROR_VARIABLE_TIMES_VARIABLE
};

#endif