// This toy program is intended to be documentation, by giving quick-and-dirty examples of
// key routines, and also a unit test, by verifying that data can be written and read back.

#include <iostream>
#include "sp_hdf5.hpp"

using namespace H5;
using namespace std;
using namespace sp_hdf5;


template<typename T>
inline void assert_equal(const char *msg, const T &x, const T &y)
{
    if (x != y)
	throw runtime_error(msg + string(": wrong value!"));
    cout << msg << ": looks good\n";
}


int main(int argc, char **argv)
{
    cout << "opening example.hdf5...\n";
    H5File f = hdf5_open("example.hdf5");

    unordered_set<string> attrs = hdf5_get_attribute_names(f);

    cout << "attrs:";
    for (const string &attr_name: attrs)
	cout << " " << attr_name;
    cout << "\n";

    int i = hdf5_read_attribute<int>(f, "ATTR_INT");
    double d = hdf5_read_attribute<double>(f, "ATTR_DOUBLE");
    string s = hdf5_read_attribute<string>(f, "ATTR_STRING");
    vector<double> attr1d = hdf5_read_attribute<double> (f, "ATTR_1D", {3});
    vector<int> attr2d = hdf5_read_attribute<int> (f, "ATTR_2D", {3,2});
    vector<string> attr_s1d = hdf5_read_attribute<string> (f, "ATTR_STRING_1D", {3});
    vector<string> attr_s2d = hdf5_read_attribute<string> (f, "ATTR_STRING_2D", {2,3});

    vector<double> dset = hdf5_read_dataset<double> (f, "DSET", {2,3});

    assert_equal("ATTR_INT", i, 5);
    assert_equal("ATTR_DOUBLE", d, 5.8);
    assert_equal("ATTR_1D", attr1d, vector<double>({2.0, 2.5, 3.0}));
    assert_equal("ATTR_2D", attr2d, vector<int>({4,5,6,7,8,9}));
    assert_equal("ATTR_STRING", s, string("hello"));
    assert_equal("ATTR_STRING_1D", attr_s1d, vector<string> {"hello1", "hello2", "hello3"});
    assert_equal("ATTR_STRING_2D", attr_s2d, vector<string> {"aa","bb","cc","dd","ee","ff"});
    assert_equal("DSET", dset, vector<double>({10.,11.,12.,13.,14.,15.}));

    assert_equal("dset exists", hdf5_dataset_exists(f,"DSET"), true);
    assert_equal("nonexistent dset", hdf5_dataset_exists(f,"NONEXISTENT"), false);
    assert_equal("attr instead of dset", hdf5_dataset_exists(f,"ATTR_INT"), false);

    vector<int> dset2 = hdf5_read_dataset<int> (f, "DSET2", {2,6,3});
    for (int i = 0; i < 2; i++) {
	for (int j = 0; j < 6; j++) {
	    for (int k = 0; k < 3; k++) {
		int expected_val = (i+1)*100 + (j+1)*10 + (k+1);
		int actual_val = dset2[18*i + 3*j + k];
		if (expected_val != actual_val) {
		    cerr << "DSET2 mismatch: (i,j,k) = (" << i << "," << j << "," << k 
			 << "): expected " << expected_val << ", got " << actual_val << endl;
		    exit(1);
		}
	    }
	}
    }
    cout << "dset2: looks good\n";

    vector<string> string_dset = hdf5_read_dataset<string> (f, "STRING_DSET", {3,2});
    assert_equal("STRING_DSET", string_dset, vector<string> {"A","BB","CCC","DDDD","EEEEE","FFFFFF"});

    return 0;
}
