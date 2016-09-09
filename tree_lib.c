#include "tree_lib.h"
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <inttypes.h>


/* Create Children c in quadnode with its corresponding attributes*/
void quadnode_createChild(quadnode_t *quadnode, int c){
    int i;
    /* Allocate memory */
    quadnode->child[c] = (quadnode_t *) malloc(sizeof(quadnode_t));
    
    /* Initialize data */
    quadnode->child[c]->d = quadnode->d/2;
    
    quadnode->child[c]->r[0] = quadnode->r[0]+(c%2)*quadnode->d/2;
    quadnode->child[c]->r[1] = quadnode->r[1]+(-(c/2)+1)*quadnode->d/2;
    
    quadnode->child[c]->n = 0;
    /* Set Children pointer NULL */
    for(i=0;i<4;i++)
        quadnode->child[c]->child[i] = NULL;
}



/*        
 *          -----
 *          |0|1|  
 *          |---|  
 *          |2|3|
 *          -----
 *         /
 *   quadnode.r
 */

/* Given a particle structure, indicates in which quadrant it is inside a node */
int quadnode_whichChild(quadnode_t *quadnode, particle_t particle){
    return (  (particle.r[0]>=(quadnode->r[0]+quadnode->d/2))+
            2*(particle.r[1]<(quadnode->r[1]+quadnode->d/2))  );
}

/* Insert particle into node */
void quadnode_insert(quadnode_t *quadnode, particle_t particle){
    int c;
    /* If we have more than one particle in node, go deeper */
    if(quadnode->n > 1){
        c = quadnode_whichChild(quadnode,particle);
        quadnode_insert(quadnode->child[c],particle);
    }
    /* If we have one, create children and move the previous particle */
    else if(quadnode->n == 1){
        for (c=0;c<4;c++)
            quadnode_createChild(quadnode,c);
        c = quadnode_whichChild(quadnode,quadnode->particle);
        quadnode_insert(quadnode->child[c],quadnode->particle);
        c = quadnode_whichChild(quadnode,particle);
        quadnode_insert(quadnode->child[c],particle);
    }
    /* If we have zero, put particle inside and give parameters to node*/
    else{
        quadnode->particle.m = particle.m;
        quadnode->particle.r[0] = particle.r[0];
        quadnode->particle.r[1] = particle.r[1];
    }
    /* We have one more particle in the node */
    quadnode->n++;
}

/* Get rid of empty nodes with zero particles */
void quadnode_clean(quadnode_t *quadnode){
    int i;
    for(i=0;i<4;i++){
        if(quadnode->child[i]->n == 0){
            free(quadnode->child[i]);
            quadnode->child[i] = NULL;
        }
        else if(quadnode->child[i]->n > 1)
            quadnode_clean(quadnode->child[i]);
    }
}


/* Compute mass and center of mass in each node */
void quadnode_compMass(quadnode_t *quadnode){
    int i;
    double m=0.;
    double cm[2]={0.,0.};
    /* We do this only if we are not in a leaf */
    if(quadnode->n > 1){
        for(i=0;i<4;i++){
            if(quadnode->child[i] != NULL){
                quadnode_compMass(quadnode->child[i]);
                m += quadnode->child[i]->particle.m;
                cm[0] += quadnode->child[i]->particle.m*quadnode->child[i]->particle.r[0]; 
                cm[1] += quadnode->child[i]->particle.m*quadnode->child[i]->particle.r[1]; 
            }
        }
        quadnode->particle.m = m;
        quadnode->particle.r[0] = cm[0]/m;
        quadnode->particle.r[1] = cm[1]/m;
    }
}

/* Gravity force of particle 1 onto particle 2 */
void force(particle_t p1, particle_t p2, double *F){
    int i;
    double r;
    double dx = p2.r[0]-p1.r[0];
    double dy = p2.r[1]-p1.r[1];

    r = sqrt(POW2(dx)+POW2(dy));
    if(r!=0){
        F[0] += G*p1.m*p2.m*dx/r;
        F[1] += G*p1.m*p2.m*dy/r;
    }
}


/* Compute the force of the node onto particle */
void quadnode_force(quadnode_t *quadnode, particle_t particle, double *F){
    double r;
    int i;
    /* If it only has one particle, apply formula */
    if(quadnode->n == 1)
        force(quadnode->particle,particle,F);
    /* If it has more, check condition to go further or not */
    else{
        r = sqrt(POW2(particle.r[0]-quadnode->particle.r[0])+POW2(particle.r[1]-quadnode->particle.r[1]));
        if((quadnode->d/r)<Theta)
            force(quadnode->particle, particle, F);
        else
            for(i=0;i<4;i++)
                if(quadnode->child[i] != NULL)
                    quadnode_force(quadnode->child[i],particle,F);
    }
}

/* Free memory of quadnode and subtrees */
void quadnode_free(quadnode_t *quadnode){
    int i;
    if(quadnode->n == 1)
        free(quadnode);
    else{
        for(i=0;i<4;i++)
            if(quadnode->child[i]!=NULL)
                quadnode_free(quadnode->child[i]);
        free(quadnode);
    }
}
