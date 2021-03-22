#include "include/Standard_Output.h"

#include <stack>
#include <unordered_map>

using namespace std;

#define is_digit(x) ('0' <= x && x <= '9')
#define is_word(x) (('a' <= x && x <= 'z') || ('A' <= x && x <= 'Z'))

template <typename _Type>
void StreamProcessor<_Type>::Process(iostream *_stream)
{
    variables_name.clear();
    variables_name.reserve(0);
    matrix.clear();
    matrix.reserve(0);

    stack<char> cOperators;
    cOperators.push('#');

    unordered_map<string, int> variables_map;

    vector<Data> proposition; // >=0? =>variable : <0? => (-proposition[i] - 1) is the original operation

    char c = 0;
    string buffer;
    _Type _temp_line_constant = 0.0;
    vector<_Type> _temp_column, _temp_line_coefficient;

    bool isReadingVariable = false, isReadingNumeral = false;

    _stream->get(c);
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
            buffer += c;
        }
        else if (is_digit(c))
        {
            if (!isReadingNumeral)
            {
                if (!isReadingVariable)
                    isReadingNumeral = true;
            }
            buffer += c;
        }
        else
        {
            switch (c)
            {
            case '_':
                if (!isReadingVariable)
                    throw GLOBAL_ERROR_BAD_INPUT;
            case '.':
                if (!isReadingNumeral)
                    throw GLOBAL_ERROR_BAD_INPUT;
                buffer += c;
            case ' ':
            case '\t':
                break;
            default:
                /////////
                int tempPriority = icp(c);

                if (isReadingNumeral)
                {
                    proposition.push_back(Data(-1, stod(buffer)));
                    buffer = "";
                    isReadingNumeral = false;
                }

                if (isReadingVariable)
                {
                    if (variables_map.count(buffer) == 0)
                    {
                        variables_map[buffer] = variables_name.size();
                        variables_name.push_back(buffer);
                    }
                    proposition.push_back(Data(variables_map[buffer], 1.0));
                    isReadingVariable = false;
                    buffer = "";
                }

                if (tempPriority == -1)
                    throw GLOBAL_ERROR_BAD_INPUT;

                while (isp(cOperators.top()) > tempPriority)
                {
                    proposition.push_back(Data(-2, (double)(cOperators.top())));
                    cOperators.pop();
                }

                if (c == '=')
                {
                    _temp_line_coefficient.resize(variables_name.size(), 0);
                    Propos_Com_Variables(proposition, _temp_line_coefficient, _temp_line_constant, false);
                    proposition.clear();
                    proposition.reserve(0);
                    break;
                }

                if (c == '\n' || c == '#')
                {
                    ////////
                    if (proposition.size() == 0)
                        break;

                    Propos_Com_Variables(proposition, _temp_line_coefficient, _temp_line_constant, true);
                    _temp_column.push_back(_temp_line_constant);
                    matrix.push_back(_temp_line_coefficient);

                    _temp_line_constant = 0.0;
                    _temp_line_coefficient.clear();
                    _temp_line_coefficient.reserve(0);
                    proposition.clear();
                    proposition.reserve(0);

                    break;
                }

                cOperators.push(c);
                break;
            }
        }
        /*if (variables_name.size() == 1001)
        {
            return;
        }*/
        if (!_stream->get(c))
        {
            if (c != '#')
                c = '#';
            else
                break;
        }
    }
    int s1 = matrix.size(), s2 = variables_name.size();
    if (s1 != s2)
        throw GLOBAL_ERROR_INCORRECT_NUMBER_OF_EQUATION;
    for (int i = 0; i < _temp_column.size(); ++i)
    {
        matrix[i].resize(variables_name.size(), 0);
        matrix[i].push_back(_temp_column[i]);
    }
}
template <typename _Type>
void StreamProcessor<_Type>::Propos_Com_Variables(vector<Data> &_pros, vector<_Type> &_output, _Type &_output_constant, bool rev)
{
    //reckon _output_constant == 0.0
    stack<int> s;
    int _max = _pros.size();
    for (int i = 0; i < _max; ++i)
    {
        if (_pros[i]._index >= -1)
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
            tempA_index = _pros[tempA]._index;
            tempB_index = _pros[tempB]._index;
            c = (int)_pros[i]._value;
            switch (c)
            {
            case '+':
                if (tempA_index == tempB_index)
                {
                    _pros[tempA]._value += _pros[tempB]._value;
                    s.push(tempA);
                }
                else if (tempA_index < 0)
                {
                    _pros[tempB].push_index(tempA);
                    s.push(tempB);
                }
                else
                {
                    _pros[tempA].push_index(tempB);
                    s.push(tempA);
                }
                break;
            case '-':
                if (tempA_index == tempB_index)
                {
                    _pros[tempA]._value -= _pros[tempB]._value;
                    s.push(tempA);
                }
                else if (tempA_index < 0)
                {
                    _pros[tempB]._value = -_pros[tempB]._value;
                    _pros[tempB].push_index(tempA);
                    s.push(tempB);
                }
                else
                {
                    _pros[tempA].push_index(tempB);
                    s.push(tempA);
                }
                break;
            case '*':
                if (tempA_index == tempB_index)
                {
                    if (tempA_index >= 0)
                        throw GLOBAL_ERROR_VARIABLE_TIMES_VARIABLE;
                    _pros[tempA]._value *= _pros[tempB]._value;
                    s.push(tempA);
                }
                else if (tempA_index < 0)
                {
                    _pros[tempB]._value *= _pros[tempA]._value;
                    s.push(tempB);
                }
                else
                {
                    if (tempB_index >= 0)
                        throw GLOBAL_ERROR_VARIABLE_TIMES_VARIABLE;
                    _pros[tempA]._value *= _pros[tempB]._value;
                    s.push(tempA);
                }
                break;
            case '/':
                if (tempA_index == tempB_index)
                {
                    if (tempA_index >= 0)
                        throw GLOBAL_ERROR_VARIABLE_TIMES_VARIABLE;
                    _pros[tempA]._value /= _pros[tempB]._value;
                    s.push(tempA);
                }
                else if (tempA_index < 0)
                {
                    _pros[tempB]._value /= _pros[tempA]._value;
                    s.push(tempB);
                }
                else
                {
                    if (tempB_index >= 0)
                        throw GLOBAL_ERROR_VARIABLE_TIMES_VARIABLE;
                    _pros[tempA]._value /= _pros[tempB]._value;
                    s.push(tempA);
                }
                break;
            }
        }
    }
    //get coefficient
    static int ceil, id;
    static double t;
    if (rev)
    {
        while (s.size())
        {
            Data tempData = _pros[s.top()];
            s.pop();
            t = tempData._value;
            id = tempData._index;
            if (id >= 0)
            {
                _output[id] -= t;
                if (!tempData.neighborTable)
                    continue;
                ceil = tempData.neighborTable->size();
                for (int i = 0; i < ceil; ++i)
                {
                    static int temp;
                    temp = tempData.neighborTable->operator[](i);
                    _pros[temp]._value *= t / tempData.neighborCo->operator[](i);
                    s.push(temp);
                }
            }
            else
                _output_constant += t;
        }
    }
    else
        while (s.size())
        {
            Data tempData = _pros[s.top()];
            s.pop();
            t = tempData._value;
            id = tempData._index;
            if (id >= 0)
            {
                _output[id] += t;
                if (!tempData.neighborTable)
                    continue;
                ceil = tempData.neighborTable->size();
                for (int i = 0; i < ceil; ++i)
                {
                    static int temp;
                    temp = tempData.neighborTable->operator[](i);
                    _pros[temp]._value *= t / tempData.neighborCo->operator[](i);
                    s.push(temp);
                }
            }
            else
                _output_constant -= t;
            tempData.neighborTable = nullptr;
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
        return 2;
    case ')':
        return 8;
    case '=':
    case '\n':
        return 1;
    case '+':
    case '-':
        return 4;
    case '*':
    case '/':
        return 6;
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
        return 8;
    case ')':
        return 2;
    case '=':
    case '\n':
        return 1;
    case '+':
    case '-':
        return 3;
    case '*':
    case '/':
        return 5;
    }
    return -1;
}
