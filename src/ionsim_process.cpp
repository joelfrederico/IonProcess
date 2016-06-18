#include "ionsim.h"
#include "ionsim_process.h"
#include "ionsim_electrons.h"
#include <iostream>
#include <vector>
#include <gflags/gflags.h>

DEFINE_string(file, "output.h5", "File to process.");

int writedata(FileCreate &file, std::string name, std::vector<int> hist, std::vector<int> size)
{
	herr_t status;
	DatasetAccess hist_dataset(file.file_id, name, size, H5T_NATIVE_INT);

	status = H5Dwrite(hist_dataset.dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, hist.data());

	return 0;
}

int main(int argc, char **argv)
{
	// ==============================================
	// Parse command line flags
	// ==============================================
	gflags::ParseCommandLineFlags(&argc, &argv, true);

	/* std::cout << "Argv: " << argv[0] << std::endl; */

	// ==============================================
	// Setup vars
	// ==============================================
	std::vector<int> hist;
	std::vector<double> range;
	int bins = 100;

	// ==============================================
	// Create output file
	// ==============================================
	FileCreate file(FLAGS_file + ".process.h5");
	FileOpen filein(FLAGS_file);
	AttributeOpen z_end(filein.file_id, "z_end");

	// ==============================================
	// Retrieve histogram
	// ==============================================
	std::vector<int> size = {bins, bins};

	hist = ionsim_process_electrons_phase(FLAGS_file, bins, 0, 2, range, false);
	writedata(file, "hist_xy", hist, size);

	range = {0, double(z_end.read())};
	IS_PRINT(range[1]);

	hist = ionsim_process_electrons_phase(FLAGS_file, bins, 4, 2, range, true);
	writedata(file, "hist_zy", hist, size);

	hist = ionsim_process_electrons_phase(FLAGS_file, bins, 4, 0, range, true);
	writedata(file, "hist_zx", hist, size);
	return 0;
}
