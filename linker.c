/*
	Instituição: UERGS
	Curso: Engenharia de computação
	Data: 09/12/2017
	Autor: Guilherme Lague
	E-mail: guilhermelaguebmx@gmail.com
	Descrição: Ligador de dois passos
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int x = 0, y = 0;

//==================================================================
// struct de tabela de instruções
// name: nome da instrução
// cod: codigo da instrução 
//==================================================================
struct Tab_instruction{
    char name[5];
    int cod;    
}tab_instruction[17];

//=================================================================
// Struct de linha lida do arquivo
// word: palavra lida
// num_word: numero de palavras na linha
// m3: indica se existem 3 palavras na linha
//=================================================================
struct Word_line{
    char word[3][12];
    int num_word;
    int m3;   
}word_line;

//===========================================================================
// struct da area de codigo
// label: nome do label
// minemonico: nome da instrução
// variavel: nome da variavel
//===========================================================================
struct program{
	char label[20];
	char minemonico[10];
	char variavel[20];	
}prog[100];

//===========================================================================
// struct da area de dados
// variavel: nome da variavel
// type: SPACE, GLOBAL OU EXTERN
// size: 1
//===========================================================================
struct Data{
	char variavel[10];
	char type[10];
	char size[10];	
}data[32];

void printData();
void printProgram();
void load_instructions();
void passo1(char *file_name);
void passo2(char *file_dest);
void get_line(FILE *file);
int is_instruction(char *word);

//===========================================================================
// Função principal
// argc: numero de argumentos
// argv: vetor de argumentos
//===========================================================================
int main(int argc, char *argv[]){
	int i, num_progs;
	
	num_progs = argc - 2;

	if(argc < 3){
		printf("Numero insuficiente de argumentos!\n Exemplo para montagem: nome_do_programa1.txt nome_do_programa2.txt\n");
	}
	else{
		printf("%d Programas serão ligados...\n", num_progs);

		load_instructions();

		for (i = 1; i <= num_progs; i++){
			printf("Programa[%d]: %s\n", i , argv[i]);
            passo1(argv[i]);
		}

		printf("Ligação concluida com exito! arquivo gerado: \"%s\"\n", argv[num_progs+1]);
		passo2(argv[num_progs+1]);
    }
}

//===========================================================================
// Esta função gera uma tabela de instruções
//===========================================================================
void load_instructions(){
    strcpy(tab_instruction[0].name, "LA"); 
    tab_instruction[0].cod = 0;
    strcpy(tab_instruction[1].name, "SA"); 
    tab_instruction[1].cod = 1;
    strcpy(tab_instruction[2].name, "AA"); 
    tab_instruction[2].cod = 2;
    strcpy(tab_instruction[3].name, "MUL"); 
    tab_instruction[3].cod = 3;
    strcpy(tab_instruction[4].name, "DIV"); 
    tab_instruction[4].cod = 4;
    strcpy(tab_instruction[5].name, "SUB"); 
    tab_instruction[5].cod = 5;
    strcpy(tab_instruction[6].name, "JMP"); 
    tab_instruction[6].cod = 6;
    strcpy(tab_instruction[7].name, "JEQ"); 
    tab_instruction[7].cod = 7;
    strcpy(tab_instruction[8].name, "JGT"); 
    tab_instruction[8].cod = 8;
    strcpy(tab_instruction[9].name, "JLT"); 
    tab_instruction[9].cod = 9;
    strcpy(tab_instruction[10].name, "PW"); 
    tab_instruction[10].cod = 10;
    strcpy(tab_instruction[11].name, "RW"); 
    tab_instruction[11].cod = 11;
    strcpy(tab_instruction[12].name, "STOP"); 
    tab_instruction[12].cod = 12;
    strcpy(tab_instruction[13].name, "SPACE");
    strcpy(tab_instruction[14].name, "GLOBAL");
    strcpy(tab_instruction[15].name, "EXTERN");
    strcpy(tab_instruction[16].name, "END");
}

//===========================================================================
// Esta função verifica se é uma instrução.
// *word: palavra lida do arquivo.
//===========================================================================
int is_instruction(char *word){
    for(int i = 0; i < 15; i++){
        if(strcmp(word, tab_instruction[i].name) == 0){
            return 0;
            break;
        }
    }
    return 1; 
}

//===========================================================================
// Esta função separa as areas de dados e instruções de cada programa.
// file_name: nome do arquivo onde contem o programa
//===========================================================================
void passo1(char *file_name){ //grava tudo nas structs
	FILE *file;


	file = fopen(file_name,"rt");

    if(file == 0){
        printf("Arquivo %s não encontrado!\n", file_name);
        exit(0);
    }
    else{
    	while(!feof(file)){
    	   	get_line(file);

    	   	if(word_line.num_word == 3){ // no caso de declaraçao
	   			if((strcmp(word_line.word[1], "SPACE") == 0) || (strcmp(word_line.word[1], "GLOBAL") == 0)) {
                	strcpy(data[x].variavel, word_line.word[0]);
                	strcpy(data[x].type, word_line.word[1]);
                	strcpy(data[x].size, word_line.word[2]);
                	x++;
                }
            	else{// no caso de labels
            		if(is_instruction(word_line.word[1]) == 0){
            			strcpy(prog[y].label, word_line.word[0]);
            			strcpy(prog[y].minemonico, word_line.word[1]);
            			strcpy(prog[y].variavel, word_line.word[2]);
            			y++;
            		}	
            	}
            }
    	   	else if(word_line.num_word == 2){ // no caso de apenas instrução com variavel
   				if(is_instruction(word_line.word[0]) == 0){
        			strcpy(prog[y].label, word_line.word[0]);
        			strcpy(prog[y].minemonico, word_line.word[1]);
        			strcpy(prog[y].variavel, word_line.word[2]);
        			y++;
        		}	
    	   	}
    	   	else{// no caso de achar stop ou end
    	   		if(strcmp(word_line.word[0], "END") != 0){ // se achar stop, grava
    	   			strcpy(prog[y].label, word_line.word[0]);
	    			strcpy(prog[y].minemonico, word_line.word[1]);
	    			strcpy(prog[y].variavel, word_line.word[2]);
	    			y++;	
    	   		}
    	   	} 
    	}
    }
}

//===========================================================================
// Esta função liga os codigos separados de cada programa em um unico arquivo
// *file_dest: arquivo destino onde será gravado o codigo assembly.
//===========================================================================
void passo2(char *file_dest){ // le as structs e grava tudo no arquivo
	int i = 0;

    FILE *dest;

    dest = fopen(file_dest, "w");
    
    if(dest == 0){
        printf("Arquivo %s não foi criado!\n", file_dest);
    }
    else{
    	for(i = 0; i < y; i++){ // escreve primeiro a area de programa
    		fprintf(dest, "%s %s %s\n", prog[i].label, prog[i].minemonico, prog[i].variavel);
    	}

		for(i = 0; i < x; i++){ // escreve area de dados
    		fprintf(dest, "%s %s %s\n", data[i].variavel, data[i].type, data[i].size);
    	}
    	fprintf(dest, "END\n");    	
    }
}

//===========================================================================
// Esta função le uma linha do arquivo e separa as palavras
// *file: arquivo onde contém o programa. 
//===========================================================================
void get_line(FILE *file){
    char linha[50], *tk;
    int i;

    fgets(linha, 40, file);

    word_line.m3 = 0;
    word_line.num_word = 0;

    word_line.word[0][0] = '\0'; 
    word_line.word[1][0] = '\0';
    word_line.word[2][0] = '\0';
    
    tk = strtok(linha, " ");

    for (i = 0; i < 3 && tk != NULL; ++i) {
        strcpy(word_line.word[i], tk);
        tk = strtok(NULL, " ");
        if (tk == NULL) {
            break;
        }
    }

    word_line.word[i][strlen(word_line.word[i])] = '\0';
    
    if (word_line.word[0][0] == '\0') {
        get_line(file);
    }
    if (i > 0) {
        word_line.num_word = i + 1;
    }
    if (word_line.word[0][0] == '\0') {
        get_line(file);
    }
    if (tk != NULL){
        word_line.m3 = 1;
    }
}
