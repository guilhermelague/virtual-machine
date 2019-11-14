/*
	Instituição: UERGS
	Curso: Engenharia de computação
	Data: 14/09/2017
	Autor: Guilherme Lague
	E-mail: guilhermelaguebmx@gmail.com
	Descrição: Implementar o montador de duas passagens para a MV. O código gerado pelo montador deveráser executado pela MV. 
               O montador deverá identificar os seguintes erros: símbolo não definido,símbolo redefinido e instrução inválida.
*/
// 323 linhas de codigo. 91 linhas de comentarios.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int free_address = 0, e = 0;

//==================================================================
// struct de tabela de instruções
// name: nome da instrução
// cod: codigo da instrução 
//==================================================================
struct Tab_instruction{
    char name[5];
    int cod;    
}tab_instruction[17];

//==================================================================
// struct de tabela de simbolos
// type: tipo de simbolo
// name: nome do simbolo
// address: endereço do simbolo no programa
//==================================================================
struct Tab_symbols{
	char type;
    char name[10];
	int address;	
}tab_symbol[32];

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

//==================================================================
// Struct de Erros
// type: tipo de erro
// line: linha onde se encontra o erro
// var: simbolo que possui erro
// name: nome do erro
//==================================================================
struct Error{
    int type;
    int line;
    char var[20];
    char name[100];
}tab_error[64];
//==================================================================

int symbol_exists(char *symbol);
int is_instruction(char *word);
int get_symbol(char *symbol);
int get_instruction(char *word);
void print_errors();
void load_instructions();
void get_line(FILE *file);
void passo1(FILE *file);
void passo2(char *file_origin, char *file_dest);
void insert_tab_simbols(char *symbol, int address_program);
void create_tab_error(int error_type, int line, int address, char *symbol);

//===========================================================================
// Função principal
// argc: numero de argumentos
// argv: vetor de argumentos
//===========================================================================
int main(int argc, char *argv[]){
    FILE *file;

    if(argc < 3){
        printf("Numero insuficiente de argumentos!\n Exemplo para montagem: nome_do_programa.txt nome_de_saida.txt\n");
    }
    else{
        file = fopen(argv[1],"rt");
        if(file == 0){
            printf("Arquivo %s não encontrado!\n", argv[1]);
            exit(0);
        }
        else{
            load_instructions();
            passo1(file);    
            
            if(e == 0){
                passo2(argv[1], argv[2]);
                if(e > 0){
                    printf("O programa contém erros, Assembly não pode ser gerado!\n");
                    print_errors();
                }
                else{
                    printf("Montagem realizada com sucesso, arquivo gerado: %s\n", argv[2]);    
                }   
            }
            else{
                printf("O programa contém erros, Assembly não pode ser gerado!\n");
                print_errors();
            }
        }    
    }
    return 0;
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
// Esta função grava o simbolo na tabela.
// *symbol: palavra lida do arquivo.
// address_program: endereço onde o simbolo está no arquivo.
//===========================================================================
void insert_tab_simbols(char *symbol, int address_program){
    int address = 0;

    while(address < 32){
        if(strcmp(symbol, "") == 0){
            strcpy(tab_symbol[address].name, symbol);
            tab_symbol[address].address = address_program; 
            address = 0;
            break;   
        }
        else{
            if(symbol_exists(symbol) == 0){
                create_tab_error(1, address_program + 1, address_program, symbol);
                address = 0;
                break;
            }
            else{
                strcpy(tab_symbol[free_address].name, symbol);
                tab_symbol[free_address].address = address_program; 
                free_address++;
                address = 0;
                break;
            }
        }
    }
}

//===========================================================================
// Esta função verifica se ja existe um simbolo na tabela
// *symbol: palavra lida do arquivo.
// retorna: 0 se encontral o simbolo
//===========================================================================
int symbol_exists(char *symbol){
    int i = 0;
    while(i <= 32){
        if(strcmp(tab_symbol[i].name, symbol) == 0){
            return 0;
        }
        else{
            i++;
        }
    }   
}

//===========================================================================
// Esta função monta a tabela de simbolos (variaveis)
// gera erros de variaveis já declaradas e instruções invalidas
// *file: arquivo onde contém o programa.
//===========================================================================
void passo1(FILE *file){
	int address = 0;

    while(!feof(file)){
        get_line(file);
        if(word_line.num_word == 3){
            if((strcmp(word_line.word[1], "SPACE") == 0) || (strcmp(word_line.word[1], "GLOBAL") == 0) || (strcmp(word_line.word[1], "EXTERN") == 0)) {
                insert_tab_simbols(word_line.word[0], address);
            }
            else if(is_instruction(word_line.word[1]) == 0){
                insert_tab_simbols(word_line.word[0], address);
            }
            else if(is_instruction(word_line.word[1]) != 0){
                create_tab_error(0, address + 1, address, word_line.word[1]);   
            }
        }
        else if(word_line.num_word == 2){
            if(is_instruction(word_line.word[0]) != 0){
                create_tab_error(0, address + 1, address, word_line.word[0]);    
            }
        }
        address++;
    }
    fclose(file);
}

//===========================================================================
// Esta função gera o codigo
// gera erros de variaveis não declaradas e instruções invalidas.
// *file_origin: arquivo origem onde contém o programa.
// *file_dest: arquivo destino onde será gravado o codigo de maquina.
//===========================================================================
void passo2(char *file_origin, char *file_dest){
    int address = 0;

    FILE *origin, *dest;

    dest = fopen(file_dest, "w");
    
    if(dest == 0){
        printf("Arquivo %s não foi criado!\n", file_dest);
    }
    else{
        origin = fopen(file_origin, "r");

        fscanf(origin, "%s", &word_line.word[0]);
        
        while(strcmp(word_line.word[0], "END")){
            if (get_instruction(word_line.word[0]) == -1) {
                fscanf(origin, "%s", word_line.word[0]);
            }

            if (strcmp(word_line.word[0], "STOP") == 0){
                fprintf(dest, "%d %d\n", get_instruction(word_line.word[0]), 0);
            }
            else if(get_instruction(word_line.word[0]) != -1){ // se instrução valida
                fscanf(origin, "%s", word_line.word[1]); // pega o simbolo
                if(symbol_exists(word_line.word[1]) == 0){
                    if(get_symbol(word_line.word[1]) != -1) { // pega o endereço do simbolo
                        fprintf(dest, "%d %d\n", get_instruction(word_line.word[0]), get_symbol(word_line.word[1]));
                    }
                }
                else{
                    create_tab_error(2, address + 1, address, word_line.word[1]); 
                    break;
                }
            }
            fscanf(origin, "%s", word_line.word[0]);
            address++;
        }
        fclose(origin);
    }
    fclose(dest);  
}

//==========================================================================
// Esta função busca o codigo da instrução.
// *word: palavra lida do arquivo.
// retorna: codigo da instrução se for valida ou -1 se não for valida.
//==========================================================================
int get_instruction(char *word){
    for(int i = 0; i < 13; i++) {
        if(strcmp(word, tab_instruction[i].name) == 0){
            return tab_instruction[i].cod;
        }
    }
    return -1;
}

//==========================================================================
// Esta função busca o endereço do simbolo.
// *symbol: palavra lida do arquivo.
// retorna: endereço do simbolo se declarado ou -1 se não declarado.
//==========================================================================
int get_symbol(char *symbol){
    for(int i = 0; i < 32; i++){
        if(strcmp(symbol, tab_symbol[i].name) == 0) {
            return tab_symbol[i].address;
        }
    }
    return -1;
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

//===========================================================================
// Esta função imprime a tabela de erros
//===========================================================================
void print_errors(){
    for(int i = 0; i < e; i++){
        printf("%s\n", tab_error[i].name);
    }
}

//===========================================================================
// Esta função cria uma tabela de erros encontrados.
// error_type: tipo de erro encontrado.
// line: linha onde se encontra o erro.
// address: endereço onde se encontra o erro.
// *symbol: simbolo que contém erro.
//===========================================================================
void create_tab_error(int error_type, int line, int address, char *symbol){
    char temp[20];
    tab_error[e].line = line;
    tab_error[e].type = error_type;
    strcpy(tab_error[e].var, symbol);

    strcpy(tab_error[e].name, "Na linha ");
    sprintf(temp, "%i: ", tab_error[e].line, ", ");
    strcat(tab_error[e].name, temp);
    
    switch(error_type){
        case 0:
            strcat(tab_error[e].name, " instrução ");
            strcat(tab_error[e].name, tab_error[e].var);
            strcat(tab_error[e].name, " não é válida");
        break;
        case 1:
            strcat(tab_error[e].name, " a variavel ");
            strcat(tab_error[e].name, tab_error[e].var);
            strcat(tab_error[e].name, " já foi declarada");
        break;
        case 2:
            strcat(tab_error[e].name, " a variavel ");
            strcat(tab_error[e].name, tab_error[e].var);
            strcat(tab_error[e].name, " não foi declarada.");
        break;
        default:
            printf("só existem 3 tipos de erros, 0, 1 e 2\n");
    }
    e++;
}