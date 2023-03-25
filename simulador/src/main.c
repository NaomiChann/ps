#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "kernighan.h"

#define N 0
#define I 1
#define X 2
#define B 3
#define P 4
#define E 5
#define LESSER 0
#define EQUAL 1
#define GREATER 2

char * IntToBinaryByteString( int n );
int BinaryByteStringToInt( char * bin );
char * IntToHex( int n );
char * SixBitOp( char * obj, int * opcodeInt, char * flags, char * lastBits );
char * InstructionTable( const char *s );
void AlolanExeggcutor( int iopcode, int * regL, int * regA, int * regB, int * regS, int * regT, int * regX, int * regPC, char * regSW, int * addressM );
int HexStringToDecimal( char * string, int size );
int Addressing( int target, bool * flag , int regB,  int regX, int regPC );
int SIC( char * flags, bool * flag, int regX, char * lastBits );

int main()
{
	FILE *inputFile = fopen( "cod-obj.txt", "r" );
	char objCode[9];
	char opcode[3], flags[7], lastBits[6];
	int instSize, opcodeInt, target;
	bool flag[6];
	
	char temp[2];
	int regL = 0, regA = 0, regB = 0, regS = 0, regT = 0, regX = 0, regPC = 0, regSW = 0;
	int tempI;

// if error reading file
	if( !inputFile ) 
	{
			fputs( "inputFile error.\n", stderr );
			return EXIT_FAILURE;
	}
// reads until eof
	printf( "\n============= \n" );
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
		printf( "%X | %s ", regPC, objCode );
		regPC += instSize / 2;
// if memory full
		if ( regPC > 1024 )
		{
			printf( "\n=!=!=!=!=!=!= \n\nOUT OF MEMORY \n\n=!=!=!=!=!=!= \n" );
			return EXIT_FAILURE;
		}
// each pair of hex values is a byte
		switch ( instSize / 2 )
		{
			case 1:
// there are no 1 byte instructions on our table
				printf( "\n" );
				break;
			case 2:
				char temp2[3];
// 1st byte as op, 2nd byte both r1 and r2 each with 4 bits
				memset( temp, '\0', 2 );
				memset( temp2, '\0', 3 );
				memcpy( temp2, objCode, 2 );
				printf( "\nopcode %s \n", temp2 ); // temp2 = opcode

				memcpy( temp, &objCode[2], 1 );
				printf( "r1 %s ", temp ); // temp = r1
				regS = atoi( temp );

				memcpy( temp, &objCode[3], 1 );
				printf( "r2 %s \n", temp ); // temp = r2
				regT = atoi( temp );

				InstructionTable( temp2 );
				break;
			case 3:
				flag[E] = false;
				strcpy( opcode, SixBitOp( objCode, &opcodeInt, flags, lastBits ) );
				InstructionTable( opcode );
				tempI = HexStringToDecimal( lastBits, 3 );

				for( int i = 0; i < 5; i++ )
				{
					memcpy( temp, &flags[i], 1 );
					if ( strcmp( temp, "1" ) == 0 ) {
						flag[i] = true;
					} else {
						flag[i] = false;
					}
				}
				break;
			case 4:
				flag[E] = true;
				strcpy( opcode, SixBitOp( objCode, &opcodeInt, flags, lastBits ) );
				InstructionTable( opcode );
				tempI = HexStringToDecimal( lastBits, 5 );

				for( int i = 0; i < 5; i++ )
				{
					memcpy( temp, &flags[i], 1 );
					if ( strcmp( temp, "1" ) == 0 ) {
						flag[i] = true;
					} else {
						flag[i] = false;
					}
				}
				break;
			default:
				printf( "SIZE ERROR \n" );
				break;
		}

		printf( "nixbpe " );
		for ( int i = 0; i < 6; i++ )
		{
			if ( ( instSize / 2 ) > 2 && flag[i] == true )
			{
				printf( "1" );
			} else {
				printf( "0" );
			}
		}

		if ( ( ( instSize / 2 ) == 3  || ( instSize / 2 ) == 4 ) )
		{
			if ( !flag[N] && !flag[I] ) //00
			{
				target = SIC( flags, flag, regX, lastBits );
			} else {
				if( flag[N] && flag[I] ) //11
				{	
					target = Addressing( tempI, flag, regB, regX, regPC ); // address
				}

				if( flag[N] && !flag[I] ) //10
				{
					target = Addressing( tempI, flag, regB, regX, regPC ); // pointer
				}

				if( !flag[N] && flag[I] ) //01
				{
					target = Addressing( tempI, flag, regB, regX, regPC ); // value
				}

				
			}
		}

		AlolanExeggcutor( opcodeInt, &regL, &regA, &regB, &regS, &regT, &regX, &regPC, &regSW, &target );
		printf( "\nnext address %X", target );
		
		printf( "\n============= \n" );

	}
	printf( "Memory used: %d Bytes \n", regPC );
	fclose( inputFile );

	return 0;
}

/*
===================
IntToBinaryByteString
-------------------
Converts an integer into 4 bit binary code
and returns it as a string 
===================
*/
char * IntToBinaryByteString( int n )
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
		itoaB( bin[i], temp, 10 );
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
BinaryByteStringToInt
-------------------
Converts a 4 bit binary code string
into an integer
===================
*/
int BinaryByteStringToInt( char * bin )
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
	itoaB( n, hex, 16 );

	return hex;
}

/*
===================
SixBitOp
-------------------
Treats the object code for 3 and 4 byte formats
extracting its instruction opcode and flags
===================
*/
char * SixBitOp( char * obj, int * opcodeInt, char * flags, char * lastBits )
{
	static char opcode[3];
	char holder[2] = "";
	int decimal = 0;
	memset( opcode, '\0', 3 );
	memset( lastBits, '\0', 6 );
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
				strcpy( opcode, IntToHex( BinaryByteStringToInt( IntToBinaryByteString( decimalHex ) ) ) );
				break;
			case 1: // byte 2
				char temp[3] = "", sup[3] = "";
				strcpy( holder2, IntToBinaryByteString( decimalHex ) );
				memcpy( temp, holder2, 2 );
				strcpy( op, temp );
// adds two bits 0 at the end for a total of 4 bits
// and translates it back to hex
				strcat( op, "00" );
				strcat( opcode, IntToHex( BinaryByteStringToInt( op ) ) );

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
				strcat( flags, IntToBinaryByteString( decimalHex ) );
				//printf( "flags %s \n", flags );
				break;
			default:
				break;
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

// changes opcodeInt through pointers so function
// can still return opcode's string for printing
	*opcodeInt = HexStringToDecimal( opcode, 2 );;
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
		printf( "instruction %s \n",instructions );
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

/*
===================
AlolanExeggcutor
-------------------
Executes
===================
*/
void AlolanExeggcutor( int iopcode, int * regL, int * regA, int * regB, int * regS, int * regT, int * regX, int * regPC, char * regSW, int * addressM )
{
		switch ( iopcode ) // r1 S r2 T
		{
		case 24: // ADD
			*regA = *regA + addressM;
			break;
		case 144: // ADDR
			*regT = *regT + *regS;
			break;
		case 64: // AND
			*regA = *regA;
			break;
		case 180: // CLEAR
			*regS = 0;
			break;
		case 40: // COMP
			if(*regA == addressM){
				*regSW = EQUAL;
			}
			else if(*regA >= addressM){
				*regSW = GREATER;
			}
			else{
				*regSW = LESSER;
			}
			break;
		case 160: // COMPR
			if(*regS == *regT){
				*regSW = EQUAL;
			}
			else if(*regS >= *regT){
				*regSW = GREATER;
			}
			else{
				*regSW = LESSER;
			}
			break;
		case 36: // DIV
			*regA = *regA / *addressM;
			break;
		case 156: // DIVR
			*regT = *regT / *regS;
			break;
		case 60: // J
			*regPC = *addressM; 
			break;
		case 48: // JEQ
			if( regSW == EQUAL )
			{
				*regPC = addressM;
			}
			break;
		case 52: // JGT
			if( regSW == GREATER )
			{
				*regPC = addressM;
			}
			break;
		case 56: // JLT
			if( regSW == LESSER )
			{
				*regPC = addressM;
			}
			break;
		case 72: // JSUB
			*regL = *regPC;
			*regPC = addressM; 
			break;
		case 0: // LDA
			*regA = addressM;
			break;
		case 104: // LDB
			*regB = addressM;
			break;
		case 80: // LDCH
			// m as hex into temp
			// regA to string of hex
			memcpy( &regA[5], addressM, 1 );
			break;
		case 8: // LDL
			*regL = addressM;
			break;
		case 108: // LDS
			*regS = addressM;
			break;
		case 116: // LDT
			*regT = addressM;
			break;
		case 4: // LDX
			*regX = addressM;
			break;
		case 32: // MUL
			*regA = (*regA) * *addressM;
			break;
		case 152: // MULR
			*regT = (*regT) * (*regS);
			break;
		case 68: // OR
			*regA = (*regA) || addressM;
			break;
		case 172: // RMO
			*regT = *regS;
			break;
		case 76: // RSUB
			*regPC = *regL;
			break;
		case 164: // SHIFTL
			*regS = (*regS)*2;
			break;
		case 168: // SHIFTR
			*regS = (*regS)/2;
			break;
		case 12: // STA
			addressM = *regA;
			break;
		case 120: // STB
			addressM = *regB;
			break;
		case 84: // STCH
			// both regA and m as strings
			memcpy( addressM, &regA[5], 1 );
			break;
		case 20: // STL
			addressM = *regL;
			break;
		case 124: // STS
			addressM = *regS;
			break;
		case 132: // STT
			addressM = *regT;
			break;
		case 16: // STX
			addressM = *regX;
			break;
		case 28: // SUB
			*regA = *regA - *addressM;
			break;
		case 148: // SUBR
			*regT = (*regT) - (*regS);
			break;
		case 44: // TIX
			*regX = (*regX) + 1;
			if(*regX == addressM){
				*regSW = EQUAL;
			}
			else if(*regX >= addressM){
				*regSW = GREATER;
			}
			else{
				*regSW = LESSER;
			}
			break;
		case 184: // TIXR
			*regX = (*regX) + 1;
			if(*regX == *regS){
				*regSW = EQUAL;
			}
			else if(*regX >= *regS){
				*regSW = GREATER;
			}
			else{
				*regSW = LESSER;
			}
			break;
		
		default:
			printf( "ERROR" );
			break;
	}
}

int HexStringToDecimal( char * string, int size )
{
	int j = size - 1, decimal = 0;
	char holder[2] = "";
	for ( int i = 0; i < size; i++ )
    {
        memcpy( holder, &string[i], 1 );

        if ( j == 0 )
        {
            decimal += ( int ) strtol( holder, NULL, 16 );
        } else {
            decimal = 16 * j * ( ( int ) strtol( holder, NULL, 16 ) );
        }

        j--;
    }

	return decimal;
}

int Addressing( int target, bool * flag , int regB,  int regX, int regPC )
{
	if ( flag[X] )
	{
		target += regX;
	}

	if ( flag[B] )
	{
		target += regB;
	}

	if ( flag[P] )
	{
		target += regPC;
	}

	return target;
}

int SIC( char * flags, bool * flag, int regX, char * lastBits )
{
	char temp2[4] = "0";
	int target;
	strcat( temp2, &flags[3] );
	if ( flag[E] == true ) {
		target = ( BinaryByteStringToInt( temp2 ) * 5 ) + HexStringToDecimal( lastBits, 5 );
	} else {
		target = ( BinaryByteStringToInt( temp2 ) * 3 ) + HexStringToDecimal( lastBits, 3 );
	}
	
	if( flag[X] ){
		target += regX;
	}
}