// g++ -std=c++11 -Wall -I. -o example_write example_write.cpp -lhdf5_cpp -lhdf5

#include "sp_hdf5.hpp"

using namespace std;
using namespace sp_hdf5;

int main(int argc, char **argv)
{
    // allowed flags: H5F_ACC_TRUNC, H5F_ACC_EXCL, H5F_ACC_RDONLY, H5F_ACC_RDWR
    H5::H5File f("example.hdf5", H5F_ACC_TRUNC);

    // write shape-(2,3) dataset
    std::vector<double> data = { 10., 11., 12., 13., 14., 15. };
    hdf5_write_dataset(f, "DSET3", data, {2,3});
    
    return 0;
}
