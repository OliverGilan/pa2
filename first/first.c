#include <stdio.h>
#include <stdlib.h>

double ** transpose(double **, int, int);
double ** multiply(double **, int, int, double **,int, int);
double ** inverse(double **, int, int);

int main(int argc, char **argv)
{
    FILE* training; 
    FILE* data;
    int K = 0; //cols
    int N = 0; //rows
    
    training = fopen(argv[1], "r");
    data = fopen(argv[2], "r");
    if(training == NULL || data == NULL){
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
    fclose(training);

    //Read in test matrix
    int num = 0;
    fscanf(data, "%d", &num);
    double **Xtest = (double **)calloc(num, sizeof(double));
    for(int i =0; i<num;i++){
        Xtest[i] = (double *)calloc(K+1, sizeof(double));
        *Xtest[i] = (double)1;
        for(int j=1;j<K+1;j++){
            fscanf(data, "%lf%*c ", &Xtest[i][j]);
        }
    }
    fclose(data);

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

    printf("\n\n\n");
    double **Y = (double **)calloc(N,sizeof(double));
    for(int i=0; i< N; i++){
        Y[i] = (double *)calloc(1,sizeof(double));
        int j = K;
        *Y[i] = Z[i][j];
        printf("%lf\n", *Y[i]);
    }

    double **Xt = transpose(X, N, K+1); 
    printf("\n\n\n");
    for(int i = 0; i<K+1; i++){
        for(int j =0;j< N;j++){
            printf("%lf ", Xt[i][j]);
        }
        printf("\n");
    }

    double **XX = multiply(X, N, K+1, Xt, K+1, N);
    printf("\n\n\n");
    for(int i = 0; i<N; i++){
        for(int j =0;j< N;j++){
            printf("%lf ", XX[i][j]);
        }
        printf("\n");
    }
    printf("DONE\n");
    double **XXinv = inverse(XX, N, N);
    printf("\n\n\n");
    if(XXinv == NULL){
        printf("NULL\n");
    }
    // for(int i = 0; i<N; i++){
    //     for(int j =0;j<N;j++){
    //         printf("%lf ", XXinv[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("done\n");
    double **XInv = multiply(Xt, K+1, N, XXinv, N, N);
    double **W = multiply(XInv, K+1, N, Y, N, 1);
    double **Final = multiply(Xtest, num, K+1, W, K+1, 1);
    for(int i=0;i<num;i++){
        printf("%0.0lf\n", *Final[i]);
    }

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
    //Creates augmented matrix
    double **augm = (double**)calloc(y, sizeof(double));
    for(int i=0;i<y;i++){
        augm[i] = (double*)calloc(2*x,sizeof(double));
        for(int j=0; j<x; j++){
            augm[i][j] = mtx[i][j];
        }
        for(int k=x; k<2*x; k++){
            if(k-x == i){
                augm[i][k] = 1;
            }else{
                augm[i][k] = 0;
            }
        }
    }

    printf("\n\n\n");
    for(int i = 0; i<y; i++){
        for(int j =0;j< 2*x;j++){
            printf("%0.0lf ", augm[i][j]);
        }
        printf("\n");
    }
    //Row reduction EF
    for(int i = 0; i<y; i++){
        int pivot = i;
        //Scales row to make pivot 1
        if(augm[i][pivot] != 1 && augm[i][pivot] != 0){
            for(int j=i; j<2*x; j++){
                augm[i][j] = augm[i][j]/augm[i][pivot];
            }
        }
        //Reduces all rows below pivot
        for(int k=i+1; k<y; k++){
            if(augm[k][pivot] == 0) break;
            int scalar = augm[k][pivot];
            for(int l=pivot; l<2*x;l++){
                augm[k][l] = augm[k][l] - (scalar*augm[i][l]);
            }
        }
    }
    printf("\n\n\n");
    for(int i = 0; i<y; i++){
        for(int j =0;j< 2*x;j++){
            printf("%0.0lf ", augm[i][j]);
        }
        printf("\n");
    }
    //Row Reduction RREF
    for(int i = y-1; i>0; i--){
        for(int j = x-1; j >=0; j--){
            for(int k = i-1; k>=0; k--){
                int scalar = augm[k][j];
                for(int l = j; l<2*x; l++){
                    augm[k][l] = augm[k][l] - (scalar * augm[i][l]); 
                }
            }
        }
    }
    //Check if inverse
    // int inverse = 1;
    // for(int i =0; i<y; i++){
    //     for(int j=0;j<x;j++){
    //         if(j == i) {
    //             if(augm[i][j] != 1) inverse=0;
    //         }else{
    //             if(augm[i][j] != 0) inverse=0;
    //         }
    //     }
    // }
    //Get inverse from augm
        double **inv = (double **)calloc(y, sizeof(double));
        for(int i = 0; i<y;i++){
            inv[i] = (double *)calloc(x, sizeof(double));
            for(int j=0; j<x; j++){
                inv[i][j] = augm[i][x+j];
            }
        }
    return inv;
}