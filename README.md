## sp_hdf5

Header-only C++ library containing "thin" wrappers around libhdf5_cpp.

For documentation, it's easiest to start by looking at the example programs sp_hdf5/example_read.cpp
and sp_hdf5/example_write.cpp.  

For further details, I suggest reading the sp_hdf5.hpp source file, which should be pretty
digestible (I hope!)

## Installation

- You'll need to install HDF5 (including C++ support), if it's not already installed.
  HDF5 1.8.12 or later is required.  Some installation hints:

   - osx: `brew install homebrew/science/hdf5`

   - linux: you'll need to compile from scratch, see the next item!
     (Both centos and ubuntu do define packages, but they don't enable C++, at least last time I checked)

   - Compiling from scratch without root privs:
     ```
     ./configure --enable-cxx --prefix=$HOME
     make all
     make install
     ```

   - Compiling from scratch with root privs:
     ```
     ./configure --enable-cxx --prefix=/usr/local
     make all
     sudo make install
     ```

   - If you compiled libhdf5 from scratch, you may want to create an `hdf5.pc` file.
     so that pkg-config can find hdf5.  (This is usually unnecessary, but I
     needed to do this to install Kiyo Masui's bitshuffle plugin.)  See
     instructions in [README_pc.md](./README_pc.md).


## To do list

   - Error messages could be improved
   - Not very feature-complete.  Some missing features offhand:
       - Retrieve list of datasets/groups/etc
       - Partial reads/writes
       - String-valued datasets

   - Minor loose ends
       - Remove copy in hdf5_read_attribute(), N-dimensional string case (see FIXME in sp_hdf5_implementation.hpp)
