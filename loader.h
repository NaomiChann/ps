#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "maquinae.h"

/*
===================
Linking
-------------------
Join all the assembled codes
===================
*/
void Linking(char *fileName)
{
    
    FILE* fileInput = fopen( "program/assembled.txt", "r" );
    FILE* fileFlagged = fopen( "program/flagged.txt", "r" );
    FILE* fileOutput = fopen( "program/linked.txt", "w" );
    char line[256], flag[256], *result;
    while (fgets(line, 256, fileInput))
    {
        if (strstr(line, "BABABABE"))
        {
            result = strstr(line, "BABABABE");
            *result = '\0';
            fprintf( fileOutput, "%s", result );
            //if needs the whole information besides the addresses
            while (fgets(flag, 256, fileFlagged))
            {
                fprintf( fileOutput, "%s", flag );
            }
            
        }
    }
    fclose(fileOutput);
    fclose(fileFlagged);
    fclose(fileInput);
}



/*
===================
linkLoaderFirstPass
-------------------
First pass of the loader, gets the whole symbols 
===================
*/
void linkLoaderFirstPass()
{
    //we're only using 1 file, but it needs another to join it so we already have every symbol
    FILE* fileSymbol = fopen( "table/symbol.txt", "r" );
    FILE* fileOutput = fopen( "program/linked.txt", "w" );
    char line[256], *result, *symbol;
    int currentAddr = 0, i=0;

    while (fgets(line, sizeof(line), fileSymbol))
    {
        //global symbols table with the addresses
        result = strtok(line, " \t\n");
        currentAddr = (int)&result;;

        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }

        // adds the address in the end of the symbol
        strcat(line, " ");
        sprintf(symbol, "%d", currentAddr);
        strcat(line, symbol);
        fprintf(fileOutput, "%s\n", line);
    }

    fclose(fileOutput);
    fclose(fileSymbol);

}

/*
===================
linkLoaderSecondPass
-------------------
Second pass of the loader, changes the addresses to absolute
===================
*/

void linkLoaderSecondPass()
{
FILE* fileInput = fopen( "program/assembled.txt", "r" );
int* memoryBeg = (int*)0x00000000;
int currentAddr = 0;

char line[256];
while (fgets(line, sizeof(line), fileInput))
{
    // obtem o opcode e operando
    char opcode[10], operand[100];
    sscanf(line, "%s %s", opcode, operand);

    // verifica se o operando é um símbolo
    if (operand[0] == '$')
    {
        // remove o caractere '$' do símbolo
        memmove(&operand[0], &operand[1], strlen(operand));

        // procura o endereço do símbolo
        int symbolAddr;
        FILE* fileSymbol = fopen( "table/symbol.txt", "r" );
        char symbolLine[256];
        while (fgets(symbolLine, sizeof(symbolLine), fileSymbol))
        {
            char symbol[100], addrStr[100];
            sscanf(symbolLine, "%s %s", symbol, addrStr);
            if (strcmp(symbol, operand) == 0)
            {
                symbolAddr = atoi(addrStr);
                break;
            }
        }
        fclose(fileSymbol);

        // calcula o endereço relativo e converter para string hexadecimal
        int relativeAddr = symbolAddr - currentAddr;
        char relativeAddrStr[100];
        sprintf(relativeAddrStr, "%X", relativeAddr);

        // substitui o símbolo pelo endereço absoluto na linha
        char* symbolPtr = strstr(line, operand);
        strncpy(symbolPtr, relativeAddrStr, strlen(relativeAddrStr));
    }

    // escreve a linha modificada no arquivo de saída
    printf("%s", line); // apenas para fins de teste
}

fclose(fileInput);
}