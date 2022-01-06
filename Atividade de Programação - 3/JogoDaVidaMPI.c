#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "mpi.h"
#define n 2048

double start, end, total=0;
//int vivos = 0;
int getNeighbours(int **grid, int i, int j, int noProcesses){
        int vizinhos = 0;
        int a1 = i-1, a2 = i , a3 = i+1, b1 = j-1, b2 = j, b3 = j+1; //a == i, b == j
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

int main(int argc, char * argv[]){
    int i,j,k; 
    int processId; /* rank dos processos */
    int noProcesses; /* Número de processos */
    int anterior, proximo;
    MPI_Status status;
    int **grid,**newgrid;
    int lin = 2, col = 1;
    int vivos;
    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &processId);
    MPI_Comm_size (MPI_COMM_WORLD, &noProcesses);
    
    grid = (int **) calloc((n/noProcesses+2), sizeof(int *));
    for(i=0;i<2+n/noProcesses;i++){
        grid[i] = (int *) calloc(n, sizeof(int));
    }
    newgrid = (int **) calloc((n/noProcesses), sizeof(int *));
    for(i=0;i<n/noProcesses;i++){
        newgrid[i] = (int *) calloc(n, sizeof(int));
    }

    if(processId == 0){
        //GLIDER
        grid[lin  ][col+1] = 1;
        grid[lin+1][col+2] = 1;
        grid[lin+2][col  ] = 1;
        grid[lin+2][col+1] = 1;
        grid[lin+2][col+2] = 1;

        //R-pentomino
        lin =11; col = 30;
        grid[lin  ][col+1] = 1;
        grid[lin  ][col+2] = 1;
        grid[lin+1][col  ] = 1;
        grid[lin+1][col+1] = 1;
        grid[lin+2][col+1] = 1;
    }
    

    anterior = processId - 1;
    if(anterior < 0) anterior = noProcesses - 1;
        
    proximo = processId + 1;
    if(proximo >= noProcesses) proximo = 0;

    double tempo = 0.0;
    if(processId == 0) tempo -= MPI_Wtime();

    for(k=0;k<2000;k++){
        MPI_Sendrecv(grid[1], n, MPI_INT, anterior, 1, grid[n/noProcesses+1], n, MPI_INT, proximo, 1, MPI_COMM_WORLD, &status);
        MPI_Sendrecv(grid[n/noProcesses], n, MPI_INT, proximo, 2, grid[0], n, MPI_INT, anterior, 2, MPI_COMM_WORLD, &status);

        int vizinhos;
        for(i=1;i<=n/noProcesses;i++){
            for(j=0;j<n;j++){
                vizinhos = getNeighbours(grid,i,j,noProcesses);
                if(grid[i][j] == 1){
                    if(vizinhos < 2){
                        newgrid[i-1][j] = 0;
                    }
                    else if(vizinhos == 2 || vizinhos == 3){
                        newgrid[i-1][j] = 1;
                    }   
                    else if(vizinhos >= 4){
                        newgrid[i-1][j] = 0;
                    }
                }
                else{
                    if(vizinhos == 3){
                        newgrid[i-1][j] = 1;
                    }
                }
            }
        }
        
        for(i = 0; i < n/noProcesses; i++){
            for(j = 0; j < n; j++){
                grid[i+1][j] = newgrid[i][j];
            }
        }
    }
    vivos = 0;
    for(i = 1; i <= n/noProcesses; i++){
        for(j = 0; j < n; j++){
            vivos += grid[i][j];
        }
    }
    int totalVivos;
    MPI_Reduce(&vivos, &totalVivos, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        if(processId==0){
            printf("Geração: %d vivos: %d\n",k,totalVivos);
        }
    if(processId == 0){
        tempo+=MPI_Wtime();
        printf("Parte paralela: %lf segundos", tempo);
    }
    MPI_Finalize();
    return 0;
}