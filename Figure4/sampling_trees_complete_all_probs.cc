//here we sample completely a tree with rewards 1 or -1 prob ARBITRARY (equivalent to Bernoulli with alpha=beta=1)

#include <stdio.h> 
#include <stdlib.h>
//#include <math.h>
#include "randomc.h"                   // define classes for random number generators
#include "userintf.cpp"                // define system specific user interface
#include "mersenne.cpp"                // members of class TRandomMersenne


int main(void)
{
	//Parameters

	const int     n = 10; //depth of the tree (excludes the initial root)
	const int     bf = 2; //branching factor (number of child/leaves per root/parent)

	const double  p = 0.50; //success probability

	const int     iterations = 10000;

	const int     seed =  4032; //random seed
	
	//Definitions
	int	     i, j, k, iter, a;
	double   r, epsilon;

	double   *J_vec, *Q_vec, *R_vec;
	double   *J_vec_n_aver;

	
	FILE *value_actions;

	value_actions = fopen("value_actions.m", "w"); 

	//seed
    TRandomMersenne rg(seed);

	//allocation of vectors
	J_vec = (double*)malloc(pow(bf, n) * sizeof(double));
	Q_vec = (double*)malloc(pow(bf, n) * sizeof(double));
	R_vec = (double*)malloc(pow(bf, n) * sizeof(double));
	J_vec_n_aver = (double*)malloc(n * sizeof(double)); //vector of value of J for each depth from 1 to n

	//defs
	epsilon = p / (1 - p); //to make average reward 0

	//
	for (j = 0; j < n; j++) {
		J_vec_n_aver[j] = 0;
	}

	for (iter = 0; iter < iterations; iter++) {

		//initial condition all J_vec are zero, corresponding to the max depth of the tree
		for (j = 0; j < pow(bf, n); j++) {
			J_vec[j] = 0;
		}

		for (i = 0; i < n; i++) { //loop over depth //i=0 correspond to last branches, really deep

			for (j = 0; j < pow(bf, n - i); j++) { //loop over all the breadth at a given depth
				r = rg.Random();
				if (r < p) {
					R_vec[j] = 1.;
				}
				if (r >= p) {
					R_vec[j] = -epsilon;
				}

				Q_vec[j] = R_vec[j] + J_vec[j]; //action value Q is the immediate reward after the action + state value
				//note that first time Q_vec is computed, J_vec is zero due to the initial condition
			}

			//J_vec is the max of the Q's in groups of bf, the branches from each root
			for (j = 0; j < pow(bf, n - 1 - i); j++) { 
				J_vec[j] = Q_vec[bf*j + 0]; //choose one at random, first one
				for (k = 0; k < bf; k++){
					if (Q_vec[bf*j + k] > J_vec[j]) { //uploading the value of J_vec if in the groups of bf there is a higher Q
						J_vec[j] = Q_vec[bf*j + k];
					}
				}
			}

			for (j = 0; j < pow(bf, n - 1 - i); j++) {
				J_vec_n_aver[i] = J_vec_n_aver[i] + J_vec[j];

			}

		}

	}

	for (j = 0; j < n; j++) {
		J_vec_n_aver[j] = J_vec_n_aver[j]/ (1.*iterations * pow(bf, n - 1 - j)); 

		printf("%i %f \n", j+1, J_vec_n_aver[j]);
		
		fprintf(value_actions, "%i %f \n", j + 1, J_vec_n_aver[j]);
	}


	fclose( value_actions );
	
	return 0;
}
