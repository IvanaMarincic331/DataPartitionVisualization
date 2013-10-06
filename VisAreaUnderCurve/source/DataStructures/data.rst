***************
Data Structures
***************

The data structures can be found in the structs.h file. There are four **structs** called *area*, *curve*, *rect* and *display*. The mathematical functions are also declared in this file as well as some constants.

area struct
***********
* this is the main struct that holds general information as well as all other structs of the program

.. literalinclude:: structs.h
	:language: c
	:lines: 75-92

curve struct
************
* this struct holds all variables that are related to the domain of the curve, such as boundaries, width, etc.

.. literalinclude:: structs.h
	:language: c
	:lines: 63-73

rect struct
***********
* this struct holds all variables that describe and are related to a rectangle

.. literalinclude:: structs.h
	:language: c
	:lines: 54-61

display struct
**************
* this struct holds all variables that describe and are related to the display and graphics for Xlib and MPE. You don't have to worry about understanding these variables, since these are only responsible for the graphical output.

.. literalinclude:: structs.h
	:language: c
	:lines: 32-52

.. Note about colors:: In this struct you will notice a good amount of colors being declared. In this program a total of 25 colors are defined, which allows you to specifiy up to 25 processes or threads, or a combination of these such that the number of processes multiplied by the number of threads doesn't exceed 25. For demonstration purposes, you shouldn't be needing to specifiy a number larger than that. If you specify the -x 'no-display' option you don't have to pay attention to this limit. If, however, you do wish you have a larger number of colors available, there is a chapter at the end that explains how to add more colors.

MPI/OpenMP and structs
######################

In order to be able to do the activities it is important to understand how these structures are accessed by multiple processes and/or threads. 

When running the program in MPI each process has its own copy of each struct. They cannot override the information of each other unless some kind of communication is specified such as sending/receiving or broadcasting. In this program the only communication is happening using a function called MPI_Reduce, where the master process reduces the sums of all other processes to one variable. 

When running the program in parallel using OpenMP, by default these structures are shared among the threads. It is possible to specify a struct as shared or private in the *#pragma omp parallel private() shared()* clause. It is very important to understand which structs are supposed to be shared and which ones private.

When running the program in a hybrid way that combines MPI and OpenMP, each process has its own copy of all structures, but the threads within each process access and share that one copy by default.
