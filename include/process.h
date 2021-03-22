#ifndef PROCESS_H
#define PROCESS_H

#include <vector>
#include <iostream>
#include <string>

using namespace std;

template <typename _Type>
class StreamProcessor
{
private:
    class Data
    {
    public:
        int _index = -1; // >0 => this is a variable;(_index == -1) => this is a constant; =-2 =>this is a operation
        _Type _value;
        vector<int> *neighborTable = nullptr;
        vector<double> *neighborCo = nullptr;
        Data() = default;
        Data(const Data &d) : _index(d._index), _value(d._value), neighborTable(d.neighborTable), neighborCo(d.neighborCo){};
        Data(const _Type &_x) : _index(-1), _value(_x), neighborTable(nullptr), neighborCo(nullptr){};
        Data(int _id, const _Type &_x) : _index(_id), _value(_x), neighborTable(nullptr){};
        ~Data()
        {
            if (neighborTable)
                delete neighborTable;
        }
        inline void clear_table()
        {
            if (neighborTable)
            {
                delete neighborTable;
                delete neighborCo;
            }
            neighborTable = nullptr;
        }
        inline void push_index(int x)
        {
            if (neighborTable == nullptr)
            {
                neighborTable = new vector<int>(0);
                neighborCo = new vector<double>(0);
            }
            neighborTable->push_back(x);
            neighborCo->push_back(_value);
        }
    };
    int isp(const char &) const;
    int icp(const char &) const;

public:
    vector<vector<_Type>> matrix;
    vector<string> variables_name;

    StreamProcessor() = default;

    void Process(iostream *);

    void Propos_Com_Variables(vector<Data> &, vector<_Type> &, _Type &, bool);
};

#include "../process.cpp"

#endif
