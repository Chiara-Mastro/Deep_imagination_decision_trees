//here we sample completely a tree with rewards 1 with prob 1/n+1 or negative reward -p/(1-p)
//here is the THEORY based on the alternation of the diffusion and maximization steps. The reward can be computed bychoosing the branching factor and the depth of the tree.

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

double* diffusion(double *J, double *Q, double *k, int layer, int depth, int branch, int n, int length); //diffusion function which receives the probability of J for the previouslevel and computes the probability of the Q for a level
double* maximization(double *J, double *Q, int layer, int depth, int branch, int n, int length); //maximization function which gives the probability of J for a level from the probability of Q of that level
double reward(int depth, int branch, int n); //reward function which computes the value of a tree with branching factor and depth fixed, with probability of positive reward p=1/n+1

int main ()
{  
    //choose here the parameters of your tree        
    int depth=10;
    int branch=2;
    int n=9; //defining p=1/(n+1)

    //Writing on the file all the values for a given branching factors and all possible levels up to the chosen depth

    FILE *Exhaustive_p_1;

    Exhaustive_p_1 = fopen("value_actions_theory.m", "w");

    for (int i = 1; i <= depth; ++i)
    {
        fprintf(Exhaustive_p_1,"%d %lf \n", i, reward(i, branch, n));
    }

    fclose(Exhaustive_p_1);

    return 0;
}

double* diffusion(double *J, double *Q, double *k, int layer, int depth, int branch, int n, int length)
    {
        int i,l=0, posQ, posJ;
       posQ=(depth-layer);

       //boundary condition: contribution to Q comes from a single term
        Q[posQ]=1./((double)(n+1))*((double)n*J[posQ+1]);
        posJ=posQ+1;
        posQ=posQ+depth;

        //DIFFUSION
        for (i = 1; i < layer; i++)
        {
            Q[posQ]=1./((double)(n+1))*(J[posJ]+(double)n*J[posQ+1]);
            posJ=posQ+1;
            posQ=posQ+(depth-(i-1));
        }

        //boundary condition: contribution to Q comes from a single term
        Q[posQ]=1./((double)(n+1))*(J[posJ]);

        return Q;
    }

double* maximization(double *J, double *Q, int layer, int depth, int branch, int n, int length)
    {
        double cum=0.; int i, posJ;
        posJ=depth-layer;
        //Initializing to zero all the probabilities of J
        for (i = 0; i < length; ++i){J[i]=0;}

        //Probability that the accumulated reward is the smallest possible value
        J[posJ]=pow(Q[posJ], branch);
        cum+=Q[posJ];
        posJ+=depth;

        //Maximization is the difference of the cumulate probabiities
        for (i = 1; i <= layer; i++)
            {
                J[posJ]=pow(cum+Q[posJ], branch)-pow(cum, (double)branch);
                cum+=Q[posJ];
                posJ=posJ+(depth-(i-1));
            }

        return J;
    }

double reward(int depth, int branch, int n)
    {
        int i,l, length;
        double reward=0.;
        
        length=depth;
        for (int i = 1; i <=depth; ++i)
        {
            length+=i;
        }

        double *J=(double*) calloc(length, sizeof (double));
        double *k=(double*) calloc(length, sizeof (double));
        double *Q=(double*) calloc(length, sizeof (double));

        //initializing the set of possible values 
        int contatore=0;
        for (i=0; i<=depth; ++i)
        {
            for (int m = depth-i; m >=0; --m)
            {
                if (i!=0 || m!=0)
                {
                    k[contatore]=(double)i-(double)(m)/(double)n;
                    contatore++;
                }
            }
        }

        //INITIAL CONDITIONS
        J[2*depth-1]= 1.-pow((double)((double)n/(double)(n+1)),(double) branch); //J_1 =1     
        J[depth-1]= pow((double)n/(double)(n+1),(double) branch); //J_1 =-1/n

        //Other layers are explored alternating diffusion and maximization steps
        for (i = 2; i <= depth; ++i)
        {
            Q=diffusion(J, Q, k, i, depth, branch, n, length);
            J=maximization(J, Q, i, depth, branch, n, length);
        }
        
        //Computing in the end the value as the expected value of J
        for (i=0; i<length; ++i)
            {
                reward+=k[i]*J[i];
            }

        free(k);
        free(J);
        free(Q);
        return reward;

    }