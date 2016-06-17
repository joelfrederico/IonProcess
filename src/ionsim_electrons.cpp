#include "ionsim_electrons.h"
#include "ionsim.h"
#include "ionsim_process_classes.h"

std::vector<int> ionsim_process_electrons_phase(std::string filename)
{
	std::vector<double> x, y;
	/* ============================================== */
	/* Open file */
	/* ============================================== */
	FileOpen filein(filename);

	/* ============================================== */
	/* Get electron data */
	/* ============================================== */
	GroupAccess step(filein.file_id, "Step_0000");
	DatasetOpen electrons(step.group_id, "electrons");

	std::vector<int> spec_dim(1, 0);
	x = electrons.get_single(0, spec_dim);
	spec_dim[0] = 2;
	y = electrons.get_single(0, spec_dim);

	Hist2D hist(x, y, 100);

	return hist.dat;
}
