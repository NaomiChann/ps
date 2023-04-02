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
void LeftStrip( char * s ) 
{
    char * p = s;
    int l = strlen(p);

    while( isspace( p[l - 1] ) )
	{
		p[--l] = 0;
	}
    while( * p && isspace(* p) )
	{
		++p, --l;
	}
	strcat( p, "\n" );
    memmove( s, p, l + 1 );
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
void LocUpdate( char * str ) //for instructions
{
	FILE* inputFile = fopen( "InstructionTable.txt", "r" );
	char *instructionName, *strSize, line[256];
	int size;
	bool plus = false;

	while( fgets( line, sizeof( line ), inputFile ) )
	{
		instructionName = strtok( line, " " );
		strSize = strtok( NULL, "\n" );
		size = atoi( strSize );

		if( strcmp( str,instructionName ) ==0 )
		{
			if( str[0] == '+' )
			{
				plus = true;
			}

			if( plus ) //loc update
			{
				if( size == 3 )
				{
					locC += 4;
				}
				else
				{
					printf( "ERROR INVALID INSTRUCTION" );
				}
			}
			else
			{
				locC += size;
			}
		}
	}
	fclose( inputFile );
}

/*
===================
LocUpdate
-------------------
Puts the label, name of directive, operands and 
updated location counter in the Symbol table.
===================
*/
void Directives( char *label,char *directive, char* operands )
{
	FILE* file = fopen( "SymbolTable.txt", "a+" );	
	FILE* output = fopen( "output.txt", "a+" );	
	char line[256];
	bool hasHeader=false;
	int tam =0;

	if( file != NULL )
	{
        while( fgets(line, sizeof( line ), file ) )
		{
            if ( strstr( line, "AXLBSTPC" ) != NULL )
			{ //searchs for header AXLBSTPC
				hasHeader = true;
			}          
        }
		if( hasHeader == false )
		{
			fprintf( file, "%s\n", "AXLBSTPC" ); // adds it if not
		}

		//resw resb word byte e equ
		if( strcmp( directive, "EQU" ) == 0 )
		{
			fprintf( file, "%d\t%s\t%s\t%s\n", locC, label, directive, operands );
		}
		else 
		{
			if( strcmp( directive, "RESW" ) == 0 )
			{
				locC += 4*( atoi( operands ) );
				fprintf( file, "%d\t%s\t%s\t%s\n", locC, label, directive, operands );
			}
			else if( strcmp( directive, "RESB" ) == 0 )
			{
				locC += atoi( operands );
				fprintf( file, "%d\t%s\t%s\t%s\n", locC, label, directive, operands );
			}
			else if( strcmp( directive, "WORD" ) == 0 )
			{
				locC += 4;
				fprintf( file, "%d\t%s\t%s\t%s\n", locC, label, directive, operands );
			}
			else if( strcmp( directive, "BYTE" ) == 0 )
			{
				tam = strlen( operands ) / 2;
				locC += ceil( tam );//round it up
				fprintf( file, "%d\t%s\t%s\t%s\n", locC, label, directive, operands );
			}
			else if( strcmp( directive, "END" ) == 0 )
			{
				fprintf( output, "TCHURAP" );
				fprintf( output, "TCHURUM" );
				fprintf( output, "OUAAAAAA" );
			}
		}
		

    }
    fclose( file );
	fclose( output );
}
int Mounty()
{
	FILE* inputFile = fopen( "assembly.asm", "r" );
	char *label, *operation, *operands, line[256],  *diretrizes[] = {"RESW", "RESB", "BYTE", "WORD", "EQU", "END"};
	bool directive = false; 

	if( !inputFile )
	{
		fputs( "\n=!=!=!=!=!=!= \n\nINPUT FILE ERROR \n\n=!=!=!=!=!=!= \n\n", stderr );
		return EXIT_FAILURE;
	}

	while( fgets( line, sizeof( line ), inputFile )  )
	{
		directive = false;
		if( isspace( line[0] ) )
		{
			//no label:

			LeftStrip( line );
			operation = strtok( line, "\t" );
			operands = strtok( NULL, "\n" );
			printf( "%s\n",operation );
			printf( "%s\n",operands );
			LocUpdate( operation );
			printf( "LOC %d \n\n\n",locC );

		}
		else
		{
			//with label
			label = strtok( line, "\t" );
			operation = strtok( NULL, "\t" );
			operands = strtok( NULL, "\n" );

			if( strcmp( operation,"START" ) == 0 )
			{
				locC = atoi( operands ); //initial value for loc counter	
			}

			printf( "%s\n",label );
			printf( "%s\n",operation );
			printf( "%s\n",operands );
			
			//check directives:
			for ( int i=0; i< 6; i++ )
			{
				if( strcmp( operation,diretrizes[i] ) == 0 ) //if it has a directive in .asm file
				{
					
					directive = true;
					Directives( label, operation, operands );
					printf( "LOC %d \n\n",locC );
				}
			}
			if( directive == false )
			{
				LocUpdate( operation ); //if it isn't a directive that i'm implementing
				printf( "LOC %d \n\n",locC );
			}
		
		}
	}
	fclose( inputFile );
} 
int main()
{
    Mounty();

    return 0;
}
