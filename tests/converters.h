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

/*
===================
DecimalToBinary
-------------------
Converts an integer into 4 bit binary code
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
Converts a 4 bit binary code string
into an integer
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
Converts a decimal into its
hex equivalent, up to 6 bytes
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

/*
===================
HexToDecimal
-------------------
Converts a hex string into its
decimal equivalent from length
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

char* HexDigitToBinary4bit( char* hex )
{
	return DecimalToBinary( HexToDecimal( hex, 1 ), 4 );
}

char* Binary4bitToHexDigit( char* bin )
{
	return DecimalToHex( BinaryToDecimal( bin, 4 ) );
}

// adds 0 to the left until it's a 4 character string
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