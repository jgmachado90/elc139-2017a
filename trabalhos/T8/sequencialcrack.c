#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#define COL 33
#define PATH_MAX 2048

char text[] = "crackme.txt";

//conta linhas
int conta_linha(){
	FILE *stream;
	char *line = NULL;
	size_t len = 33;
	ssize_t nread;
	int cont = 0;
	stream = fopen(text, "r");
	if (stream == NULL) {
		perror("fopen");
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


//preenche matriz
char** p_matriz(char **a){
	FILE *fp = fopen(text,"r");
	if(fp == NULL) {
		printf("Erro ao abrir arquivo\n");
		exit(1);
	}
	int i=0,j=0,ch;
	do {
		ch = fgetc(fp);
		if( feof(fp) ) {
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

//pega o tempo
long wtime()
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return t.tv_sec*1000000 + t.tv_usec;
}

//printa o arquivo na tela
void get_popen(char comand[]){

	FILE *fp;
	int status;
	int cont = 0,i,cont2 =0,anterior = 0;
	int c,entrei = 0,acabou = 0;	
	char *resp = (char*)malloc(sizeof(char)*COL); 	
	fp = popen(comand, "r");
	if (fp == NULL)
		exit(1);
	
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
	//print da senha comentado
	fprintf(stderr,"Senha crackeada: ");
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
	free(resp);
	resp = NULL;
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

//libera a matriz
char** libera(char **mat,int lines){
	int i;
	printf("\nliberando \n");
	for (i=0; i<lines; i++) free (mat[i]); /* libera as linhas da matriz */
	free (mat);
	return NULL;
}
//aloca a matriz
char** malloca( int lines){
	char **a = (char **) malloc(sizeof(char*) * lines);
	if (a == NULL ){
		printf("Erro de alocacao de memoria\n");
		exit(1);
	}
	int i;
	for(i = 0 ; i < lines ; i++){
		a[i] = (char*) malloc (sizeof(char)* COL);
		if (a[i] == NULL ){
			printf("Erro de alocacao de memoria\n");
			exit(1);
		}
	}
	return a;
}

int main(int argc, char* argv[]){
	long start_time, end_time;
	int lines=0;
	char **a;
	int i;

	//Conta linhas
	lines = conta_linha();

	//Maloca vetor
	a = malloca(lines);
	printf("linhas: (%d) \n",lines);

	//Preenche matriz
	a = p_matriz(a);

	//Imprime matriz
	//imprime(a,lines);


	char Str[150];

	start_time = wtime();
	for(i = 0 ; i < lines ; i++){
		strcpy (Str,"./mdcrack -M MD5 -s abcdefghijklmnopqrstuvwxyz ");
		strcat (Str,a[i]);
		puts (Str);

		get_popen(Str);
	}
	end_time = wtime();
	printf("Tempo: %ld", (long) (end_time - start_time));

	a = libera(a,lines);



	return 0;
}
