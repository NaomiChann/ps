#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

char * IntToBinary( int n );
int BinaryToInt( char * bin );
char * IntToHex( int n );
void Memory( int numBytes );
char * SixBitOp( char * obj, int * iopcode );
char * InstructionTable( const char *s );

int globalMem;

int main()
{
	FILE *inputFile = fopen( "cod-obj.txt", "r" );
	char objCode[9], temp[2], temp2[3], opcode[3];
	int instSize, iopcode;

// if error reading file
	if( !inputFile ) 
	{
			fputs( "inputFile error.\n", stderr );
			return EXIT_FAILURE;
	}
// reads until eof
	while ( 1 )
	{
		fscanf( inputFile, "%s", objCode );
		if ( feof( inputFile ) )
		{
			break;
		}

// checks the string size to determine the instruction format
		instSize = ( ( int ) strlen( objCode ) );
// debug
		printf( "%s ", objCode );
// each pair of hex values is a byte
		switch ( instSize / 2 )
		{
			case 1:
// since there are no 1 byte instructions on our table
// this can only be a variable label
				Memory( 1 );
				break;
			case 2:
// 1st byte as op, 2nd byte both r1 and r2 each with 4 bits
				memcpy( temp2, objCode, 2 );
				printf( "%s ", temp2 );
				memcpy( temp, &objCode[2], 1 );
				printf( "r1 %s ", temp );
				memcpy( temp, &objCode[3], 1 );
				printf( "r2 %s \n", temp );
				Memory( 2 );
				break;
			case 3:
				strcpy( opcode, SixBitOp( objCode, &iopcode ) );
				InstructionTable( opcode );
				Memory( 3 );
				break;
			case 4:
				strcpy( opcode, SixBitOp( objCode, &iopcode ) );
				InstructionTable( opcode );
				break;
			default:
				printf( "SIZE ERROR \n" );
				break;
		}
		printf( "============= \n" );
	}
	printf( "Memory used: %d Bytes \n", globalMem );
	fclose( inputFile );

	return 0;
}

/*
===================
IntToBinary
-------------------
Converts an integer into 4 bit binary code
and returns it as a string 
===================
*/
char * IntToBinary( int n )
{
	int bin[5], i;
	static char string[5];
	char temp[5] = "";
	memset( string, '\0', 5 );
	
	for( i = 0; n > 0; i++ )
	{
		bin[i] = n % 2;
		n = n / 2;
	}

	for( i = i - 1; i >= 0; i-- )
	{
		itoa( bin[i], temp, 10 );
		strcat( string, temp );
	}

	strcpy( temp, "0" );

	while ( ( int ) strlen( string ) < 4 )
	{
		strcat( temp, string );
		strcpy( string, temp );
		strcpy( temp, "0" );
	}

	return string;
}

/*
===================
BinaryToInt
-------------------
Converts a 4 bit binary code string
into an integer
===================
*/
int BinaryToInt( char * bin )
{
	char string[4] = "", temp[2] = "";
	strcpy( string, bin );
	int i, n = 0, j = 1;
	
	for( i = 3; i >= 0; i-- )
	{
		memcpy( temp, &string[i], 1 );
		if ( strcmp( temp, "1" ) == 0 ) {
			n += j;
		}
		j = j * 2;
	}

	return n;
}

/*
===================
IntToHex
-------------------
Converts an integer into a string
containing the value as a hexadecimal
===================
*/
char * IntToHex( int n )
{
	static char hex[4];
	itoa( n, hex, 16 );

	return hex;
}

/*
===================
Memory
-------------------
===================
*/
void Memory( int numBytes ) 
{
	globalMem += numBytes;

	if( globalMem > 1000 )
	{
		printf( "You have exceeded the memory size of 1KB \n" );
		exit( 1 );
	}
}

/*
===================
SixBitOp
-------------------
Treats the object code for 3 and 4 byte formats
extracting its instruction opcode and flags
===================
*/
char * SixBitOp( char * obj, int * iopcode )
{
	static char opcode[3];
	int decimal = 0;
	memset( opcode, '\0', 3 );
	for ( int i = 0; i < 3; i++ )
	{
		char holder[2] = "", op[5];
		memcpy( holder, &obj[i], 1 );
		int decimalHex = ( int ) strtol( holder, NULL, 16 );

		switch ( i )
		{
			case 0:
				strcpy( opcode, IntToHex( BinaryToInt( IntToBinary( decimalHex ) ) ) );
				break;
			case 1:
				char temp[3] = "", sup[3] = "";
				memcpy( temp, IntToBinary( decimalHex ), 2 );
				strcpy( op, temp );
				strcat( op, "00" );
				strcat( opcode, IntToHex( BinaryToInt( op ) ) );

				for ( int j = 0; j < 2; j++ )
				{ 
					sup[j] = toupper( opcode[j] ); 
				}
				strcpy( opcode, sup );
				printf( "%s \n", opcode );
				break;
			default:
				break;
		}
	}
	for ( int i = 0; i < 2; i++ )
	{
		char holder[2];
		memcpy( holder, &opcode[i], 1 );
		if ( i == 0 )
		{
			decimal = 16 * ( ( int ) strtol( holder, NULL, 16 ) );
		} else {
			decimal += ( int ) strtol( holder, NULL, 16 );
		}
	}
	*iopcode = decimal;
	return opcode;
}

/*
===================
InstructionTable
-------------------
Checks the opcode in hexadecimal format 
and returns the name of the instruction
===================
*/
char * InstructionTable( const char *s )
{
	char line[10], temp[4] = " ";
	static char instructions[10];
	FILE *table = fopen( "InstructionTable.txt", "r" );
	memset( instructions, '\0', 10 );

	while ( fgets( line , sizeof( line ) , table ) != NULL )
    {
      if ( strstr( line, s ) )
      {
		strcat( temp, s );
		memcpy( instructions, line, ( int ) strcspn( line, temp ) );
		printf( "%s \n",instructions );
      }
    }
	fclose( table );
	return instructions; 
}