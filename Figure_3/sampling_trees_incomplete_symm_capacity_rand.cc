//here we sample partially/incompletely a tree with rewards 1 or -1 prob 1/2 (equivlanet to Bernoulli with alpha=beta=1)
//in addition, the prob q_i of sampling at each depth can be different

#include <stdio.h> 
#include <stdlib.h>
//#include <math.h>
#include "randomc.h"                   // define classes for random number generators
#include "userintf.cpp"                // define system specific user interface
#include "mersenne.cpp"                // members of class TRandomMersenne


int main(void)
{
	//Parameters

	const int     n_max = 10; //depth of the tree (excludes the initial root)
	//it has to be equal to C

	const int     bf_ini = 1;
	const int     bf_end = 30; //branching factor (number of child/leaves per root/parent)

	const double  p = 0.50; //p=prob(R=1), probability of positive reward, in most of the codes we use implicitely p=1/2

	const double  C = 10; //capacity, number of samples (given to the first layers until they are exhausted)

	const int     iterations = 1000000;

	const int     seed =  70121; //random seed
	
	//Definitions
	int	     i, j, k, iter, a;
	int      bf, i_bf, n;

	double   r1, r2, epsilon;
	double   C_acum, q_res;

	double   *J_vec, *Q_vec, *R_vec, *q_vec;
	double   *J_vec_n_aver;

	
	FILE *value_actions;

	value_actions = fopen("value_actions_capacity.m", "w");

	//seed
    srand(seed);

	//defs
	epsilon = p / (1 - p); //to make aver reward 0

	//loop over branching factors with the same capacity

	for (i_bf = bf_ini; i_bf < bf_end + 1; i_bf++) {

		bf = i_bf; //branching factor

				   //do while we give samples less than capacity
		C_acum = 0;
		n = 0; //number of loaded layers

			   //if acummulated capacity is less than capacity, then we asign samples to next layer, and so on
		while (C_acum < C) {
			n = n + 1;
			C_acum = C_acum + pow(bf, n);
		}

		//we remove the capacity to the last loaded layer, sometimes is excess
		if (C_acum >= C) {
			C_acum = C_acum - pow(bf, n);
		}

		//compute the capacity corresponding to the last layer, which somtimes is not fully sampled, that is, q=<1
		q_res = (C - C_acum) / pow(bf, n);


		//allocation of vectors
		J_vec = (double*)malloc(pow(bf, n) * sizeof(double));
		Q_vec = (double*)malloc(pow(bf, n) * sizeof(double));
		R_vec = (double*)malloc(pow(bf, n) * sizeof(double));
		J_vec_n_aver = (double*)malloc(n * sizeof(double));
		q_vec = (double*)malloc(n * sizeof(double));

		//define q for each layer (note, last year is layer 0, and it is the one with residual q)
		for (j = 0; j < n; j++) {
			q_vec[j] = 1; //initializing
		}
		q_vec[0] = q_res;

		//in. cond
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

					//probabiliyt of sampling the node
					r1 = (double) rand()/RAND_MAX;
					//sampled
					if (r1 < q_vec[i]) {
						r2 = (double) rand()/RAND_MAX;
						if (r2 <= p) {
							R_vec[j] = 1.;
						}
						if (r2 > p) {
							R_vec[j] = -epsilon; //to make aver reward 0
						}
					}
					//not sampled
					if (r1 >= q_vec[i]) {
						R_vec[j] = 0;
					}

					Q_vec[j] = R_vec[j] + J_vec[j]; //action value Q is the inmediate reward after the action + state value
					//note that first time Q_vec is computed, J_vec is zero due to the initial condition
				}

				//J_vec is the max of the Q's in groups of bf, the branches from each root
				for (j = 0; j < pow(bf, n - 1 - i); j++) {
					J_vec[j] = Q_vec[bf*j + 0]; //choose one at random, first one
					for (k = 0; k < bf; k++) {
						if (Q_vec[bf*j + k] > J_vec[j]) {
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
			J_vec_n_aver[j] = J_vec_n_aver[j] / (1.*iterations * pow(bf, n - 1 - j));
		}

		printf("%i %i %f %f %f %i \n", bf, n, J_vec_n_aver[n - 1], C_acum, q_res, n_max); //last to should be 1, sum of all probs

		fprintf(value_actions, "%i %i %f %f \n", bf, n, J_vec_n_aver[n - 1], C);


	}


	fclose( value_actions );
	
	return 0;
}
