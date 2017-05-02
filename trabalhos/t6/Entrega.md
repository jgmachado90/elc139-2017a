João Gonçalves Machado - t6 - Programação Paralela

particionamento:

      part_size = n / ntasks;

comunicação

		 MPI_Recv(&dot, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		 MPI_Send(&mydot, 1, MPI_DOUBLE, MASTER, msg_tag, MPI_COMM_WORLD);
   		 Os processos filhos enviam os seus calculos

aglomeração

		 if (taskid == MASTER){
      		for (i = 0; i < ntasks-1; i++)
      	{
         MPI_Recv(&dot, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
         printf("Valor parcial recebido do processo %d = %f\n", status.MPI_SOURCE, dot);
         mydot += dot;
      	}
      	printf("Produto escalar = %f\n", mydot);
      	printf("Tempo de calculo = %ld usec\n", (long) (end_time - start_time));      
   		}


mapeamento

		MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
		MPI_Comm_size(MPI_COMM_WORLD, &ntasks);





vetor:30000000  
repetições:100  
processos: 1
lsc4 Tempo de calculo = 15504566 usec

vetor:30000000  
repetições:100  
processos: 2
lsc4 Tempo de calculo = 7932547 usec

vetor:30000000  
repetições:100  
processos: 4
lsc4 Tempo de calculo = 4155842 usec
