***************************
Resources and Customization
***************************

Resources
#########

For this module you are required to have the following:

* C compiler that includes OpenMP
* MPI compiler
* MPE graphics library
* X11/Xlib graphics library

Downloads
*********

Below are all the downloadable files from each activity. Most of them are solutions for the activities. Try the activities on your own first before you look at the solutions. **The solutions are not unique**. Therefore, it is important that you try to find your way first.

The original file: :download:`area <area.tgz>`

The global Makefile: :download:`Makefile <Makefile>`

Activity 0 - Load Balancing: :download:`area_load_balancing <area_load_balancing.tgz>`

Activity 1 - OpenMP Reduction: :download:`area_OpenMP_reduction <area_OpenMP_reduction.tgz>`

Activity 2 - OpenMP Striping: :download:`area_OpenMP_striping <area_OpenMP_striping.tgz>`

Activity 3 - MPI Striping: :download:`area_MPI_striping <area_MPI_striping.tgz>`

.. Setting up the Source Code:: Each of these downloads is a file that contains all the source code. Inside each folder there will be a Makefile. The single Makefile that is available for download separately is a different Makefile from each of the Makefiles inside each of the folders. The inside Makefiles refer to the outside Makefile. A recommendation would be to create a folder where you will be placing all the source code and the activities. Then place folders that correspond to the activities and the single Makefile inside that folder. The point is to have the Makefile outside of any folder with the source code.

Customization
#############

Makefiles
*********

For the source code to work for you. you may need to alter the single Makefile that is separately available for download above. 
Change the line below:

.. literalinclude:: Makefile
	:language: c
	:lines: 34

such that it contains the path to where your MPE libraries are stored.

Inside each of the folders above you will find different Makefiles, which are almost identical (they differ only in the amount of header files that are supposed to be included). These Makefiles look like this:

.. literalinclude:: Makefile2
	:language: c
	:lines: 1-20

Depending where you placed the outside Makefile you may need to change the very last line of the inside Makefile

Prerequisites
#############

In order to successfully complete this module, you should have some basic understanding of C code as well as know the basic concepts of MPI and OpenMP. In addition, you should also have an understanding of C preprocessor directives. Throughout the source code provided here you will be seeing a lot of *#ifdef* statements, so it is recommended that you understand the meaning of these conditionals. A list of these directives can be found here_. 

.. _here: http://msdn.microsoft.com/en-us/library/3sxhs2ty.aspx