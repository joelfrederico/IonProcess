#include "ionsim_electrons.h"
#include "ionsim.h"
#include "ionsim_process_classes.h"

std::vector<double> get_coords(FileOpen &file, int coord)
{
	GroupAccess step(file.file_id, "Step_0000");
	DatasetOpen electrons(step.group_id, "electrons");

	std::vector<int> spec_dim;
	spec_dim = {coord};
	return electrons.get_single(0, spec_dim);

}

Hist2D ionsim_process_electrons_phase(std::string filename, int bins, int xind, int yind, std::vector<double> range, bool userange)
{
	Hist2D hist;
	std::vector<double> x, y;
	std::vector<int>spec_dim;
	/* ============================================== */
	/* Open file */
	/* ============================================== */
	FileOpen filein(filename);

	/* ============================================== */
	/* Get electron data */
	/* ============================================== */
	x = get_coords(filein, xind);
	y = get_coords(filein, yind);

	std::vector<std::string> coord_name = {"x", "xp", "y", "yp", "z", "zp"};

	if (userange)
	{
		hist = Hist2D(x, y, coord_name[xind], coord_name[yind], bins, range);
	} else {
		hist = Hist2D(x, y, coord_name[xind], coord_name[yind], bins);
	}

	return hist;
}
