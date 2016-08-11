// g++ -std=c++11 -Wall -I. -o example_write example_write.cpp -lhdf5_cpp -lhdf5

#include "sp_hdf5.hpp"

using namespace std;
using namespace sp_hdf5;

int main(int argc, char **argv)
{
    H5::H5File f = hdf5_open_rdonly("example.hdf5");

    std::vector<double> data(6);
    // hdf5_write_dataset(f, "DSET", data, {2,3});
    
    return 0;
}
