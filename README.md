## sp_hdf5

Header-only C++ library containing "thin" wrappers around libhdf5_cpp.

For documentation, it's easiest to start by looking at the example programs sp_hdf5/example_read.cpp
and sp_hdf5/example_write.cpp.  

For further details, I suggest reading the sp_hdf5.hpp source file, which should be pretty
digestible (I hope!)

## Installing an out-of-date HDF5 (!!)

Note: to use sp_hdf5, you'll need to install an **out-of-date** version of the HDF5 library!
This is getting to be a big nuisance issue, and and I hope to fix it soon!  In the meantime you'll
need to download an old version of HDF5 (I recommend 1.8.20) and install it in your user directory 
as follows:

  ```
  cd ~
  wget https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.8/hdf5-1.8.20/src/hdf5-1.8.20.tar.gz
  tar zxvf hdf5-1.8.20.tar.gz
  cd hdf5-1.8.20/
  ./configure --enable-cxx --prefix=$HOME
  make -j 20
  make install
  ```

You may want to create an `hdf5.pc` file. so that pkg-config can find hdf5.  
(This is not always necessary, but I needed to do this to install Kiyo Masui's bitshuffle plugin.)
See instructions in [README_pc.md](./README_pc.md).


## Installing sp_hdf5

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
