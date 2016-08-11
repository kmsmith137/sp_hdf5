// g++ -std=c++11 -Wall -I. -o example_write example_write.cpp -lhdf5_cpp -lhdf5

#include "sp_hdf5.hpp"

using namespace std;
using namespace sp_hdf5;

int main(int argc, char **argv)
{
    H5::H5File f = hdf5_open_trunc("example.hdf5");

    hdf5_write_attribute(f, "ATTR_INT", 5);
    hdf5_write_attribute(f, "ATTR_DOUBLE", 5.8);
    hdf5_write_attribute(f, "ATTR_1D", vector<double> ({2.0, 2.5, 3.0}));
    hdf5_write_attribute(f, "ATTR_2D", vector<int> ({4,5,6,7,8,9}), {3,2});

    // write shape-(2,3) dataset
    std::vector<double> data = { 10., 11., 12., 13., 14., 15. };
    hdf5_write_dataset(f, "DSET", data, {2,3});
    
    return 0;
}
