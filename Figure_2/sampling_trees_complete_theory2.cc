//here we sample completely a tree with rewards 1 or -1 prob 1/2 (equivlanet to Bernoulli with alpha=beta=1)
//here is the THEORY (proposed algorthm)

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
	
	//Definitions
	int	     i, j, k;
	double   p_Q_semi_cum, p_J_semi_cum;

	double   *p_J_vec, *p_Q_vec;
	double   *J_vec_n_mean;

	
	FILE *value_actions;

	value_actions = fopen("value_actions_theory.m", "w"); 


	//allocation of vectors
	p_J_vec = (double*)malloc((2 * n + 1) * sizeof(double));
	p_Q_vec = (double*)malloc((2 * n + 1) * sizeof(double));
	J_vec_n_mean = (double*)malloc(n * sizeof(double));


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
	p_J_vec[n + 1] = 1 - pow(1. / 2., bf);
	p_J_vec[n - 1] = pow(1. / 2.,bf);
	J_vec_n_mean[0] = 1.*p_J_vec[n + 1] - 1.*p_J_vec[n - 1]; //mean of the above two weighted by 1 and -1
	//print last depth
	printf("%i %f \n", 1, J_vec_n_mean[0]);
	fprintf(value_actions, "%i %f \n", 1, J_vec_n_mean[0]);


	for (i = 2; i < n+1; i++) { //loop over depth //i=2 corresponds to last branches, where J_1 is used as initial condition
		
		for (j = 0; j < (2 * n + 1); j++) {
			p_Q_vec[j] = 0;
		}

		//diffusion step
		for (j = -i; j <= i; j += 2) { //steps of 2, loop over all the breadth at a given depth
			
			if (j == -i) {
				p_Q_vec[n + j] = 0.5 * p_J_vec[n + j + 1];
			}
			if (j == i) {
				p_Q_vec[n + j] = 0.5 * p_J_vec[n + j - 1];
			}
			if (j != -i && j != i) {
				p_Q_vec[n + j] = 0.5 * p_J_vec[n + j + 1] + 0.5 * p_J_vec[n + j - 1];
			}

		}

		//maximization step
		p_Q_semi_cum = 0;
		p_J_semi_cum = 0;

		for (j = -i; j <= i; j += 2) { //steps of 2, loop over all the breadth at a given depth

			p_J_vec[n + j] = pow(p_Q_semi_cum + p_Q_vec[n + j], bf) - pow(p_Q_semi_cum, bf);
			
			p_Q_semi_cum = p_Q_semi_cum + p_Q_vec[n + j];

			p_J_semi_cum = p_J_semi_cum + p_J_vec[n + j];

		}

		//normalize prob to avoid explosion (without this, it is unstable for n>50!!!)
		for (j = -i; j <= i; j += 2) { //steps of 2, loop over all the breadth at a given depth

			p_J_vec[n + j] = p_J_vec[n + j] / p_J_semi_cum;

		}


		//computing J means
		for (j = -i; j <= i; j += 2) { //steps of 2, loop over all the breadth at a given depth

			J_vec_n_mean[i-1] = J_vec_n_mean[i-1] + p_J_vec[n + j] * j;

		}

		printf("%i %f %f %f \n", i, J_vec_n_mean[i-1], p_Q_semi_cum, p_J_semi_cum); //last to should be 1, sum of all probs

		fprintf(value_actions, "%i %f \n", i, J_vec_n_mean[i-1]);

	}




	fclose( value_actions );
	
	return 0;
}
