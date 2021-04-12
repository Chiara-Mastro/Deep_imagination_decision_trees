//here we sample partially/incompletely a tree with rewards 1 or -1 prob 1/2 (equivlanet to Bernoulli with alpha=beta=1)
//in addition, the prob q_i of sampling at each depth can be different
//here is the THEORY
//loop over branching factor at fixed capacity C
//capacity C is given to the first layers, and the remaining to the last one

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

	//note that in this code p=0.5, always, for the theory

	const double  C = 10; //capacity, number of samples (given to the first layers until they are exhausted)


	//Definitions
	int	     i, j, k;
	int      bf, i_bf, n;

	double   p_Q_semi_cum, p_J_semi_cum, C_acum;
	double   q, q_res;

	double   *p_J_vec, *p_Q_vec, *q_vec;
	double   *J_vec_n_mean;

	
	FILE *value_actions;

	value_actions = fopen("value_actions_theory_capacity.m", "w");


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
		p_J_vec = (double*)malloc((2 * n + 1) * sizeof(double));
		p_Q_vec = (double*)malloc((2 * n + 1) * sizeof(double));
		J_vec_n_mean = (double*)malloc(n * sizeof(double));
		q_vec = (double*)malloc(n * sizeof(double));

		//define q for each layer (note, last year is layer 0, and it is the one with residual q)
		for (j = 0; j < n; j++) {
			q_vec[j] = 1; //innitializing
		}
		q_vec[0] = q_res;

		//zeroing
		for (j = 0; j < n; j++) {
			J_vec_n_mean[j] = 0; //means
		}
		//initial condition all probabilities of J
		for (j = 0; j < (2 * n + 1); j++) {
			p_J_vec[j] = 0;
			p_Q_vec[j] = 0;
		}
		//initial condition at depth n = 1;
		q = q_vec[0]; //denote the q per layer
		p_Q_vec[n + 1] = 0.5*q;
		p_Q_vec[n] = 1 - q;
		p_Q_vec[n - 1] = 0.5*q;
		p_Q_semi_cum = 0;
		for (j = -1; j <= 1; j++) { //steps of 1, loop over all the breadth at a given depth
			p_J_vec[n + j] = pow(p_Q_semi_cum + p_Q_vec[n + j], bf) - pow(p_Q_semi_cum, bf);
			p_Q_semi_cum = p_Q_semi_cum + p_Q_vec[n + j];
		}
		J_vec_n_mean[0] = 1.*p_J_vec[n + 1] - 1.*p_J_vec[n - 1]; //mean of the above two weighted by 1 and -1
		//print last depth
		printf("%i %f \n", 1, J_vec_n_mean[0]);

		for (i = 2; i < n + 1; i++) { //loop over depth //i=2 corresponds to last branches, where J_1 is used as initial condition

			for (j = 0; j < (2 * n + 1); j++) {
				p_Q_vec[j] = 0;
			}

			//sampling prob in each layer
			q = q_vec[i - 1];

			//diffusion step
			for (j = -i; j <= i; j++) { //steps of 1, loop over all the breadth at a given depth

				if (j == -i) { //border
					p_Q_vec[n + j] = 0.5 * q * p_J_vec[n + j + 1];
				}
				if (j == i) { //border
					p_Q_vec[n + j] = 0.5 * q * p_J_vec[n + j - 1];
				}
				if (j == -i + 1) { //next to border
					p_Q_vec[n + j] = 0.5 * q * p_J_vec[n + j + 1] + (1 - q) * p_J_vec[n + j];
				}
				if (j == i - 1) { //next to border
					p_Q_vec[n + j] = (1 - q) * p_J_vec[n + j] + 0.5 * q * p_J_vec[n + j - 1];
				}
				if (j != -i && j != i && j != -i + 1 && j != i - 1) { //inner
					p_Q_vec[n + j] = 0.5 * q * p_J_vec[n + j + 1] + (1 - q) * p_J_vec[n + j] + 0.5 * q * p_J_vec[n + j - 1];
				}

			}

			//maximization step
			p_Q_semi_cum = 0;
			p_J_semi_cum = 0;

			for (j = -i; j <= i; j++) { //steps of 1, loop over all the breadth at a given depth

				p_J_vec[n + j] = pow(p_Q_semi_cum + p_Q_vec[n + j], bf) - pow(p_Q_semi_cum, bf);

				p_Q_semi_cum = p_Q_semi_cum + p_Q_vec[n + j];

				p_J_semi_cum = p_J_semi_cum + p_J_vec[n + j];

			}

			//normalize prob to avoid explosion (without this, it is unstable for n>50!!!)
			for (j = -i; j <= i; j++) { //steps of 1, loop over all the breadth at a given depth

				p_J_vec[n + j] = p_J_vec[n + j] / p_J_semi_cum;

			}


			//computing J means
			for (j = -i; j <= i; j++) { //steps of 1, loop over all the breadth at a given depth

				J_vec_n_mean[i - 1] = J_vec_n_mean[i - 1] + p_J_vec[n + j] * j;

			}

		}

		printf("%i %i %f %f %f %f %f %i \n", bf, n, J_vec_n_mean[n - 1], p_Q_semi_cum, p_J_semi_cum, C_acum, q_res, n_max); //last to should be 1, sum of all probs

		fprintf(value_actions, "%i %i %f %f \n", bf, n, J_vec_n_mean[n - 1], C);


	}




	fclose( value_actions );
	
	return 0;
}
