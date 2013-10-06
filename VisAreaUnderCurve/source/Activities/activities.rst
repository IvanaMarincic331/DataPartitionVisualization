**********
Activities
**********

In the following activities you will be asked to improve the *handle_rectangles()* function. For each activity it is recommended that you create separate versions of the function. 

Compiling of the Activities
***************************

Before you write your functions, there are several things you need to keep in mind about compiling. These instructions are valid only for MPI striping, and any form of hybrid srtiping. For OpenMP only striping, do not change the way you compile the program and do not add any additional header files. How to compile each version will be indicated in the activities.

In general, when compiling make sure you specify make [target] STRIPING=1. This is done so that you don’t have to remove all functions and header files related to X windowing.

Preparing the Code for Activities
*********************************

Make sure you download the :download:`MPEarea.h <MPEarea.h>` file and the :download:`MPEcolors.h <MPEcolors.h>` file and add them to the folder where the rest of your code is. Follow the steps below to include these files.

In **area.h**:

1. On top of the code add

	.. literalinclude:: area.h
		:language: c
		:lines: 15

2. In *area_under_curve()* after calling *setup_rectangles()* change the rest of the code to this:

	.. literalinclude:: area.h
		:language: c
		:lines: 95-130

3. Change the *finish()* function to this:

	.. literalinclude:: area.h
		:language: c
		:lines: 304-312

In **structs.h**:

1. On top of the code add

	.. literalinclude:: structs.h
		:language: c
		:lines: 12-15

2. Change the display struct to this:

	.. literalinclude:: structs.h
		:language: c
		:lines: 36-63


ACTIVITY 2 - Striping in MPI
############################

Write a function called handle_rectangles_MPIstriping(struct area_t * area) with a loop that will result in striping among the processes.
.. Tip:: Complete the function below. Think about what the for loop should be like and how that affects the *current_left* variable.

.. literalinclude:: area_MPIstriping.h
	:language: c
	:lines: 183-221

.. Compile using:: **make area.c-mpi STRIPING=1**.

If you get stuck download the solution: :download:`area_MPI_striping.tgz <area_MPI_striping.tgz>`.

ACTIVITY 3 - Striping in OpenMP
###############################

The MPE library is not relevant for this activity. You may go back to any original versions of the program to implement this activity. If you made the changes above to include MPE libraries and if you placed proper *#ifdef* conditionals as it was indicated in the steps above, then you can still run versions that do not require the use of MPE libraries. 

.. MPE/Xlib?:: Which graphics library you have to include depends on how you are using MPI. The rule is every time you need to do striping with MPI, inclue the MPE library. This includes striping using both MPI and OpenMP. The reason: when using MPE, only one display is initialized to which all processes write. When using Xlib each process has its own display.

Write a function called handle_rectangles_OpenMPstriping(struct area_t * area) with a loop that will result in striping among the threads within each process.
.. Tip:: Recall your knowledge from patternlets and that you can obtain a thread ID by calling **omp_get_thread_num()**. Make sure you call draw_rectangle() instead of draw_rectangle_mpe().    

.. Compile using:: **make area.c-openmp** and/or 
	**make area.c-mpi-openmp**

Notice that if you run the area.c-openmp executable, you will perform blocking with threads within a single process, and if you run the area.c-mpi-openmp exexcutable you will be performing blocking with threads within multiple processes. 

:Aside: The way striping is done in the patternlets as well as the way you did it in this acitivity is not the only way we can achieve this. We can dig deeper and investigate additional clauses OpenMP has to offer. Take the original handle_rectangles() function displayed below. Add **schedule(dynamic)** to the pragma clause, recompile and run the program and see what happens.More about schduling clauses can be found online_.

.. literalinclude:: area.h
		:language: c
		:lines: 227-268

.. _online: http://msdn.microsoft.com/en-us/library/9w1x7937.aspx

If you get stuck download the solution: :download:`area_OpenMP_striping.tgz <area_OpenMP_striping.tgz>`.
