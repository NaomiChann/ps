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

		if( strcmp( str,instructionName ) == 0 )
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
				}else{
					printf( "ERROR INVALID INSTRUCTION" );
				}
			}else{
				locC += size;
			}
		}
	}
	fclose( inputFile );
}

/*
===================
ASCIItoHex
-------------------
translates ASCII to hex
===================
*/
char * ASCIItoHex(char * ascii )
{
	FILE* inputFile = fopen( "ascii-table.txt", "r" );
	char * asciiTable, ascii2[2], *hex, line[256];

	ascii2[0] = *ascii;
	ascii2[1] = '\0';

	printf("\nC:  %c    \n",*ascii);
	while(fgets( line, sizeof( line ), inputFile ) )
	{
		asciiTable = strtok( line, " " );
		strcpy(asciiTable + strlen(asciiTable), "\0");

		hex =  strtok( NULL, " " );
		if( strcmp( asciiTable,ascii2 ) == 0 )
			{
				return hex;
			}
	}
	return ascii;
}


/*
===================
Directives
-------------------
Puts the label, name of directive, operands and 
updated location counter in the Symbol table.
===================
*/
void Directives( char *label,char *directive, char* operands )
{
	FILE* file = fopen( "SymbolTable.txt", "a+" );	
	FILE* asciiT = fopen( "ascii-table.txt", "a+" );	
	FILE* output = fopen( "output.txt", "a+" );	
	char line[256] = { '\0' }, lineASCII[256] = { '\0' }, *XorC, *ASCII, *hexOperand, temp[3] = { '\0' };
	bool hasHeader = false;
	int tam = 0;

	if( file != NULL )
	{
        while( fgets( line, sizeof( line ), file ) )
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
			fprintf( file, "%s\t%s\n", label, operands );
		}
		else 
		{
			if( strcmp( directive, "RESW" ) == 0 )
			{
				locC += 4*( atoi( operands ) );
				fprintf( file, "%s\t%s\n", label, operands );

			} else if( strcmp( directive, "RESB" ) == 0 ){
				locC += atoi( operands );
				fprintf( file, "%s\t%s\n", label, operands );

			} else if( strcmp( directive, "WORD" ) == 0 ){
				locC += 4;
				fprintf( file, "%s\t%s\n", label, operands );

			} else if( strcmp( directive, "BYTE" ) == 0 ){

				if (strstr( operands, "X'" )  != NULL || strstr( operands, "C'" )  != NULL  ) //X' or C'
				{
					if ( strstr( operands, "X'" ) != NULL )
					{
						XorC = strtok( operands, "'" );
						operands =  strtok( NULL, "'" );

					} else if (strstr( operands, "C'" ) ) {

						XorC = strtok( operands, "'" ); // C
						operands =  strtok( NULL, "'" ); //example: EOF
						//printf("%s", operands);

						hexOperand = ( char * ) malloc ( ( ( strlen( operands ) * 2 ) + 1 ) * sizeof( char ) );
						memset( hexOperand, '\0', ( ( strlen( operands ) * 2 ) + 1 ) );

						char *token = strtok(operands, "");
    
						while (token != NULL) {
							int i;
							for (i = 0; i < strlen(token); i++) { //char by char
								memcpy(temp, ASCIItoHex(&token[i]), 2);
								strcat( hexOperand, temp );
							}
							token = strtok(NULL, "");
						}

						strcpy( operands, hexOperand );
						free( hexOperand );
					}
				}



				tam = strlen( operands ) / 2;
				if ( strlen( operands ) % 2 == 0 )
				{
					locC += tam;
				} else {
					locC += tam + 1;
				}
				fprintf( file, "%s\t%s\n", label, operands );

			} else if( strcmp( directive, "END" ) == 0 ){
				fprintf( output, "TCHURAP" );
				fprintf( output, "TCHURUM" );
				fprintf( output, "OUAAAAAA" );
			}
		}
		

    }
    fclose( file );
	fclose( output );
}

#include <stdio.h>
#include <string.h>
#include <stdbool.h>


/*
===================
LabelDuplicates
-------------------
Receives a vector of words (labels), and it's size,
checks for dupicates, 1 for true, 0 for false.
===================
*/
int LabelDuplicates() 
{
	FILE* inputFile = fopen( "SymbolTable.txt", "r" );
	char line[256], **words = NULL, *label;
	int numLabels = 0;

	while(fgets( line, sizeof( line ), inputFile ) )
	{
		label = strtok( line, "\t" );

		//vector of words
		words = realloc( words, ( numLabels + 1 ) * sizeof( char * ) );
		words[numLabels] = malloc( strlen( label ) + 1 );
		strcpy( words[numLabels], label );

		numLabels++;

	}
    for ( int i = 0; i < numLabels - 1; i++ ) 
	{
        for ( int j = i + 1; j < numLabels; j++ ) 
		{
            if ( strcmp(words[i], words[j]) == 0 ) 
			{
				printf( "\n=!=!=!=!=!=!=!=!=!=!=!=!= \n\n %s = %s \n",words[i], words[j] );
                return 1;

            }
        }
    }
    return 0;
}

/*
===================
ClearFile
-------------------
Clear file in the beginning of each run since w
mode deletes all of its contents if it exists.
(truncates file)
===================
*/
void clearFile( const char* filename ) 
{
    FILE* file = fopen( filename, "w" );
    if ( file != NULL ) 
	{
        fclose( file );
    }
}


/*
===================
Mounty
-------------------
Mounty
===================
*/
int Mounty()
{
	FILE* inputFile = fopen( "assembly.asm", "r" );
	char *label, *operation, *operands, line[256],  *diretrizes[] = {"RESW", "RESB", "BYTE", "WORD", "EQU", "END"};
	bool directive = false; 
	char **words = NULL;
	int numLabels = 0;

	clearFile("SymbolTable.txt");
	//input file check
	if( !inputFile )
	{
		fputs( "\n=!=!=!=!=!=!= \n\nINPUT FILE ERROR \n\n=!=!=!=!=!=!= \n\n", stderr );
		return EXIT_FAILURE;
	}

	while(fgets( line, sizeof( line ), inputFile ) )
	{
		directive = false;
		if( isspace( line[0] ) )
		{
			//no label:

			printf( "current LOC %d \n",locC );
			LeftStrip( line );
			operation = strtok( line, "\t" );
			operands = strtok( NULL, "\t" );

			printf( "%s\n",operation );
			printf( "%s\n",operands );
			LocUpdate( operation );
			printf( "\nLOC %d \n",locC );

		}
		else
		{
			//with label
			printf( "current LOC %d \n",locC );
			label = strtok( line, "\t" );
			operation = strtok( NULL, "\t" );
			operands = strtok( NULL, "\t" );


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
					printf( "\nLOC %d \n",locC );
				}
			}
			if( directive == false )
			{
				LocUpdate( operation ); //if it isn't a directive that i'm implementing
				printf( "\nLOC %d \n",locC );
			}
		
		}
		printf("\n\n");


        numLabels++;
	}

	fclose( inputFile );
	if( LabelDuplicates() == 1 )
	{
		fputs( "\nDUPLICATE LABEL ERROR \n\n=!=!=!=!=!=!=!=!=!=!=!=!= \n\n", stderr );
		return EXIT_FAILURE;
	}

} 
int main()
{
    Mounty();

    return 0;
}
