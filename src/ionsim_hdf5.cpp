#include "ionsim_hdf5.h"
#include <typeinfo>
#include <stdexcept>

const bool DEBUG_FLAG = false;

// ==============================================
// Debug
// ==============================================
Debug::Debug(const bool debug_flag) :
	debug(debug_flag)
{
}

herr_t Debug::close(herr_t (*f)(hid_t), hid_t attr_id, std::string name)
{
	herr_t status;

	status = f(attr_id);
	if (debug)
	{
		if (status < 0)
		{
			std::cerr << "Couldn't close: " << name << std::endl;
		} else {
			std::cerr << "Successfully closed: " << name << std::endl;
		}
	}

	return status;
}


// ==============================================
// GroupAccess
// ==============================================
GroupAccess::GroupAccess(hid_t &loc_id, std::string group_str) :
	Debug(DEBUG_FLAG),
	_group_str(group_str)
{
	_loc_id = loc_id;

	herr_t status;
	H5G_info_t objinfo;

	// ==============================================
	// Access or create a new group
	// ==============================================
	H5Eset_auto(NULL, NULL, NULL);
	group_id = H5Gopen(loc_id, _group_str.c_str(), H5P_DEFAULT);
	if (group_id < 0)
	{
		group_id = H5Gcreate(loc_id, _group_str.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		status = H5Gget_info_by_name(loc_id, _group_str.c_str(), &objinfo, H5P_DEFAULT);
		if ((status < 0) || (group_id < 0))
		{
			std::cout << "Warning: could not create " << _group_str << std::endl;
		}
	}
}

GroupAccess::~GroupAccess()
{
	close(&H5Gclose, group_id, _group_str);
}

// ==============================================
// GroupStepAccess
// ==============================================
std::string _getstep(unsigned int step)
{
	std::string out;
	char buf[10];
	sprintf(buf, "Step_%04u", step);
	out = buf;
	return out;
}

GroupStepAccess::GroupStepAccess(hid_t &loc_id, unsigned int step) :
	GroupAccess(loc_id, _getstep(step))
{
}

// ==============================================
// DatasetOpen
// ==============================================
DatasetOpen::DatasetOpen(hid_t &loc_id, std::string dataset_str) :
	Debug(DEBUG_FLAG),
	_dataset_str(dataset_str)
{
	_loc_id = loc_id;
	
	// ==============================================
	// Open dataset
	// ==============================================
	dataset_id = H5Dopen(_loc_id, dataset_str.c_str(), H5P_DEFAULT);
	dataspace_id = H5Dget_space(dataset_id);
}

DatasetOpen::~DatasetOpen()
{
	close(&H5Sclose, dataspace_id, _dataset_str + ":dataspace");
	close(&H5Dclose, dataset_id, _dataset_str);
}

std::vector<double> DatasetOpen::get_single(int dim, std::vector<int> spec_dim)
{
	std::vector<double> out;

	double *buf;
	int ndims;

	int j;
	hsize_t *offset, *count;
	hsize_t *dims;
	hsize_t current_dims[1];
	hssize_t npoints;
	hid_t memspace_id;
	hid_t readspace_id;
	herr_t status;

	// ==============================================
	// Get info about dimensionality
	// ==============================================
	ndims = H5Sget_simple_extent_ndims(dataspace_id);

	if (dim >= ndims) throw std::runtime_error("Outside of dims.");

	dims   = new hsize_t[ndims];
	offset = new hsize_t[ndims];
	count  = new hsize_t[ndims];

	H5Sget_simple_extent_dims(dataspace_id, dims, NULL);

	// ==============================================
	// Set up vars
	// ==============================================
	current_dims[0] = dims[dim];
	memspace_id = H5Screate_simple(1, current_dims, NULL);

	j=0;
	for (int i=0; i < ndims; i++)
	{
		if (i == dim)
		{
			offset[i] = 0;
			npoints = dims[i];
			count[i] = dims[i];
		} else {
			offset[i] = spec_dim[j];
			count[i] = 1;
			j++;
		}
	}

	readspace_id = H5Dget_space(dataset_id);
	status = H5Sselect_hyperslab(readspace_id, H5S_SELECT_SET, offset, NULL, count, NULL);

	buf = new double[npoints];
	out.resize(npoints);

	status = H5Dread(dataset_id, H5T_NATIVE_DOUBLE, memspace_id, readspace_id, H5P_DEFAULT, buf);

	for (int i=0; i < npoints; i++)
	{
		out[i] = buf[i];
	}

	H5Sclose(memspace_id);
	H5Sclose(readspace_id);
	delete[] count;
	delete[] offset;
	delete[] buf;
	delete[] dims;
	return out;
}

std::vector<double> DatasetOpen::getdata()
{
	std::vector<double> out;
	double *buf;
	int ndims;
	hsize_t *dims;
	hssize_t npoints;

	herr_t status;

	ndims = H5Sget_simple_extent_ndims(dataspace_id);
	dims = new hsize_t[ndims];

	H5Sget_simple_extent_dims(dataspace_id, dims, NULL);
	npoints = H5Sget_simple_extent_npoints(dataspace_id);

	buf = new double[npoints];
	out.resize(npoints);

	status = H5Dread(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, buf);

	for (int i=0; i < npoints; i++)
	{
		out[i] = buf[i];
	}

	delete[] buf;
	delete[] dims;
	return out;
}

// ==============================================
// DatasetAccess
// ==============================================
DatasetAccess::DatasetAccess(hid_t &loc_id, std::string dataset_str, int rank, hsize_t *dims) :
	Debug(DEBUG_FLAG),
	_dataset_str(dataset_str),
	_loc_id(loc_id)
{
	std::vector<int> size;
	size.resize(rank);
	for (int i=0; i < rank; i++)
	{
		size[i] = dims[i];
	}

	_init(size, H5T_NATIVE_DOUBLE);
}

DatasetAccess::DatasetAccess(hid_t &loc_id, std::string dataset_str, std::vector<int> size) :
	Debug(DEBUG_FLAG),
	_dataset_str(dataset_str),
	_loc_id(loc_id)
{
	_init(size, H5T_NATIVE_DOUBLE);
}

DatasetAccess::DatasetAccess(hid_t &loc_id, std::string dataset_str, std::vector<int> size, hid_t memtype_id) :
	Debug(DEBUG_FLAG),
	_dataset_str(dataset_str),
	_loc_id(loc_id)
{
	_init(size, H5T_NATIVE_DOUBLE);
}

int DatasetAccess::_init(std::vector<int> size, hid_t memtype_id)
{

	dataspace = new DataspaceCreate(size);
	dataspace_id = dataspace->dataspace_id;

	// ==============================================
	// Create dataset
	// ==============================================
	dataset_id = H5Dcreate(_loc_id, _dataset_str.c_str(), memtype_id, (*dataspace).dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

	return 0;
}

DatasetAccess::~DatasetAccess()
{
	close(&H5Dclose, dataset_id, _dataset_str);

	delete dataspace;
}

// ==============================================
// DataspaceCreate
// ==============================================
DataspaceCreate::DataspaceCreate(int rank, hsize_t *dims) :
	Debug(DEBUG_FLAG)
{
	dataspace_id = H5Screate_simple(rank, dims, NULL);
}

DataspaceCreate::DataspaceCreate(std::vector<int> size) :
	Debug(DEBUG_FLAG)
{
	int rank = size.size();
	hsize_t *dims;
	dims = new hsize_t[rank];

	for (int i=0; i < rank; i++)
	{
		dims[i] = size[i];
	}

	dataspace_id = H5Screate_simple(rank, dims, NULL);

	delete[] dims;
}

DataspaceCreate::DataspaceCreate(H5S_class_t type) :
	Debug(DEBUG_FLAG)
{
	dataspace_id = H5Screate(type);
}

DataspaceCreate::~DataspaceCreate()
{
	close(&H5Sclose, dataspace_id, "dataspace");
}

// ==============================================
// PlistCreate
// ==============================================
PlistCreate::PlistCreate(hid_t cls_id) :
	Debug(DEBUG_FLAG)
{
	_cls_id = cls_id;
	plist_id = H5Pcreate(cls_id);
}

PlistCreate::~PlistCreate()
{
	H5Pclose(plist_id);
}

// ==============================================
// AttributeOpen
// ==============================================
AttributeOpen::AttributeOpen(hid_t loc_id, std::string attr_name) :
	Debug(DEBUG_FLAG),
	_loc_id(loc_id)
{
	herr_t status;
	attr_id = H5Aopen(loc_id, attr_name.c_str(), H5P_DEFAULT);
}

AttributeOpen::~AttributeOpen()
{
	close(&H5Aclose, attr_id, _attr_name);
}

int AttributeOpen::read(void* buf)
{
	herr_t status;

	attr_type = H5Aget_type(attr_id);

	status = H5Aread(attr_id, attr_type, buf);

	close(&H5Tclose, attr_type, _attr_name + ":AttributeType");

	return status;
}

// ==============================================
// AttributeCreate
// ==============================================
AttributeCreate::~AttributeCreate()
{
	if (_init_success)
	{
		close(&H5Aclose, attr_id, _attr_name);
		delete _dataspace;
	}
}

// ==============================================
// FileOpen
// ==============================================
hid_t _fileopen(std::string filename, unsigned flags)
{
	return H5Fopen(filename.c_str(), flags, H5P_DEFAULT);
}

FileOpen::FileOpen(std::string filename) :
	Debug(DEBUG_FLAG)
{
	_filename = filename;
	file_id = _fileopen(filename, H5F_ACC_RDONLY);
}

FileOpen::FileOpen(std::string filename, unsigned flags) :
	Debug(DEBUG_FLAG)
{
	_filename = filename;
	file_id = _fileopen(filename, flags);
}

FileOpen::~FileOpen()
{
	close(&H5Fclose, file_id, _filename);
}

// ==============================================
// FileCreate
// ==============================================
hid_t _filecreate(std::string filename, unsigned flags)
{
	return H5Fcreate(filename.c_str(), flags, H5P_DEFAULT, H5P_DEFAULT);
}

FileCreate::FileCreate(std::string filename) :
	Debug(DEBUG_FLAG)
{
	_filename = filename;
	file_id = _filecreate(filename, H5F_ACC_TRUNC);
}

FileCreate::FileCreate(std::string filename, unsigned flags) :
	Debug(DEBUG_FLAG)
{
	_filename = filename;
	file_id = _filecreate(filename, flags);
}

FileCreate::~FileCreate()
{
	close(&H5Fclose, file_id, _filename);
}
