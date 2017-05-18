/*
  Algoritmo de Dijkstra
  Fernando Emilio Puntel
  Programa de Pós-Graduação em Informática
  Universidade Federal de Santa Maria
  Maio/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <mpi.h>

// Total de cidades para construção do grafo
#define TOTALCIDADES 400

#define MASTER 0

int taskid, ntasks;

// Grafo de distancia entre as cidades
int distancias[TOTALCIDADES*TOTALCIDADES];
double custos[TOTALCIDADES][TOTALCIDADES];

/*
  Funcao criaGrafo
  - Rand para o total de ligacoes que o grafo irá ter
  - Rand para qual cidade origem e destino
  - Verifica se nao eh a mesma cidade e se ja nao possui ligacao entre as mesmas
  - Rand para a distância
  - Vetor distancias:
      - Pos 0 ate (TOTALCIDADES - 1) = cidade 1
      - Pos (TOTALCIDADES) ate ((2*TOTALCIDADES) -1) = cidade 2
*/
void criaGrafo(){
  int origem, destino, totalLigacoes, i, ok, distancia;

  totalLigacoes = rand() % (TOTALCIDADES * 4);
  if (taskid == MASTER)
    printf("TOTAL LIGACOES: %i\n", totalLigacoes);

  for(i = 0; i < totalLigacoes; i++){
    ok = 0;
    while (ok == 0){
      origem = rand() % TOTALCIDADES;
      destino = rand() % TOTALCIDADES;
      if (origem != destino){
        if (distancias[(origem) * TOTALCIDADES + destino] == -1){
         distancia = (rand() % 20) + 1;
         distancias[(origem) * TOTALCIDADES + destino] = distancia;
         ok = 1;
          if (taskid == MASTER)
           printf ("Ligacao entre as cidades: %i e %i com distancia: %i\n", origem, destino, distancia);
       }
      }
    }
  }
}

/*
  Funcao menorCaminho
  - Recebe a origem e destino para calculo
  - Aloca vetor necessário
  - Verifica as ligacoes que direta que a "cidade" possui
  - Por fim, é feito o calculo do menor caminho
  - Impresso o resultado
*/

void dijkstra(int origem, int destino){
  int *anterior, i, aux;
  int *verticesNoCaminho, calculo; // Vertices que sao acessados para o menor caminho
  double distMinima, auxDist; // Custoo com os caminhos
  double custoslocais[TOTALCIDADES]; //Representação de custos para a função dijkstra############################

  verticesNoCaminho = calloc (TOTALCIDADES, sizeof (int *));
  if (verticesNoCaminho == NULL){
    printf("ERROR: Erro na alocacao \n");
    printf("ERROR: Erro na alocacao \n");
    exit(-1);
  }


  for (i = 0; i < TOTALCIDADES; i++){
    verticesNoCaminho[i] = 0;
    if (distancias[(origem) * TOTALCIDADES + i] != -1){
      custoslocais[i] = distancias[(origem) * TOTALCIDADES + i];
    }else{
      custoslocais[i] = HUGE_VAL;
    }
  }

  verticesNoCaminho[origem] = 1;
  custoslocais[origem] = 0;

  // Principal laço
  do{
    distMinima = HUGE_VAL;

    for(i = 0; i < TOTALCIDADES; i++){
      if (!verticesNoCaminho[i]){
        if (custoslocais[i] >= 0 && custoslocais[i] < distMinima){
            distMinima = custoslocais[i];
            aux = i;
        }
      }
    }

    if (distMinima != HUGE_VAL && aux != destino){
      verticesNoCaminho[aux] = 1;
      for(i = 0; i < TOTALCIDADES; i++){
        if (!verticesNoCaminho[i]){
          if (distancias[aux * TOTALCIDADES + i] != -1 && custoslocais[aux] + distancias[aux * TOTALCIDADES + i] < custoslocais[i]){
            custoslocais[i] = custoslocais[aux] + distancias[aux * TOTALCIDADES + i];
          }
        }
      }
    }
  }while (aux != destino && distMinima != HUGE_VAL);

  //Salva na matriz custos o valor calculado da distancia, da origem até o destino
  custos[origem][destino] = custoslocais[destino];

  //Libera memória alocada pro vetor de vertices
  free(verticesNoCaminho);
}

/*
  Funcao calculoDistancia
  - Dois for's que chamam a funcao para calculo do menor caminho
*/

void calculoDistancia(){
  int i, j;

  int totalwork = TOTALCIDADES*TOTALCIDADES;
  int mywork = totalwork/ntasks;
  int nomanwork = totalwork%ntasks;

  for(i=(taskid*mywork); i< taskid*mywork+mywork; i++)
    dijkstra(i/TOTALCIDADES, i%TOTALCIDADES);

  //restinho de trabalho
  int lastworkdone = totalwork - nomanwork;
  if(taskid < nomanwork){
    i = lastworkdone + taskid;
    dijkstra(i/TOTALCIDADES, i%TOTALCIDADES);
  }

  double* custosReduce = NULL;
  if(taskid == MASTER)
    custosReduce = calloc(TOTALCIDADES*TOTALCIDADES, sizeof(double));

  MPI_Reduce(custos, custosReduce, totalwork, MPI_DOUBLE, MPI_MAX, MASTER, MPI_COMM_WORLD);

  if(taskid == MASTER){
    for(i=0;i<TOTALCIDADES;i++){
      for(j=0;j<TOTALCIDADES;j++){
         //IMPRIME RESULTADO
        //printf("Distancia de %i ate %i: ", i, j);
        //printf("Custo: %lf\n", custosReduce[i * TOTALCIDADES + j]);

      }
    }
}
  if (taskid == MASTER)
    free(custosReduce);
}



/*
  Todas as distancias e custos sao zeradas, pois na hora do algoritmo eh verificado
  as cidades que tem ligacoes.
*/
void zeraDistancia(){
  int i, j;
  for(i = 0; i < TOTALCIDADES * TOTALCIDADES; i++){
    distancias[i] = -1;
  }

  /* Zera custos*/
  for(i = 0; i < TOTALCIDADES; i++){
    for(j=0;j<TOTALCIDADES;j++){
      custos[i][j] = -1;
    }
  }
}

int main(int argc, char** argv){


  double t1, t2;


  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
  MPI_Comm_size(MPI_COMM_WORLD, &ntasks);


  srand((unsigned)TOTALCIDADES);
  zeraDistancia();
  criaGrafo();

  MPI_Barrier(MPI_COMM_WORLD);

  t1 = MPI_Wtime();

  calculoDistancia();

  t2 = MPI_Wtime();

  if(taskid == MASTER)
    printf( "Elapsed time is %f\n", t2 - t1 );

  MPI_Finalize();

  return 0;
}
