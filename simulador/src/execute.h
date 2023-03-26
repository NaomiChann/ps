#include <stdio.h>
#include "converters.h"

enum controlCode_t
{
	LESSER,
	EQUAL,
	GREATER
};

typedef struct
{
	int a;
	int x;
	int l;
	int b;
	int s;
	int t;
	int pc;
	int sw;
} registers_t;

/*
===================
InstructionTable
-------------------
Checks the opCode in hexadecimal format 
and returns the name of the instruction
===================
*/
char* InstructionTable( const char* opCode )
{
	FILE* table = fopen( "InstructionTable.txt", "r" );
	static char instruction[7];
	char line[10] = { '\0' }, temp[4] = { '\0' };

	memset( instruction, '\0', 7 );

	while ( fgets( line, sizeof( line ), table ) != NULL )
	{
		if ( strstr( line, opCode ) )
		{

			strcat( temp, opCode );
			memcpy( instruction, line, ( int ) strcspn( line, temp ) );

			printf( "instruction %s \n", instruction );

			fclose( table );
			return instruction; 
		}
	}
	
	fputs( "Instruction not found \n", stderr );
	return NULL;
}

void AlolanExeggcutor( int opCodeInt, registers_t* reg, int* addressM )
{
	switch ( opCodeInt ) // r1 S r2 T
	{
		case 24: // ADD
			reg->a = reg->a + *addressM;
			break;

		case 144: // ADDR
			reg->t = reg->t + reg->s;
			break;

		case 64: // AND
			reg->a = reg->a;
			break;

		case 180: // CLEAR
			reg->s = 0;
			break;

		case 40: // COMP
			if ( reg->a == *addressM )
			{
				reg->sw = EQUAL;
			} else if ( reg->a >= *addressM ) {
				reg->sw = GREATER;
			} else {
				reg->sw = LESSER;
			}
			break;

		case 160: // COMPR
			if ( reg->s == reg->t ) {
				reg->sw = EQUAL;
			}
			else if ( reg->s >= reg->t ) {
				reg->sw = GREATER;
			} else {
				reg->sw = LESSER;
			}
			break;

		case 36: // DIV
			reg->a = reg->a / *addressM;
			break;

		case 156: // DIVR
			reg->t = reg->t / reg->s;
			break;

		case 60: // J
			reg->pc = *addressM; 
			break;

		case 48: // JEQ
			if ( reg->sw == EQUAL )
			{
				reg->pc = *addressM;
			}
			break;

		case 52: // JGT
			if ( reg->sw == GREATER )
			{
				reg->pc = *addressM;
			}
			break;

		case 56: // JLT
			if ( reg->sw == LESSER )
			{
				reg->pc = *addressM;
			}
			break;

		case 72: // JSUB
			reg->l = reg->pc;
			reg->pc = *addressM; 
			break;

		case 0: // LDA
			reg->a = *addressM;
			break;

		case 104: // LDB
			reg->b = *addressM;
			break;

		case 80: // LDCH
			// m as hex into temp
			// regA to string of hex
			// memcpy( &regA[5], *addressM, 1 );
			break;

		case 8: // LDL
			reg->l = *addressM;
			break;

		case 108: // LDS
			reg->s = *addressM;
			break;

		case 116: // LDT
			reg->t = *addressM;
			break;

		case 4: // LDX
			reg->x = *addressM;
			break;

		case 32: // MUL
			reg->a = ( reg->a ) * *addressM;
			break;

		case 152: // MULR
			reg->t = ( reg->t ) * ( reg->s );
			break;

		case 68: // OR
			reg->a = ( reg->a ) || addressM;
			break;

		case 172: // RMO
			reg->t = reg->s;
			break;

		case 76: // RSUB
			reg->pc = reg->l;
			break;

		case 164: // SHIFTL
			reg->s = ( reg->s ) * 2;
			break;

		case 168: // SHIFTR
			reg->s = ( reg->s ) / 2;
			break;

		case 12: // STA
			*addressM = reg->a;
			break;

		case 120: // STB
			*addressM = reg->b;
			break;

		case 84: // STCH
			// both regA and m as strings
			//char AString[6], MString;
			//memcpy( addressM, &AString[5], 1 );
			break;

		case 20: // STL
			*addressM = reg->l;
			break;

		case 124: // STS
			*addressM = reg->s;
			break;

		case 132: // STT
			*addressM = reg->t;
			break;

		case 16: // STX
			*addressM = reg->x;
			break;

		case 28: // SUB
			reg->a = reg->a - *addressM;
			break;

		case 148: // SUBR
			reg->t = ( reg->t ) - ( reg->s );
			break;

		case 44: // TIX
			reg->x = ( reg->x ) + 1;
			if ( reg->x == *addressM )
			{
				reg->sw = EQUAL;
			} else if ( reg->x >= *addressM ) {
				reg->sw = GREATER;
			} else {
				reg->sw = LESSER;
			}
			break;

		case 184: // TIXR
			reg->x = (reg->x) + 1;
			if(reg->x == reg->s){
				reg->sw = EQUAL;
			} else if ( reg->x >= reg->s ) {
				reg->sw = GREATER;
			} else {
				reg->sw = LESSER;
			}
			break;

		case 255: // NON INSTRUCTION
			// this is a variable label
			break;
		
		default:
			fputs( "ERROR \n", stderr );
			break;
			
	}
}