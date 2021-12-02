#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#define n 2048

double start, end, total=0;
int vivos = 0;
int getNeighbours(int **grid, int i, int j){
        int vizinhos = 0;
        int a1 = i-1, a2 = i , a3 = i+1, b1 = j-1, b2 = j, b3 = j+1; //a == i, b == j
         // bordas horizontais
        if(i == n-1){
            a1 = n-2;
            a2 = n-1;
            a3 = 0;
        }
        if(i == 0){
           a1 = n-1;
           a2 = 0;
           a3 = 1;
        }
        // bordas verticais
        if(j == n-1){
            b1 = n-2;
            b2 = n-1;
            b3 = 0;
        }
        if(j == 0){
            b1 = n-1;
            b2 = 0;
            b3 = 1;
        }

        vizinhos += grid[a1][b2] + grid[a1][b3] + grid[a1][b1] + grid[a2][b3] + grid[a2][b1] + grid[a3][b1] + grid[a3][b2] + grid[a3][b3];
        return vizinhos;
 }

void vivasso(int **grid){
    int i,j;
    vivos =0;
    start = omp_get_wtime();
	{
	    for(i=0;i<n;i++){
	        for(j=0;j<n;j++){
	            vivos= vivos+grid[i][j];
	        }
	    }
	}
	end = omp_get_wtime();
    total += end - start;
}

void next_generation(int **grid, int **newgrid){    // JOGO DA VIDA
    int vizinhos,i,j,th_id;
    
    #pragma omp parallel 
    {
        #pragma omp for private(i,j,vizinhos)
            for(i=0;i<n;i++){
                for(j=0;j<n;j++){
                    vizinhos = getNeighbours(grid,i,j);
                    if(grid[i][j] == 1){
                        if(vizinhos < 2){
                            newgrid[i][j] = 0;
                        }
                        else if(vizinhos == 2 || vizinhos == 3){
                            newgrid[i][j] = 1;
                        }   
                        else if(vizinhos >= 4){
                            newgrid[i][j] = 0;
                        }
                    }
                    else{
                        if(vizinhos == 3){
                            newgrid[i][j] = 1;
                        }
                    }
                }
            }
    }
    
}

int main(){
    int i,j,k,pr=0; 
    int **grid,**newgrid,quantidade;
    int lin = 1, col = 1;

    grid = (int **) calloc(n, sizeof(int *));
    for(i=0;i<n;i++){
        grid[i] = (int *) calloc(n, sizeof(int));
    }
    newgrid = (int **) calloc(n, sizeof(int *));
    for(i=0;i<n;i++){
        newgrid[i] = (int *) calloc(n, sizeof(int));
    }

    //GLIDER
    grid[lin  ][col+1] = 1;
    grid[lin+1][col+2] = 1;
    grid[lin+2][col  ] = 1;
    grid[lin+2][col+1] = 1;
    grid[lin+2][col+2] = 1;

    //R-pentomino
    lin =10; col = 30;
    grid[lin  ][col+1] = 1;
    grid[lin  ][col+2] = 1;
    grid[lin+1][col  ] = 1;
    grid[lin+1][col+1] = 1;
    grid[lin+2][col+1] = 1;

    omp_set_num_threads(4); //numero de threads
    for(k=0;k<2000;k++){
        next_generation(grid,newgrid);
        for(i=0;i<n;i++){
            for(j=0;j<n;j++){
                grid[i][j] = newgrid[i][j];
            }
        }
        // if(pr<5){
        //     for(i=0;i<50;i++){
        //         for(j=0;j<50;j++){
        //             printf("%d ",grid[i][j]);
        //         }
        //     }
        //     pr++;
        // }
    }
    vivasso(grid);
    printf(" %d\n",vivos);
    printf("Durou %f segundos a area paralela.\n", total);
    return 0;
}
