#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "kernighan.h"

char* DecimalToBinary( int n, int length );
int BinaryToDecimal( char* bin, int length );
char* DecimalToHex( int n );
int HexToDecimal( char* hex, int length );
char* HexDigitToBinary4bit( char* hex );
char* Binary4bitToHexDigit( char* bin );
char* Filler( char* string, int length );
void StringToUpper( char* string );
void LeftStrip( char * s );

// todo: some of the length reciever functions
// could just do a strlen check inside themselves

/*
===================
DecimalToBinary
-------------------
Converts a decimal integer into binary
recieves both the int and the desired length
for the returned binary string
===================
*/
char* DecimalToBinary( int n, int length )
{
	if ( length > 24 )
	{
		fputs( "\nSIZE TOO BIG \n", stderr );
		return NULL;
	}

	static char bin[25];
	memset( bin, '\0', 25 );
	itoaB( n, bin, 2 );

	Filler( bin, length );

	return bin;
}

/*
===================
BinaryToDecimal
-------------------
Converts a binary code string into a decimal integer
recieves both the binary string and its length
===================
*/
int BinaryToDecimal( char* bin, int length )
{
	char temp[2];
	memset( temp, '\0', 2 );
	int i, j = 1, decimal = 0;
	
	for( i = ( length - 1 ); i >= 0; i-- )
	{
		memcpy( temp, &bin[i], 1 );
		if ( strcmp( temp, "1" ) == 0 ) {
			decimal += j;
		}
		j = j * 2;
	}

	return decimal;
}

/*
===================
DecimalToHex
-------------------
Converts a decimal integer into a hex string, up to 6 digits
===================
*/
char* DecimalToHex( int n )
{
	static char hex[7];
	memset( hex, '\0', 7 );
	itoaB( n, hex, 16 );
	StringToUpper( hex );

	return hex;
}

char* DecimalToAlphanumeric( int n )
{
	static char an[3];
	char* temp = ( char* ) malloc( sizeof( char ) * ( 2 + 1 ) );
	memset( an, '\0', 3 );
	itoaAlpha( n, an, 37 );

	while ( ( int ) strlen( an ) < 2 )
	{	
		strcpy( temp, "A" );
		strcat( temp, an );
		strcpy( an, temp );
	}

	free( temp );

	return an;
}

/*
===================
HexToDecimal
-------------------
Converts a hex string into a decimal integer
recieves the hex string and its length
===================
*/
int HexToDecimal( char* hex, int length )
{
	int j = length - 1, decimal = 0;
	char holder[2];
	memset( holder, '\0', 2 );

	for ( int i = 0; i < length; i++ )
	{
		memcpy( holder, &hex[i], 1 );

		decimal += ( pow( 16, j ) ) * ( ( int ) strtol( holder, NULL, 16 ) );

		j--;
	}

	return decimal;
}

/*
===================
HexDigitToBinary4bit
-------------------
Converts a hex digit into a 4 bit binary string
===================
*/
char* HexDigitToBinary4bit( char* hex )
{
	return DecimalToBinary( HexToDecimal( hex, 1 ), 4 );
}

/*
===================
Binary4bitToHexDigit
-------------------
Converts a 4 bit binary string into a hex digit
===================
*/
char* Binary4bitToHexDigit( char* bin )
{
	return DecimalToHex( BinaryToDecimal( bin, 4 ) );
}

/*
===================
Filler
-------------------
Fills the left side of a string with "0"
until it's a 4 character string
===================
*/
char* Filler( char* string, int length )
{
	char* temp = ( char* ) malloc( sizeof( char ) * ( length + 1 ) );

	while ( ( int ) strlen( string ) < length )
	{	
		strcpy( temp, "0" );
		strcat( temp, string );
		strcpy( string, temp );
	}

	free( temp );
	return string;
}

/*
===================
StringToUpper
-------------------
Changes all (applicable) characters in a string to uppercase
===================
*/
void StringToUpper( char* string )
{
	int length = strlen( string );
	char* upper = ( char* ) malloc( ( sizeof( char ) * length ) + 1 );
	memset( upper, '\0', ( length + 1 ) );

	for ( int j = 0; j < length; j++ )
	{ 
		upper[j] = toupper( string[j] ); 
	}

	strcpy( string, upper );

	free( upper );
	return;
}


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
	//strcat( p, "\n" );
	memmove( s, p, l + 1 );
}

/*
===================
ClearFile
-------------------
Clear symboltable file in the beginning of each run since w
mode deletes all of its contents if it exists.
(truncates file)
also sets all registers as labels so they can't be
declared acagin in the symbol table
===================
*/
void ClearFile( const char* filename ) 
{
	FILE* file = fopen( filename, "w" );
	if ( file != NULL ) 
	{
		if ( strcmp( filename, "tables/symbol.txt" ) == 0 )
		{
		fprintf( file, "A\t0\nX\t1\nL\t2\nB\t3\nS\t4\nT\t5\nP\t8\nC\t9\n" );
		}
		fflush( file );
		fclose( file );
	}
}

char * ASCIItoHex(char * ascii )
{
	FILE* inputFile = fopen( "tables/ascii.txt", "r" );
	char * asciiTable, ascii2[2], *hex, line[256];

	ascii2[0] = *ascii;
	ascii2[1] = '\0';

	printf("%c",*ascii);
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