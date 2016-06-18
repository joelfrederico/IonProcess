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

std::vector<int> ionsim_process_electrons_phase(std::string filename, int bins, int xind, int yind, std::vector<double> range, bool userange)
{
	Hist2D *hist;
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

	if (userange)
	{
		hist = new Hist2D(x, y, bins, range);
	} else {
		hist = new Hist2D(x, y, bins);
	}

	return hist->dat;
}
