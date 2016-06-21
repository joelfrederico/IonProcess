#include "ionsim.h"
#include "ionsim_process.h"
#include <iostream>
#include <vector>
#include <gflags/gflags.h>

DEFINE_bool(verbose, false, "Verbose mode");

int main(int argc, char **argv)
{
	// ==============================================
	// Parse command line flags
	// ==============================================
	gflags::ParseCommandLineFlags(&argc, &argv, true);

	if (argc < 2)
	{
		std::cerr << "No file given: aborting" << std::endl;
		return -1;
	}

	std::string filename = argv[1];
	std::cout << "==============================================" << std::endl;
	std::cout << "Opening file: " << filename << std::endl;
	std::cout << "==============================================" << std::endl;

	// ==============================================
	// Setup vars
	// ==============================================
	Hist2D hist;
	std::vector<double> range;
	double z_end;
	int bins = 100;

	// ==============================================
	// Create output file
	// ==============================================
	FileCreate file(filename + ".process.h5");
	FileOpen filein(filename);
	AttributeOpen z_end_attr(filein.file_id, "z_end");
	z_end_attr.read(&z_end);

	// ==============================================
	// Phase space histogram
	// ==============================================
	range = {0, z_end};

	std::vector<std::string> coord_name = {"x", "xp", "y", "yp", "z", "zp"};
	bool userange;

	for (int i=0; i < 5; i+=2)
	{
		if (i == 4)
		{
			userange = true;
		} else {
			userange = false;
		}

		hist = ionsim_process_electrons_phase(filename, bins, i, i+1, range, userange);
		hist.writedata(file.file_id);
	}

	// ==============================================
	// Config space histograms
	// ==============================================
	// X-Y Histogram
	hist = ionsim_process_electrons_phase(filename, bins, 0, 2, range, false);
	hist.writedata(file.file_id);


	// Z-Y Histogram
	hist = ionsim_process_electrons_phase(filename, bins, 4, 2, range, true);
	hist.writedata(file.file_id);

	// Z-X Histogram
	hist = ionsim_process_electrons_phase(filename, bins, 4, 0, range, true);
	hist.writedata(file.file_id);

	// ==============================================
	// Ion histograms
	// ==============================================
	
	return 0;
}
