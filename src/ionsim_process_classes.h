#ifndef __IONSIM_PROCESS_CLASSES_H_INCLUDED__
#define __IONSIM_PROCESS_CLASSES_H_INCLUDED__

#include <vector>
#include <gsl/gsl_histogram2d.h>

class Hist2D
{
	private:
		gsl_histogram2d * _h;
		double x_mag;
		double y_mag;
		double dat_size;
		int _bins;

		int index(int i, int j);

	public:
		Hist2D(const std::vector<double> &x, const std::vector<double> &y, int bins);
		~Hist2D();

		std::vector<int> dat;
		int data(int i, int j);
};

#endif
