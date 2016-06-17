# AM_CPPFLAGS=$(GSL_CFLAGS) $(HDF5_CPPFLAGS)
AM_CPPFLAGS=$(MYAM_CPPFLAGS)

bin_PROGRAMS=ionprocess

ionprocess_SOURCES=\
		   ionsim_process.cpp \
		   ionsim_process_classes.cpp

ionprocess_LDADD=$(MYLDADD) $(GSL_LIBS) libionsim.la
ionprocess_LDFLAGS=$(GSL_CFLAGS)

lib_LTLIBRARIES=libionsim.la

include_HEADERS=\
		ionsim.h \
		ionsim_electrons.h \
		ionsim_hdf5.h \
		ionsim_ion_xz_hist.h

libionsim_la_SOURCES=\
			 ionsim_electrons.cpp \
			 ionsim_hdf5.cpp \
			 ionsim_ion_xz_hist.cpp

libionsim_la_LIBADD=$(HDF5_LIBS)
libionsim_la_LDFLAGS=$(HDF5_LDFLAGS)
