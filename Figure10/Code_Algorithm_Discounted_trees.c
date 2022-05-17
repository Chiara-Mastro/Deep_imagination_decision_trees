#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "Functions_discounted_trees.c"

#define DELTAX 0.01
#define PI 3.141592653589793
#define GAMMA_EULERO 0.57721566490153286060

int main(void)
{

    //unsigned long long init[4]={0x12345ULL, 0x23456ULL, 0x34567ULL, 0x45678ULL}, length=4;
	//init_by_array64(init, length);
	//srand(time(NULL));
	int branch=3, C=20, M=7, n=99, iterations=10000000, depth=0, d=0, Cr=0;
	double p=0.1;

	double gamma_vec[11]={0.9, 0.91, 0.92, 0.93, 0.94, 0.95, 0.96, 0.97, 0.98, 0.99, 1};
	double gamma_space[11]={0.01, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.99};
	int num_gamma=11;

	int C_vec[10]={5, 10, 15, 30, 50, 100, 200, 500, 700, 1000};
	int num_C=10;

	int num_branch=30;

	double value_alg=0., value_space_alg = 0.;

   	FILE *value_in_b, *value_in_space;
   	n=1;
   	p=0.50;
    value_in_b = fopen("Value_b_gamma_p_50.m", "w");
    value_in_space = fopen("Space_C_gamma_p_50.m", "w");
    fprintf(value_in_b,"%% alg branch depth gamma C\n");
    fprintf(value_in_space,"%% alg branch depth gamma C\n");

    for (int i = 0; i < num_C; ++i)
    {
    	for (int j = 0; j < num_gamma; ++j)
		{
			for (branch = 1; branch<=num_branch; ++branch)
		    {
		    	d=0; Cr=C_vec[i];
		    	 while(Cr>0)
		        {
		            d+=1;
		            Cr-=pow((double)branch, (double)d);
		        }
		        Cr+=pow((double)branch, (double)d);
		        depth=d;

		        value_alg=reward_1(C_vec[i], branch, n, gamma_vec[j]); 
		        value_space_alg = reward_1(C_vec[i], branch, n, gamma_space[j]);

		        printf("%lf %d %d %lf %d \n", value_alg, branch, depth, gamma_vec[j], C_vec[i]);
		        fprintf(value_in_b,"%lf %d %d %lf %d \n", value_alg, branch, depth, gamma_vec[j], C_vec[i]);
		        fprintf(value_in_space,"%lf %d %d %lf %d \n", value_space_alg, branch, depth, gamma_space[j], C_vec[i]);
		    }
		}
    }

    fclose(value_in_b);
	fclose(value_in_space);

	return 0;

}
