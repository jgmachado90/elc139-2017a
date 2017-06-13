#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>
#include <string.h>

#define MASTER 0
#define COL 33
#define WORK_TAG    1
#define DATA_TAG    2
#define STOP_TAG    3

char text[] = "crackme.txt";

//CONTA LINHAS
int conta_linha(){
	FILE *stream;
	char *line = NULL;
	size_t len = 33;
	ssize_t nread;
	int cont = 0;

	stream = fopen(text, "r");
	if (stream == NULL) {
		perror("fopen");
		MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		exit(EXIT_FAILURE);
	}

	while ((nread = getline(&line, &len, stream)) != -1) {
		cont++;
	}
	//printf("%d",cont);
	free(line);
	fclose(stream);
	return cont;
	}


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
		if( feof(fp) ){
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
	//fprintf(stderr,"liberando \n");
	for (i=0; i<lines; i++) free (mat[i]); /* libera as linhas da matriz */
  	free (mat);
	return NULL;
}
//aloca matriz
char** malloca(int lines){
	char **a = (char **) malloc(sizeof(char*) * lines);
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
//funcao que executa o comando md
char* get_popen(char comand[],char *resp){

	FILE *fp;
	int status;
	int cont = 0,i,cont2 =0,anterior = 0;
	int c,entrei = 0,acabou = 0;
	fp = popen(comand, "r");
	if (fp == NULL)
		exit(1);
//inicializa o vetor de resp
	for( i = 0; i < COL; ++i){
		*(resp+i) = ' ';
	}
// percorre a saida e busca somente a senha requerida : -> possivel melhora necessaria para desempenho
	while ((c = fgetc (fp)) != EOF){
		if(c == '='){//ve se o char eh =
			anterior = 1;
		}
		if(anterior == 1 && c == '>'){// se o anterior for = e o atual > coloca um marcador
			cont = 1;
		}
		if(c != '='){//se o char for difente o anterior fica 0
			anterior = 0;
		}
		if(entrei == 1 && (c == '\n' || c == '\0')){//entrei comeca em 0 logo esse if eh para quando ja achou a senha e precisa colocar o /0
			*(resp+cont2) = '\0';
			acabou = 1;
		}

		if(cont == 1 && cont2 <= 31 && c !='\n' && c != ' ' && acabou == 0){//marcador em 1 logo passou do => e aqui entra na seta > depois na 1 letra, marca o entrei
			*(resp+cont2) = c;
			cont2++;
			entrei = 1;
		}
		if(cont2 == 32 && acabou == 0){//caso estoure o limite de 32 na senha só é colocado o /0 e avisado
			*(resp+cont2) = '\0';printf("senha muito grande");
		}
		//putchar (c);
	}
	//print da senha 
	for( i = 0; i < COL; ++i){
		fprintf(stderr,"%c", *(resp+i));
	}
	fprintf(stderr,"\n");
	status = pclose(fp);
	if (status == -1) {
		exit(1);
	}
	else {
        /* Use macros described under wait() to inspect `status' in order
           to determine success/failure of command executed by popen() */

	}
	return resp;

}

int main(int argc, char **argv)
{
	int taskid, ntasks;
	long start_time, end_time;
	int lines=0;
	char **a;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
	MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

	//Conta linhas do arquivo -->funciona se tiver com uma linha em branco para baixo ,linhas extras em branco causam conflito para compilacao --> nao tratado pois nao era o foco
	lines = conta_linha();
	//fprintf(stderr,"%d \n",lines);
	//Maloca matriz//
	a = malloca(lines);
	//Preenche matriz
	a = preenche_matriz(a);
	//imprime(a,lines);
	if (taskid == MASTER)
	{
		fprintf(stderr,"lines (%d) \n",lines);
		int i,nproc = 0,j;
		MPI_Status status;
		int linhaaux = 0,linha_parte;
		int id;
		int resto = lines - (ntasks-1);
		char respostas[lines][COL];
		char *aux2 = (char*)malloc(sizeof(char)*COL);
		//caso se tenha mais processos q linhas deve se parar os processos extras criados pelo mpi, e ajustar o ntasks para o for enviar.s
		if( (ntasks-1) > lines){
		// linha = 2 proc = 5 escravo = 4 ,0 1 2 3 4.			
			j = lines+1;
			//printf("entrou %d\n",j);			
			for(i = j ; i < ntasks; i++){
				MPI_Send(&linhaaux, 0, MPI_INT, i, STOP_TAG, MPI_COMM_WORLD);
			}			
			ntasks = lines+1;
		}
		//numero minimo para executar o programa com mestre e escravo
		if(ntasks <=1){
			printf("n de tarefas invalido\n");
			MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
			exit(EXIT_FAILURE);
		}

		start_time = wtime();

		//manda os processos iniciais , soma processos ativos e linha da matriz para enviar
		for (i = 1; i < ntasks; i++){
			MPI_Send(&linhaaux, 1, MPI_INT, i, WORK_TAG, MPI_COMM_WORLD);
			//fprintf(stderr,"for,enviei %d para o %d\n ",linhaaux,i);
			++linhaaux;
			++nproc;
		}
		//linha inicial para que quando o 1 processo voltar possa enviar
		linha_parte = linhaaux-1;
		
		while(nproc > 0){//enquanto existir processos ativos
			MPI_Recv(&linhaaux, 1, MPI_INT, MPI_ANY_SOURCE,DATA_TAG, MPI_COMM_WORLD, &status);
			MPI_Recv(aux2, COL, MPI_CHAR, status.MPI_SOURCE,DATA_TAG, MPI_COMM_WORLD, &status);//recebe o vet de resposta da mesma source que a linha para evitar condicao de corrida

			for(i = 0; i < COL ; i++){
				respostas[linhaaux][i] = *(aux2+i); //coloca as respostas na posicao certa
			//fprintf(stderr,"%c",respostas[linhaaux][i]);
			}
			//fprintf(stderr,"\n");
			--nproc;//chegou um processo diminuir ativos
			//printf("np %d\n",nproc);
			id = status.MPI_SOURCE;

			//fprintf(stderr,"id %d linhaaux: %d \n",id,linhaaux);

			if(resto > 0){//numero de processos restantes
				++linha_parte;//correcao da linha para mandar

			//fprintf(stderr," laço id %d linhaparte: %d \n",id,linha_parte);

				MPI_Send(&linha_parte, 1, MPI_INT, id, WORK_TAG, MPI_COMM_WORLD);
				++nproc;//enviou processo aumentar ativos
				--resto;// diminuir de quantos processos faltam
			}
			
			if(nproc == 0){//quando os processos chegarem em 0 mata todos com a tag
				for(i = 1 ; i < ntasks ; i++){
					MPI_Send(&linhaaux, 0, MPI_INT, i, STOP_TAG, MPI_COMM_WORLD);
				}
			}
		}
		end_time = wtime();
		fprintf(stderr,"Tempo: %ld \n", (long) (end_time - start_time));

		//imprime matrz de RESPOSTAS
		for(i = 0 ; i < lines ; i++){
			for(j = 0 ; j < COL ; j++){
				fprintf(stderr,"%c",respostas[i][j]);
			}
			fprintf(stderr,"\n");
		}
		//----------------------------
		free(aux2);
		aux2 = NULL;

	}
	else {
		MPI_Status status;
		int linhaaux = 0,i;
		char aux[COL];
		char Str[COL * 4];
		//char respfinal[COL];
		char *resp = (char*)malloc(sizeof(char)* COL);
		while ( ((MPI_Recv(&linhaaux, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD,&status)) == MPI_SUCCESS) &&
					(status.MPI_TAG == WORK_TAG) )//recebe a linha com a tag de trabalho
		{
			//fprintf(stderr," recebi do mestre a linha %d,task %d\n",linhaaux,taskid);


			for(i = 0 ; i < COL; i++){//for para passar ao auxiliar para concatenar usando a funcao
				aux[i] = a[linhaaux][i];
			}


			/*funcao que executa o md5 concatenando para mandar a get_popen*/
			strcpy (Str,"./mdcrack -M MD5 -s abcdefghijklmnopqrstuvwxyz ");
			strcat (Str,aux);
			puts (Str);
			resp = get_popen(Str,resp);
			//-------------------------------//
			//for(i = 0 ; i < COL ; i++){
				//respfinal[i] = *(resp+i);
				//respfinal[i] = 'c';
				//fprintf(stderr,"%c",respfinal[i]);
			//}
			//fprintf(stderr,"\n");
			//fprintf(stderr,"ACABEI %d",taskid);
			MPI_Send(&linhaaux, 1, MPI_INT, 0, DATA_TAG, MPI_COMM_WORLD);
			MPI_Send(resp, COL, MPI_CHAR, 0, DATA_TAG, MPI_COMM_WORLD);

		}
		free(resp);
		resp = NULL;
	}

	a = libera(a,lines);

	MPI_Finalize();
	return EXIT_SUCCESS;
}
