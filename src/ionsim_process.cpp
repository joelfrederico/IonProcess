#include "ionsim.h"
#include <iostream>
#include <vector>
#include <gflags/gflags.h>

DEFINE_bool(verbose, false, "Verbose mode");
DEFINE_double(slicewidth, 2e-6, "Slice width in y");

void status(std::string msg)
{
	std::cout << "----------------------------------------------" << std::endl;
	std::cout << msg << std::endl;
	std::cout << "----------------------------------------------" << std::endl;
}

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
	std::vector<double> range(2);
	double z_end;
	int bins = 101;
	bool userange;

	// ==============================================
	// Create output file
	// ==============================================
	status("Creating output files...");
	FileCreate file(filename + ".process.h5");
	FileOpen filein(filename);
	AttributeOpen z_end_attr(filein.file_id, "z_end");
	z_end_attr.read(&z_end);
	GroupAccess electron_group(file.file_id, "electrons");
	GroupAccess ion_group(file.file_id, "ions");

	// ==============================================
	// Phase space histogram
	// ==============================================
	status("Processing phase space histograms...");
	range[0] = 0;
	range[1] = z_end;

	std::vector<std::string> coord_name(6);
	coord_name[0] = "x";
	coord_name[1] = "xp";
	coord_name[2] = "y";
	coord_name[3] = "yp";
	coord_name[4] = "z";
	coord_name[5] = "zp";

	for (int i=0; i < 5; i+=2)
	{
		std::cout << coord_name[i] << " plane..." << std::endl;
		if (i == 4)
		{
			userange = true;
		} else {
			userange = false;
		}

		hist = ionsim_process_electrons_phase(filename, bins, i, i+1, range, userange);
		hist.writedata(electron_group.group_id);
	}

	// ==============================================
	// Config space histograms
	// ==============================================
	status("Processing config space histograms...");
	// X-Y Histogram
	hist = ionsim_process_electrons_phase(filename, bins, 0, 2, range, false);
	hist.writedata(electron_group.group_id);

	// Z-Y Histogram
	hist = ionsim_process_electrons_phase(filename, bins, 4, 2, range, true);
	hist.writedata(electron_group.group_id);

	// Z-X Histogram
	hist = ionsim_process_electrons_phase(filename, bins, 4, 0, range, true);
	hist.writedata(electron_group.group_id);

	// ==============================================
	// Ion histograms
	// ==============================================
	status("Processing ion slice histogram...");
	hist = ion_xz_hist(filename, bins, 0, FLAGS_slicewidth);
	hist.writedata(ion_group.group_id);
	return 0;
}
