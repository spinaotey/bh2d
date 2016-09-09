# Barnes-Hut Algorithm in 2D

## Introduction

The Barnes-Hut algorithm is a clever scheme for grouping together bodies
that are sufficiently nearby to compute the gravitational force for all
of the particles of the system. It recursively divides the set of bodies into
groups by storing them in a quad-tree.

To calculate the net force on a particular body, traverse the nodes of the
tree, starting from the root. If the center-of-mass of an internal node is
sufficiently far from the body, approximate the bodies contained in that
part of the tree as a single body, whose position is the group’s center of
mass and whose mass is the group’s total mass. The algorithm is fast
because we don’t need to individually examine any of the bodies in thegroup.

This software computes the algorithm for a given system of 2-dimensional
particles introduced by the user in order to obtain the forces for all of them.

## Compilation

Just use the command `make`.

## Usage

By executing the program `./bh2d` without any additional arguments, 
we are shown that it is used as `./bh2d npar inputfile`.

The arguments are the following:
* `npart`: number of particles.
* `inputfile`: name of the file containing the initial state of the system.
  * The file has to contain at least `npart` rows, each of three columns, which
  correspond to the mass, the x-position and the y-position of the particles.
  Note that the positions have to be inside the initial quadtree square, which
  by default has the length of `1e22`. This can be changed in the `bh2d.c` file.
