#include "ionsim_ion_xz_hist.h"
#include "ionsim_hdf5.h"
#include "ionsim_process_classes.h"
#include "ionsim.h"
#include <cmath>

std::string _getion(unsigned int step)
{
	std::string out;
	char buf[10];
	sprintf(buf, "ions_%04u", step);
	out = buf;
	return out;
}

Hist2D ion_xz_hist(std::string filename, int xbins, int step, double slicewidth)
{
	// ==============================
	// Initialize Vars 
	// ==============================
	double *pts;
	bool *pts_bool;

	double delx;

	int ind;
	int n_filtered;
	double z_end;
	double x_abs_max;
	double *count, *z_array;
	int n_field_z;

	DatasetOpen *dataset;

	// ==============================
	// Open files for reading/writing
	// ==============================
	FileOpen data(filename);

	// ==============================
	// Find ions and number of them
	// ==============================
	GroupStepAccess stepgroup(data.file_id, step);
	GroupAccess ionstep(stepgroup.group_id, "ions_steps");

	AttributeOpen n_field_z_attr(data.file_id, "n_field_z");
	n_field_z_attr.read(&n_field_z);
	AttributeOpen z_end_attr(data.file_id, "z_end");
	z_end_attr.read(&z_end);

	std::vector<double> x, x_save, y, z, z_save;
	std::vector<int> dimselect(1);

	std::cout << "Processing steps..." << std::endl;
	for (int i=0; i < n_field_z+1; i++)
	{
		std::cout << "\rStep: " << i << std::flush;
		dataset = new DatasetOpen(ionstep.group_id, _getion(i));
		dimselect[0] = 0;
		x = dataset->get_single(0, dimselect);
		dimselect[0] = 2;
		y = dataset->get_single(0, dimselect);
		dimselect[0] = 4;
		z = dataset->get_single(0, dimselect);
		delete dataset;
		for (int j=0; j < x.size(); j++)
		{
			if (std::abs(y[j]) <= slicewidth)
			{
				x_save.push_back(x[j]);
				/* z_save.push_back(z[j]); */
				/* z_save.push_back(z_end * i / n_field_z); */
				z_save.push_back(i);
			}
		}
	}
	std::cout << "\r" << std::flush;

	IS_PRINT(z_save.size());
	std::vector<double> range(2);
	range[0] = 0;
	range[1] = n_field_z;

	return Hist2D(z_save, x_save, "z", "x", n_field_z+1, xbins, range);
}
