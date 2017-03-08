#ifndef _SP_HDF5_IMPLEMENTATION_HPP
#define _SP_HDF5_IMPLEMENTATION_HPP

namespace sp_hdf5 {
#if 0
}; // pacify emacs c-mode
#endif


// -------------------------------------------------------------------------------------------------
//
// General


// Maps C++ type to HDF5 type
template<typename T> inline const H5::PredType & hdf5_type();

// Reference: https://www.hdfgroup.org/HDF5/doc/cpplus_RM/class_h5_1_1_pred_type.html
template<> inline const H5::PredType & hdf5_type<char>()                { return H5::PredType::NATIVE_CHAR; }
template<> inline const H5::PredType & hdf5_type<unsigned char>()       { return H5::PredType::NATIVE_UCHAR; }
template<> inline const H5::PredType & hdf5_type<short>()               { return H5::PredType::NATIVE_SHORT; }
template<> inline const H5::PredType & hdf5_type<unsigned short>()      { return H5::PredType::NATIVE_USHORT; }
template<> inline const H5::PredType & hdf5_type<int>()                 { return H5::PredType::NATIVE_INT; }
template<> inline const H5::PredType & hdf5_type<unsigned int>()        { return H5::PredType::NATIVE_UINT; }
template<> inline const H5::PredType & hdf5_type<long>()                { return H5::PredType::NATIVE_LONG; }
template<> inline const H5::PredType & hdf5_type<unsigned long>()       { return H5::PredType::NATIVE_ULONG; }
template<> inline const H5::PredType & hdf5_type<long long>()           { return H5::PredType::NATIVE_LLONG; }
template<> inline const H5::PredType & hdf5_type<unsigned long long>()  { return H5::PredType::NATIVE_ULLONG; }
template<> inline const H5::PredType & hdf5_type<float>()               { return H5::PredType::NATIVE_FLOAT; }
template<> inline const H5::PredType & hdf5_type<double>()              { return H5::PredType::NATIVE_DOUBLE; }
template<> inline const H5::PredType & hdf5_type<long double>()         { return H5::PredType::NATIVE_LDOUBLE; }

// Multiply elements of a vector (for range-checking, see below)
inline hsize_t hdf5_vprod(const std::vector<hsize_t> &v)
{
    hsize_t ret = 1;
    for (unsigned int i = 0; i < v.size(); i++)
	ret *= v[i];
    return ret;
}

inline std::string hdf5_vstr(const std::vector<hsize_t> &v)
{
    std::stringstream ss;
    ss << "[";
    for (unsigned int i = 0; i < v.size(); i++)
	ss << (i ? "," : "") << v[i];
    ss << "]";
    return ss.str();
}

inline std::vector<hsize_t> hdf5_get_shape(const H5::DataSpace &ds)
{
    std::vector<hsize_t> ret(ds.getSimpleExtentNdims(), 0);
    if (ret.size() > 0)
	ds.getSimpleExtentDims(&ret[0], NULL);
    return ret;
}

// surprisingly this is not in libhdf5_cpp, so we have to use the C-API
inline std::string hdf5_get_name(const H5::IdComponent &x)
{
    ssize_t len = H5Iget_name(x.getId(), NULL, 0);
    if (len <= 0)
	throw std::runtime_error("libhdf5: H5Iget_name() failed?!");

    std::vector<char> buf(len+2, 0);

    ssize_t len2 = H5Iget_name(x.getId(), &buf[0], len+2);
    if (len != len2)
	throw std::runtime_error("libhdf5: H5Iget_name() failed?!");

    return std::string(&buf[0]);
}


// -------------------------------------------------------------------------------------------------
//
// Attributes


inline std::vector<hsize_t> hdf5_get_shape(const H5::Attribute &attr)
{
    return hdf5_get_shape(attr.getSpace());
}

inline std::vector<hsize_t> hdf5_get_attribute_shape(const H5::H5Location &x, const std::string &attr_name)
{
    return hdf5_get_shape(x.openAttribute(attr_name));
}

inline void _attr_enumerate(H5::H5Location &loc, const H5std_string attr_name, void *opaque)
{
    auto s = reinterpret_cast<std::unordered_set<std::string> *> (opaque);
    s->insert(attr_name);
}

inline std::unordered_set<std::string> hdf5_get_attribute_names(const H5::H5Location &x)
{
    std::unordered_set<std::string> s;
    H5::H5Location &y = const_cast<H5::H5Location &> (x);

    int ret = y.iterateAttrs(_attr_enumerate, NULL, &s);
    if (ret != 0)
	throw std::runtime_error("sp_hdf5: not all attributes were processed in hdf5_get_attr_names()");

    return s;
}

inline void hdf5_check_shape(const H5::Attribute &a, const std::vector<hsize_t> &expected_shape)
{
    std::vector<hsize_t> actual_shape = hdf5_get_shape(a);

    if (actual_shape != expected_shape) {
	std::string msg = hdf5_get_name(a) + " has shape=" + hdf5_vstr(actual_shape) + ", expected_shape=" + hdf5_vstr(expected_shape);
	throw std::runtime_error(msg);
    }
}

// hdf5_read_attribute: zero-dimensional non-string case
template<typename T> inline T hdf5_read_attribute(const H5::Attribute &a)
{
    T ret;
    hdf5_read_attribute<T> (a, &ret, std::vector<hsize_t> ());
    return ret;
}

// hdf5_read_attribute: zero-dimensional string case
template<> inline std::string hdf5_read_attribute(const H5::Attribute &a)
{
    hdf5_check_shape(a, std::vector<hsize_t>());

    std::string ret;
    H5::StrType strtype(H5::PredType::C_S1, H5T_VARIABLE);

    a.read(strtype, ret);
    return ret;
}

// hdf5_read_attribute: N-dimensional non-string case
template<typename T> inline void hdf5_read_attribute(const H5::Attribute &a, T *data, const std::vector<hsize_t> &expected_shape)
{
    hdf5_check_shape(a, expected_shape);
    a.read(hdf5_type<T>(), data);
}

// hdf5_read_attribute: N-dimensional string case
template<> inline void hdf5_read_attribute(const H5::Attribute &a, std::string *data, const std::vector<hsize_t> &expected_shape)
{
    hdf5_check_shape(a, expected_shape);
    H5::StrType strtype(H5::PredType::C_S1, H5T_VARIABLE);
    
    hsize_t n = hdf5_vprod(expected_shape);
    std::vector<char *> c_ptrs(n, nullptr);
    a.read(strtype, &c_ptrs[0]);

    // FIXME the std::string constructor allocates a new buffer and copies the data.
    // Is there a way to tell the constructor that it can claim ownership of the buffer instead?
    for (hsize_t i = 0; i < n; i++) {
	data[i] = std::string(c_ptrs[i]);
	free(c_ptrs[i]);
	c_ptrs[i] = nullptr;
    }
}

// hdf5_read_attribute: alternate N-dimensional interface (applies to both string or non-string cases)
template<typename T> inline std::vector<T> hdf5_read_attribute(const H5::Attribute &a, const std::vector<hsize_t> &expected_shape)
{
    std::vector<T> ret(hdf5_vprod(expected_shape));
    hdf5_read_attribute(a, &ret[0], expected_shape);
    return ret;
}

// hdf5_read_attribute: interface which opens and reads attribute in single call
template<typename T> inline T hdf5_read_attribute(const H5::H5Location &x, const std::string &attr_name)
{
    return hdf5_read_attribute<T> (x.openAttribute(attr_name));
}

// hdf5_read_attribute: interface which opens and reads attribute in single call
template<typename T> inline std::vector<T> hdf5_read_attribute(const H5::H5Location &x, const std::string &attr_name, const std::vector<hsize_t> &expected_shape)
{
    return hdf5_read_attribute<T> (x.openAttribute(attr_name), expected_shape);
}

// hdf5_read_attribute: interface which opens and reads attribute in single call
template<typename T> inline void hdf5_read_attribute(const H5::H5Location &x, const std::string &attr_name, const T *data, const std::vector<hsize_t> &expected_shape)
{
    hdf5_read_attribute(x.openAttribute(attr_name), data, expected_shape);
}


// hdf5_write_attribute(): zero-dimensional non-string case
template<typename T> inline void hdf5_write_attribute(const H5::H5Location &x, const std::string &attr_name, const T &val)
{
    H5::DataSpace attrspace(H5S_SCALAR);
    H5::Attribute a = x.createAttribute(attr_name, hdf5_type<T> (), attrspace);
    a.write(hdf5_type<T> (), &val);
}

// hdf5_write_attribute(): zero-dimensional string case
template<> inline void hdf5_write_attribute(const H5::H5Location &x, const std::string &attr_name, const std::string &val)
{
    H5::DataSpace attrspace(H5S_SCALAR);
    H5::StrType strtype(H5::PredType::C_S1, H5T_VARIABLE);
    H5::Attribute a = x.createAttribute(attr_name, strtype, attrspace);
    a.write(strtype, val);
}

// hdf5_write_attribute(): N-dimensional non-string case
template<typename T> inline void hdf5_write_attribute(const H5::H5Location &x, const std::string &attr_name, const T *data, const std::vector<hsize_t> &shape)
{
    H5::DataSpace attrspace(shape.size() ? H5S_SIMPLE : H5S_SCALAR);
    if (shape.size() > 0)
	attrspace.setExtentSimple(shape.size(), &shape[0]);

    H5::Attribute a = x.createAttribute(attr_name, hdf5_type<T>(), attrspace);
    a.write(hdf5_type<T>(), data);
}

// hdf5_write_attribute(): N-dimensional string case
template<> inline void hdf5_write_attribute(const H5::H5Location &x, const std::string &attr_name, const std::string *data, const std::vector<hsize_t> &shape)
{
    H5::StrType strtype(H5::PredType::C_S1, H5T_VARIABLE);

    H5::DataSpace attrspace(shape.size() ? H5S_SIMPLE : H5S_SCALAR);
    if (shape.size() > 0)
	attrspace.setExtentSimple(shape.size(), &shape[0]);

    hsize_t n = hdf5_vprod(shape);
    std::vector<const char *> c_strings(n);
    for (hsize_t i = 0; i < n; i++)
	c_strings[i] = data[i].c_str();

    H5::Attribute a = x.createAttribute(attr_name, strtype, attrspace);
    a.write(strtype, &c_strings[0]);
}

// hdf5_write_attribute(): one-dimensional non-string case
template<typename T> inline void hdf5_write_attribute(const H5::H5Location &x, const std::string &attr_name, const std::vector<T> &val)
{
    hsize_t n = val.size();
    H5::DataSpace attrspace(H5S_SIMPLE);
    attrspace.setExtentSimple(1, &n);

    H5::Attribute a = x.createAttribute(attr_name, hdf5_type<T>(), attrspace);
    a.write(hdf5_type<T>(), &val[0]);
}

// hdf5_write_attribute(): one-dimensional string case
template<> inline void hdf5_write_attribute(const H5::H5Location &x, const std::string &attr_name, const std::vector<std::string> &val)
{
    hdf5_write_attribute(x, attr_name, &val[0], { val.size() });
}

// hdf5_write_attribute(): alternate interface for N-dimensional case
template<typename T> inline void hdf5_write_attribute(const H5::H5Location &x, const std::string &attr_name, const std::vector<T> &data, const std::vector<hsize_t> &shape)
{
    if (data.size() != hdf5_vprod(shape))
	throw std::runtime_error("hdf5_write_attribute: " + attr_name + ": length of data vector is inconsistent with shape array");
    hdf5_write_attribute(x, attr_name, &data[0], shape);    
}


// -------------------------------------------------------------------------------------------------
//
// Datasets


inline std::vector<hsize_t> hdf5_get_shape(const H5::DataSet &ds)
{
    return hdf5_get_shape(ds.getSpace());
}

inline std::vector<hsize_t> hdf5_get_dataset_shape(const H5::CommonFG &f, const std::string &dataset_name)
{
    return hdf5_get_shape(f.openDataSet(dataset_name));
}

// FIXME is there a better way to implement this?
inline bool hdf5_dataset_exists(const H5::H5Location &f, const std::string &dataset_name)
{
    htri_t ret = H5Lexists(f.getId(), dataset_name.c_str(), H5P_DEFAULT);
    if (ret <= 0)
	return false;

    H5O_info_t infobuf;
    herr_t status = H5Oget_info_by_name(f.getId(), dataset_name.c_str(), &infobuf, H5P_DEFAULT);
    if (status < 0)
	throw std::runtime_error("H5Oget_info_by_name() failed?!");

    return (infobuf.type == H5O_TYPE_DATASET);
}

template<typename T>
inline void hdf5_read_dataset(const H5::DataSet &d, T *out, const std::vector<hsize_t> &expected_shape)
{
    H5::DataSpace filespace = d.getSpace();
    H5::DataSpace memspace(expected_shape.size(), &expected_shape[0]);

    std::vector<hsize_t> actual_shape = hdf5_get_shape(filespace);
    
    if (actual_shape != expected_shape) {
	std::string msg = hdf5_get_name(d) + " has shape=" + hdf5_vstr(actual_shape) + ", expected_shape=" + hdf5_vstr(expected_shape);
	throw std::runtime_error(msg);
    }

    d.read(out, hdf5_type<T>(), memspace, filespace);
}

template<typename T> 
inline std::vector<T> hdf5_read_dataset(const H5::DataSet &ds, const std::vector<hsize_t> &expected_shape)
{
    std::vector<T> ret(hdf5_vprod(expected_shape));
    hdf5_read_dataset(ds, &ret[0], expected_shape);
    return ret;
}

template<typename T> 
inline void hdf5_read_dataset(const H5::CommonFG &f, const std::string &dataset_name, T *out, const std::vector<hsize_t> &expected_shape)
{
    hdf5_read_dataset(f.openDataSet(dataset_name), out, expected_shape);
}

template<typename T> 
inline std::vector<T> hdf5_read_dataset(const H5::CommonFG &f, const std::string &dataset_name, const std::vector<hsize_t> &expected_shape)
{
    return hdf5_read_dataset<T> (f.openDataSet(dataset_name), expected_shape);
}

template<typename T> 
inline void hdf5_write_dataset(const H5::CommonFG &f, const std::string &dataset_name, const T *data, const std::vector<hsize_t> &shape)
{
    H5::DataSpace dataspace(shape.size(), &shape[0]);
    H5::DataSet dataset = f.createDataSet(dataset_name, hdf5_type<T>(), dataspace);
    dataset.write(data, hdf5_type<T>());
}

template<typename T> 
inline void hdf5_write_dataset(const H5::CommonFG &f, const std::string &dataset_name, const std::vector<T> &data, const std::vector<hsize_t> &shape)
{
    if (data.size() != hdf5_vprod(shape))
	throw std::runtime_error("hdf5_write_dataset: " + dataset_name + ": length of data vector is inconsistent with shape array");
    hdf5_write_dataset(f, dataset_name, &data[0], shape);
}


// -------------------------------------------------------------------------------------------------
//
// hdf5_extendable_dataset


template<typename T>
hdf5_extendable_dataset<T>::hdf5_extendable_dataset(const H5::CommonFG &x, const std::string &dataset_name, const std::vector<hsize_t> &chunk_shape, int axis_)
{
    this->axis = axis_;

    int ndim = chunk_shape.size();

    if (ndim < 1)
        throw std::runtime_error("hdf5_extendable_dataset constructor: attempt to create zero-dimensional dataset");
    if ((axis < 0) || (axis >= ndim))
        throw std::runtime_error("hdf5_extendable_dataset constructor: axis is out of range");
    for (auto s: chunk_shape)
	if (s <= 0)
            throw std::runtime_error("hdf5_extendable_dataset constructor: all chunk_shape dimensions must be positive");

    this->curr_shape = chunk_shape;
    this->curr_shape[axis] = 0;

    std::vector<hsize_t> max_shape = chunk_shape;
    max_shape[axis] = H5S_UNLIMITED;

    // hid_t space_id = H5Screate_simple(ndim, &curr_shape[0], &max_shape[0]);
    H5::DataSpace data_space(chunk_shape.size(), &curr_shape[0], &max_shape[0]);

    // hid_t prop_id = H5Pcreate(H5P_DATASET_CREATE);
    // herr_t err = H5Pset_chunk(prop_id, ndim, &chunk_shape[0]);
    H5::DSetCreatPropList prop_list;
    prop_list.setChunk(chunk_shape.size(), &chunk_shape[0]);

    this->dataset = x.createDataSet(dataset_name, hdf5_type<T>(), data_space, prop_list);
}

template<typename T>
inline void hdf5_extendable_dataset<T>::write(const T *data, const std::vector<hsize_t> &data_shape)
{
    if (data_shape.size() != curr_shape.size())
	throw std::runtime_error("hdf5_extendable_dataset::write(): data_shape has wrong number of dimensions");

    for (int i = 0; i < (int)data_shape.size(); i++) {
	if (data_shape[i] <= 0)
            throw std::runtime_error("hdf5_extendable_dataset::write(): all data_shape dimensions must be positive");
        if ((i != axis) && (data_shape[i] != curr_shape[i]))
	    throw std::runtime_error("hdf5_extendable_dataset::write(): data_shape dimensions are mismatched to dimensions specified at construction");
    }

    std::vector<hsize_t> offsets(data_shape.size(), 0);
    offsets[axis] = curr_shape[axis];

    std::vector<hsize_t> new_shape = curr_shape;
    new_shape[axis] += data_shape[axis];

    // herr_t status = H5Dset_extent(dataset_id, &new_shape[0]);
    this->dataset.extend(&new_shape[0]);

    // hid_t file_space_id = H5Dget_space(dataset_id); 
    // status = H5Sselect_hyperslab(file_space_id, H5S_SELECT_SET, &offsets[0], NULL, &shape[0], NULL);
    H5::DataSpace file_space = this->dataset.getSpace();
    file_space.selectHyperslab(H5S_SELECT_SET, &data_shape[0], &offsets[0]);

    // hid_t mem_space_id = H5Screate_simple(shape.size(), &shape[0], NULL);
    H5::DataSpace mem_space(data_shape.size(), &data_shape[0]);

    // H5Dwrite(dataset_id, type, mem_space_id, file_space_id, H5P_DEFAULT, data);
    // write(const void *buf, const DataType &mem_type, const DataSpace &mem_space=DataSpace::ALL, const DataSpace &file_space=DataSpace::ALL, const DSetMemXferPropList &xfer_plist=DSetMemXferPropList::DEFAULT
    this->dataset.write(data, hdf5_type<T>(), mem_space, file_space);
    this->curr_shape = new_shape;
}


template<typename T>
inline void hdf5_extendable_dataset<T>::write(const std::vector<T> &data, const std::vector<hsize_t> &data_shape)
{
    if (data.size() != hdf5_vprod(data_shape))
	throw std::runtime_error("hdf5_extendable_dataset::write(): length of data vector is inconsistent with shape array");
    this->write(&data[0], data_shape);
}


}  // namespace sp_hdf5


#endif // _SP_HDF5_IMPLEMENTATION_HPP
