#ifndef __IONSIM_PROCESS_CLASSES_H_INCLUDED__
#define __IONSIM_PROCESS_CLASSES_H_INCLUDED__

#include <vector>
#include <gsl/gsl_histogram2d.h>

class Hist2D
{
	private:
		gsl_histogram2d * _h;
		double x_mag, y_mag;
		double x_min, x_max, y_min, y_max;
		double dat_size;
		const int _nbins;

		int index(int i, int j);
		int _init(const std::vector<double> &x, const std::vector<double> &y, int bins);

	public:
		Hist2D(const std::vector<double> &x, const std::vector<double> &y, int bins);
		Hist2D(const std::vector<double> &x, const std::vector<double> &y, int bins, std::vector<double> range);
		~Hist2D();

		const int xbins, ybins;

		std::vector<int> dat;
		int data(int i, int j);
};

#endif
