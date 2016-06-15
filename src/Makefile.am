# AM_CPPFLAGS=$(GSL_CFLAGS) $(HDF5_CPPFLAGS)
AM_CPPFLAGS=$(MYAM_CPPFLAGS)

lib_LTLIBRARIES = libionsim.la

include_HEADERS = ionsim.h ionsim_hdf5.h ionsim_process.h

libionsim_la_SOURCES=\
			 ionsim_process.cpp \
			 ionsim_hdf5.cpp
