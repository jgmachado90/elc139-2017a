#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>

#define MASTER 0
#define COL 33

char text[] = "crackme.txt";

//COLOCA OS DADOS DO ARQUIVO NO VETOR
char** preenche_matriz(char** a){
	int ch;
	FILE *fp = fopen(text,"r");
	if(fp == NULL){
		printf("Erro ao abrir arquivo\n");
		MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		exit(EXIT_FAILURE); 
  }
	int i=0;int j=0;
	do{
	  ch = fgetc(fp);
		if( feof(fp) )
		{
			break ;
		}
		//printf("%c", ch);
		if(ch != '\n'){			
			a[i][j] = ch;
			j++;
		}
		else{
			a[i][j]= '\0';
			i++;
			j = 0;
		}   	 
	}while(1);
	fclose(fp);
return a;
}


//CONTA LINHAS
int conta_linha(){
	int lines,ch;
	FILE *fp = fopen(text,"r");
	if(fp == NULL){
		printf("Erro ao abrir arquivo\n");
		MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		exit(EXIT_FAILURE); 
	}
	int j = 0;int i = 0;
	while(!feof(fp)){
		ch = fgetc(fp);
		if(ch == '\n'){
			lines++;
		}
	}  
	fclose(fp);
	return lines;
}

//funcao para pegar tempo
long wtime()
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return t.tv_sec*1000000 + t.tv_usec;
}

//imprime a matriz
void imprime(char **a,int lines){
	int i ,j;	
	for(i = 0 ; i < lines ; i++){
		for(j = 0 ; j < COL; j++){			
			printf("%c",a[i][j]);
		}
		printf("\n");
	}
}

//libera matrizes
char** libera(char **mat,int lines){
	int i;	
	printf("liberando \n");
	for (i=0; i<lines; i++) free (mat[i]); /* libera as linhas da matriz */
  free (mat);   
	return NULL;
}
//aloca matriz
char** malloca(char **a, int lines){
	a = (char **) malloc(sizeof(char*) * lines);
	if (a == NULL ) {
		printf("Erro de alocacao de memoria\n");
		MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		exit(EXIT_FAILURE);   
  }
	int i;
	for(i = 0 ; i < lines ; i++){
		a[i] = (char*) malloc (sizeof(char)* COL);
		if (a[i] == NULL ) {
			printf("Erro de alocacao de memoria\n");
			MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
			exit(EXIT_FAILURE);
   	}
	}
	return a;
}

int main(int argc, char **argv)
{
	int taskid, ntasks;
	long start_time, end_time;
	int lines=0;	
	char **a;
	int linhaaux = 0;
	int contdeprocessos,dest, source, rc, count, tag=1;
	MPI_Status Stat;
	
  //Conta linhas do arquivo
	lines = conta_linha();
	
	  

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
	MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

	if (taskid == MASTER)
	{
		int i;		
		//Maloca matriz// 
		a = malloca(a,lines);
		printf("%d \n",lines);
	
		//Preenche matriz
		a = preenche_matriz(a);
	
		//Imprime matriz
		//imprime(a,lines);		

		if( (ntasks-1) > lines){
			ntasks = lines;
		}
		if(ntasks <=1){
			printf("n de tarefas invalido\n");
			MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
			exit(EXIT_FAILURE); 
		}
		contdeprocessos = lines;
				
		for (i = 1; i < ntasks; i++){
			rc = MPI_Send(&linhaaux, 1, MPI_INT, i, tag, MPI_COMM_WORLD);				
			linhaaux++;					
		}
		
		for (i = 1; i < ntasks; i++){
			rc = MPI_Recv(&linhaaux, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &Stat);
			//printf("%d",rc);	   
			rc = MPI_Get_count(&Stat, MPI_INT, &count);
  		printf("Processo %d: Recebeu %d int  from task %d com a tag %d \n",
        	 			taskid, count, Stat.MPI_SOURCE, Stat.MPI_TAG);
		
		}
	//a	=	libera(a,lines);	
	/*
		
		while(contdeprocessos != 0){			
				
			rc = MPI_Recv(&linhaaux, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &Stat);
			contdeprocessos--;
			rc = MPI_Get_count(&Stat, MPI_INT, &count);
			rc = MPI_Send(&linhaaux, 1, MPI_INT,Stat.MPI_SOURCE, tag, MPI_COMM_WORLD);
			linhaaux++;
			printf("teste");			
			}			
					
	*/	
	}
	else if(taskid != 0)
	{
		dest = 0;
  	source = 0;
		rc = MPI_Recv(&linhaaux, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
    rc = MPI_Send(&linhaaux, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
		//printf("%d,%d\n",linhaaux,taskid);		
	}
	if (taskid < ntasks && taskid > 0) {
	  rc = MPI_Get_count(&Stat, MPI_INT, &count);
  	printf("Processo %d: Recebeu %d int  from task %d com a tag %d \n",
        	 taskid, count, Stat.MPI_SOURCE, Stat.MPI_TAG);
  }
	

	MPI_Finalize();
	return EXIT_SUCCESS;
}
