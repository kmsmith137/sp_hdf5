// This is a header-only library, but you'll still need to link the HDF5 library with -lhdf5_cpp -lhdf5
//
// For documentation, it's easiest to start by looking at the example programs sp_hdf5/example_read.cpp
// and sp_hdf5/example_write.cpp.  For further details, it's easiest to refer to this .hpp file, which
// should be pretty readable (I hope!).
//
// Note: declarations are here; all definitions are in sp_hdf5_implementation.hpp.

#ifndef _SP_HDF5_HPP
#define _SP_HDF5_HPP

#if (__cplusplus < 201103) && !defined(__GXX_EXPERIMENTAL_CXX0X__)
#error "This source file needs to be compiled with C++11 support (g++ -std=c++11)"
#endif

#include <vector>
#include <sstream>
#include <stdexcept>
#include <unordered_set>
#include <H5Cpp.h>

#if ((H5_VERS_MAJOR != 1) || (H5_VERS_MINOR != 10))
#error "Fatal: HDF5 version 1.10 is required!"
#else

namespace sp_hdf5 {
#if 0
}; // pacify emacs c-mode
#endif

// Some of these inlines are a little silly, but I like having a uniform syntax!

inline H5::H5File hdf5_open(const std::string &filename)        { return H5::H5File(filename, H5F_ACC_RDONLY); }
inline H5::H5File hdf5_open_trunc(const std::string &filename)  { return H5::H5File(filename, H5F_ACC_TRUNC); }
inline H5::H5File hdf5_open_excl(const std::string &filename)   { return H5::H5File(filename, H5F_ACC_EXCL); }

inline H5::Group hdf5_open_group(const H5::H5Location &x, const std::string &name)    { return x.openGroup(name); }
inline H5::Group hdf5_create_group(const H5::H5Location &x, const std::string &name)  { return x.createGroup(name); }


// -------------------------------------------------------------------------------------------------
//
// Atributes


inline H5::Attribute hdf5_open_attribute(const H5::H5Object &x, const std::string &attr_name)  { return x.openAttribute(attr_name); }

inline bool hdf5_attribute_exists(const H5::H5Object &x, const std::string &attr_name)  { return x.attrExists(attr_name); }

inline std::unordered_set<std::string> hdf5_get_attribute_names(const H5::H5Object &x);

inline std::string hdf5_get_name(const H5::IdComponent &x);
inline std::vector<hsize_t> hdf5_get_shape(const H5::DataSpace &ds);
inline std::vector<hsize_t> hdf5_get_shape(const H5::Attribute &attr);
inline std::vector<hsize_t> hdf5_get_attribute_shape(const H5::H5Object &x, const std::string &attr_name);

template<typename T> inline T hdf5_read_attribute(const H5::Attribute &a);
template<typename T> inline void hdf5_read_attribute(const H5::Attribute &a, T *data, const std::vector<hsize_t> &expected_shape);
template<typename T> inline std::vector<T> hdf5_read_attribute(const H5::Attribute &a, const std::vector<hsize_t> &expected_shape);

template<typename T> inline T hdf5_read_attribute(const H5::H5Object &x, const std::string &attr_name);
template<typename T> inline void hdf5_read_attribute(const H5::H5Object &x, const std::string &attr_name, T *data, const std::vector<hsize_t> &expected_shape);
template<typename T> inline std::vector<T> hdf5_read_attribute(const H5::H5Object &x, const std::string &attr_name, const std::vector<hsize_t> &expected_shape);

template<typename T> inline void hdf5_write_attribute(const H5::H5Object &x, const std::string &attr_name, const T &val);     // scalar
template<typename T> inline void hdf5_write_attribute(const H5::H5Object &x, const std::string &attr_name, const std::vector<T> &val);   // 1-dim
template<typename T> inline void hdf5_write_attribute(const H5::H5Object &x, const std::string &attr_name, const T *data, const std::vector<hsize_t> &shape);
template<typename T> inline void hdf5_write_attribute(const H5::H5Object &x, const std::string &attr_name, const std::vector<T> &data, const std::vector<hsize_t> &shape);


// -------------------------------------------------------------------------------------------------
//
// Datasets
//
// Note: the 'compression' argument should be a list of compression algorithms, to be tried in order.
// Currently supported algorithms are "none" and "bitshuffle" (FIXME: more to come).
//
// For example, compression = { "bitshuffle", "none" } corresponds to optional bitshuffle compression,
// whereas compression = { "bitshuffle" } corresponds to mandatory bitshuffle compression.  If 'compression'
// is an empty list, this is equivalent to { "none" }.


inline H5::DataSet hdf5_open_dataset(const H5::H5Location &x, const std::string &dataset_name) { return x.openDataSet(dataset_name); }

inline bool hdf5_dataset_exists(const H5::H5Location &f, const std::string &dataset_name);

inline std::vector<hsize_t> hdf5_get_shape(const H5::DataSet &ds);
inline std::vector<hsize_t> hdf5_get_dataset_shape(const H5::H5Location &f, const std::string &dataset_name);


template<typename T> inline void hdf5_read_dataset(const H5::DataSet &ds, T *out, const std::vector<hsize_t> &expected_shape);
template<typename T> inline std::vector<T> hdf5_read_dataset(const H5::DataSet &ds, const std::vector<hsize_t> &expected_shape);

template<typename T> inline void hdf5_read_dataset(const H5::H5Location &f, const std::string &dataset_name, T *out, const std::vector<hsize_t> &expected_shape);
template<typename T> inline std::vector<T> hdf5_read_dataset(const H5::H5Location &f, const std::string &dataset_name, const std::vector<hsize_t> &expected_shape);

template<typename T> inline void hdf5_read_partial_dataset(const H5::DataSet &ds, T *out, const std::vector<hsize_t> &offset, const std::vector<hsize_t> &size);
template<typename T> inline std::vector<T> hdf5_read_partial_dataset(const H5::DataSet &ds, const std::vector<hsize_t> &offset, const std::vector<hsize_t> &size);


template<typename T> 
inline void hdf5_write_dataset(const H5::H5Location &f, const std::string &dataset_name, const T *data, 
			       const std::vector<hsize_t> &shape, const std::vector<std::string> &compression = std::vector<std::string>());

template<typename T> 
inline void hdf5_write_dataset(const H5::H5Location &f, const std::string &dataset_name, const std::vector<T> &data, 
			       const std::vector<hsize_t> &shape, const std::vector<std::string> &compression = std::vector<std::string>());


// -------------------------------------------------------------------------------------------------
//
// Extendable datasets


template<typename T>
struct hdf5_extendable_dataset {
    H5::DataSet dataset;
    std::vector<hsize_t> curr_shape;
    int axis;

    // For the meaning of the 'compression' argument, see above.
    hdf5_extendable_dataset(const H5::H5Location &x, const std::string &dataset_name, const std::vector<hsize_t> &chunk_shape, int axis,
			    const std::vector<std::string> &compression = std::vector<std::string>());

    inline void write(const T *data, const std::vector<hsize_t> &data_shape);
    inline void write(const std::vector<T> &data, const std::vector<hsize_t> &data_shape);
};


}  // namespace sp_hdf5


#include "sp_hdf5_implementation.hpp"

#endif // ((H5_VERS_MAJOR != 1) || (H5_VERS_MINOR != 10))

#endif // _SP_HDF5_HPP
