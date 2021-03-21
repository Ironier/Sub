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
        int _index = -1; // >0 => this is a variable;(_index == -1) => this is a constant; =-2 =>this is a operation
        _Type _value;
        vector<int> *neighborTable = nullptr;
        Data() = default;
        Data(const _Type &_x) : _index(-1), _value(_x), neighborTable(nullptr){};
        Data(int _id, const _Type &_x) : _index(_id), _value(_x), neighborTable(nullptr){};
        ~Data()
        {
            if(neighborTable)
                delete neighborTable;
        }
        inline void clear_table()
        {
            if (neighborTable)
                delete neighborTable;
            neighborTable = nullptr;
        }
        inline void push_index(Data &obj, int x)
        {
            if (neighborTable == nullptr)
                neighborTable = new vector<int>();
            neighborTable->push_back(x);
        }
    };
    int isp(const char &) const;
    int icp(const char &) const;

public:
    vector<vector<_Type>> matrix;
    vector<string> variable_name;

    StreamProcessor() = default;
    void Process(iostream *);

    void Propos_Com_Variables(vector<Data> *, vector<_Type> *, _Type *);
};

#endif
