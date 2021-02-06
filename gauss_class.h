#ifndef GAUSS_CLASS_H
#define GAUSS_CLASS_H

#include <string>
#include <vector>
#include <sstream>

#include "error_enum.h"

using namespace std;


template <typename  Data_Type>
class Calculator
{
private:
    class Pro_Set
    {
    private:
        vector<string> _name;

    public:

        string get_name(int _x)
        {
            if(_x>=_name.size()||_x<0)
                throw ERROR_SEGMENTATION_FAULT;
            return _name[_x];
        }
        void insert(string _na)
        {
            _name.push_back(_na);
        }
        int search(string _na)
        {
            int max = _name.size();
            for (int i=0;i<max;++i)
            {
                if(_name[i] == _na)
                    return i;
            }
            return -1;
        }
        void clear()
        {
            _name.clear();
        }
    }variables;

    class Matrix
    {
    public:
        vector<vector<Data_Type>> _matrix;
        void insert(vector<Data_Type> & _matr)
        {
            _matrix.push_back(_matr);
        }
        void clear()
        {
            _matrix.clear();
        }
    }cal_matr;

    bool is_digit(char _c)
    {
        return ('0' <= _c) && (_c <= '9');
    }
    bool is_word(char _c)
    {
        if('A'<= _c && _c <= 'Z') return true;
        return ('a'<= _c && _c <= 'z');
    }
public:

    int init(const string &);
    int get_ans(vector<Data_Type> &);
    string get_name(int);

};

#endif // GAUSS_CLASS_H
