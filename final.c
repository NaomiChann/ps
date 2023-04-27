#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 200
#define MAX_ARGS 10
#define MAX_DEF_LINES 50
#define MAX_NAME_LENGTH 30
#define MAX_TABLE_SIZE 50
#define MAX_EXP_LINES 200

typedef struct {
    char name[MAX_NAME_LENGTH];
    int start;
    int end;
} tipNamTab;

typedef struct {
    char op[MAX_NAME_LENGTH];
    char args[MAX_ARGS][MAX_NAME_LENGTH];
    int num_args;
} tipDefTab;

tipNamTab namtab[MAX_TABLE_SIZE];
int namtab_size = 0;

tipDefTab deftab[MAX_DEF_LINES];
int deftab_size = 0;

char exp_lines[MAX_EXP_LINES][MAX_LINE_LENGTH];
int exp_lines_size = 0;

char argtab[MAX_ARGS][MAX_NAME_LENGTH];
int argtab_size = 0;

char curr_line[MAX_LINE_LENGTH];
FILE *input_fp, *output_fp;
int level = 0;
int expanding = 0;
char OPCODE[MAX_NAME_LENGTH];

void processLine();
void define();
void expand(int namtab_index);
void getLine();
int search_namtab(char opcode[]);
void substitute_args(char line[]);
void macroProcessor();
void extraiOPCODE(char *str, char *opcode);
int searchNamTab(char opcode[]);

int main(){
    input_fp = fopen("entrada.asm", "r");
    if (input_fp == NULL) 
    {
        printf("Erro ao abrir o arquivo de entrada.\n");
        exit(1);
    }
    output_fp = fopen("assembly.asm","w");
    if (output_fp == NULL) 
    {
        printf("Erro ao abrir o arquivo de entrada.\n");
        exit(1);
    }
    printf("Arquivos abertos com sucesso...\n\n");
    macroProcessor();

    return 0;
}

void macroProcessor() 
{
    printf("Entrou no macroProcessor()...\n");
    expanding = 0;
    while (strcmp(OPCODE, "END") != 0) {
        getLine();
        processLine();
    }
}

void getLine() {
    if (expanding) 
    {
        // Pega a prox linha da definição de macro da defTab
        strcpy(curr_line, deftab[deftab_size-1].args[argtab_size]);
        argtab_size++;
        //substituteArgs(curr_line);

    } else {
        // Pega a prox linha do arquivo de entrada
        fgets(curr_line, MAX_LINE_LENGTH, input_fp);
    }
}

void processLine() {
    extraiOPCODE(curr_line, OPCODE);
    printf("\nOP: %s", OPCODE);
    int namtab_index = searchNamTab(OPCODE);
    if (namtab_index >= 0 && expanding) {
        //expand(namtab_index);
    } else if (strcmp(OPCODE, "MACRO") == 0) {
        define();
    } else {
        fprintf(output_fp, curr_line);
    }
}

void define()
{
    printf("\nDEFINE IMPLEMENTACAO");
}

void extraiOPCODE(char *str, char *opcode) 
{
    int i, j;
    
    // Verifica se a string começa com espaços em branco
    if (isspace(str[0])) 
    {
        // Ignora os espaços em branco
        i = 0;
        while (isspace(str[i])) 
        {
            i++;
        }
        // Encontra a primeira substring após os espaços em branco
        j = i;
        while (!isspace(str[j])) 
        {
            j++;
        }
        // Copia a substring encontrada para o array de caracteres
        strncpy(opcode, str + i, j - i);
        opcode[j - i] = '\0';
    }
    else 
    {
        // Encontra a primeira substring
        i = 0;
        while (!isspace(str[i])) 
        {
            i++;
        }
        // Ignora os espaços em branco após a primeira substring
        while (isspace(str[i])) 
        {
            i++;
        }
        // Encontra a próxima substring após os espaços em branco
        j = i;
        while (!isspace(str[j])) 
        {
            j++;
        }
        // Copia a substring encontrada para o array de caracteres
        strncpy(opcode, str + i, j - i);
        opcode[j - i] = '\0';
    }
}

int searchNamTab(char opcode[]) 
{
    int i;
    for (i = 0; i < namtab_size; i++) 
    {
        if (strcmp(namtab[i].name, opcode) == 0) 
        {
            return i; // retorna o indice da namTab que o OPCODE esta
        }
    }
    return -1; // não existe o OPCODE na tabela ainda
}
