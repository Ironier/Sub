#ifndef PROCESS_H
#define PROCESS_H

#include <vector>
#include <iostream>

using namespace std;

template <typename _Type>
class StreamProcessor
{
private:
    class Data
    {
    public:
        int _index = -1; // >0=> this is a variable; -1 => this is a numeral ;(<-1)? => this is a operation
        _Type _value;
        Data() = default;
        Data(const _Type &_x) : _index(-1), _value(_x){};
        Data(int _id, const _Type &_x) : _index(_id), _value(_x){};
    };
    int isp(const char &) const;
    int icp(const char &) const;

public:
    vector<vector<_Type>> matrix;
    vector<string> variable_name;

    StreamProcessor() = default;
    void Process(iostream *);

    void Propos_Com_Variables(vector<Data> *, vector<_Type> *);
    void Propos_Com_Constant(vector<Data> *, _Type *);
};

#endif
