#include "ionsim_process_classes.h"
#include <gsl/gsl_histogram2d.h>
#include <algorithm>
#include <math.h>
#include <stdexcept>
#include <iostream>
#include "ionsim_hdf5.h"

const bool VERBOSE = true;

Hist2D::Hist2D() :
	xbins(-1),
	ybins(-1),
	_nbins(-1)
{
	dat.resize(1);
	dat[0] = -1;
	_h = gsl_histogram2d_alloc(xbins, ybins);
}

Hist2D::Hist2D(const std::vector<double> &x, const std::vector<double> &y, const std::string xlabel, const std::string ylabel, const int bins) :
	xbins(bins), ybins(bins), _nbins(bins*bins)
{
	// ==============================================
	// Find ranges
	// ==============================================
	x_mag = 0;
	for (double x_val: x)
	{
		x_mag = std::max(x_mag, fabs(x_val));
	}
	x_min = -x_mag;
	x_max = x_mag;

	y_mag = 0;
	for (double y_val: y)
	{
		y_mag = std::max(y_mag, fabs(y_val));
	}
	y_min = -y_mag;
	y_max = y_mag;

	_init(x, y, xlabel, ylabel, bins);
}

Hist2D::Hist2D(const std::vector<double> &x, const std::vector<double> &y, const std::string xlabel, const std::string ylabel, const int bins, const std::vector<double> range) :
	xbins(bins), ybins(bins), _nbins(bins*bins)
{
	x_min = range[0];
	x_max = range[1];

	y_mag = 0;
	for (double y_val: y)
	{
		y_mag = std::max(y_mag, fabs(y_val));
	}
	y_min = -y_mag;
	y_max = y_mag;

	_init(x, y, xlabel, ylabel, bins);
}

int Hist2D::_copy(const Hist2D &hist)
{
	// double x_mag, y_mag, x_min, x_max, y_min, y_max, dat_size;
	// int xbins, ybins, _nbins;
	x_min      = hist.x_min;
	x_max      = hist.x_max;
	x_mag      = hist.x_mag;
	y_min      = hist.y_min;
	y_max      = hist.y_max;
	y_mag      = hist.y_mag;
	dat_size   = hist.dat_size;
	xbins      = hist.xbins;
	ybins      = hist.ybins;
	_nbins     = hist._nbins;
	xlabel_str = hist.xlabel_str;
	ylabel_str = hist.ylabel_str;

	_h = gsl_histogram2d_clone(hist._h);
	dat = hist.dat;

	return 0;
}

Hist2D::Hist2D(const Hist2D &hist) :
	xbins(hist.xbins), ybins(hist.ybins), _nbins(hist._nbins)
{
	_copy(hist);
}

Hist2D& Hist2D::operator=(const Hist2D &rhs)
{
	if (this == &rhs)
	{
		return *this;
	}

	gsl_histogram2d_free(_h);

	_copy(rhs);
	return *this;
}

int Hist2D::_init(const std::vector<double> &x, const std::vector<double> &y, const std::string xlabel, const std::string ylabel,int bins)
{
	// ==============================================
	// Initialize stuff
	// ==============================================
	xlabel_str = xlabel;
	ylabel_str = ylabel;
	_h = gsl_histogram2d_alloc(xbins, ybins);
	dat.resize(_nbins);

	// ==============================================
	// Check sizes
	// ==============================================
	if (x.size() != y.size())
	{
		throw std::length_error("Vector sizes don't match");
	}

	// ==============================================
	// Do histogramming
	// ==============================================
	gsl_histogram2d_set_ranges_uniform(_h, x_min, x_max, y_min, y_max);

	for (int i=0; i < x.size(); i++)
	{
		if ((gsl_histogram2d_increment(_h, x[i], y[i]) != 0) && VERBOSE)
		{
			std::cerr << "----------------------------------------------" << std::endl;
			std::cerr << "Increment error: ";
			std::cerr << "x: " << x[i] << ", y: " << y[i] << std::endl;
			std::cerr << "X range: [" << x_min << ", " << x_max << "], ";
			std::cerr << "Y range: [" << y_min << ", " << y_max << "]" << std::endl;
			std::cerr << "----------------------------------------------" << std::endl;
		}
	}

	// ==============================================
	// Copy data to vector
	// ==============================================
	for (int i=0; i < _nbins; i++)
	{
		dat[i] = _h->bin[i];
	}

	return 0;
}

Hist2D::~Hist2D()
{
	gsl_histogram2d_free(_h);
}

int Hist2D::index(int i, int j)
{
	return i * ybins + j;
}

int Hist2D::writedata(hid_t loc_id)
{
	std::vector<int> size = {xbins, ybins};
	std::string name = "hist_" + xlabel_str + ylabel_str;
	herr_t status;
	DatasetAccess hist_dataset(loc_id, name, size, H5T_NATIVE_INT);

	status = H5Dwrite(hist_dataset.dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, dat.data());

	AttributeCreate x_min_attr(hist_dataset.dataset_id, "x_min", x_min);
	AttributeCreate x_max_attr(hist_dataset.dataset_id, "x_max", x_max);

	AttributeCreate y_min_attr(hist_dataset.dataset_id, "y_min", y_min);
	AttributeCreate y_max_attr(hist_dataset.dataset_id, "y_max", y_max);

	AttributeCreate xbins_attr(hist_dataset.dataset_id, "xbins", xbins);
	AttributeCreate ybins_attr(hist_dataset.dataset_id, "ybins", ybins);

	AttributeCreate xlabel(hist_dataset.dataset_id, "xlabel", xlabel_str.c_str());
	AttributeCreate ylabel(hist_dataset.dataset_id, "ylabel", ylabel_str.c_str());
	return 0;
}

