###############################################################################
# Makefile for the modules included in ~bccd on the BCCD (http://bccd.net)
# 
# Add NO_X11=1 to the make command line to omit X libraries.
# Add STRIPING=1 to include the mpe libraries
###############################################################################

# Test for Intel compilers, act accordingly
ifeq ($(findstring intel,$(LOADEDMODULES)),intel)
	CC = icc
	OMPFLAGS = -openmp -DOMP
else
	CC = gcc
	OMPFLAGS = -fopenmp -DOMP
endif

MPICC = mpicc
MPICXX = mpicxx
NVCC = nvcc

CFLAGS = -O2
NVCCFLAGS = --compiler-bindir=gcc
NVMPICCFLAGS = --compiler-bindir=mpicc
MPIFLAGS = -D_MPI

########for Area-under-curve visualization###############
ifdef STRIPING
	NO_X11 = 1
	CFLAGS += -DSTRIPING
	LIBS += -lmpe -lX11
	LDFLAGS += -L/usr/X11R6/lib
	INCFLAGS += -I/bccd/software/openmpi/1.4.5/Linux/BCCD/x86_64/include
endif
#########################################################

ifdef NO_X11
	CFLAGS += -DNO_X11
else
	LIBS += -lX11
	LDFLAGS += -L/usr/X11R6/lib
endif

ifdef DEBUG
	CFLAGS += -ggdb
endif

ifdef PROFILE
	CFLAGS += -pg
	LDFLAGS += -pg
endif


LIBS += -lm
LDFLAGS += $(LIBS)
	
$(PROGRAM).c-serial:
	$(CC) $(CSRCS) $(EXTRA_COBJS) $(CFLAGS) $(INCFLAGS) $(LDFLAGS) -o $@

$(PROGRAM).c-mpi:
	$(MPICC) $(CSRCS) $(EXTRA_COBJS) $(CFLAGS) $(INCFLAGS) $(MPIFLAGS) $(LDFLAGS) -o $@

$(PROGRAM).cxx-mpi:
	$(MPICXX) $(CXXSRCS) $(EXTRA_CXXOBJS) $(CFLAGS) $(INCFLAGS) $(MPIFLAGS) $(LDFLAGS) -o $@

$(PROGRAM).c-openmp:
	$(CC) $(CSRCS) $(EXTRA_COBJS) $(CFLAGS) $(INCFLAGS) $(OMPFLAGS) $(LDFLAGS) -o $@

$(PROGRAM).c-mpi-openmp:
	$(MPICC) $(CSRCS) $(EXTRA_COBJS) $(CFLAGS) $(INCFLAGS) $(OMPFLAGS) $(MPIFLAGS) $(LDFLAGS) -o $@

$(PROGRAM).c-cuda: 
	$(NVCC) $(CUDASRCS) $(EXTRA_COBJS) $(CFLAGS) $(INCFLAGS) $(NVCCFLAGS) $(LDFLAGS) -o $@

help:

	@echo "The following options are supported by most modules:"
	@echo "  NO_X11=1 (do not include an X11 GUI component)"
	@echo "  STAT_KIT=1 (enables the StatKit performance collector tool)"
	@echo "  STRIPING=1 (enables striping in Area-under-curve)"
	@echo "You can also cd to a module's directory and type make (options optional)."

build-all:
	@echo "-------------------------------------"
	cd CUDA; $(MAKE) 
	@echo "-------------------------------------"
	cd Life; $(MAKE)
	@echo "-------------------------------------"
	cd Area-under-curve; $(MAKE)
	@echo "-------------------------------------"
	cd Pandemic; $(MAKE)
	@echo "-------------------------------------"


clean-all:
	@echo "-------------------------------------"
	cd CUDA; $(MAKE) clean
	@echo "-------------------------------------"
	cd Life; $(MAKE) clean
	@echo "-------------------------------------"
	cd Area-under-curve; $(MAKE) clean
	@echo "-------------------------------------"
	cd Pandemic; $(MAKE) clean
	@echo "-------------------------------------"
	cd StatKit/petakit; $(MAKE) clean
	@echo "-------------------------------------"
	
clean:
	/bin/rm -f $(CLEAN)
