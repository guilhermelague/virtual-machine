/*
	Instituição: UERGS
	Curso: Engenharia de computação
	Data: 24/08/2017
	Autor: Guilherme Lague
	E-mail: guilhermelaguebmx@gmail.com
	Descrição: A MV é uma máquina que possui uma memória de 128 palavras, um registrador (PC) que aponta para a
				próxima instrução a ser executada e um acumulador (AC) e dois registradores (R0 e R1) usados para gerenciar
				a memória. A máquina possui as instruções apresentadas na tabela a seguir. A primeira coluna apresenta o
				mnemônico da instrução, a segunda a função que é executada, a terceira o código de operação (em decimal),
				a quarta o número de operandos e a última a descrição da instrução. 
	
	ler um arquivo.txt onde tem apenas os codigos das operações e os numeros e carregar na memoria da MV

*/


#include <stdio.h>
#include <stdlib.h>

// Estrutura da memoria ram da mv
struct memory{
	int operador;
	int operando;
};

// controla a quantidade de linhas de codigo
int total_lines = 0;

// registradores
int R0, R1;

// memoria ram de 128 posições
struct memory mem[128];

// Definição das instruções
enum instruction{
	LA, SA, ADD, MUL, DIV, SUB, JMP, JEQ, JGT, JLT, PW, RW, STOP
};

// Protótipo das funções
void loader();
void list_men();
void cpu();

//===========================================================================
//	Função principal
//===========================================================================
int main(int argc, char *argv[]){
	if(argc < 2){
		printf("Argumentos insulficientes!\n");
		exit(0);
	}
	else{
		loader(argv[1]);
		printf("Carregamento:\n");
		list_men(); 
		cpu();
		printf("\nSaida:\n");
		list_men(); 	
	}
	return 0;
}

//===========================================================================
// Esta função é responsável por carregar o arquivo .txt na memoria da mv
//===========================================================================
void loader(char *file_name){
	FILE *file;

	file = fopen(file_name, "rt");
    
    while(!feof(file)){
        fscanf(file, "%d %d", &mem[total_lines].operador, &mem[total_lines].operando);
        total_lines++;
    }

    fclose(file);
}

//===========================================================================
// Esta função é responsável por listar os dados que estão carregados na memoria, é só para teste
//===========================================================================
void list_men(){
	printf("===============================\n");
	printf("| Linha | Operador | Operando |\n");
	printf("|-----------------------------|\n");

	int j = 0;

	for (j = 0; j < total_lines; j++){
		printf("| %5d | %8d | %8d |\n", j, mem[j].operador, mem[j].operando);
	}

	printf("===============================\n");
}

//===========================================================================
// Esta função é responsável por simular o funcionamento do processador
//===========================================================================
void cpu(){
	int pc = 0;
	float ac = 0;

	while(mem[pc].operador != STOP){
		switch(mem[pc].operador){
			case LA:
				ac = mem[mem[pc].operando].operador;
				//printf("LOAD, pc = %d, ac = %.2f\n", pc, ac);
				pc++;
			break;
			case SA:
				mem[mem[pc].operando].operador = ac;
				//printf("STORE, pc = %d, ac = %.2f, linha[%d] = [%d]\n", pc, ac, mem[pc].operando, mem[mem[pc].operando].operador);
				pc++;
			break;
			case ADD:
				ac = ac + mem[mem[pc].operando].operador;
				//printf("ADD, pc = %d, ac = %.2f\n", pc, ac);
				pc++;
			break;
			case MUL:
				ac = ac * mem[mem[pc].operando].operador;
				//printf("MULT, pc = %d, ac = %.2f\n", pc, ac);
				pc++;
			break;
			case DIV:
				ac = ac / mem[mem[pc].operando].operador;
				//printf("DIV, pc = %d, ac = %.2f\n", pc, ac);
				pc++;
			break;
			case SUB:
				ac = ac - mem[mem[pc].operando].operador;
				//printf("SUB, pc = %d, ac = %.2f\n", pc, ac);
				pc++;
			break;
			case JMP:
				pc = mem[pc].operando;
				//printf("JMP, pc = %d, ac = %.2f\n", pc, ac);
			break;
			case JEQ:
				if(ac == 0){
					pc = mem[pc].operando;
					//printf("JEQ, pc = %d, ac = %.2f\n", pc, ac);
				}
				else{
					pc++;
				}
			break;
			case JGT:
				if(ac > 0){
					pc = mem[pc].operando;
					//printf("JGT, pc = %d, ac = %.2f\n", pc, ac);
				}
				else{
					pc++;
				}
			break;
			case JLT:
				if(ac < 0){
					pc = mem[pc].operando;
					//printf("JLT, pc = %d, ac = %.2f\n", pc, ac);
				}
				else{
					pc++;
				}
			break;
			case PW:
				//printf("PW, pc = %d, ac = %.2f\n", pc, ac);
				printf("WRITE: %d\n", mem[mem[pc].operando].operador);
				pc++;
			break;
			case RW:
				//printf("RW, pc = %d, ac = %.2f\n", pc, ac);
				printf("Digite um valor: ");
				scanf("%d", &mem[mem[pc].operando].operador);
				pc++;
			break;
			default:
				// caso de label
				pc++;
			break;
		}
	}
}
