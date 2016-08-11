// Note: declarations are here; all definitions are in sp_hdf5_implementation.hpp

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
inline H5::H5File hdf5_open(const std::string &filename)        { return H5::H5File(filename, H5F_ACC_RDONLY); }
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
template<typename T> inline void hdf5_read_attribute(const H5::Attribute &a, T *data, const std::vector<hsize_t> &expected_shape);
template<typename T> inline std::vector<T> hdf5_read_attribute(const H5::Attribute &a, const std::vector<hsize_t> &expected_shape);

template<typename T> inline T hdf5_read_attribute(const H5::H5Location &x, const std::string &attr_name);
template<typename T> inline void hdf5_read_attribute(const H5::H5Location &x, const std::string &attr_name, T *data, const std::vector<hsize_t> &expected_shape);
template<typename T> inline std::vector<T> hdf5_read_attribute(const H5::H5Location &x, const std::string &attr_name, const std::vector<hsize_t> &expected_shape);

template<typename T> inline void hdf5_write_attribute(const H5::H5Location &x, const std::string &attr_name, const T &val);     // scalar
template<typename T> inline void hdf5_write_attribute(const H5::H5Location &x, const std::string &attr_name, const std::vector<T> &val);   // 1-dim
template<typename T> inline void hdf5_write_attribute(const H5::H5Location &x, const std::string &attr_name, const T *data, const std::vector<hsize_t> &shape);
template<typename T> inline void hdf5_write_attribute(const H5::H5Location &x, const std::string &attr_name, const std::vector<T> &data, const std::vector<hsize_t> &shape);

// Dataset syntax
//   H5DataSet d = x.openDataSet(name)
//
// plus the routines below!

inline std::vector<hsize_t> hdf5_get_shape(const H5::DataSet &ds);
inline std::vector<hsize_t> hdf5_get_dataset_shape(const H5::CommonFG &f, const std::string &dataset_name);
inline bool hdf5_dataset_exists(const H5::H5Location &f, const std::string &dataset_name);

template<typename T> inline void hdf5_read_dataset(const H5::DataSet &ds, T *out, const std::vector<hsize_t> &expected_shape);
template<typename T> inline std::vector<T> hdf5_read_dataset(const H5::DataSet &ds, const std::vector<hsize_t> &expected_shape);

template<typename T> inline void hdf5_read_dataset(const H5::CommonFG &ds, T *out, const std::vector<hsize_t> &expected_shape);
template<typename T> inline std::vector<T> hdf5_read_dataset(const H5::DataSet &ds, const std::vector<hsize_t> &expected_shape);

template<typename T> inline void hdf5_write_dataset(const H5::CommonFG &f, const std::string &dataset_name, const T *data, const std::vector<hsize_t> &shape);
template<typename T> inline void hdf5_write_dataset(const H5::CommonFG &f, const std::string &dataset_name, const std::vector<T> &data, const std::vector<hsize_t> &shape);


}  // namespace sp_hdf5

#include "sp_hdf5_implementation.hpp"

#endif // _SP_HDF5_HPP
