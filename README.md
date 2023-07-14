# Steady-State-Temperature
Compute the steady-state temperature of a two-dimensional homogenous plate by randomly walking on the plate

## Problem Statement
*Imagine a very thin plate of homogeneous material. Compute the steady-state
temperature at a particular point in the plate. The top and the bottom of the
plate are insulated, and the temperature at any point is solely determined by
the temperatures surrounding it, except for the temperatures at the edges of the
plate, which are fixed.* [[1](#1)]

### What is a steady-state temperature?
Imagine that an object has heat applied to it at various contact points, and then removed. Over time, the heat is transferred to other parts of the object. In the beginning, at each point in the object, the temperature at that point is in flux; it is changing as a function of time. Eventually, though, the temperature within the object becomes independent of time; it reaches a state where, for each point in the object, the temperature at that point is independent of time. When this has happened, the object has reached a **steady-state temperature** distribution. [[2](#2)]

## Solution Design
* The problem is an application of the general Dirichlet's problem, solving which requires us to find the solution to Laplace's equation.
* We aim for a numerical solution because an analytical solution may not exist.
* We solve the discrete version of the problem (because it's computationally faster) using finite difference methods.
* The solution boils down to observing that the **temperature at each point can be computed as the average of its four (north, east, south, west) neighbours**.
* Iteratively compute this average until the temperatures converge, i.e. the difference in temperatures at any given point is below a threshold.

### Define a random walk.
A **random walk** is a chain that is both time-homogeneous and space-homogeneous. [[2](#2)]

## References
<a id="1">[1]</a> M.J. Quinn. *Parallel Programming in C with MPI and OpenMP*. McGraw-Hill Higher Education, 2004.

<a id="2">[2]</a> Prof. Stewart Weiss. *CSci 493.65 Parallel Computing*, [Chapter 10: Monte Carlo Methods](http://www.compsci.hunter.cuny.edu/~sweiss/course_materials/csci493.65/lecture_notes/chapter08.pdf). Hunter College, 2019.
