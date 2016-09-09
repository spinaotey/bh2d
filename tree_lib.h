#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <inttypes.h>

#ifndef DEFINE_H_INCLUDED
#define DEFINE_H_INCLUDED

#define SUCCESS 1
#define FAILURE 0 

#define POW2(x) ((x)*(x)) // Computes the square of a number
#define POW3(x) ((x)*(x)*(x)) // Computes the square of a number
#define POW4(x) ((x)*(x)*(x)*(x)) // Computes the square of a number
#define POW5(x) ((x)*(x)*(x)*(x)*(x) // Computes the square of a number



#define G 6.67408e-11
#define M_s 1.98855e30

#define Theta 0.9

#endif

typedef struct {
    double m;
    double r[2];
} particle_t;

typedef struct node{
    uint64_t n;             // number of particles contained in the node
    particle_t particle;    // mass and center of mass of the node
    double r[2];           // location of the node in space
    double d;               // lenght of each side in space
    struct node *child[4];      // vectorpointer to four children of the node
} quadnode_t;   


void quadnode_createChild(quadnode_t *quadnode, int c);


int quadnode_whichChild(quadnode_t *quadnode, particle_t particle);


void quadnode_insert(quadnode_t *quadnode, particle_t particle);



void quadnode_clean(quadnode_t *quadnode);



void quadnode_compMass(quadnode_t *quadnode);

    
    
void force(particle_t p1, particle_t p2, double *F);



void quadnode_force(quadnode_t *quadnode, particle_t particle, double *F);



void quadnode_free(quadnode_t *quadnode);
