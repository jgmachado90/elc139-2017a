#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#define COL 33
#define PATH_MAX 2048

char text[] = "crackme.txt";

//conta linhas
int conta_linha(){
	FILE *fp = fopen(text,"r");
	if(fp == NULL) {
  	printf("Erro ao abrir arquivo\n");
		exit(1); 
  }
	int lines,ch;
	while(!feof(fp)){
		ch = fgetc(fp);
		if(ch == '\n'){
			lines++;
		}
	}  
  fclose(fp);
	return lines;
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
		printf("%c", ch);
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
	char path[PATH_MAX];
	
	fp = popen(comand, "r");
	if (fp == NULL)
		exit(1);
	
	while (fgets(path, PATH_MAX, fp) != NULL)
		printf("%s", path);
    
	status = pclose(fp);
  if (status == -1) {
  	exit(1);     
  } 
	else {
        /* Use macros described under wait() to inspect `status' in order
           to determine success/failure of command executed by popen() */
        
  }
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
	printf("liberando \n");
	for (i=0; i<lines; i++) free (mat[i]); /* libera as linhas da matriz */
  free (mat);   
	return NULL;
}
//aloca a matriz
char** malloca(char **a, int lines){
	a = (char **) malloc(sizeof(char*) * lines);
	if (a == NULL ) {		
		printf("Erro de alocacao de memoria\n");
		exit(1);   
  }
	int i;
	for(i = 0 ; i < lines ; i++){
		a[i] = (char*) malloc (sizeof(char)* COL);
		if (a[i] == NULL ) {
			printf("Erro de alocacao de memoria\n");
			exit(1);
   	}
	}
	return a;
}

int main(int argc, char* argv[]){
	int n;
	long start_time, end_time;
	int ch=0;
	int lines=0;	
	char **a;
	int i,j;

	//Conta linhas
	lines = conta_linha();
	
	//Maloca vetor 
	a = malloca(a,lines);
	printf("%d \n",lines);
	
	//Preenche matriz
	a = p_matriz(a);

	//Imprime matriz
	//imprime(a,lines);  

		
	char Str[80];

	start_time = wtime();
	for(i = 0 ; i < lines ; i++){
		strcpy (Str,"./mdcrack -M MD5 -s abcdefghijklmnopqrstuvwxyz ");
		strcat (Str,a[i]);
		puts (Str);
		get_popen(Str);
	}
	end_time = wtime();
	printf("Tempo: %ld", (long) (end_time - start_time));

	a	=	libera(a,lines);
		


return 0;
}