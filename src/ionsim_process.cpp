#include "ionsim.h"
#include "ionsim_process.h"
#include "ionsim_electrons.h"
#include <iostream>
#include <gflags/gflags.h>

DEFINE_string(file, "output.h5", "File to process.");

int main(int argc, char **argv)
{
	gflags::ParseCommandLineFlags(&argc, &argv, true);

	ionsim_process_electrons_phase(FLAGS_file);

	return 0;
}
