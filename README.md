summer2013
==========

Summer Research Project with Elizabeth Shoop June-August 2013

The project I worked on this summer is a visualization of numerical integration, that shows several versions of the program:
serial, parallel (shared memory), distributed and heterogeneous (parallel & distributed) computing.
This program is meant is geared towards students who are new to parallel computing and towards faculty as a teaching
supplement in an introductory class for parallel computing.

VisAreaUnderCurve is the modules folder. The "Visualizing Area under the Curve" module is a sequence of chapters that
explain the problem, the source code and provide activites. Each subfolder in the VisAreaUnderCurve folder 
contains .rst files and sources such as images or source code. They can be compiled to produce web pages. The module is 
not yet available online.

The other folders are source code folders. These folders correspond to activies in the module. The "area" folder is the
starting point for students, where they see a so called "blocking" version of the computation. Other folders contain some 
changes to the area.h file (possibly include new header files) that result in an added feature.

Bugs: The program occasionally doesn't run properly when using OpenMP, depending on the type of hardware this program is
being executed on. Improvements to this program are still in progress.
