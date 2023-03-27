#include <stdio.h>
#include "converters.h"

enum controlCode_t
{
	LESSER,
	EQUAL,
	GREATER
};

enum flag_t
{
	F_N,
	F_I,
	F_X,
	F_B,
	F_P,
	F_E
};

enum reg_t
{
	R_A,
	R_X,
	R_L,
	R_B,
	R_S,
	R_T,
	R_PC = 8,
	R_SW
};

typedef struct
{
	int size;
	int opCodeInt;
	char objCode[9];
	char opCode[3];
	char lastBits[6];
	bool flag[6];
} mem_t;

extern int reg_g[10];

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
	
	fputs( "INSTRUCTION NOT IMPLEMENTED \n", stderr );
	return NULL;
}

/*
===================
AlolanExeggcutor
-------------------
Main executioner function, resovles every implemented
instruction according to their definitions
===================
*/
void AlolanExeggcutor( int opCodeInt, int* addressM, int* r, mem_t* memory )
{
	char rightByte[3] = { '\0' }, temp[7] = { '\0' };
	char dummy[25] = { '\0' }, dummy2[25] = { '\0' };
	int amount, holdies[10];

	if ( memory)
	
	switch ( opCodeInt ) // r1 S r2 T
	{
		case 24: // ADD
			reg_g[R_A] = reg_g[R_A] + *addressM;
			break;

		case 144: // ADDR
			reg_g[r[1]] = reg_g[r[1]] + reg_g[r[0]];
			break;

		case 64: // AND
			reg_g[R_A] = reg_g[R_A] & *addressM;
			break;

		case 180: // CLEAR
			reg_g[r[0]] = 0;
			break;

		case 40: // COMP
			if ( reg_g[R_A] == *addressM )
			{
				reg_g[R_SW] = EQUAL;
			} else if ( reg_g[R_A] >= *addressM ) {
				reg_g[R_SW] = GREATER;
			} else {
				reg_g[R_SW] = LESSER;
			}
			break;

		case 160: // COMPR
			if ( reg_g[r[0]] == reg_g[r[1]] ) {
				reg_g[R_SW] = EQUAL;
			}
			else if ( reg_g[r[0]] >= reg_g[r[1]] ) {
				reg_g[R_SW] = GREATER;
			} else {
				reg_g[R_SW] = LESSER;
			}
			break;

		case 36: // DIV
			reg_g[R_A] = reg_g[R_A] / *addressM;
			break;

		case 156: // DIVR
			reg_g[r[1]] = reg_g[r[1]] / reg_g[r[0]];
			break;

		case 60: // J
			reg_g[R_PC] = *addressM; 
			break;

		case 48: // JEQ
			if ( reg_g[R_SW] == EQUAL )
			{
				reg_g[R_PC] = *addressM;
			}
			break;

		case 52: // JGT
			if ( reg_g[R_SW] == GREATER )
			{
				reg_g[R_PC] = *addressM;
			}
			break;

		case 56: // JLT
			if ( reg_g[R_SW] == LESSER )
			{
				reg_g[R_PC] = *addressM;
			}
			break;

		case 72: // JSUB
			reg_g[R_L] = reg_g[R_PC];
			reg_g[R_PC] = *addressM; 
			break;

		case 0: // LDA
			reg_g[R_A] = *addressM;
			break;

		case 104: // LDB
			reg_g[R_B] = *addressM;
			break;

		case 80: // LDCH
			strcpy( temp, DecimalToHex( *addressM ) );
			Filler( temp, 6 );
			memcpy( rightByte, &temp[4], 2 );
			reg_g[R_A] = HexToDecimal( rightByte, 2 );
			break;

		case 8: // LDL
			reg_g[R_L] = *addressM;
			break;

		case 108: // LDS
			reg_g[R_S] = *addressM;
			break;

		case 116: // LDT
			reg_g[R_T] = *addressM;
			break;

		case 4: // LDX
			reg_g[R_X] = *addressM;
			break;

		case 32: // MUL
			reg_g[R_A] = reg_g[R_A] * *addressM;
			break;

		case 152: // MULR
			reg_g[r[1]] = reg_g[r[1]] * reg_g[r[0]];
			break;

		case 68: // OR
			reg_g[R_A] = reg_g[R_A] | *addressM;
			break;

		case 172: // RMO
			reg_g[r[1]] = reg_g[r[0]];
			break;

		case 76: // RSUB
			reg_g[R_PC] = reg_g[R_L];
			break;

		case 164: // SHIFTL
			dummy[25] = '\0';
			dummy2[25] = '\0';
			amount = ( reg_g[r[1]] + 1 );

	// circular shift
			strcpy( dummy2, DecimalToBinary( reg_g[r[0]], 24 ) );
			strcpy( dummy, &dummy2[amount] );
			memcpy( dummy, dummy2, amount );
			reg_g[r[0]] = BinaryToDecimal( dummy, 24 );
			break;

		case 168: // SHIFTR
			reg_g[r[0]] = reg_g[r[0]] >> ( reg_g[r[1]] + 1 );
			break;

		case 12: // STA
			memory[*addressM].opCodeInt = reg_g[R_A];
			break;

		case 120: // STB
			memory[*addressM].opCodeInt = reg_g[R_B];
			break;

		case 84: // STCH
			strcpy( temp, DecimalToHex( reg_g[R_A] ) );
			Filler( temp, 6 );
			memcpy( rightByte, &temp[4], 2 );
			*addressM = HexToDecimal( rightByte, 2 );
			break;

		case 20: // STL
			memory[*addressM].opCodeInt = reg_g[R_L];
			break;

		case 124: // STS
			memory[*addressM].opCodeInt = reg_g[R_S];
			break;

		case 132: // STT
			memory[*addressM].opCodeInt = reg_g[R_T];
			break;

		case 16: // STX
			memory[*addressM].opCodeInt = reg_g[R_X];
			break;

		case 28: // SUB
			reg_g[R_A] = reg_g[R_A] - *addressM;
			break;

		case 148: // SUBR
			reg_g[r[1]] = reg_g[r[1]] - reg_g[r[0]];
			break;

		case 44: // TIX
			reg_g[R_X] = reg_g[R_X] + 1;
			if ( reg_g[R_X] == *addressM )
			{
				reg_g[R_SW] = EQUAL;
			} else if ( reg_g[R_X] >= *addressM ) {
				reg_g[R_SW] = GREATER;
			} else {
				reg_g[R_SW] = LESSER;
			}
			break;

		case 184: // TIXR
			reg_g[R_X] = reg_g[R_X] + 1;
			if( reg_g[R_X] == reg_g[r[0]] )
			{
				reg_g[R_SW] = EQUAL;
			} else if ( reg_g[R_X] >= reg_g[r[0]] ) {
				reg_g[R_SW] = GREATER;
			} else {
				reg_g[R_SW] = LESSER;
			}
			break;
		
		default: // yeah
			fputs( "\nNOT IMPLEMENTED ", stderr );
			break;
			
	}
}
