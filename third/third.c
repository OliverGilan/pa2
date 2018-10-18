#include <stdio.h>
#include <stdlib.h>

int fill(int **);
void clean(int **, int, int);

int main(int argc, char **argv)
{
    FILE *input;
    input = fopen(argv[1], "r");
    if(input == NULL){
        printf("No file\n");
        return 0;
    }

    int **grid = (int **)calloc(9, sizeof(grid));
    for(int i=0;i<9;i++){
        grid[i] = (int *)calloc(9,sizeof(grid[i]));
        int n = 0;
        while(n<9){
            char c;
            fscanf(input, "%c ", &c);
            if(c == '-'){
                grid[i][n] = 0;
            }else{
                int cd = c-'0';
                grid[i][n] = cd;
            }
            n++;
        }
    }
    fclose(input);

    fill(grid);
    for(int i = 0; i < 9; i++){
        for(int j=0;j<9;j++){
            if(grid[i][j] == 0){
                printf("no-solution");
                return 0;
            }
        }
    }
    for(int i = 0; i < 9; i++){
        for(int j=0;j<9;j++){
            printf("%d\t", grid[i][j]);
        }
        printf("\n");
    }

    clean(grid, 9, 9);
    
    
    return 0;
}

void clean(int **matrix, int x, int y){
    for(int i=0;i<x;i++){
        free(matrix[i]);
    }
    free(matrix);
}

int fill(int **grid){
    for(int i = 0; i < 9; i++){
        for(int j=0;j<9;j++){
            if(grid[i][j] == 0){
                int *set = (int *)calloc(9, sizeof(int));
                for(int k=0; k<9;k++){
                    set[k] = k+1;
                }
                //Check row
                for(int l=0; l<9; l++){
                    if(grid[i][l] != 0){
                        set[grid[i][l]-1] = 0;
                    }
                }
                //Check col
                for(int l=0; l<9; l++){
                    if(grid[l][j] != 0){
                        set[grid[l][j]-1] = 0;
                    }
                }
                //Check subgrid
                for(int l=(i-(i%3)); l<(i-(i%3)+3); l++){
                    for(int m=(j-(j%3)); m<(j-(j%3))+3; m++){
                        if(grid[l][m] != 0)
                        set[grid[l][m]-1] = 0;
                    }
                }
                //Count possibilities
                int count = 0;
                for(int k=0; k<9;k++){
                    if(set[k]!= 0){
                        count++;
                    }
                }
                //Return as failure if no possible solution
                if(count == 0){
                    return 1;
                }
                int tries = 0;
                for(int k=0; k<9;k++){
                    if(set[k]!= 0 && grid[i][j] == 0){
                        grid[i][j] = set[k];
                        set[k] = 0;
                        int ans = fill(grid);

                        //Return as failure if all possible solutions have been tried
                        if(ans == 1){
                            tries++;
                            grid[i][j] = 0;
                            if(tries == count){
                                return 1;
                            }
                        }
                    }
                }
                free(set);
            }
        }
    }
    return 0;
}