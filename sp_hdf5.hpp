#ifndef _SP_HDF5_HPP
#define _SP_HDF5_HPP

#include <vector>
#include <sstream>
#include <H5Cpp.h>

namespace sp_hdf5 {
#if 0
}; // pacify emacs c-mode
#endif

// These inlines are a little silly, but I never remember the syntax otherwise!
inline H5::H5File hdf5_open_rdonly(const std::string &filename) { return H5::H5File(filename, H5F_ACC_RDONLY); }
inline H5::H5File hdf5_open_trunc(const std::string &filename)  { return H5::H5File(filename, H5F_ACC_TRUNC); }
inline H5::H5File hdf5_open_excl(const std::string &filename)   { return H5::H5File(filename, H5F_ACC_EXCL); }

// Group syntax 
//     H5Group g = x.openGroup(name)          where x is an H5CommonFG (= H5File or H5Group)
//     H5Group g = x.createGroup(name)


// Attribute syntax 
//     bool e = x.attrExists(name)             where x is an H5Location (base class of H5File, H5Group, H5DataSet)
//     H5Attribute a = x.openAttribute(name)
//
// plus the functions below!

inline std::string hdf5_get_name(const H5::IdComponent &x);
inline std::vector<hsize_t> hdf5_get_shape(const H5::DataSpace &ds);
inline std::vector<hsize_t> hdf5_get_shape(const H5::Attribute &attr);
inline std::vector<hsize_t> hdf5_get_attribute_shape(const H5::H5Location &x, const std::string &attr_name);

template<typename T> inline T hdf5_read_attribute(const H5::Attribute &a);
template<typename T> inline T hdf5_read_attribute(const H5::H5Location &x, const std::string &attr_name);
template<typename T> inline void hdf5_read_attribute(const H5::Attribute &a, T *data, const std::vector<hsize_t> &expected_shape);
template<typename T> inline void hdf5_read_attribute(const H5::H5Location &x, const std::string &attr_name, T *data, const std::vector<hsize_t> &expected_shape);

template<typename T> inline T hdf5_write_attribute(const H5::H5Location &x, const std::string &attr_name, const T &val);
template<typename T> inline T hdf5_write_attribute(const H5::H5Location &x, const std::string &attr_name, const T *data, const std::vector<hsize_t> &shape);


// Dataset syntax
//   bool e = x.xxx     where x is an H5CommonFG (= H5File or H5Group)
// 

inline std::vector<hsize_t> hdf5_get_shape(const H5::DataSet &ds);
inline std::vector<hsize_t> hdf5_get_dataset_shape(const H5::CommonFG &f, const std::string &dataset_name);

template<typename T> inline void hdf5_read_dataset(const H5::DataSet &ds, T *out, const std::vector<hsize_t> &expected_shape);


}  // namespace sp_hdf5

#include "sp_hdf5_implementation.hpp"

#endif // _SP_HDF5_HPP
