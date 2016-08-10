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

EXAMPLES=example_write

all: $(EXAMPLES)

example_write: example_write.cpp sp_hdf5.hpp
	$(CPP) -o $@ $< -lhdf5_cpp -lhdf5

install:
	mkdir -p $(INCDIR)
	cp -f sp_hdf5.hpp $(INCDIR)/

clean:
	rm -f *~ $(EXAMPLES) example.hdf5
