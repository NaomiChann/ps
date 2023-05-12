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



// Função para realizar a ligação e relocação de endereços
void linkLoaderFirstPass()
{
    FILE* fileSymbol = fopen( "table/symbol.txt", "r" );
    FILE* fileOutput = fopen( "program/pass1.txt", "w" );
    char line[256], *result, *symbol;
    int currentAddr = 0, i=0;

    while (fgets(line, sizeof(line), fileSymbol))
    {
        //for each symboll, new addres is used
        result = strtok(line, " \t\n");
        currentAddr = (int)&result;;

        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }

        // adds the address in the end of the symbol
        strcat(line, " ");
        sprintf(symbol, "%d", currentAddr);
        strcat(line, symbol);
        fprintf(fileOutput, "%s\n", currentAddr);
    }

    fclose(fileOutput);
    fclose(fileSymbol);

}

void linkLoaderSecondPass()
{
    FILE* fileInput = fopen( "program/assembled.txt", "r" );
    FILE* fileFinal = fopen( "table/linked.txt", "r" );
    FILE* fileOutput = fopen( "program/pass1.txt", "w" );
    int* memoryBeg = (int*)0x00000000;
    int currentAddr;
    char line[256];

    while (fgets(line, sizeof(line), fileOutput))
    {
        currentAddr = (int)line;
        //precisa calcular o endereço absoluto
        int* addrAbsolute = memoryBeg + currentAddr;

        //substituir os símbolos para o endereço absoluto
        fputs(line, fileFinal);

        //realocação já está pronta aparentemente
    }

    //pular para o endereço de início de execução
    //goto currentAddr;
}