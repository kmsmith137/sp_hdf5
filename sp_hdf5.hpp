#ifndef _SP_HDF5_HPP
#define _SP_HDF5_HPP

#include <vector>
#include <H5Cpp.h>

namespace sp_hdf5 {
#if 0
}; // pacify emacs c-mode
#endif

// These inlines are a little silly, but I never remember the syntax otherwise!
inline H5::H5File hdf5_open_rdonly(const std::string &filename)  { return H5::H5File(filename, H5F_ACC_RDONLY); }
inline H5::H5File hdf5_open_wtrunc(const std::string &filename)  { return H5::H5File(filename, H5F_ACC_TRUNC); }
inline H5::H5File hdf5_open_wexcl(const std::string &filename)   { return H5::H5File(filename, H5F_ACC_EXCL); }

// Maps C++ type to HDF5 type (see end of file for instantiations)
template<typename T> inline const H5::PredType & hdf5_type();

// multiply elements of a vector (for range-checking, see below)
inline hsize_t hdf5_prod(const std::vector<hsize_t> &v)
{
    hsize_t ret = 1;
    for (unsigned int i = 0; i < v.size(); i++)
	ret *= v[i];
    return ret;
}

// Non range checked version
// Note: H5::CommonFG is a base class of H5File and H5Group
template<typename T> 
inline void hdf5_write_dataset(const H5::CommonFG &f, const std::string &dataset_name, const T *data, const std::vector<hsize_t> &shape)
{
    H5::DataSpace dataspace(shape.size(), &shape[0]);
    H5::DataSet dataset = f.createDataSet(dataset_name, hdf5_type<T>(), dataspace);
    dataset.write(data, hdf5_type<T>());
}

// Range checked version
template<typename T> 
inline void hdf5_write_dataset(const H5::CommonFG &f, const std::string &dataset_name, const std::vector<T> &data, const std::vector<hsize_t> &shape)
{
    if (data.size() != hdf5_prod(shape))
	throw std::runtime_error("xx");
    hdf5_write_dataset(f, dataset_name, &data[0], shape);
}

// Reference: https://www.hdfgroup.org/HDF5/doc/cpplus_RM/class_h5_1_1_pred_type.html
template<> inline const H5::PredType & hdf5_type<int>()            { return H5::PredType::NATIVE_INT; }
template<> inline const H5::PredType & hdf5_type<float>()          { return H5::PredType::NATIVE_FLOAT; }
template<> inline const H5::PredType & hdf5_type<double>()         { return H5::PredType::NATIVE_DOUBLE; }
template<> inline const H5::PredType & hdf5_type<unsigned char>()  { return H5::PredType::NATIVE_UCHAR; }

}  // namespace sp_hdf5

#endif // _SP_HDF5_HPP
