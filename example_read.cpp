// g++ -std=c++11 -Wall -I. -o example_write example_write.cpp -lhdf5_cpp -lhdf5

#include <iostream>
#include "sp_hdf5.hpp"

using namespace std;
using namespace sp_hdf5;


template<typename T>
inline void assert_equal(const char *msg, const T &x, const T &y)
{
    if (x != y)
	throw std::runtime_error(msg + string(": wrong value!"));
    cout << msg << ": looks good\n";
}


int main(int argc, char **argv)
{
    H5::H5File f = hdf5_open("example.hdf5");

    int i = hdf5_read_attribute<int>(f, "ATTR_INT");
    double d = hdf5_read_attribute<double>(f, "ATTR_DOUBLE");
    string s = hdf5_read_attribute<string>(f, "ATTR_STRING");
    vector<double> attr1d = hdf5_read_attribute<double> (f, "ATTR_1D", {3});
    vector<int> attr2d = hdf5_read_attribute<int> (f, "ATTR_2D", {3,2});

    assert_equal("ATTR_INT", i, 5);
    assert_equal("ATTR_DOUBLE", d, 5.8);
    assert_equal("ATTR_STRING", s, string("hello"));
    assert_equal("ATTR_1D", attr1d, vector<double>({2.0, 2.5, 3.0}));
    assert_equal("ATTR_2D", attr2d, vector<int>({4,5,6,7,8,9}));
        
    return 0;
}
