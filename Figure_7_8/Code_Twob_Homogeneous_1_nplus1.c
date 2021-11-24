//here we sample completely a tree with rewards 1 with prob 1/n+1 or negative reward -p/(1-p)
//here is the THEORY based on the alternation of the diffusion and maximization steps using two branching factors and homogeneous allocations.
//In each point of the (p,C) space he optimal value is computed by spanning the possible branching factors 
//and depth of switching and choosing the largest one.
//For each (p,C), the value for specific heuristics is saved.

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

double* diffusion(double *J, double *Q, double *q, int layer, int depth, int branch, int n, int length);
double* maximization(double *J, double *Q, double *q, int layer, int depth, int branch, int n, int length);
double reward(int depth, int d1, int b1, int b2, int C, int n);


int main ()
{           
    int depth=0, d1=0, b1=0, b2=0, C=0, i=0, contatore=0, depth_b1=0;
    double value=0., Cr=0.;
    int n=1;
    double p=1./((double)n+1.);
    double opt_value=0.;
    int opt_b1=0, opt_b2=0, opt_d=0;

    int C_vec[9]= {3, 5, 10, 20, 50, 100, 200, 500, 1000};
    int n_vec[6]= {1, 2, 3, 5, 9, 99};

    FILE *Optimal_V, *Loss_b1b2, *Loss_b1b2d1;
    Optimal_V = fopen("Twob_1_optd.m", "w");
    Loss_b1b2 = fopen("Twob_1_loss_oneb.m", "w");
    Loss_b1b2d1 = fopen("Twob_1_loss_twob.m", "w");

    fprintf(Optimal_V, "\n\n %%Value* b1 b2 d1 C p\n");
    fprintf(Loss_b1b2, "\n\n %%Value Value* b1 b1* b2 b2* d1 d1* C p\n");
    fprintf(Loss_b1b2d1, "\n\n %%Value Value* b1 b1* b2 b2* d1 d1* C p\n");

    for (int n_index = 0; n_index < 6; ++n_index)
    {
        n=n_vec[n_index];
        p=1./((double)n+1.);
        
        for (int capacity = 0; capacity < 9; ++capacity)
        {
            C=C_vec[capacity];
            opt_value=-(double)C*(p/(1.-p));
            opt_b1=0; opt_b2=0; opt_d=0;
            
            for (b1 = 1; b1<=30; ++b1) 
            {
                i=0;
                Cr=(double)C;
                
                while(Cr>0.)
                {
                    Cr-=pow(b1,i+1);
                    ++i;
                }
                depth_b1=i;

                for (b2 = 1;  b2<=30; ++b2)
                {
                    for (d1 = 1; d1 <= depth_b1; ++d1)
                    {
                        i=0;
                        if (C<=1){i=1;}

                        Cr=(double)C;
                        
                        for (int i_muto = 0; i_muto < d1; ++i_muto)
                        {
                            Cr-=pow(b1,i_muto+1);
                        }

                        i=0;

                        while (Cr>0.)
                        {
                            ++i;
                            Cr-=pow(b1,d1)*pow(b2,i);
                        } 
                        depth=i+d1;

                        value=reward(depth, d1, b1, b2, C, n);
                        
                        if (value>opt_value)
                        {
                            opt_value=value;
                            opt_d=d1;
                            opt_b1=b1;
                            opt_b2=b2;
                        }
                    }
                }
            }
            fprintf(Optimal_V, "%lf %d %d %d %d %f\n", opt_value, opt_b1, opt_b2, opt_d, C, p);
            printf("%lf %d %d %d %d %f\n", opt_value, opt_b1, opt_b2, opt_d, C, p);

            //Comparing value with what happens when b1=b2=2;
            
            Cr=(double)C; 
            b1=2; 
            b2=2;
            
            for (int i_muto = 0; i_muto < opt_d; ++i_muto)
            {
                Cr-=pow(b1,i_muto+1);
            }

            i=0;

            while (Cr>0.)
            {
                ++i;
                Cr-=pow(b1,opt_d)*pow(b2,i);
            } 
            depth=i+opt_d;

            value=reward(depth, opt_d, b1, b2, C, n);
            fprintf(Loss_b1b2, "%lf %lf %d %d %d %d %d %d %d %f\n", value, opt_value, b1, opt_b1, b2, opt_b2, opt_d, opt_d, C, p); 
            
            //Comparing value with what happens when b1=2, b2=1 and d1=1;
            
            Cr=(double)C; 
            b1=2; 
            b2=1; 
            d1=1;
            
            for (int i_muto = 0; i_muto < d1; ++i_muto)
            {
                Cr-=pow(b1,i_muto+1);
            }

            i=0;

            while (Cr>0.)
            {
                ++i;
                Cr-=pow(b1,d1)*pow(b2,i);
            } 
            depth=i+1;

            value=reward(depth, d1, b1, b2, C, n);
            fprintf(Loss_b1b2d1, "%lf %lf %d %d %d %d %d %d %d %f\n", value, opt_value, b1, opt_b1, b2, opt_b2, d1, opt_d, C, p); 
        }
    }

    fclose(Optimal_V);
    fclose(Loss_b1b2);    
    fclose(Loss_b1b2d1);

    return 0;   
}

//diffusion step
double* diffusion(double *J, double *Q, double *q, int layer, int depth, int branch, int n, int length)
    {
        int i, posQ, posJ;
        posQ=(depth-layer);

        Q[posQ]=1./((double)(n+1))*q[depth-layer+1]*((double)n*J[posQ+1]);

        for (i = depth-layer+1; i < depth; ++i)
        {
            Q[i]=(1-q[depth-layer+1])*J[i]+1./((double)(n+1))*q[depth-layer+1]*((double)n*J[i+1]);
        }
        posQ=depth;
        Q[depth]=(1-q[depth-layer+1])*J[depth];
    
        for (i = 1; i < layer; ++i)
        {
            posQ+=depth-layer;
            for (int j= 0; j < layer-i+1; ++j)
            {
                posQ+=1;
                Q[posQ]=(1-q[depth-layer+1])*J[posQ]+1./((double)(n+1))*q[depth-layer+1]*J[posQ-depth+(i-1)]+1./((double)(n+1))*(double)n*q[depth-layer+1]*J[posQ+1];
            }
        }

            posQ+=depth-layer+1;
            Q[posQ]=1./((double)(n+1))*q[depth-layer+1]*(J[posQ-(depth-layer+1)]);

        return Q;
    }

//maximization step
double* maximization(double *J, double *Q, double *q, int layer, int depth, int branch, int n, int length)
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


//Function computing the value as the expected cumulative reward obtained by alternating the diffusion and 
//maximization step over the depth=d1+d2 levels of the tree. q1 is chosen according to homogeneous policies.
//The first d2 levels (backward!) are allocated with b2 branches, b1 otherwise.
double reward(int depth, int d1, int b1, int b2, int C, int n)
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
        
        int contatore=0;
        for (i=0; i<=depth; ++i)
        {
            for (int m = depth-i; m >=0; --m)
            {
                    k[contatore]=(double)i-(double)(m)/(double)n;
                    contatore++;
            }
        }

        for (i = 1; i <= d1; ++i)
        {
            Cr=Cr-pow(b1, i);
            q[i]=1;
        }
        
        if (Cr<=0.)  //enter this cycle only IF all resources will be assigned using b1 and we need to homogeneously allocate the samples using b1
        {
            Cr=Cr+pow(b1, d1);
        }


        for (i = 1; i < depth-d1; ++i)
        {
            Cr=Cr-pow(b1, d1)*pow(b2, i);
            q[d1+i]=1;
        }

        q[depth]=Cr/(pow(b1, d1)*pow(b2, depth-d1)); 

        Q[2*depth]= (1./((double)(n+1)))*q[depth]; //Q_1=1  
        Q[depth]= 1-q[depth];     //Q_1=0
        Q[depth-1]= ((double)n/((double)(n+1)))*q[depth]; //Q_1=-1/n
        
        if (depth>d1)
        {
            J[2*depth]= 1-(pow(Q[depth-1]+Q[depth],(double)b2)); //J_1=1  
            J[depth]=pow(Q[depth-1]+Q[depth],(double)b2)-pow(Q[depth-1],(double)b2);     //J_1=0
            J[depth-1]= pow(Q[depth-1],(double)b2); //J_1=-1/n

            for (i = 2; i <= depth-d1; ++i)
            {
                Q=diffusion(J, Q, q, i, depth, b2, n, length);
                J=maximization(J, Q, q, i, depth, b2, n, length);
            }

            if (depth>1)
            {
                for (i = depth-d1+1; i <= depth; ++i)
                {
                    Q=diffusion(J, Q, q, i, depth, b1, n, length);
                    J=maximization(J, Q, q, i, depth, b1, n, length);
                }
            }
        }
        
        if (depth==d1)
        {
            J[2*depth]= 1-(pow(Q[depth-1]+Q[depth],(double)b1)); //J_1=1  
            J[depth]=pow(Q[depth-1]+Q[depth],(double)b1)-pow(Q[depth-1],(double)b1);     //J_1=0
            J[depth-1]= pow(Q[depth-1],(double)b1); //J_1=-1/n

            for (i = 2; i <= depth; ++i)
            {
                Q=diffusion(J, Q, q, i, depth, b1, n, length);
                J=maximization(J, Q, q, i, depth, b1, n, length);
            }
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
