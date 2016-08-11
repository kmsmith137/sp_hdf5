# Makefile.local must define the following variables (see examples in site/)
#   INCDIR   install dir for C++ headers
#   CPP      c++ compiler command line

include Makefile.local

ifndef CPP
$(error Fatal: Makefile.local must define CPP variable)
endif

ifndef INCDIR
$(error Fatal: Makefile.local must define INCDIR variable)
endif

EXAMPLES=example_read example_write
INCFILES=sp_hdf5.hpp sp_hdf5_implementation.hpp

all: $(EXAMPLES)

example_read: example_read.cpp $(INCFILES)
	$(CPP) -o $@ $< -lhdf5_cpp -lhdf5

example_write: example_write.cpp $(INCFILES)
	$(CPP) -o $@ $< -lhdf5_cpp -lhdf5

install:
	mkdir -p $(INCDIR)
	cp -f $(INCFILES) $(INCDIR)/

clean:
	rm -f *~ $(EXAMPLES) example.hdf5
