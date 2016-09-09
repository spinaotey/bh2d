#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include "tree_lib.h"

/* BARNES-HUT ALGORITHM IN 2D
 *
 * The Barnes-Hut algorithm is a clever scheme for grouping together bodies
 * that are sufficiently nearby to compute the gravitational force for all
 * of the particles of the system. It recursively divides the set of bodies into
 * groups by storing them in a quad-tree.
 *
 * VARIABLES:
 *    -npar: number of particles.
 *    -inputfile: name of the inputfile that stores the data of the initial
 *                condition of the particles in three columns: mass xcoord ycoord.
 */

int main (int argc, char *argv[]) {

    /* VARIABLES */
    uint64_t npar,i;
    double **F;
    FILE *finput, *foutput;
    particle_t *particle;
    quadnode_t *quadnode;

    /* INPUT */
    if (argc < 3
       || sscanf(argv[1],"%"SCNi64,  &npar)!=1
       ) {
           fprintf(stderr,"%s  npar inputfile\n",argv[0]);
           return FAILURE;
    }

    
    /* ALLOC MEMORY */
    particle = (particle_t *) malloc(sizeof(particle_t)*npar); assert(particle);
    F = (double **) malloc(sizeof(double *)*npar); assert(F);
    quadnode = (quadnode_t *) malloc(sizeof(quadnode_t));

    /* Initialize first node */
    quadnode->n=0;
    quadnode->r[0]=0;
    quadnode->r[1]=0;
    quadnode->d = 1e22;


    /* Read and put particles into the tree */
    finput = fopen(argv[2],"r");
    
    for(i=0;i<npar;i++){
        F[i] = (double *) calloc(sizeof(double),2); 
        fscanf(finput,"%lf %lf %lf",&particle[i].m, &particle[i].r[0], &particle[i].r[1]);
        quadnode_insert(quadnode,particle[i]);
    }
    fclose(finput);
    

    /* Clean tree and compute super particles */
    quadnode_clean(quadnode);
    quadnode_compMass(quadnode);

    /* Compute and print forces */
    foutput = fopen("Falg.dat","w");
    for(i=0;i<npar;i++){
        quadnode_force(quadnode,particle[i],F[i]);
        fprintf(finput,"%6.4e %6.4e\n",F[i][0],F[i][1]);
    }
    fclose(foutput);

    /* Free memory */
    quadnode_free(quadnode);

    for(i=0;i<npar;i++)
        free(F[i]);
    free(F);
    free(particle);

    return SUCCESS;
}


