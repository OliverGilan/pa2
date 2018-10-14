#include <stdio.h>
#include <stdlib.h>

double ** transpose(double **, int, int);
double ** multiply(double **, int, int, double **,int, int);
double ** inverse(double **, int, int);

int main(int argc, char **argv)
{
    FILE* training; 
    int K = 0; //cols
    int N = 0; //rows
    
    training = fopen(argv[1], "r");
    if(training == NULL){
        return 0;
    }

    fscanf(training, "%d", &K);
    fscanf(training, "%d", &N);

    //Read in training matrix
    double **Z = (double **)calloc(N, sizeof(double));
    for(int i = 0; i < N; i++){
        Z[i] = (double *)calloc(K+1, sizeof(double));
        for(int j = 0; j <= K; j++){
            fscanf(training, "%lf%*c ", &Z[i][j]);
            // printf("%lf\t", Z[i][j]);
        }
        // printf("\n");
    }

    //DEBUG
    // for(int i = 0; i<N; i++){
    //     for(int j =0;j<=K;j++){
    //         printf("%lf\t", Z[i][j]);
    //     }
    //     printf("\n");
    // }

    double **X = (double **)calloc(N, sizeof(double));
    for(int i = 0; i < N; i++){
        X[i] = (double *)calloc(K+1, sizeof(double));
        *X[i] = (double)1;
        for(int j = 1; j <= K; j++){
            X[i][j] = Z[i][j-1];
            // printf("%lf\t", X[i][j]);
        }
        // printf("\n");
    }

    //DEBUG
    printf("\n\n\n");
    for(int i = 0; i<N; i++){
        for(int j =0;j<=K;j++){
            printf("%lf\t", X[i][j]);
        }
        printf("\n");
    }


    double *Y = (double *)calloc(N,sizeof(double));
    for(int i=0; i< N; i++){
        int j = K;
        Y[i] = Z[i][j];
        // printf("%lf\n", Y[i]);
    }

    double **Xt = transpose(X, N, K+1); 
    // printf("\n\n\n XX:");
    double **XX = multiply(X, N, K+1, Xt, K+1, N);
    // printf("%lf", XX[0][0]);


    return 0;
}

double ** transpose(double **mtx, int N, int K){
    // printf("STARTED\n");
    double **Xt = (double **)calloc(K, sizeof(double));
    for(int i = 0; i<K; i++){
        Xt[i] = (double *)calloc(N, sizeof(double));
        for(int j = 0; j<N; j++){
            Xt[i][j] = mtx[j][i];
            // printf("%lf ",Xt[i][j]);
        }
        // printf("\n");
    }
    return Xt;
}

double ** multiply(double **mtx, int y, int x, double **mtx2, int y2, int x2){
    double **XX = (double **)calloc(y, sizeof(double));
    for(int i =0; i<y; i++){
        XX[i] = (double *)calloc(x2,sizeof(double));
        for(int j=0;j<x2;j++){
            int sum = 0;
            for(int k=0; k<y2; k++){
                sum += mtx[i][k] * mtx2[k][j];
            }
            XX[i][j] = sum;
            // printf("%lf  ", XX[i][j]);
        }
        // printf("\n");
    }

    return XX;
}

double ** inverse(double **mtx, int y, int x){
    double **augm = (double**)calloc(y, sizeof(double));
    for(int i=0;i<y;i++){
        augm[i] = (double*)calloc(2*x,sizeof(double));
        for(int j=0; j<x; j++){
            
        }
    }
}