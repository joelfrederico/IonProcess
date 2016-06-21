#ifndef __IONSIM_PROCESS_CLASSES_H_INCLUDED__
#define __IONSIM_PROCESS_CLASSES_H_INCLUDED__

#include <vector>
#include <gsl/gsl_histogram2d.h>
#include <hdf5.h>

class Hist2D
{
	private:
		gsl_histogram2d * _h;
		double x_mag, y_mag, x_min, x_max, y_min, y_max, dat_size;
		int xbins, ybins, _nbins;
		std::vector<int> dat;

		int index(int i, int j);
		/* int _init(const std::vector<double> &x, const std::vector<double> &y, int bins); */
		int _init(const std::vector<double> &x, const std::vector<double> &y, const std::string xlabel, const std::string ylabel,int bins);
		int _copy(const Hist2D &hist);

	public:
		std::string xlabel_str, ylabel_str;

		Hist2D();
		Hist2D(const std::vector<double> &x, const std::vector<double> &y, const std::string xlabel, const std::string ylabel, const int bins);
		Hist2D(const std::vector<double> &x, const std::vector<double> &y, const std::string xlabel, const std::string ylabel, const int bins, const std::vector<double> range);
		Hist2D(const Hist2D &hist);
		~Hist2D();

		Hist2D& operator=(const Hist2D &rhs);

		int writedata(hid_t loc_id);
};

#endif
