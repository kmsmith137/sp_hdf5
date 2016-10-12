## sp_hdf5

Header-only C++ library containing "thin" wrappers around libhdf5_cpp.

For documentation, it's easiest to start by looking at the example programs sp_hdf5/example_read.cpp
and sp_hdf5/example_write.cpp.  

For further details, I suggest reading the sp_hdf5.hpp source file, which should be pretty
digestible (I hope!)

## Installation

- You'll need to install HDF5 (including C++ support), if it's not already installed.
  HDF5 1.8.12 or later is required.  Some installation hints:

  ```
  brew install homebrew/science/hdf5                          # osx
  yum install hdf5-devel                                      # linux centos
  ./configure --prefix=$HOME --enable-cxx; make all install   # compiling from scratch
  ```

- Create a file ./Makefile.local defining the variables CPP and INCDIR.  See examples in the site/ subdirectory.
  (You can probably just copy or symlink one of these examples to ./Makefile.local)

- `make all install`

## To do list

   - Error messages could be improved
   - Not very feature-complete.  Some missing features offhand:
       - Retrieve list of datasets/groups/etc
       - Partial reads/writes
       - Extendable reads/writes
       - String-valued datasets

