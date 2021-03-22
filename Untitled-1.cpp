#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "include/process.h"

using namespace std;

signed main()
{
    try
    {
        fstream ss("random.txt");
        //stringstream ss("2*x2+3*y1=0\nx2*3+y1*4=9");
        StreamProcessor<double> p;
        p.Process(&ss);
        ss.close();
        return 0;
    }
    catch (int ERR)
    {
        cerr << ERR << endl;
    }
    return 0;
}