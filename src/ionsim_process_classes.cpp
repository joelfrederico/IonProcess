#include "ionsim_process_classes.h"
#include <gsl/gsl_histogram2d.h>
#include <algorithm>
#include <math.h>
#include <stdexcept>
#include <iostream>
#include "ionsim.h"


Hist2D::Hist2D(const std::vector<double> &x, const std::vector<double> &y, int bins) :
	xbins(bins), ybins(bins), _nbins(bins*bins)
{
	/* ============================================== */
	/* Find ranges                                    */
	/* ============================================== */
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

	_init(x, y, bins);
}

Hist2D::Hist2D(const std::vector<double> &x, const std::vector<double> &y, int bins, std::vector<double> range) :
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

	_init(x, y, bins);
}

int Hist2D::_init(const std::vector<double> &x, const std::vector<double> &y, int bins)
{
	/* ============================================== */
	/* Initialize stuff */
	/* ============================================== */
	_h = gsl_histogram2d_alloc(xbins, ybins);
	dat.resize(_nbins);

	/* ============================================== */
	/* Check sizes */
	/* ============================================== */
	if (x.size() != y.size())
	{
		throw std::length_error("Vector sizes don't match");
	}

	IS_PRINT(x_min);
	IS_PRINT(x_max);

	/* ============================================== */
	/* Do histogramming */
	/* ============================================== */
	gsl_histogram2d_set_ranges_uniform(_h, x_min, x_max, y_min, y_max);

	for (int i=0; i < x.size(); i++)
	{
		if (gsl_histogram2d_increment(_h, x[i], y[i]) != 0)
		{
			std::cerr << "Increment error" << std::endl;
			std::cerr << "x: " << x[i] << ", y: " << y[i] << std::endl;
		}
	}

	/* ============================================== */
	/* Copy data to vector */
	/* ============================================== */
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
