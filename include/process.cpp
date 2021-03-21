#include "../headers/process.h"
#include "../headers/Standard_Output.h"

#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>

using namespace std;

#define is_digit(x) ('0' <= x && x <= '9')
#define is_word(x) (('a' <= x && x <= 'z') || ('A' <= x && x <= 'Z'))

template <typename _Type>
void StreamProcessor<_Type>::Process(iostream *_stream)
{
    variable_name.clear();
    matrix.clear();

    stack<char> cOperators;
    cOperators.push('#');

    unordered_map<string, int> varialbes_map;
    int curSize = 0;

    vector<Data> proposition; // >=0? =>variable : <0? => (-proposition[i] - 1) is the original operation

    char c;
    string buffer;
    _Type _temp_line_constant;
    vector<_Type> _temp_column, _temp_line_coefficient;

    bool isReadingVariable = false, isReadingNumeral = false;

    while (!_stream->eof() || c == '#')
    {
        if (is_word(c))
        {
            if (isReadingNumeral)
                throw GLOBAL_ERROR_INVALID_VARIABLE;
            if (!isReadingVariable)
            {
                isReadingVariable = true;
            }
        }
        else if (is_digit(c))
        {
            if (!isReadingNumeral)
            {
                if (!isReadingVariable)
                    isReadingNumeral = true;
            }
        }
        else
        {
            switch (c)
            {
            case '_':
                if (isReadingVariable)
                    break;
                else
                    throw GLOBAL_ERROR_BAD_INPUT;
            case '.':
                if (isReadingNumeral)
                    break;
                else
                    throw GLOBAL_ERROR_BAD_INPUT;
            case ' ':
            case '\t':
                continue;
            default:
                /////////
                int tempPriority = icp(c);

                if (c == '=')
                {
                    _temp_line_coefficient.resize(curSize, 0);
                    Propos_Com_Variables(&proposition, &_temp_line_coefficient, &_temp_line_constant);
                    continue;
                }

                if (c == '\n')
                {
                    ////////
                    if (isReadingNumeral)
                    {
                        _temp_line_constant += stod(buffer);
                        _temp_column.push_back(_temp_line_constant);
                        matrix.push_back(_temp_line_coefficient);

                        _temp_line_constant = 0.0;
                        _temp_line_coefficient.clear();
                        proposition.clear();
                    }
                    else
                        throw GLOBAL_ERROR_NO_CONSTANT_ON_RIGHT;
                    continue;
                }

                if (isReadingNumeral)
                {
                    proposition.push_back(Data(- 1, stod(buffer)));
                    buffer = "";
                }

                if (isReadingVariable)
                {
                    if (varialbes_map.find(buffer) == varialbes_map.end())
                    {
                        varialbes_map[buffer] = curSize++;
                        variable_name.push_back(buffer);
                    }
                    proposition.push_back(Data(varialbes_map[buffer], 1.0));

                    buffer = "";
                }

                if (tempPriority == -1)
                    throw GLOBAL_ERROR_BAD_INPUT;

                while (isp(cOperators.top()) > tempPriority)
                {
                    proposition.push_back(Data(-2, (double)(cOperators.top())));
                    cOperators.pop();
                }

                if (c == '#')
                {
                    break;
                }

                cOperators.push(c);
                continue;
            }
        }
        buffer += c;
        if (_stream->peek() == EOF)
            c = '#';
    }
    if (matrix.size() != curSize)
        throw GLOBAL_ERROR_INCORRECT_NUMBER_OF_EQUATION;
    for (int i = 0; i < curSize; ++i)
    {
        matrix[i].resize(curSize, 0);
        matrix[i].push_back(_temp_column[i]);
    }
}
template <typename _Type>
void StreamProcessor<_Type>::Propos_Com_Variables(vector<Data> *_pros, vector<_Type> *_output, _Type *_output_constant)
{
    //reckon _output_constant == 0.0
    stack<int> s;
    int _max = _pros->size();
    for (int i = 0; i < _max; ++i)
    {
        if (_pros->operator[](i)._index >=-1)
        {
            s.push(i);
        }
        else
        {
            static char c;
            static int tempA, tempB;
            static int tempA_index, tempB_index;
            tempB = s.top();
            s.pop();
            tempA = s.top();
            s.pop();
            tempA_index = _pros->operator[](tempA)._index;
            tempB_index = _pros->operator[](tempB)._index;
            c = 2 - _pros->operator[](i)._index;
            switch (c)
            {
            case '+':
                if (tempA_index == tempB_index)
                {
                    _pros->operator[](tempA)._value += _pros->operator[](tempB)._value;
                    s.push(tempA);
                }
                else if (tempA_index < 0)
                {
                    _pros->operator[](tempB).push_index(tempA);
                    s.push(tempB);
                }
                else
                {
                    _pros->operator[](tempA).push_index(tempB_index);
                    s.push(tempA);
                }
                break;
            case '-':
                if (tempA_index == tempB_index)
                {
                    _pros->operator[](tempA)._value -= _pros->operator[](tempB)._value;
                    s.push(tempA);
                }
                else if (tempA_index < 0)
                {
                    _pros->operator[](tempB)._value = -_pros->operator[](tempB)._value;
                    _pros->operator[](tempB).push_index(tempA);
                    s.push(tempB);
                }
                else
                {
                    _pros->operator[](tempA).push_index(tempB_index);
                    s.push(tempA);
                }
                break;
            case '*':
                if (tempA_index == tempB_index)
                {
                    if (tempA_index >= 0)
                        throw GLOBAL_ERROR_VARIABLE_TIMES_VARIABLE;
                    _pros->operator[](tempA)._value *= _pros->operator[](tempB)._value;
                    s.push(tempA);
                }
                else if (tempA_index < 0)
                {
                    _pros->operator[](tempB)._value *= _pros->operator[](tempA)._value;
                    s.push(tempB);
                }
                else
                {
                    if (tempB_index >= 0)
                        throw GLOBAL_ERROR_VARIABLE_TIMES_VARIABLE;
                    _pros->operator[](tempA)._value *= _pros->operator[](tempB)._value;
                    s.push(tempA);
                }
                break;
            case '/':
                if (tempA_index == tempB_index)
                {
                    if (tempA_index >= 0)
                        throw GLOBAL_ERROR_VARIABLE_TIMES_VARIABLE;
                    _pros->operator[](tempA)._value /= _pros->operator[](tempB)._value;
                    s.push(tempA);
                }
                else if (tempA_index < 0)
                {
                    _pros->operator[](tempB)._value /= _pros->operator[](tempA)._value;
                    s.push(tempB);
                }
                else
                {
                    if (tempB_index >= 0)
                        throw GLOBAL_ERROR_VARIABLE_TIMES_VARIABLE;
                    _pros->operator[](tempA)._value /= _pros->operator[](tempB)._value;
                    s.push(tempA);
                }
                break;
            }
        }
    }
    //get coefficient
    static int ceil, id;
    static double t;
    while (s.size())
    {
        Data tempData = _pros->operator[](s.top());
        s.pop();
        t = tempData._value;
        id = tempData._index;
        if(id>=0)
        {
            _output[id] += t;
            ceil = tempData.neighborTable->size();
            for (int i = 0; i < _max;++i)
            {
                static int temp;
                temp = tempData.neighborTable->operator[](i);
                _pros->operator[](temp)._value *= t;
                s.push(temp);
            }
        }
        else
            *_output_constant -= t;
    }
}

template <typename _Type>
int StreamProcessor<_Type>::isp(const char &_c) const
{
    switch (_c)
    {
    case '#':
        return INT_MIN;
    case '(':
        return 0;
    case ')':
        return 6;
    case '+':
    case '-':
        return 2;
    case '*':
    case '/':
        return 4;
    }
    return -1;
}
template <typename _Type>
int StreamProcessor<_Type>::icp(const char &_c) const
{
    switch (_c)
    {
    case '#':
        return INT_MIN;
    case '(':
        return 6;
    case ')':
        return 0;
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 3;
    }
    return -1;
}
