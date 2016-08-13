## sp_hdf5

Header-only C++ library containing "thin" wrappers around libhdf5_cpp.

For documentation, it's easiest to start by looking at the example programs sp_hdf5/example_read.cpp
and sp_hdf5/example_write.cpp.  For further details, it's easiest to refer to sp_hdf5.hpp file, which
should be pretty readable (I hope!).

To do list:
   - Error messages could be improved
   - Not very feature-complete.  Some missing features offhand:
       - Retrieve list of datasets/groups/etc
       - Partial reads/writes
       - Extendable reads/writes
       - String-valued datasets

