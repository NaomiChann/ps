#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 100

// Definição da struct NAMTAB
typedef struct {
    char name[MAX_LINE_LENGTH];
    int start;
    int end;
} NAMTAB;

// Definição da struct DEFTAB
typedef struct {
    char line[MAX_LINE_LENGTH];
} DEFTAB;

// Definição da struct ARGTAB
typedef struct {
    char arg[MAX_LINE_LENGTH];
} ARGTAB;

// Definição da struct Macro
typedef struct {
    NAMTAB namtab[MAX_LINE_LENGTH];
    DEFTAB deftab[MAX_LINE_LENGTH];
    ARGTAB argtab[MAX_LINE_LENGTH];
    int namtab_size;
    int deftab_size;
    int argtab_size;
} Macro;

// Declaração da variável global macro
Macro macro;

//GLOBAIS
char OPCODE[MAX_LINE_LENGTH];
char line[100];
int deftab_index = 0;

// Função para adicionar uma entrada na NAMTAB
void add_NAMTAB_entry(char* name, int start, int end) {
    strncpy(macro.namtab[macro.namtab_size].name, name, MAX_LINE_LENGTH);
    macro.namtab[macro.namtab_size].start = start;
    macro.namtab[macro.namtab_size].end = end;
    macro.namtab_size++;
}

// Função para adicionar uma entrada na DEFTAB
void add_DEFTAB_entry(char* line) {
    strncpy(macro.deftab[macro.deftab_size].line, line, MAX_LINE_LENGTH);
    macro.deftab_size++;
}

// Função para adicionar uma entrada na ARGTAB
void add_ARGTAB_entry(char* arg) {
    strncpy(macro.argtab[macro.argtab_size].arg, arg, MAX_LINE_LENGTH);
    macro.argtab_size++;
}

FILE* abreArquivo(int op);
void extraiOPCODE(char *str, char *opcode);
int procurarNaNamtab(char *nome);
char* proximaLinhaDefTab();
void substitute_parameters(char* line);
int enter_into_NAMTAB(char* name);
int enter_into_DEFTAB();
int is_not_comment(char* line);
int get_DEFTAB_index();
void set_NAMTAB_pointer(int namtab_index, int deftab_start, int deftab_end);
void set_up_arguments(char* line);
void PROCESSLINE(FILE* outputFile, FILE* inputFile, int* EXPANDING);
void EXPAND(FILE* inputFile, FILE* outputFile, int EXPANDING);

void GETLINE(FILE* inputFile, int* EXPANDING) 
{
    if (*EXPANDING == 1) {
        // get next line of macro definition from DEFTAB
        char* pline = proximaLinhaDefTab();
        // substitute arguments from ARGTAB for positional notation
        char substituted_line[MAX_LINE_LENGTH];
        substitute_parameters(substituted_line);
        strncpy(line, substituted_line, MAX_LINE_LENGTH);
    } else {
        // read next line from input file
        fgets(line, MAX_LINE_LENGTH, inputFile);
        extraiOPCODE(line, OPCODE);
    }
}

void DEFINE(FILE* inputFile, int* EXPANDING) 
{
    // enter macro name into NAMTAB
    int namtab_index = enter_into_NAMTAB(OPCODE);

    // enter macro prototype into DEFTAB
    deftab_index = enter_into_DEFTAB();
    
    int level = 1;
    while (level > 0) {
        GETLINE(inputFile, EXPANDING);
        if (is_not_comment(line) != 0) 
        {
            // substitute positional notation for parameters
            char substituted_line[MAX_LINE_LENGTH];
            substitute_parameters(substituted_line);
            
            // enter line into DEFTAB
            add_DEFTAB_entry(substituted_line);

            if (strcmp(OPCODE, "MACRO") == 0) {
                level++;
            } else if (strcmp(OPCODE, "MEND") == 0) {
                level--;
            }
        }
    }

    // store in NAMTAB pointer to beginning and end of definition
    set_NAMTAB_pointer(namtab_index, deftab_index, get_DEFTAB_index() - 1);
}



void macro_processor(FILE* inputFile, FILE* outputFile) 
{
    int EXPANDING = 0;
    while (strcmp(OPCODE, "END") != 0) {
        GETLINE(inputFile, &EXPANDING);
        PROCESSLINE(outputFile, inputFile, &EXPANDING);
    }
}

int main() {
    //Abre os arquivos de entrada e saida;
    FILE* inputFile = abreArquivo(0);
    FILE* outputFile = abreArquivo(1);

    //chama o "main" do processador de macros
    macro_processor(inputFile, outputFile);

    // Fecha os arquivos de entrada e saida;
    fclose(inputFile);
    fclose(outputFile);
    return 0;
}

void PROCESSLINE(FILE* outputFile, FILE* inputFile, int* EXPANDING) 
{
    // search NAMTAB for OPCODE
    int namtab_index = procurarNaNamtab(OPCODE);
    if (namtab_index >= 0) { // if found
        EXPAND(inputFile, outputFile, *EXPANDING);
    } else if (strcmp(OPCODE, "MACRO") == 0) {
        DEFINE(inputFile, EXPANDING);
    } else { // write source line to expand file
        fputs(line, outputFile);
    }
}

void EXPAND(FILE* inputFile, FILE* outputFile, int EXPANDING) {
    EXPANDING = 1;
    //get first line of macro definition {prototype} from DEFTAB
    strcpy(line, macro.deftab[deftab_index].line);
    deftab_index++;
    //set up arguments from macro invocation in ARGTAB
    set_up_arguments(line);
    // write macro invocation to expanded file as a comment
    fprintf(outputFile, "/* %s */\n", line);
    // process all lines of macro definition
    while (strcmp(OPCODE, "MEND") != 0) {
        GETLINE(inputFile, &EXPANDING);
        PROCESSLINE(outputFile, inputFile, &EXPANDING);
    }
    EXPANDING = 0;
}

void set_up_arguments(char* line) 
{
    int i, j = 0, arg_count = 0;
    char arg[MAX_LINE_LENGTH];

    // iterate through the line to extract arguments
    for (i = 0; line[i] != '\0'; i++) {
        if (line[i] == ' ' || line[i] == '\t') {
            // skip whitespace characters
            continue;
        } else if (line[i] == ',') {
            // store the extracted argument and move on to the next one
            arg[j] = '\0';
            strcpy(macro.argtab[arg_count].arg, arg);
            arg_count++;
            j = 0;
        } else {
            // add the current character to the current argument
            arg[j] = line[i];
            j++;
        }
    }

    // add the last argument (if any) to the list
    if (j > 0) {
        arg[j] = '\0';
        strcpy(macro.argtab[arg_count].arg, arg);
        arg_count++;
    }

    // set the size of the ARGTAB to the number of arguments extracted
    macro.argtab_size = arg_count;
}



void set_NAMTAB_pointer(int namtab_index, int deftab_start, int deftab_end) 
{
    macro.namtab[namtab_index].start = deftab_start;
    macro.namtab[namtab_index].end = deftab_end;
}

int get_DEFTAB_index() 
{
    return macro.deftab_size;
}


int is_not_comment(char* line) 
{
    // Se o primeiro caractere da linha for ';', a linha é um comentário
    if (line[0] == ';') {
        return 0; // é um comentário
    } else {
        return 1; // não é um comentário
    }
}


int enter_into_DEFTAB() 
{
    strncpy(macro.deftab[macro.deftab_size].line, line, MAX_LINE_LENGTH);
    int index = macro.deftab_size;
    macro.deftab_size++;
    return index;
}

int enter_into_NAMTAB(char* name)
{
    strncpy(macro.namtab[macro.namtab_size].name, name, MAX_LINE_LENGTH);
    int index = macro.namtab_size;
    macro.namtab_size++;
    return index;
}


void substitute_parameters(char* line) 
{
    char arg_name[MAX_LINE_LENGTH];
    char arg_value[MAX_LINE_LENGTH];
    int i = 0;
    while (line[i] != '\0') {
        if (line[i] == '&') {
            // this is a parameter reference, extract parameter name
            int j = 0;
            i++;
            while (line[i] != '&' && line[i] != '\0') {
                arg_name[j] = line[i];
                i++;
                j++;
            }
            arg_name[j] = '\0';
            // find parameter value in ARGTAB
            int arg_idx = atoi(arg_name) - 1;
            if (arg_idx >= 0 && arg_idx < macro.argtab_size) {
                strncpy(arg_value, macro.argtab[arg_idx].arg, MAX_LINE_LENGTH);
            } else {
                fprintf(stderr, "Error: invalid parameter reference &%s\n", arg_name);
                arg_value[0] = '\0';
            }
            // substitute parameter value
            int k = 0;
            while (arg_value[k] != '\0') {
                line[i-j-1+k] = arg_value[k];
                k++;
            }
        } else {
            // this is not a parameter reference, copy character to output line
            line[i] = line[i];
        }
        i++;
    }
    // terminate output line
    line[i] = '\0';
}


char* proximaLinhaDefTab() 
{
    static int posicao_atual = 0;
    if (posicao_atual < macro.deftab_size) {
        char* linha = macro.deftab[posicao_atual].line;
        posicao_atual++;
        return linha;
    } else {
        return NULL;
    }
}


int procurarNaNamtab(char *nome) 
{
    int i;
    for (i = 0; i < macro.namtab_size; i++) {
        if (strcmp(macro.namtab[i].name, nome) == 0) {
            return 1; // encontrado
        }
    }
    return -1; // não encontrado
}



void extraiOPCODE(char *str, char *opcode) 
{
    int i, j;
    
    // Verifica se a string começa com espaços em branco
    if (isspace(str[0])) {
        // Ignora os espaços em branco
        i = 0;
        while (isspace(str[i])) {
            i++;
        }
        // Encontra a primeira substring após os espaços em branco
        j = i;
        while (!isspace(str[j])) {
            j++;
        }
        // Copia a substring encontrada para o array de caracteres
        strncpy(opcode, str + i, j - i);
        opcode[j - i] = '\0';
    }
    else {
        // Encontra a primeira substring
        i = 0;
        while (!isspace(str[i])) {
            i++;
        }
        // Ignora os espaços em branco após a primeira substring
        while (isspace(str[i])) {
            i++;
        }
        // Encontra a próxima substring após os espaços em branco
        j = i;
        while (!isspace(str[j])) {
            j++;
        }
        // Copia a substring encontrada para o array de caracteres
        strncpy(opcode, str + i, j - i);
        opcode[j - i] = '\0';
    }
}

FILE* abreArquivo(int op) 
{
    if (op == 0)
    {
        FILE* inputFile = fopen("entrada.asm", "r");
        if (inputFile == NULL) 
        {
            printf("Erro ao abrir o arquivo de entrada.\n");
            exit(1);
        }
        return inputFile;
    }
    
    else
    {
        FILE* inputFile = fopen("assembly.asm", "w");
        if (inputFile == NULL) 
        {
            printf("Erro ao abrir o arquivo de entrada.\n");
            exit(1);
        }
        return inputFile;
    }
}