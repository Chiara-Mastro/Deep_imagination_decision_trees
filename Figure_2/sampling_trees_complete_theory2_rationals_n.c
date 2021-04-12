//here we sample completely a tree with rewards 1 with prob n/n+1 or negative reward -p/(1-p)
//here is the THEORY based on the alternation of the diffusion and maximization steps. The reward can be computed bychoosing the branching factor and the depth of the tree.


#include <stdlib.h>
#include <stdio.h>
#include <math.h>

double* diffusion(double *J, double *Q, int layer, int depth, int branch, int n);//diffusion function which receives the probability of J for the previouslevel and computes the probability of the Q for a level
double* maximization(double *J, double *Q, int layer, int depth, int branch, int n);//maximization function which gives the probability of J for a level from the probability of Q of that level
double reward(int depth, int branch, int n); //reward function which computes the value of a tree with branching factor and depth fixed, with probability of positive reward p=1/n+1


int main ()
{           

    //choose here the parameters of your tree 
    int depth=10;
    int branch=2;
    int n=3; //defining p=n/(n+1)

    //Writing on the file all the values for a given branching factors and all possible levels up to the chosen depth
    FILE *Exhaustive_p_75;

    Exhaustive_p_75 = fopen("value_actions_theory.m", "w");

    for (int i = 1; i <= depth; ++i)
    {
        fprintf(Exhaustive_p_75,"%d %lf \n", i, reward(i, branch, n));
    }

    fclose( Exhaustive_p_75 );

return 0;
}

double* diffusion(double *J, double *Q, int layer, int depth, int branch, int n)
    {
        int i,l=0;
       
        for (i = 0; i <= layer; i++)
        {
            //DIFFUSION
            Q[n*(depth-layer)+i*(n+1)]=1./((double)(n+1))*(J[n*(depth-layer)+i*(n+1)+n]+(double)n*J[n*(depth-layer)+i*(n+1)-1]);
            //boundary conditions: contribution to Q comes from a single term
            if(n*(depth-layer)+i*(n+1)+n>(n+1)*depth)
                {Q[n*(depth-layer)+i*(n+1)]=1./((double)(n+1))*((double)n*J[n*(depth-layer)+i*(n+1)-1]);}
            if(n*(depth-layer)+i*(n+1)-1<0)
                {Q[n*(depth-layer)+i*(n+1)]=1./((double)(n+1))*(J[n*(depth-layer)+i*(n+1)+n]);}
        }
       
        return Q;
    }

double* maximization(double *J, double *Q, int layer, int depth, int branch, int n)
    {
        double cum=0.; int i;
        //Initializing to zero all the probabilities of J
        for (i = 0; i < (n+1)*depth+1; ++i){J[i]=0;}

        //Maximization is the difference of the cumulate probabiities
        for (i = 0; i <= layer; i++)
            {
                J[n*(depth-layer)+i*(n+1)]=pow(cum+Q[n*(depth-layer)+i*(n+1)], branch)-pow(cum, (double)branch);
                cum+=Q[n*(depth-layer)+i*(n+1)];
            }
        return J;
    }

double reward(int depth, int branch, int n)
    {
        int i,l;
        double reward=0.;
        double *J=(double*) calloc((n+1)*depth+1, sizeof (double));
        double *k=(double*) calloc((n+1)*depth+1, sizeof (double));
        double *Q=(double*) calloc((n+1)*depth+1, sizeof (double));
        
        //initializing the set of possible values 
        int d=-depth*n;
        for (i=0; i<(n+1)*depth+1; ++i)
        {
            k[i]=d;
            d++;
        }

        //INITIAL CONDITIONS
        J[n*depth+1]= 1.-pow((double)(1./(double)(n+1)),(double) branch); //J_1 =1       
        J[n*depth-n]= pow((double)(1./(double)(n+1)),(double) branch); //J_1 =-n
        
        //Other layers are explored alternating diffusion and maximization steps
        for (i = 2; i <= depth; ++i)
        {
            Q=diffusion(J, Q, i, depth, branch, n);
            J=maximization(J, Q, i, depth, branch, n);
        }
        
        //Computing in the end the value as the expected value of J
        for (i=0; i<(n+1)*depth+1; ++i)
            {
                reward+=k[i]*J[i];
            }

        free(k);
        free(J);
        free(Q);
        return reward;

    }