// This toy program is intended to be documentation, by giving quick-and-dirty examples of
// key routines, and also a unit test, by verifying that data can be written and read back.

#include <iostream>
#include "sp_hdf5.hpp"

using namespace H5;
using namespace std;
using namespace sp_hdf5;

int main(int argc, char **argv)
{
    H5File f = hdf5_open_trunc("example.hdf5");

    hdf5_write_attribute(f, "ATTR_INT", 5);
    hdf5_write_attribute(f, "ATTR_DOUBLE", 5.8);
    hdf5_write_attribute(f, "ATTR_1D", vector<double> ({2.0, 2.5, 3.0}));
    hdf5_write_attribute(f, "ATTR_2D", vector<int> ({4,5,6,7,8,9}), {3,2});
    hdf5_write_attribute(f, "ATTR_STRING", string("hello"));
    hdf5_write_attribute(f, "ATTR_STRING_1D", vector<string>{"hello1", "hello2", "hello3"});
    hdf5_write_attribute(f, "ATTR_STRING_2D", vector<string>{"aa","bb","cc","dd","ee","ff"}, {2,3});

    // write shape-(2,3) dataset
    vector<double> data = { 10., 11., 12., 13., 14., 15. };
    hdf5_write_dataset(f, "DSET", data, {2,3});
    
    // write shape-(2,6,3) dataset, written as three (2,2,3)-chunks
    hdf5_extendable_dataset<int> d(f, "DSET2", {2,2,3}, 1);  // last argument is 'axis'
    d.write({111, 112, 113, 121, 122, 123, 211, 212, 213, 221, 222, 223}, {2,2,3});
    d.write({131, 132, 133, 141, 142, 143, 231, 232, 233, 241, 242, 243}, {2,2,3});
    d.write({151, 152, 153, 161, 162, 163, 251, 252, 253, 261, 262, 263}, {2,2,3});

    // write string-valued dataset
    vector<string> string_data = { "A", "BB", "CCC", "DDDD", "EEEEE", "FFFFFF" };
    hdf5_write_dataset(f, "STRING_DSET", string_data, {3,2});

    // create group
    H5::Group g = hdf5_create_group(f, "GROUP");
    H5::Group sg = hdf5_create_group(g, "SUBGROUP");
    
    cout << "wrote example.hdf5\n";
    return 0;
}
