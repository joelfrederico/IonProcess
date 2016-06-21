#ifndef __IONSIM_ELECTRONS_H_INCLUDED__
#define __IONSIM_ELECTRONS_H_INCLUDED__

#include <iostream>
#include <vector>
#include "ionsim_process_classes.h"

Hist2D ionsim_process_electrons_phase(std::string filename, int bins, int xind, int yind, std::vector<double> range, bool userange);

#endif
