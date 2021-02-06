#include "gauss_class.h"

template<>
int Calculator<double>::init(const string &_input)
{
    this->cal_matr.clear();
    this->variables.clear();

    stringstream _stream(_input);
    char c;
    string temp;
    bool isReadingVariable = false;

    int v_count = 0;

    while(_stream.peek() != EOF && (c = _stream.get()))
    {
        if(!isReadingVariable)
        {
            if(is_digit(c)) continue;
            if(is_word(c))
            {
                isReadingVariable = true;
            }
            else continue;
        }
        else
        {
            if(!is_digit(c)&&!is_word(c))
            {
                if(variables.search(temp) == -1)
                {
                    variables.insert(temp);
                    ++v_count;
                    temp = "";
                }
                continue;
            }
        }
        temp+=c;
    }
    if(isReadingVariable)
    {
        if(variables.search(temp) == -1)
        {
            variables.insert(temp);
            ++v_count;
            temp = "";
        }
    }
    if(v_count <=0)
        return ERROR_INADEQUATE_EQU;
    variables.insert("=");

    bool isReadingNumeral = false;

    //int lastRec = -1;
    double lastNum;

    vector<double> q(0, v_count+1);
    while(_stream.eof() && (c = _stream.get()))
    {
        if(_stream.peek() == EOF)
        {
            c = '\n';
        }
        if(c == ' ' || c =='\r') continue;
        if(c == '\n')
        {
            if(isReadingNumeral)
            {
                q[v_count] = stod(temp);
                temp = "";
                isReadingNumeral = false;
            }
            cal_matr.insert(q);
            q = vector<double>(0,v_count+1);
            continue;
        }

        if(!isReadingVariable)
        {
            if(is_digit(c))
            {
                if(!isReadingNumeral)
                {
                    isReadingNumeral = true;
                }
            }
            else if(is_word(c))
            {
                if(isReadingNumeral)
                    return ERROR_UNFORMATTED_INPUT;
                isReadingVariable = true;
            }
            else continue;
            temp+=c;
        }
        else
        {
            switch (c)
            {
            case '*':
            {
                if(isReadingVariable)
                {
                   q[variables.search(temp)] = lastNum;
                   isReadingNumeral = false;
                }
                if(isReadingNumeral)
                {
                   lastNum = stod(temp);
                   isReadingVariable = false;
                }
                temp = "";
                continue;
            }
            case '=':
            {
                continue;
            }
            default:
            {
                if(is_word(c))
                {
                    temp +=c;
                }
                else return ERROR_UNFORMATTED_INPUT;
            }
            }
        }

    }
    return ERROR_PASS;
}

template<>
int Calculator<double>::get_ans(vector<double> &_ans)
{
    //vector<double>
    return ERROR_PASS;
}
template<>
string Calculator<double>:: get_name(int _x)
{
    return variables.get_name(_x);
}
