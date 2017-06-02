### Instructions for the hdf5.pc file

If you compiled libhdf5 from scratch, you may want to create an `hdf5.pc` file.
so that pkg-config can find hdf5.  (This is usually unnecessary, but I needed
to do this to install Kiyo Masui's bitshuffle plugin.)

The hdf5.pc file should look something like this (but you may need to change
the directory names and/or HDF5 version):
```
prefix=/usr/local
exec_prefix=${prefix}
includedir=${prefix}/include
libdir=${exec_prefix}/lib

Name: hdf5
Description: HDF5
Version: 1.8.18
Cflags: -I${includedir}
Libs: -L${libdir} -lhdf5
```

It should go in a directory which gets scanned by pkg-config.

  - Such a directory may be found by trial and error (use `pkg-config --libs hdf5`,
    not `pkg-config hdf5`, which returns silently whether the file was scanned or not).
    Alternately you can set the PKG_CONFIG_PATH environment variable.

  - In CentOS, you can use `/usr/lib64/pkgconfig` (but not `/usr/lib/pkgconfig` or
    `/usr/local/lib*/pkgconfig`)

  - In Ubuntu, you can use `/usr/local/lib/pkgconfig` (you may need to create this directory)

  