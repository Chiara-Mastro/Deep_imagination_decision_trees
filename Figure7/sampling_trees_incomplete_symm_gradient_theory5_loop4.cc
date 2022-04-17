//here we sample partially/incompletely a tree with rewards 1 or -1 prob 1/2 (equivlanet to Bernoulli with alpha=beta=1)
//in addition, the prob q_i of sampling at each depth can be different
//here is the THEORY, and a gradient to find the optimal q_vec
//here the weights are over alphas different from 1, weighting the number of options per layer
//uses alternating projection method

#include <stdio.h> 
#include <stdlib.h>
//#include <math.h>
#include "randomc.h"                   // define classes for random number generators
#include "userintf.cpp"                // define system specific user interface
#include "mersenne.cpp"                // members of class TRandomMersenne


int main(void)
{
	//Parameters

	const int     bf_vec[4] = {2, 3, 5, 10}; //branching factor (number of child/leaves per root/parent)
	const int     num_bf = 4;

	const int     iterations = 100000; //number of iterations for the gradient

	const double  eta = 0.001;
	const double  dq = 0.0000001; //small step <<1 to compute numerically derivatives of J respect to q_i, i=1,...,n
	const double  toler = 0.000000001;

	const double  C_vec[3] = { 10, 100, 1000 }; //capacity, number of samples (given to the first layers until they are exhausted)
	const int     num_C = 3;
	
	//Definitions
	int	     i, j, k, iter, i_vec, flag2, i_C, bf, i_bf, n, n_aux;
	double   p_Q_semi_cum, p_J_semi_cum, q, J, sum_q, sum_q_ini, C; 
	double   sum_J_der, sum_alfa2, J_old, J_ini;
	double   C_acum, q_res;

	double   *p_J_vec, *p_Q_vec, *J_q_vec, *alfa;
	double   *q_vec, *q_per_vec, *q_vec_ini;
	double   *J_vec_n_mean, *v_vec;

	
	FILE *q_distribution, *value_iterations;

	q_distribution = fopen("q_distribution_theory_loop4.m", "w");
	value_iterations = fopen("value_iterations_theory_loop4.m", "w");

	for (i_bf = 0; i_bf < num_bf; i_bf++) {

		bf = bf_vec[i_bf];

		for (i_C = 0; i_C < num_C; i_C++) {

			J_old = 0;

			C = C_vec[i_C];

			n = 2 * floor( log(C)/log(bf) ) + 3; 
			//max depth chosen in case all capacity wants to be allocated fully to the deepest layer

			//allocation of vectors
			p_J_vec = (double*)malloc((2 * n + 1) * sizeof(double));
			p_Q_vec = (double*)malloc((2 * n + 1) * sizeof(double));
			J_vec_n_mean = (double*)malloc(n * sizeof(double));
			J_q_vec = (double*)malloc(n * sizeof(double)); //vector of numerically estimated derivatives
			q_vec = (double*)malloc(n * sizeof(double));
			q_per_vec = (double*)malloc(n * sizeof(double));
			q_vec_ini = (double*)malloc(n * sizeof(double));
			v_vec = (double*)malloc(n * sizeof(double));
			alfa = (double*)malloc(n * sizeof(double));


			//alfa, weights for q's on each layer
			for (i_vec = 0; i_vec < n; i_vec++) { //for each q_i
				alfa[i_vec] = pow(bf, n - i_vec);
				//weights are larger for deeper layers, and then q for deeper layer would be lower to keep capacit fixed
			}

			//initial q-s
			C_acum = 0;
			n_aux = 0; //number of loaded layers

			//if acummulated capacity is less than capacity, then we asign samples to next layer, and so on
			while (C_acum < C) {
				n_aux = n_aux + 1;
				C_acum = C_acum + pow(bf, n_aux);
			}

			//we remove the capacity to the last loaded layer, sometimes is excess
			if (C_acum >= C) {
				C_acum = C_acum - pow(bf, n_aux);
			}

			//compute the capacity corresponding to the last layer, which somtimes is not fully sampled, that is, q=<1
			q_res = (C - C_acum) / pow(bf, n_aux);

			//initial conditions
			for (j = 0; j < n; j++) { //for each q_i
				q_vec[n-j] = 0;
			}
			for (j = 0; j < n_aux; j++) { //for each q_i
				q_vec[n-j] = 1;
			}
			q_vec[n-n_aux] = q_res;

			sum_q = 0;
			for (j = 0; j < n; j++) {
				sum_q = sum_q + alfa[j] * q_vec[j];
			}
			//making sure initial condition has desired capacity
			for (j = 0; j < n; j++) {
				q_vec[j] = q_vec[j] * C / sum_q;
				q_vec_ini[j] = q_vec[j];
			}

			//indicating used capacity
			sum_q = 0;
			for (j = 0; j < n; j++) {
				sum_q = sum_q + alfa[j] * q_vec[j];
			}


			//iterations for the gradient
			for (iter = 0; iter < iterations; iter++) {

				//computing derivative vector of J respect to q_i
				for (i_vec = 0; i_vec < n; i_vec++) { //for each q_i

					//perturbing q_vec, only one component //all other like q_vec
					for (j = 0; j < n; j++) { //for each q_i
						q_per_vec[j] = q_vec[j];
					}
					q_per_vec[i_vec] = q_vec[i_vec] + dq;

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
					q = q_per_vec[0]; //denote the q per layer
					p_Q_vec[n + 1] = 0.5*q;
					p_Q_vec[n] = 1 - q;
					p_Q_vec[n - 1] = 0.5*q;
					p_Q_semi_cum = 0;
					for (j = -1; j <= 1; j++) { //steps of 1, loop over all the breadth at a given depth
						p_J_vec[n + j] = pow(p_Q_semi_cum + p_Q_vec[n + j], bf) - pow(p_Q_semi_cum, bf);
						p_Q_semi_cum = p_Q_semi_cum + p_Q_vec[n + j];
					}
					J_vec_n_mean[0] = 1.*p_J_vec[n + 1] - 1.*p_J_vec[n - 1]; //mean of the above two weighted by 1 and -1


					for (i = 2; i < n + 1; i++) { //loop over depth //i=2 corresponds to last branches, where J_1 is used as initial condition

						for (j = 0; j < (2 * n + 1); j++) {
							p_Q_vec[j] = 0;
						}

						//sampling prob in each layer
						q = q_per_vec[i - 1];

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

					//perturbed values after changing q_i
					J_q_vec[i_vec] = J_vec_n_mean[n - 1];

					//printf("%i %f \n", i_vec, J_q_vec[i_vec]);

				}


				//calculation of J

				//zeroing
				for (j = 0; j < n; j++) {
					J_vec_n_mean[j] = 0; //means
				}
				//initial condition all probabilities of J
				for (j = 0; j < (2 * n + 1); j++) {
					p_J_vec[j] = 0;
					p_Q_vec[j] = 0;
				}
				//*
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

				//unperturbed value (this is the first layer value, so the value of playing the tree game)
				J = J_vec_n_mean[n - 1];

				if (iter == 0) {
					J_ini = J; //initial value
				}

				printf("%i %f %f %i %i %f \n", iter, J, sum_q, bf, n, J_ini);

				fprintf(value_iterations, "%i %f %f %i %i %f \n", iter, J, C, bf, n, J_ini);

				if (pow(J - J_old, 2) < pow(toler, 2)) {
					for (i_vec = 0; i_vec < n; i_vec++) { //for each q_i
						fprintf(q_distribution, "%i %f %f %f %f %i \n", n - i_vec, q_vec[i_vec], q_vec_ini[i_vec], alfa[i_vec], C, bf);
					}
					break;
				}
				J_old = J;

				//GRADIENT ASCENT: iterating q_vec
				//gradient and removing parallel component to alpha_vec = (1,1,...,1)
				for (i_vec = 0; i_vec < n; i_vec++) { //for each q_i
					v_vec[i_vec] = (J_q_vec[i_vec] - J) / dq;
					q_vec[i_vec] = q_vec[i_vec] + eta * v_vec[i_vec];
				}

				//project q to plane pi
				sum_J_der = 0;
				sum_alfa2 = 0; //sums of alfa squares
				for (i_vec = 0; i_vec < n; i_vec++) { //for each q_i
					sum_J_der = sum_J_der + alfa[i_vec] * q_vec[i_vec];
					sum_alfa2 = sum_alfa2 + pow(alfa[i_vec], 2);
				}
				sum_J_der = sum_J_der - C; //remove all component beyond the capacity
				for (i_vec = 0; i_vec < n; i_vec++) { //for each q_i
					q_vec[i_vec] = q_vec[i_vec] - sum_J_der * alfa[i_vec] / sum_alfa2;
				}

				//alternating projection method
				flag2 = 0;
				while (flag2 == 0) {

					//assume correct
					flag2 = 1;

					//constraints 0 <= q_i <= 1
					for (i_vec = 0; i_vec < n; i_vec++) { //for each q_i
						if (q_vec[i_vec] > 1.0000000000001) { //tolerance to stop the alternation of projections
							q_vec[i_vec] = 1;
							flag2 = 0;
						}
						if (q_vec[i_vec] < -0.0000000000001) {
							q_vec[i_vec] = 0;
							flag2 = 0;
						}
					}
					//1.000000000001

					//project q to plane pi
					sum_J_der = 0;
					sum_alfa2 = 0; //sums of alfa squares
					for (i_vec = 0; i_vec < n; i_vec++) { //for each q_i
						sum_J_der = sum_J_der + alfa[i_vec] * q_vec[i_vec];
						sum_alfa2 = sum_alfa2 + pow(alfa[i_vec], 2);
					}
					sum_J_der = sum_J_der - C; //remove all component beyond the capacity
					for (i_vec = 0; i_vec < n; i_vec++) { //for each q_i
						q_vec[i_vec] = q_vec[i_vec] - sum_J_der * alfa[i_vec] / sum_alfa2;
					}

				}

				/*
				//finally impossing the exact constraint
				//constraints 0 <= q_i <= 1
				for (i_vec = 0; i_vec < n; i_vec++) { //for each q_i
					if (q_vec[i_vec] > 1) { //tolerance to stop the alternation of projections
						q_vec[i_vec] = 1;
					}
					if (q_vec[i_vec] < -0) {
						q_vec[i_vec] = 0;
					}
				}
				*/


				//normalization to max capacity
				sum_q = 0;
				for (i_vec = 0; i_vec < n; i_vec++) { //for each q_i
					sum_q = sum_q + alfa[i_vec] * q_vec[i_vec];
				}

				/*
				//indicating final used capacity
				sum_q = 0;
				for (i_vec = 0; i_vec < n; i_vec++) { //for each q_i
				sum_q = sum_q + alfa[i_vec] * q_vec[i_vec];
				}
				*/

			}

			if (iter == iterations){
				//printing q_vec
				for (i_vec = 0; i_vec < n; i_vec++) { //for each q_i
					printf("%i %f %f %f %i \n", i_vec, q_vec[i_vec], q_vec_ini[i_vec], C, bf);

					fprintf(q_distribution, "%i %f %f %f %f %i \n", n - i_vec, q_vec[i_vec], q_vec_ini[i_vec], alfa[i_vec], C, bf);
				}
			}
			

		}

	}


	fclose( value_iterations );
	fclose( q_distribution);

	return 0;
}
