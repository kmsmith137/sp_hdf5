## sp_hdf5

Header-only C++ library containing "thin" wrappers around libhdf5_cpp.

For documentation, it's easiest to start by looking at the example programs sp_hdf5/example_read.cpp
and sp_hdf5/example_write.cpp.  

For further details, I suggest reading the sp_hdf5.hpp source file, which should be pretty
digestible (I hope!)

## Installation

- You'll need to install HDF5 (including C++ support), if it's not already installed.

  **Currently, sp_hdf5 requires HDF5 version 1.8.12 or later,
  but does not work with version 1.10.x.  This will be fixed eventually!**

  In the meantime, here are some hints for installing a version of HDF5
  which is neither too old nor too new:

   - osx: `brew install homebrew/science/hdf5` works, last time I checked.
   
   - linux: You're out of luck!  Both centos and ubuntu install hdf5 versions
     which are too old, last time I checked.  So you'll need to compile from
     scratch, see the next item...

   - Compiling from scratch without root privs:
     ```
     wget https://support.hdfgroup.org/ftp/HDF5/current18/src/hdf5-1.8.20.tar.gz
     tar zxvf hdf5-1.8.20.tar.gz
     cd hdf5-1.8.20
     ./configure --enable-cxx --prefix=$HOME
     make -j4
     make install
     ```

   - Compiling from scratch with root privs:
     ```
     wget https://support.hdfgroup.org/ftp/HDF5/current18/src/hdf5-1.8.20.tar.gz
     tar zxvf hdf5-1.8.20.tar.gz
     cd hdf5-1.8.20
     ./configure --enable-cxx --prefix=/usr/local
     make -j4
     sudo make install
     ```

   - If you compiled libhdf5 from scratch, you may want to create an `hdf5.pc` file.
     so that pkg-config can find hdf5.  (This is not always necessary, but I
     needed to do this to install Kiyo Masui's bitshuffle plugin.)  See
     instructions in [README_pc.md](./README_pc.md).

- The sp_hdf5 Makefile assumes the existence of a file `Makefile.local` which defines
  the Makefile variables:
    ```
    INCDIR     Installation directory for C++ header files
    CPP        C++ compiler executable + flags
    ```
  You can probably use one of the example Makefile.locals in `site/`.

- To build and install, do `make all install`.

- For a quick test, you should get no errors when you run the pair
  of example programs back-to-back:
  ```
  ./example-write
  ./example-read
  ```

## To do list

   - Error messages could be improved
   - Not very feature-complete.  Some missing features offhand:
       - Retrieve list of datasets/groups/etc
       - Partial reads/writes
       - String-valued datasets

   - Minor loose ends
       - Remove copy in hdf5_read_attribute(), N-dimensional string case (see FIXME in sp_hdf5_implementation.hpp)
