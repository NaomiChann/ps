#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_MACROS 5
#define MAX_FILENAME_LENGTH 5000

bool verificaMacro(char *str, char *seq);
void lerEntrada();
void geraArquivoMacro(char *nome_arquivo, char *linha);
void separarLabel(char *linha, char *label);
void geraCopiaArquivo();


char macros[MAX_MACROS][MAX_FILENAME_LENGTH];
int indice = 0;


int linhaEmBrancoOuMEND(char *str)
 {
    if (strstr(str, "MEND") != NULL)
     {
        return 1; // true
    } else {
        for (int i = 0; str[i] != '\0'; i++)
        {
            if (!isspace(str[i]))
            { // se o caractere atual não é um espaço em branco ou quebra de linha
                return 0; // a linha não está vazia
            }
        }
        return 1; // true
    }
}


void lerEntrada()
{
    FILE *file;
    char line[256];
    char nomeDaMacro[MAX_FILENAME_LENGTH];
    bool flagA = false;
    bool flagAux = false;
    int controleEscreve=0;

    file = fopen("assembly_1.asm", "r");

    if (file == NULL)
     {
        printf("Erro ao abrir o arquivo de entrada.\n");
        exit(1);
     }

    while (fgets(line, sizeof(line), file))
     {
        // Processa a linha lida aqui
        
        printf("%s",line);
        flagA = verificaMacro(line, "MACRO");
     
        if (flagA == false && controleEscreve > 0)
        {
            geraArquivoMacro(nomeDaMacro, line);
            flagAux = verificaMacro(line, "MEND");
            if (flagAux == true)
            {
                controleEscreve = 0;
            }
        } else if (flagA == true && controleEscreve == 0)
        {

            separarLabel(line, nomeDaMacro);
            geraArquivoMacro(nomeDaMacro, line);
            strncpy(macros[indice++], nomeDaMacro, MAX_FILENAME_LENGTH);
            controleEscreve++;
        }
    }
    fclose(file);

    // Print the generated file names
    printf("Generated files:\n");
    for (int i = 0; i < indice; i++)
     {
        printf("%s\n", macros[i]);
     }
}

bool verificaMacro(char *str, char *seq) 
{
    if (strstr(str, seq) != NULL) {
        return true;
    } else {
        return false;
    }
}

void geraArquivoMacro(char *nome_arquivo, char *linha)
{
    //strcat(nome_arquivo, ".asm");
    FILE *arquivo_asm = fopen(nome_arquivo, "a");

    // Verifica se o arquivo foi aberto com sucesso
    if (arquivo_asm == NULL)
     {
        printf("Erro ao abrir o arquivo!\n");
        return;
     }

    // Adiciona a linha ao arquivo
    if (strlen(linha) != 0) 
    {
        fprintf(arquivo_asm, "%s", linha);
    }

    // Fecha o arquivo
    fclose(arquivo_asm);
}

void separarLabel(char *linha, char *label)
 {
    // Procurar o espaço em branco que separa o label da palavra MACRO
    char *espaco = strstr(linha, " ");

    // Copiar o label para a variável de retorno
    strncpy(label, linha, espaco - linha);
    strcat(label, ".asm");
    label[espaco - linha + 4] = '\0'; // Adicionar o caractere nulo no final
}

void geraCopiaArquivo()
 {
    FILE *store_content_macro_file;
    FILE *output_file;
    char line[256];

    store_content_macro_file = fopen("assembly_1.asm", "r");
    if (store_content_macro_file == NULL)
     {
        printf("Erro ao abrir o arquivo de entrada.\n");
        exit(1);
     }

    output_file = fopen("copy_assembly_1.asm", "w");
    if (output_file == NULL)
     {
        printf("Erro ao criar o arquivo de saída.\n");
        exit(1);
     }

    while (fgets(line, sizeof(line), store_content_macro_file))
     {
        fprintf(output_file, "%s", line);
     }

    fclose(store_content_macro_file);
    fclose(output_file);

    printf("Copia do arquivo gerada com sucesso!\n");
}

int main(){
    geraCopiaArquivo();
    lerEntrada();
    FILE *store_content_macro_file;
    FILE *output_file;
    char line[256];

    store_content_macro_file = fopen("copy_assembly_1.asm", "r");
    if (store_content_macro_file == NULL)
     {
        printf("Erro ao abrir o arquivo de entrada.\n");
        exit(1);
     }

    output_file = fopen("temp.asm", "w");
    if (output_file == NULL)
     {
        printf("Erro ao criar o arquivo temporário.\n");
        exit(1);
     }

/*
FILE* file = fopen("RDBUFF.asm", "r");

    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    char buffer[5000];
    char linei[1000];

    while (fgets(linei, sizeof(linei), file)) {
        strcat(buffer, linei);
    }

    fclose(file);

    printf("%s\n", buffer);
*/


    char store_body_macro[5000];
    char linha[1000];
    char store_body_macros[2][MAX_FILENAME_LENGTH];

    // Percorre cada arquivo da macro e coloca o conteúdo de cada uma no vetor store_body_macros
    for(int i=0;i<indice;i++) 
    {
        FILE* file = fopen(macros[i], "r");

        if (!file)
        {
            printf("Erro ao abrir o arquivo.\n");
        }

    store_body_macro[0] = '\0';

    while (fgets(linha, sizeof(linha), file))
     {
        strcat(store_body_macro, linha);
     }

    fclose(file);


    printf("%s\n", store_body_macro);
    strcpy(store_body_macros[i], store_body_macro);
  }

    char macro_name[MAX_FILENAME_LENGTH];
    char macro_names[2][MAX_FILENAME_LENGTH];
    printf("nome das macrosssssssssss");

    for (int i = 0; i < 2; i++)
     {
        size_t len = strlen(macros[i]);
        strncpy(macro_name, macros[i], len - 4);
        macro_name[len - 4] = '\0';
        strcpy(macro_names[i], macro_name); // copia o nome da macro para o array de nomes
    }


// imprime os nomes das macros armazenados no novo valor_novo
    for(int i = 0; i < 2; i++)
    {
        printf("Macro name %d: %s\n", i, macro_names[i]);
        printf("store_body_macro %d: %s\n", i, store_body_macros[i]);
    }


    int main_program_flag = 0;
    char valor_antigo[10][20];
    char valor_novo[10][10];
    char varialvel_temp[256];

    while (fgets(line, sizeof(line), store_content_macro_file)) 
    {
        int encontrado_flag = 0;
        int i = 0;
    
   

    for (i; i < (sizeof(macro_names)/ sizeof(macro_names[0])); i++) 
    {
        if(strstr(line, "MAIN") != NULL)
        {
            main_program_flag = 1;
        }
        if (strstr(line, macro_names[i]) != NULL  && main_program_flag) 
        {
            printf(line);
            encontrado_flag = 1;
// aq
            char *pos;
            char *token;
            
            int  m = 0; 
            pos = strstr(line, macro_names[i]); // localiza a posição da substring "WRBUFF"
        if (pos != NULL) 
        {
            pos += strlen(macro_names[i]); // avança o ponteiro para a posição após "WRBUFF"
            int num_spaces = strspn(pos, " "); // encontra o número de espaços iniciais na string
            pos += num_spaces; // avança o ponteiro além dos espaços em branco
            token = strtok(pos, ","); // divide a string restante pelo delimitador ","
            
            while (token != NULL)
            {
                strcpy(valor_novo[m], token);
            
                m += 1;
            //  printf("\n %s", token);
                token = strtok(NULL, ","); // continua dividindiceo a string até não haver mais delimitadores
            }
        }


// aq
           break;
        }
    }
    

  if (encontrado_flag)
   {
        strcpy(varialvel_temp, store_body_macros[i]);
//printf(varialvel_temp);

// pega os parâmetros da variável, remove duplicidade e põe em um valor_novo

    int len = strlen(varialvel_temp);
    char param[20];
    int z = 0;
    int found_rd_buff = 0;
  
  // Extrai os parâmetros a serem alterados posteriormente em tempo de execução
    int y = 0;
    for (int i = 0; i < len; i++)
     {
        
        if (!found_rd_buff)
         {
            // Procura pela palavra RDBUFF
            if (strncmp(&varialvel_temp[i], "RDBUFF", 6) == 0 || strncmp(&varialvel_temp[i], "WRBUFF", 6) == 0) 
            {
                found_rd_buff = 1;
                i += 5; // avança o índice para depois da palavra RDBUFF
            }
        } else {
            // Extrai os parâmetros que começam com &
            if (varialvel_temp[i] == '&')
             {
                z = 0;
               while (i < len && (varialvel_temp[i] == '&' || isalpha(varialvel_temp[i]))) 
               {
                    param[z++] = varialvel_temp[i++];
                }
                param[z] = '\0';
           
                //printf("%s\n", param);
                strcpy(valor_antigo[y], param);
                y +=1;
            }
        }
    }
    
    // Remove duplicidades no vetor
     for (int i = 0; i < y; i++)
      {
        for (int z = i + 1; z < y; z++)
         {
            if (strcmp(valor_antigo[i], valor_antigo[z]) == 0) 
            {
                // se encontrar uma palavra repetida, remove-a do valor_novo
                for (int k = z; k < y - 1; k++)
                 {
                    strcpy(valor_antigo[k], valor_antigo[k+1]);
                }
                y--;
                z--;
            }
        }
    }


char store_content_macro[5000];
strcpy(store_content_macro, store_body_macros[i]);
// Loop que substitui todas as ocorrências dos parâmetros pelos tokens correspondentes
for (int p=0; p<3; p++)
 {
    int i = 0;
    char aux_1[20];
     char aux_2[20];
    while (store_content_macro[i] != '\0') 
    {
        strcpy(aux_1,valor_antigo[p]);
        strcpy(aux_2,valor_novo[p]);
        if (strncmp(&store_content_macro[i], aux_1, strlen(aux_1)) == 0) 
        {
            int len_diff = strlen(aux_2) - strlen(aux_1);
            memcpy(&store_content_macro[i], aux_2, strlen(aux_2));
            memmove(&store_content_macro[i + strlen(aux_2)], &store_content_macro[i + strlen(aux_1)], strlen(&store_content_macro[i + strlen(aux_1)]) + 1);
            i += strlen(aux_2) + len_diff;
        } else {
            i++;
        }
    }
} 

    // Se encontra uma macro, então faz a expansão, senão copia a linha e põe no arquivo 

      /// fprintf(output_file, "%s", line);
       // strcpy(macro_parameters, line);
        //fprintf(output_file, "%s", line);
        fprintf(output_file, store_content_macro);
    } else {
        fprintf(output_file, "%s", line);
    } 
}


fclose(store_content_macro_file);
fclose(output_file);

// Remove o arquivo de entrada antigo e renomeia o arquivo temporário
remove("copy_assembly_1.asm");
rename("temp.asm", "copy_assembly_1.asm");

printf("\n\nSubstituicao realizada com sucesso!\n");


editFile("copy_assembly_1.asm");

return 0;

}

// Remove linhas em branco e a palavra MEND

void editFile(char *filename) 
{
    FILE *file, *newFile;
    char *ctrl, *nomeMacro;
    char nomeSeparado[20];
    int flagCtrl = 0, flagMacro = 0;
    char line[100], editedLine[100]; // buffer para armazenar cada linha do arquivo

    file = fopen(filename, "r"); // abre o arquivo em modo leitura

    if (file == NULL) 
    { // se ocorrer um erro ao abrir o arquivo
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    newFile = fopen("saidaFinal.asm", "w"); // cria um novo arquivo para escrita

    while (fgets(line, sizeof(line), file) != NULL)
     { // lê as linhas do arquivo uma por uma
        // edite a linha como desejado e armazene em editedLine
        ctrl = strstr(line, "MAIN");
        nomeMacro = strstr(line, "MACRO");
        if (flagCtrl == 1)
        {
            if (linhaEmBrancoOuMEND(line) == 0)
            {
                fprintf(newFile, "%s", line); // escreve a linha editada no novo arquivo
            }
                
        }
        /*
        if (flagCtrl == 1 && flagMacro == 1){
            if (linhaEmBrancoOuMEND(line) == 0)
            {
                strcat(nomeSeparado, "   ");
                strcat(nomeSeparado, line);
                strcpy(line, nomeSeparado);
                fprintf(newFile, "%s", line); // escreve a linha editada no novo arquivo
                flagMacro = 0;
            }
                
        }
        */
        if(ctrl != NULL){
            flagCtrl = 1;
        }
        /*
        if (nomeMacro != NULL)
            {
                separarLabel(line, nomeSeparado);
                flagMacro = 1;
                printf(nomeSeparado);
            }
        */
        
    }

    fclose(file); // fecha o arquivo original
    fclose(newFile); // fecha o novo arquivo
}