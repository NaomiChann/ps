#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char * IntToBinary( int n );
int BinaryToInt( char * bin );
char * IntToHex( int n );

int main()
{
	FILE *inputFile = fopen( "cod-obj.txt", "r" );
	char objCode[9], op[5], opcode[3];
	int instSize;

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
		printf( "%d bytes format, ", instSize );

		for ( int i = 0; i < 3; i++ )
		{
			char holder[2];
			int decimalHex;
			memcpy( holder, &objCode[i], 1 );
			decimalHex = ( int ) strtol( holder, NULL, 16 );
			printf( "%s ", IntToBinary( decimalHex ) );
		}
/*
here three binary string of 4 bits get output

the second set gets split into two 2 bits strings
then the third and fourth sets get concatenated into
one 6 bits string (nixbpe)
*/
			strcpy( op, "1111" );
// the first set of 4 bits enters here (as op)
			strcpy( opcode, IntToHex( BinaryToInt( op ) ) );

// the remaining set of 2 enters here (as op)
			strcpy( op, "10" );
			strcat( op, "00" );
			strcat( opcode, IntToHex( BinaryToInt( op ) ) );

			printf( "%s \n", opcode );
/*
		switch ( instSize / 2 )
		{
			case 1:
				printf( "8 bits op \n" );
				break;
			case 2:
				printf( "8 bits op, 4 bits r1, 4 bits r2 \n" );
				break;
			case 3:
				printf( "6 bits op, 6 bits nixbpe \n" );
				break;
			case 4:
				printf( "6 bits op, 6 bits nixbpe \n" );
				break;
			default:
				printf( "SIZE ERROR \n" );
				break;
		}
*/
	}

	fclose( inputFile );

	return 0;
}

char * IntToBinary( int n )
{
	int bin[5], i;
	static char string[5];
	char temp[5] = "";
	strcpy( string, "" );
	
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
IntToBinary
-------------------
Converts an integer into 4 bit binary code
and returns it as a string 
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
BinaryToInt
-------------------
Converts a 4 bit binary code string
into an integer
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
IntToHex
-------------------
Converts an integer into a string
containing the value as a hexadecimal
===================
*/