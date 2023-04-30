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

typedef struct 
{
    char name[MAX_NAME_LENGTH];
    int start;
    int end;
} tipNamTab;

typedef struct 
{
    char op[MAX_NAME_LENGTH];
    char args[MAX_ARGS][MAX_NAME_LENGTH];
    int numArgs;
    char body[20][MAX_NAME_LENGTH];
    int contadorLinha;
} tipDefTab;

tipNamTab namtab[MAX_TABLE_SIZE];
int namtab_size = 0;

tipDefTab deftab[MAX_DEF_LINES];
int deftab_size = 0;

char exp_lines[MAX_EXP_LINES][MAX_LINE_LENGTH];
int exp_lines_size = 0;

char curr_line[MAX_LINE_LENGTH];
FILE *input_fp, *output_fp;
int level = 0;
int expanding = 0;
char OPCODE[MAX_NAME_LENGTH];
char macroName[MAX_NAME_LENGTH];
char *args[MAX_ARGS];
char lineTemp[MAX_LINE_LENGTH];
int indexLineBody = 0;

void processLine();
void define();
void expand();
void getLine(int index);
int search_namtab(char opcode[]);
int searchDefTab(char opcode[]);
void substitute_args(char line[]);
void macroProcessor();
void extraiOPCODE(char *str, char *opcode);
int searchNamTab(char opcode[]);
void getNome(char *str, char *nome);
void addNamTab(char* curr_line, char* nameMacro);
void setStartNamTab(tipNamTab namtab[], int index, int indiceStart);
void setEndNamTab(tipNamTab namtab[], int index, int indiceEnd);
void addMacroBody(int enderecoLinha);
void extractArgs(char *argumentos[]);
char* str_replace(char* str, char* old, char* new);
void replace(char str[], char substr[], char newstr[]);
void getLabel(char* current_line, char* extracted_string);

int main()
{
    int index = 0;
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
    while (strcmp(OPCODE, "END") != 0) 
    {
        getLine(0);
        processLine();
    }
}

void replace(char str[], char substr[], char newstr[]) 
{
   char *pos = strstr(str, substr);
   if (pos != NULL) 
   {
      int len = strlen(substr);
      memmove(pos + strlen(newstr), pos + len, strlen(pos) - len + 1);
      memcpy(pos, newstr, strlen(newstr));
   }
}

void getLabel(char* current_line, char* extracted_string) 
{
    int i;
    if (current_line[0] != ' ' && current_line[0] != '\t') 
    {
        // primeiro formato
        for (i = 0; current_line[i] != ' '; i++) 
        {
            extracted_string[i] = current_line[i];
        }
        extracted_string[i] = '\0';
    } else 
    {
        // segundo formato
        memset(extracted_string, '\0', sizeof(extracted_string));
    }
}

void getLine(int index)
{
    if (expanding) 
    {
        // Pega a prox linha da definição de macro da defTab
        char labelMacro[MAX_NAME_LENGTH];
        char lineTemp2[MAX_LINE_LENGTH] = "";
        strcpy(lineTemp, deftab[index].body[indexLineBody]);
        printf("\n######");
        printf("\nLineTemp Normal: %s", lineTemp);
        getLabel(curr_line, labelMacro);

        printf("labelMacro: %s",labelMacro);
        for (int i = 0; i < deftab[index].numArgs; i++)
        {
            replace(lineTemp, deftab[index].args[i], args[i]);
        }

        if (indexLineBody == 0)
        {
            strcat(lineTemp2, labelMacro);
            strcat(lineTemp2, lineTemp);
            strcpy(lineTemp, lineTemp2);
        }

        printf("\nLineTemp Normal: %s", lineTemp);
        strcpy(curr_line, lineTemp);
        indexLineBody++;

    } else 
    {
        // Pega a prox linha do arquivo de entrada
        fgets(curr_line, MAX_LINE_LENGTH, input_fp);
    }
}

void processLine() 
{
    extraiOPCODE(curr_line, OPCODE);
    int namtab_index = searchNamTab(OPCODE);
    if (namtab_index >= 0) {
        expand();
    } else if (strcmp(OPCODE, "MACRO") == 0) {
        define();
    } else {
        fprintf(output_fp, curr_line);
    }
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

int searchDefTab(char opcode[]) 
{
    int i;
    for (i = 0; i < deftab_size; i++) 
    {
        if (strcmp(namtab[i].name, opcode) == 0) 
        {
            return i; // retorna o indice da namTab que o OPCODE esta
        }
    }
    return -1; // não existe o OPCODE na tabela ainda
}

void getNome(char *str, char *nome) 
{
    int i = 0, j = 0;
    while (isspace(str[i])) {
        i++;
    }
    while (!isspace(str[i])) {
        nome[j++] = str[i++];
    }
    nome[j] = '\0';
}

void addNamTab(char* curr_line, char* nameMacro)
{
    getNome(curr_line, nameMacro);
    strcpy(namtab[namtab_size].name, nameMacro);
    namtab[namtab_size].start = 0;
    namtab[namtab_size].end = 0;
    namtab_size++;
}

void setStartNamTab(tipNamTab namtab[], int index, int indiceStart)
{
    namtab[index].start = indiceStart;
}

void setEndNamTab(tipNamTab namtab[], int index, int indiceEnd)
{
    namtab[index].start = indiceEnd;
}

int splitMacroArguments(char *macro, char *args[]) 
{
    int i = 0;
    char *token = strtok(macro, "\t ");
    while (token != NULL) 
    {
        if (token[0] == '&') 
        {
            args[i] = token;
            i++;
        }
        token = strtok(NULL, "\t ,");
    }
    args[i] = NULL;
    return i;
}

void addMacroDef(tipDefTab deftab[], char* definitionLine)
{
    char *args[10];
    int numArgs = 0;
    getNome(definitionLine, macroName);
    deftab[deftab_size].contadorLinha = 0;
    strcpy(deftab[deftab_size].op, macroName);
    numArgs = splitMacroArguments(definitionLine, args);
    deftab[deftab_size].numArgs = numArgs;

    for (int i = 0; i < numArgs; i++)
    {
        strcpy(deftab[deftab_size].args[i], args[i]);
    }    
}

void addMacroBody(int enderecoLinha)
{
    
    extraiOPCODE(curr_line, OPCODE);
    enderecoLinha = deftab[deftab_size].contadorLinha;
    if (strcmp(OPCODE, "MEND") != 0)
    {
        strcpy(deftab[deftab_size].body[enderecoLinha], curr_line);
        deftab[deftab_size].contadorLinha = enderecoLinha+1;
    }
}

void define()
{
    int LEVEL = 1;
    addNamTab(curr_line, macroName);
    addMacroDef(deftab, curr_line);
    int enderecoLinha = 0;
    while (LEVEL > 0)
    {
        getLine(0);
        addMacroBody(enderecoLinha);
        if (strcmp(OPCODE, "MACRO") == 0)
        {
            LEVEL++;
        }
        else if (strcmp(OPCODE, "MEND") == 0)
        {
            LEVEL--;
        }
        enderecoLinha++;
    }
    deftab_size++;
}


void splitInvocationArguments()
{
    char* token;
    int argCount = 0;
    char* macroNameLocal = OPCODE;
    char copy_curr_line[200];
    strcpy(copy_curr_line, curr_line);

    char* macro_start = strstr(copy_curr_line, macroNameLocal);

    if (macro_start == NULL) 
    {
        printf("A palavra nao foi encontrada.\n");
    }

    // Avança o ponteiro até o primeiro caractere que não é espaço em branco
    macro_start += strlen(macroNameLocal);
    while (*macro_start != '\0' && isspace(*macro_start)) 
    {
        ++macro_start;
    }

    // Extrai cada argumento até que não haja mais vírgulas ou atinja o limite
    token = strtok(macro_start, ",");

    while (token != NULL && argCount < MAX_ARGS) 
    {   
        args[argCount] = strdup(token);
        token = strtok(NULL, ",");
        argCount += 1;
    }

}

void expand()
{
    int index;
    expanding = 1;
    index = searchDefTab(OPCODE);
    splitInvocationArguments();
    int bodyLenght = deftab[index].contadorLinha;
    int lineAtt = 0;
    while (lineAtt < bodyLenght)
    {
        getLine(index);
        processLine();
        lineAtt++;   
    }
    indexLineBody = 0;
    expanding = 0;    
}