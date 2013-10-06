********************************
Additional Exercise - Finding Pi
********************************

The area under a quarter circle is :math:`1/4 * pi`. Obviously, one could never get exactly pi, since the digits of pi are infinite. One thing we could do, however, is to explore how many digits of pi can we get using our program. Approximating the area under the quarter circle using a large number of rectangles should give us a result that approximates pi. 

**Your task**: Run any of the versions of this program and see how many digits of pi can you get.

Don't forget to specifiy the "-f 2" as well as "-x no-display" options, since you won't be needing graphical output. Set the number of rectangles using the "-n" option. As you increase n, is your result more accurate?

Do the same thing for different executables, choose some versions between serial, OpenMP, MPI or OpenMP+MPI. See whether there is a difference between striping and blocking.

How many digits of pi can you get? 

Are there any differences between the different levels of parallel computing for this problem? 