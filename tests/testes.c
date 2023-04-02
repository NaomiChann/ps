#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "execute.h"


//GLOBAL
static int locC = 0;


/*
===================
LeftStrip
-------------------
Copies string to another without first tab,
returns string without tab.
===================
*/
void LeftStrip(char * s) {
    char * p = s;
    int l = strlen(p);

    while(isspace(p[l - 1])) p[--l] = 0;
    while(* p && isspace(* p)) ++p, --l;
	strcat(p, "\n");
    memmove(s, p, l + 1);
}   

/*
===================
LocUpdate
-------------------
Compares the instruction name in the .asm file with 
the instruction name in the instruction table, 
finds the size and increases the location counter
===================
*/
void LocUpdate(char * str){
	FILE* inputFile = fopen( "InstructionTable.txt", "r" );
	char *instructionName, *strSize, line[256];
	int size;
	bool plus = false;

	while(fgets(line, sizeof(line), inputFile))
	{
		instructionName = strtok(line, " ");
		strSize = strtok(NULL, "\n");
		size = atoi(strSize);

		if(strcmp(str,instructionName)==0)
		{
			if(str[0] == '+')
			{
				plus = true;
			}

			if(plus) //loc update
			{
				if(size==3)
				{
					locC+=4;
				}
				else
				{
					printf("ERROR INVALID INSTRUCTION");
				}
			}
			else
			{
				locC+=size;
			}
		}
	}
	fclose(inputFile);
}

int Mounty()
{
	FILE* inputFile = fopen( "assembly.asm", "r" );
	char *label, *operation, *operands, line[256];

	if( !inputFile )
	{
			fputs( "\n=!=!=!=!=!=!= \n\nINPUT FILE ERROR \n\n=!=!=!=!=!=!= \n\n", stderr );
			return EXIT_FAILURE;
	}

	while(fgets(line, sizeof(line), inputFile)){
		if(isspace(line[0]))
		{
			//no label

			LeftStrip(line);
    		//printf("%s\n", line);
			operation = strtok(line, "\t");
			operands = strtok(NULL, "\n");
			printf("%s\n",operation);
			printf("%s\n\n\n",operands);
			LocUpdate(operation);
			printf("LOCCCCCC %d \n",locC);
			
		}
		else
		{
			//with label
			label = strtok(line, "\t");
			operation = strtok(NULL, "\t");
			operands = strtok(NULL, "\n");

			if(strcmp(operation,"START")==0)
			{
				locC = atoi(operands); //initial value for loc counter	
			}

			printf("%s\n",label);
			printf("%s\n",operation);
			printf("%s\n\n\n",operands);
			LocUpdate(operation);
			printf("LOCCCCCC %d \n",locC);
		}
	}
	fclose(inputFile);
} 
int main()
{
    Mounty();

    return 0;
}
