*********************************
Main Computation and Finalization
*********************************

The main computation takes place in two functions: **handle_rectangles()** and **sum_rectangles()**.

handle_rectangles()
###################

This function is located in area.h, and it is responsible for calculating the *current_left* and the *current_height* variable, computing the area, incrementing a sum variable by this area and drawing the rectangle. The function contains only one for loop in which we loop through the private number of rectangles. 

.. literalinclude:: area.h
	:language: c
	:lines: 161-198

The area of each rectangle is stored in the *my_sum* variable, which is a private sum that each process keeps track of.

:Comment: 
	If the computation is to be displayed the proper scaling is applied to the circle and sine function and the *draw_rectangle()* (located in Xarea.h) is called. 

When using MPI, we can see that blocking will be performed in this for loop since we are looping through each process' private number of rectangles. If OpenMP is used we parallelize the entire for loop using the #pragma omp parallel for clause. By default this performs blocking among the threads.

:Explore:
	Execute the area.c-openmp or area.c-mpi-openmp multiple times with different numbers of rectangles. Are there any anomalies in the blocking among the threads? If so, can you explain what is happening?

sum_rectangles()
################

This function is located in area.h, and it is rather simple. It shows, however, how the *my_sum* variable of each process is reduced to a variable called *total_sum*. The rest of the function is a series of print statements that print out the result.

.. literalinclude:: area.h
	:language: c
	:lines: 201-213


Finalization
############

The finalization of the program is very short. It consists of the function *finalize()* being called in main. This function is located in area.h, it calls MPI_FInalize() and closes all X windows.

.. literalinclude:: area.h
	:language: c
	:lines: 234-246


ACTIVITY 1 - Reduction of Sum
#############################

In the code excerpt above we can see how the sum is computed using MPI_Reduction. This means that the main process of rank 0 will sum up all the partial sums from the other nodes.

In case of a heterogeneous computing or OpenMP only version of this code each thread computes its partial sum and updates the my_sum variable in the main struct area. There is another way to make the summation more efficient avoiding possible race conditions when threads are involved. We can use OpenMP to perform a reduction on the partial sums of each thread. In case of a heterogeneous version, these partial sums will be then further reduced using MPI.

**Your task:** change the handle_rectangles() function to perform OpenMP reduction. You do not have to change the lines following the if statement.
**Tip:** OpenMP will not allow you to reduce directly on *area->my_sum*, however you need to update this variable in some way since it is used in sum_rectangles().

The solution to this problem can be obtained by downloading :download:`area_OpenMP_reduction.tgz <area_OpenMP_reduction.tgz>`.
