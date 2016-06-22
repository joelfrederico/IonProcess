#include "ionsim_ion_xz_hist.h"
#include "ionsim_hdf5.h"
#include "ionsim_process_classes.h"
#include <cmath>

std::string _getion(unsigned int step)
{
	std::string out;
	char buf[10];
	sprintf(buf, "ions_%04u", step);
	out = buf;
	return out;
}

Hist2D ion_xz_hist(std::string filename, int xbins, int step)
{
	// ==============================
	// Initialize Vars 
	// ==============================
	double *pts;
	bool *pts_bool;

	double dx = 1e-5;
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

	std::vector<double> x, x_save, z, z_save;
	std::vector<int> dimselect(1);

	for (int i=0; i < n_field_z; i++)
	{
		dataset = new DatasetOpen(ionstep.group_id, _getion(0));
		dimselect[0] = 0;
		x = dataset->get_single(0, dimselect);
		dimselect[0] = 4;
		z = dataset->get_single(0, dimselect);
		for (int j=0; j < x.size(); j++)
		{
			if (std::abs(x[j]) <= dx)
			{
				x_save.push_back(x[j]);
				z_save.push_back(z[j]);
			}
		}
	}

	return Hist2D(x_save, z_save, "x", "z", xbins);
}
