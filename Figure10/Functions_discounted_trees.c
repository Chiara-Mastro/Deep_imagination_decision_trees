#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DELTAX 0.01
#define PI 3.141592653589793
#define GAMMA_EULERO 0.57721566490153286060



//STOCHASTIC DISCOUNTED FUNCTIONS: rewards are set to +1 and -p/1-p to saitsfy the zero average constraint
double* diffusion_1(double *J, double *Q, int layer, int depth, int branch, double p, int length, double gamma);
double* maximization_1(double *J, double *Q, int layer, int depth, int branch, int length);
double reward_1(int C, int branch, int n, double gamma);

void print_vec_1(double *vec, int depth, int n);

double* diffusion_1(double *J, double *Q, int layer, int depth, int branch, double p, int length, double gamma)
{
    int i, posQ, posJ;
    posQ=(depth-layer);

    Q[posQ]=gamma*(1.-p)*J[posQ+1];

    for (i = depth-layer+1; i < depth; ++i)
    {
        Q[i]=gamma*(1.-p)*J[i+1];
    }
    posQ=depth;
    //Q[depth]=0gamma*p*;
    Q[depth]=0;

    for (i = 1; i < layer; ++i)
    {
        posQ+=depth-layer;
        for (int j= 0; j < layer-i+1; ++j)
        {
            posQ+=1;
            Q[posQ]=gamma*p*J[posQ-depth+(i-1)]+gamma*(1.-p)*J[posQ+1];
        }
    }

    posQ+=depth-layer+1;
    Q[posQ]=p*gamma*(J[posQ-(depth-layer+1)]);

    Q[2*depth]= gamma*p*J[depth]+(1.-gamma)*p;
    Q[depth-1]= gamma*(1.-p)*J[depth]+(1.-gamma)*(1.-p);

    return Q;
}

double* maximization_1(double *J, double *Q, int layer, int depth, int branch, int length)
    {
        double cum=0.; int i, posJ;
        posJ=depth-layer;
        for (i = 0; i < length; ++i){J[i]=0;}

        J[posJ]=pow(Q[posJ], (double)branch);
        cum+=Q[posJ];


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

void print_vec_1(double *vec, int depth, int n)
    {
        int length=depth+1;
        double sum=0.;
        for (int i = 1; i <=depth; ++i)
        {
            length+=i;
        }

        for (int i = 0; i < length; ++i)
        {
            printf("%f ", vec[i]);
            sum+=vec[i];
        }
        printf("\n");
        printf("sum is %lf\n", sum);
    }

double reward_1(int C, int branch, int n, double gamma)
    {
        int i,l, d=0, length;
        double reward=0,Cr=(double)C, p=1./(1.+(double)n);

        while(Cr>0)
        {
            d+=1;
            Cr-=pow((double)branch, (double)d);
        }
        Cr+=pow((double)branch, (double)d);
        int depth=d;

        length=depth+1;
        for (int i = 1; i <=depth; ++i)
        {
            length+=i;
        }


        double *J=(double*) calloc(length, sizeof (double));
        double *k=(double*) calloc(length, sizeof (double));
        double *Q=(double*) calloc(length, sizeof (double));
        

        int contatore=0;
        for (i=0; i<=depth; ++i)
        {
            for (int m = depth-i; m >=0; --m)
            {
                    k[contatore]=(double)i-(double)(m)/(double)n;
                    contatore++;
            }
        }

        double q=Cr/(pow(branch, depth));

        Q[2*depth]= p*q; //Q_1=1  
        Q[depth]= 1.-q;     //Q_1=0
        Q[depth-1]= (1.-p)*q; //Q_1=-1/n
        
        J[2*depth]= 1-(pow(Q[depth-1]+Q[depth],(double)branch)); //J_1=1  
        J[depth]=pow(Q[depth-1]+Q[depth],(double)branch)-pow(Q[depth-1],(double)branch);     //J_1=0
        J[depth-1]= pow(Q[depth-1],(double)branch); //J_1=-1


        for (i = 2; i <= depth; ++i)
        {
            Q=diffusion_1(J, Q, i, depth, branch, p, length, gamma);
           // print_vec_1(Q, depth, n);
            J=maximization_1(J, Q, i, depth, branch, length);
        }
        
        for (i=0; i<length; ++i)
            {
                reward+=k[i]*J[i];
            }

        free(k);
        free(J);
        free(Q);
        return reward;

    }
