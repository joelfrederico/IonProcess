#ifndef __IONSIM_ION_XZ_HIST_H_INCLUDED__
#define __IONSIM_ION_XZ_HIST_H_INCLUDED__

#include <string>
#include <vector>

std::string _getion(unsigned int step);
int index(int i, int j, int ncols);
int index(int i, int j, int k, int ncols, int nrows);
int ion_xz_hist(std::string filename, int xbins, int step, std::vector<unsigned long long> &hist, long &histsize, int &n_field_z);
int dealloc_hist(unsigned long long *&hist);
int get_n_field_z(std::string filename, int step);

#endif
