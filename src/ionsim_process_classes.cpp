#include "ionsim_process_classes.h"
#include <gsl/gsl_histogram2d.h>
#include <algorithm>
#include <math.h>
#include <stdexcept>

Hist2D::Hist2D(const std::vector<double> &x, const std::vector<double> &y, int bins)
{
	/* ============================================== */
	/* Initialize stuff */
	/* ============================================== */
	_bins = bins;
	_h = gsl_histogram2d_alloc(bins, bins);
	dat_size=bins*bins;
	dat.resize(dat_size);

	/* ============================================== */
	/* Check sizes */
	/* ============================================== */
	if (x.size() != y.size())
	{
		throw std::length_error("Vector sizes don't match");
	}

	/* ============================================== */
	/* Find ranges                                    */
	/* ============================================== */
	x_mag = 0;
	for (double x_val: x)
	{
		x_mag = std::max(x_mag, fabs(x_val));
	}

	y_mag = 0;
	for (double y_val: y)
	{
		y_mag = std::max(y_mag, fabs(y_val));
	}

	/* ============================================== */
	/* Do histogramming */
	/* ============================================== */
	gsl_histogram2d_set_ranges_uniform(_h, -x_mag, x_mag, -y_mag, y_mag);

	for (int i=0; i < x.size(); i++)
	{
		gsl_histogram2d_increment(_h, x[i], y[i]);
	}

	/* ============================================== */
	/* Copy data to vector */
	/* ============================================== */
	for (int i=0; i < dat_size; i++)
	{
		dat[i] = _h->bin[i];
	}
}

Hist2D::~Hist2D()
{
	gsl_histogram2d_free(_h);
}

int Hist2D::index(int i, int j)
{
	return i * _bins + j;
}
