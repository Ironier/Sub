#include <iostream>
#include <string>
#include <sstream>

#include "include/process.h"

using namespace std;

signed main()
{
    try
    {
        stringstream ss("2.13*x+3*3.33*y=10*2+3\n3*x+4*y=11");
        StreamProcessor<double> p;
        p.Process(&ss);
        return 0;
    }
    catch (int ERR)
    {
        cerr << ERR << endl;
    }
    return 0;
}