************
Introduction
************

**Parallel computing** takes advantage of multi-core CPUs by performing many computations simultaneously on the cores of a CPU using sequences of execution of a program called *threads*. Writing a parallel program is more complicated than writing serial programs, because there are many software bugs and obstacles that come with this kind of computing. 

When connecting many computers via an internal network, we obtain what is known as a **cluster**. Clusters are composed of individual machines able to independently perform a task. They are connected through a TCP/IP Ethernet local area network, which enables them to communicate with each other. A form of computation that requires data to split among the processing units of a cluster is referred to as **distributed processing**.

It is possible to combine various software (and hardware) paradigms to perform so called **heterogenous computing**. Heterogeneous computing typically involves splitting the computation among several processing units (distributed processing) and further splitting it amongst the cores within each node’s CPU. 

You will be exploring these three techniques by solving the problem of finding the area under the curve. You will be investigating a couple of methods for decomposing the problem size and how you can use parallel computing and/or distributed processing to take advantage of these methods. To help you understand these methods you will be provided with a visual version of this program that illustrates how the program works. Before you begin, make sure you have the necessary software in order to run the program yourself and do the activities. The next several chapters will give you some basic theoretical background for solving the area under the curve problem. An explanation of the source code will be provided after that, followed by the activites.