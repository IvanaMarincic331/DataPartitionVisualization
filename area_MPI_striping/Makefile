PROGRAM = area
CSRCS = area.c
INCS = area.h Xarea.h structs.h colors.h MPEarea.h MPEcolors.h
CLEAN = $(PROGRAM).c-serial $(PROGRAM).c-mpi $(PROGRAM).c-openmp $(PROGRAM).c-mpi-openmp

all: c-serial c-mpi c-openmp c-mpi-openmp

c-serial: $(CSRCS) $(INCS) $(PROGRAM).c-serial
$(PROGRAM).c-serial: $(CSRCS) $(INCS) 

c-mpi: $(CSRCS) $(INCS) $(PROGRAM).c-mpi
$(PROGRAM).c-mpi: $(CSRCS) $(INCS) 

c-openmp: $(CSRCS) $(INCS) $(PROGRAM).c-openmp
$(PROGRAM).c-openmp: $(CSRCS) $(INCS) 

c-mpi-openmp: $(CSRCS) $(INCS) $(PROGRAM).c-mpi-openmp
$(PROGRAM).c-mpi-openmp: $(CSRCS) $(INCS) 

include ../../Makefile