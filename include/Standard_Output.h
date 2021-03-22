#ifndef STANDARD_OUTPUT_H
#define STANDARD_OUTPUT_H

#include <string>
using namespace std;

class Standard_Output
{
public:
    virtual void ShowInfoDialog(const string &) const;
};

#define GLOBAL_NO_ERROR 0
#define GLOBAL_ERROR_INVALID_VARIABLE 1
#define GLOBAL_ERROR_BAD_INPUT 2
#define GLOBAL_ERROR_NO_CONSTANT_ON_RIGHT 3
#define GLOBAL_ERROR_INCORRECT_NUMBER_OF_EQUATION 4
#define GLOBAL_ERROR_VARIABLE_TIMES_VARIABLE 5

#endif