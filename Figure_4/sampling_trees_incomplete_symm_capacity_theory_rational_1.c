//here we sample completely a tree with rewards 1 with prob 1/n+1 or negative reward -p/(1-p)
//here is the THEORY based on the alternation of the diffusion and maximization steps. The value can be computed by choosing the branching factor and the depth of the tree.


#include <stdlib.h>
#include <stdio.h>
#include <math.h>

double* diffusion(double *J, double *Q, double *q, int layer, int depth, int branch, int n, int length); //diffusion function which receives the probability of J for the previouslevel and computes the probability of the Q for a level
double* maximization(double *J, double *Q, double *q, int layer, int depth, int branch, int n, int length); //maximization function which gives the probability of J for a level from the probability of Q of that level
double value(int depth, int branch, int C, int n); //value function which computes the value of a tree with branching factor and depth fixed, with probability of positive reward p=1/n+1


int main ()
{           
    //choose here the capacity and the probability p
    int C=10; 
    int n=99; // defining the probability p=1/n+1
   

    int i=0, C_res=0, branch;
    FILE *value_in_b;

    value_in_b = fopen("value_actions_theory_capacity.m", "w");
 
    for (branch = 1; branch<=30; ++branch)
    {
        C_res=0; i=0;
        while (C>C_res)
            {C_res+=pow(branch,i+1);
                ++i;}

        fprintf(value_in_b,"%d %d %lf %d \n", branch, i, value(i, branch, C,n), C);
    }

    fclose(value_in_b);

 return 0;   
}



double* diffusion(double *J, double *Q, double *q, int layer, int depth, int branch, int n, int length)
    {
        int i, posQ, posJ;
        posQ=(depth-layer);

        //boundary condition: contribution to Q comes from a single term
        Q[posQ]=1./((double)(n+1))*q[depth-layer+1]*((double)n*J[posQ+1]);

        //DIFFUSION: computiing the probability up to the value Q=0
        for (i = depth-layer+1; i < depth; ++i)
        {
            Q[i]=(1-q[depth-layer+1])*J[i]+1./((double)(n+1))*q[depth-layer+1]*((double)n*J[i+1]);
        }
        posQ=depth;
        Q[depth]=(1-q[depth-layer+1])*J[depth];
    
        //DIFFUSION: computiing the probability after the value Q=0
        for (i = 1; i < layer; ++i)
        {
            posQ+=depth-layer;
            for (int j= 0; j < layer-i+1; ++j)
            {
                posQ+=1;
                Q[posQ]=(1-q[depth-layer+1])*J[posQ]+1./((double)(n+1))*q[depth-layer+1]*J[posQ-depth+(i-1)]+1./((double)(n+1))*(double)n*q[depth-layer+1]*J[posQ+1];
            }
        }

        //boundary conditions: contribution to Q comes from a single term
        posQ+=depth-layer+1;
        Q[posQ]=1./((double)(n+1))*q[depth-layer+1]*(J[posQ-(depth-layer+1)]);

        return Q;
    }

double* maximization(double *J, double *Q, double *q, int layer, int depth, int branch, int n, int length)
    {
        double cum=0.; int i, posJ;
        posJ=depth-layer;

        //Initializing to zero all the probabilities of J
        for (i = 0; i < length; ++i){J[i]=0;}

        //Probability that the accumulated reward is the smallest possible value
        J[posJ]=pow(Q[posJ], (double)branch);
        cum+=Q[posJ];

        //Maximization is the difference of the cumulate probabiities: exploring all the possible values of J
        for (i = depth-layer+1; i < depth; ++i)
        {
            J[i]=pow(cum+Q[i], (double)branch)-pow(cum, (double)branch);
            cum+=Q[i];
        }
        posJ=depth;
        J[depth]=pow(cum+Q[depth], (double)branch)-pow(cum, (double)branch);
        cum+=Q[depth];

        for (i = 1; i < layer; ++i)
        {
            posJ+=depth-layer;
            for (int j= 0; j < layer-i+1; ++j)
            {
                posJ+=1;
                J[posJ]=pow(cum+Q[posJ], (double)branch)-pow(cum, (double)branch);
                cum+=Q[posJ];
            }
        }

            posJ+=depth-layer+1;
            J[posJ]=pow(cum+Q[posJ], (double)branch)-pow(cum, (double)branch);
            cum+=Q[posJ];

        return J;
    }

double value(int depth, int branch, int C, int n)
    {
        int i,l, Cr=C, length;
        double reward=0;


        length=depth+1;
        for (int i = 1; i <=depth; ++i)
        {
            length+=i;
        }

        double *J=(double*) calloc(length, sizeof (double));
        double *k=(double*) calloc(length, sizeof (double));
        double *Q=(double*) calloc(length, sizeof (double));
        double *q=(double*) calloc(depth+1, sizeof (double));

        //initializing to 1 all the q components but last levels 
        for (i = 1; i < depth; ++i)
        {
            Cr=Cr-pow(branch, i);
            q[i]=1;
        }
        
        //initializing the set of possible values 
        int counter=0;
        for (i=0; i<=depth; ++i)
        {
            for (int m = depth-i; m >=0; --m)
            {
                    k[counter]=(double)i-(double)(m)/(double)n;
                    counter++;
            }
        }

        //Last component of q given by the constraint of the capacity: homogeneous allocation
        q[depth]=Cr/(pow(branch, depth)); //note the inverse order of q qhich is q_d for initial conditions of Q_1

        //INITIAL CONDITIONS
        Q[2*depth]= (1./((double)(n+1)))*q[depth]; //Q_1=1  
        Q[depth]= 1-q[depth];     //Q_1=0
        Q[depth-1]= ((double)n/((double)(n+1)))*q[depth]; //Q_1=-1/n
        

        J[2*depth]= 1-(pow(Q[depth-1]+Q[depth],(double)branch)); //J_1=1  
        J[depth]=pow(Q[depth-1]+Q[depth],(double)branch)-pow(Q[depth-1],(double)branch);     //J_1=0
        J[depth-1]= pow(Q[depth-1],(double)branch); //J_1=-1

        //Other layers are explored alternating diffusion and maximization steps
        for (i = 2; i <= depth; ++i)
        {
            Q=diffusion(J, Q, q, i, depth, branch, n, length);
            J=maximization(J, Q, q, i, depth, branch, n, length);
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
