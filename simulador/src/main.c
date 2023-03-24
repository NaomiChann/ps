#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

char * IntToBinary( int n );
int BinaryToInt( char * bin );
char * IntToHex( int n );
void Memory( int numBytes );
char * SixBitOp( char * obj, int * iopcode, char * flags, char * lastBits );
char * InstructionTable( const char *s );
void AlolanExeggcutor( int iopcode );

void Bitoa(int n, char s[]);
int Batoi(char s[]);
void Breverse(char s[]);

int globalMem;

int main()
{
	FILE *inputFile = fopen( "cod-obj.txt", "r" );
	char objCode[9], temp[2], temp2[3], opcode[3], flags[7], lastBits[21];
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
				printf( "\n" );
				Memory( 1 );
				break;
			case 2:
// 1st byte as op, 2nd byte both r1 and r2 each with 4 bits
				memset( temp, '\0', 2 );
				memset( temp2, '\0', 3 );
				memcpy( temp2, objCode, 2 );
				printf( "\nopcode %s \n", temp2 ); // temp2 = opcode

				memcpy( temp, &objCode[2], 1 );
				printf( "r1 %s ", temp ); // temp = r1

				memcpy( temp, &objCode[3], 1 );
				printf( "r2 %s \n", temp ); // temp = r2

				InstructionTable( temp2 );
				Memory( 2 );
				break;
			case 3:
				strcpy( opcode, SixBitOp( objCode, &iopcode, flags, lastBits ) );
				InstructionTable( opcode );
				Memory( 3 );
				break;
			case 4:
				strcpy( opcode, SixBitOp( objCode, &iopcode, flags, lastBits ) );
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
		Bitoa( bin[i], temp );
		strcat( string, temp );
	}

	strcpy( temp, "0" );

// adds 0 to the left until it's a 4 character string
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
	Bitoa( n, hex );

	return hex;
}

/*
===================
Memory
-------------------
Description required
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
char * SixBitOp( char * obj, int * iopcode, char * flags, char * lastBits )
{
	static char opcode[3];
	char holder[2] = "";
	int decimal = 0;
	memset( opcode, '\0', 3 );
	memset( flags, '\0', 7 );
	for ( int i = 0; i < 3; i++ )
	{

// individually translates each hex byte into a decimal integer
		char op[5], holder2[5];
		memcpy( holder, &obj[i], 1 );
		int decimalHex = ( int ) strtol( holder, NULL, 16 );

// treats each byte individually
		switch ( i )
		{
			case 0: // byte 1
				strcpy( opcode, IntToHex( BinaryToInt( IntToBinary( decimalHex ) ) ) );
				break;
			case 1: // byte 2
				char temp[3] = "", sup[3] = "";
				strcpy( holder2, IntToBinary( decimalHex ) );
				memcpy( temp, holder2, 2 );
				strcpy( op, temp );
// adds two bits 0 at the end for a total of 4 bits
// and translates it back to hex
				strcat( op, "00" );
				strcat( opcode, IntToHex( BinaryToInt( op ) ) );

// capitalizes any characters in the hex string
				for ( int j = 0; j < 2; j++ )
				{ 
					sup[j] = toupper( opcode[j] ); 
				}
				strcpy( opcode, sup );
				printf( "\nopcode %s \n", opcode );

				memcpy( flags, &holder2[2], 2 );
				break;
			case 2: // byte 3
				strcat( flags, IntToBinary( decimalHex ) );
				printf( "flags %s \n", flags );
				break;
			default:
				break;
		}
	}

	for ( int i = 0; i < 2; i++ )
	{
		char holder[2];
		memcpy( holder, &opcode[i], 1 );

// converts a hex string into a decimal integer
		if ( i == 0 )
		{
			decimal = 16 * ( ( int ) strtol( holder, NULL, 16 ) );
		} else {
			decimal += ( int ) strtol( holder, NULL, 16 );
		}
	}

	memcpy( lastBits, &obj[3], 3 );

	if ( strlen( obj ) == 8 ) // 4 more bits
	{
		strcat( lastBits, &obj[6] );
		printf( "address %s \n", lastBits );
	} else {
		printf( "displacement %s \n", lastBits );
	}

	

// changes iopcode through pointers so function
// can still return opcode's string
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
	bool test = false;
	FILE *table = fopen( "InstructionTable.txt", "r" );
	memset( instructions, '\0', 10 );

	while ( fgets( line , sizeof( line ) , table ) != NULL )
    {
      if ( strstr( line, s ) )
      {
		strcat( temp, s );
		memcpy( instructions, line, ( int ) strcspn( line, temp ) );
		printf( "%s \n",instructions );
		test = true;
      }
    }
	if ( test == false )
	{
		printf( "Instruction not found \n" );
	}
	fclose( table );
	return instructions; 
}

void AlolanExeggcutor( int iopcode )
{
	switch ( iopcode )
	{
	case 24: // ADD
		break;
	case 144: // ADDR
		break;
	case 64: // AND
		break;
	case 180: // CLEAR
		break;
	case 40: // COMP
		break;
	case 160: // COMPR
		break;
	case 36: // DIV
		break;
	case 156: // DIVR
		break;
	case 60: // J
		break;
	case 48: // JEQ
		break;
	case 52: // JGT
		break;
	case 56: // JLT
		break;
	case 72: // JSUB
		break;
	case 0: // LDA
		break;
	case 104: // LDB
		break;
	case 80: // LDCH
		break;
	case 8: // LDL
		break;
	case 108: // LDS
		break;
	case 116: // LDT
		break;
	case 4: // LDX
		break;
	case 32: // MUL
		break;
	case 152: // MULR
		break;
	case 68: // OR
		break;
	case 172: // RMO
		break;
	case 76: // RSUB
		break;
	case 164: // SHIFTL
		break;
	case 168: // SHIFTR
		break;
	case 12: // STA
		break;
	case 120: // STB
		break;
	case 84: // STCH
		break;
	case 20: // STL
		break;
	case 124: // STS
		break;
	case 132: // STT
		break;
	case 16: // STX
		break;
	case 28: // SUB
		break;
	case 148: // SUBR
		break;
	case 44: // TIX
		break;
	case 184: // TIXR
		break;
	
	default:
		printf( "ERROR" );
		break;
	}
}

/* Bitoa:  convert n to characters in s */
void Bitoa(int n, char s[])
{
	int i, sign;

	if ((sign = n) < 0)  /* record sign */
		n = -n;          /* make n positive */
	i = 0;
	do {       /* generate digits in Breverse order */
		s[i++] = n % 10 + '0';   /* get next digit */
	} while ((n /= 10) > 0);     /* delete it */
	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	Breverse(s);
}

/* Batoi: convert s to integer */
int Batoi(char s[])
{
	int i, n;
	n = 0;
	for(i = 0; s[i] >= '0' && s[i] <= '9'; ++i)
	{
		n = 10 * n + (s[i] - '0');
	}

	return(n);
}

 /* Breverse:  Breverse string s in place */
void Breverse(char s[])
{
	int i, j;
	char c;

	for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}  