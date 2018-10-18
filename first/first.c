#include <stdio.h>
#include <stdlib.h>

double ** transpose(double **, int, int);
double ** multiply(double **, int, int, double **,int, int);
double ** inverse(double **, int, int);
void clean(double **, int, int);

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
        }
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

    double **X = (double **)calloc(N, sizeof(double));
    for(int i = 0; i < N; i++){
        X[i] = (double *)calloc(K+1, sizeof(double));
        *X[i] = (double)1;
        for(int j = 1; j <= K; j++){
            X[i][j] = Z[i][j-1];
        }
    }

    double **Y = (double **)calloc(N,sizeof(double));
    for(int i=0; i< N; i++){
        Y[i] = (double *)calloc(1,sizeof(double));
        int j = K;
        *Y[i] = Z[i][j];
    }

    double **Xt = transpose(X, N, K+1);
    double **XX = multiply(Xt, K+1, N, X, N, K+1);
    double **XXinv = inverse(XX, K+1, K+1);
    double **XInv = multiply(XXinv, K+1, K+1, Xt, K+1, N);
    double **W = multiply(XInv, K+1, N, Y, N, 1);
    double **Final = multiply(Xtest, num, K+1, W, K+1, 1);
    for(int i=0;i<num;i++){
        printf("%0.0lf\n", *Final[i]);
    }
    clean(Z, N, K+1);
    clean(Xtest, num, K+1);
    clean(X, N, K+1);
    clean(Y, N, 1);
    clean(Xt, K+1, N);
    clean(XX, K+1, K+1);
    clean(XXinv, K+1, K+1);
    clean(XInv, K+1, N);
    clean(W, K+1, 1);
    clean(Final, num, 1);
    return 0;
}

void clean(double **matrix, int x, int y){
    for(int i=0;i<x;i++){
        free(matrix[i]);
    }
    free(matrix);
}

double ** transpose(double **mtx, int N, int K){
    double **Xt = (double **)calloc(K, sizeof(double));
    for(int i = 0; i<K; i++){
        Xt[i] = (double *)calloc(N, sizeof(double));
        for(int j = 0; j<N; j++){
            Xt[i][j] = mtx[j][i];
        }
    }
    return Xt;
}

double ** multiply(double **mtx, int y, int x, double **mtx2, int y2, int x2){
    double **XX = (double **)calloc(y, sizeof(*XX));
    for(int i =0; i<y; i++){
        XX[i] = (double *)calloc(x2,sizeof(*XX[i]));
        for(int j=0;j<x2;j++){
            double sum = 0;
            for(int k=0; k<y2; k++){
                sum += mtx[i][k] * mtx2[k][j];
            }
            XX[i][j] = sum;
        }
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

    //Row reduction EF
    for(int i = 0; i<y; i++){
        int pivot = i;
        //Scales row to make pivot 1
        if(augm[i][pivot] != 1 && augm[i][pivot] != 0){
            for(int j=i+1; j<2*x; j++){
                augm[i][j] = augm[i][j]/augm[i][pivot];
            }
            augm[i][pivot] = 1;
        }
        //Reduces all rows below pivot
        for(int k=i+1; k<y; k++){
            if(augm[k][pivot] == 0) break;
            double scalar = (double)augm[k][pivot];
            for(int l=pivot; l<2*x;l++){
                augm[k][l] = augm[k][l] - (scalar*augm[i][l]);
            }
        }
    }

    //Row Reduction RREF
    for(int i = y-1; i>0; i--){
        int j = i;
        for(int k = i-1; k>=0; k--){
            double scalar = (double) augm[k][j];
            for(int l = j+1;l<2*x;l++){
                augm[k][l] = augm[k][l] - (scalar*augm[i][l]);
            }
            augm[k][j] = 0;
        }
    }

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