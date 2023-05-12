#include <stdio.h>
#include "converters.h"
//#include "hermes.h"

// GLOBAL
extern int reg_g[10];
extern int current_g;

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

/*
===================
InstructionTable
-------------------
Checks the opCode in hexadecimal format 
and returns the name of the instruction
===================
*/
char* InstructionTable( const char* opCode )
// todo: insert behavior into the execution so there is no need to use a file
{
	FILE* table = fopen( "tables/instruction.txt", "r" );
	static char instruction[7];
	char line[16] = { '\0' }, temp[4] = { '\0' }, santiago[4] = " ";

	memset( instruction, '\0', 7 );
	strcat( santiago, opCode );

	while ( fgets( line, sizeof( line ), table ) != NULL )
	{
		if ( strstr( line, santiago ) )
		{
			strcpy( instruction, strtok( line, " " ) );

			printf( "%s \n", instruction );

			fclose( table );
			return instruction; 
		}
	}
	
	fputs( "INSTRUCTION NOT IMPLEMENTED \n", stderr );
	return NULL;
}

/*
===================
Addressing
-------------------
Calculates flags x, b and p displacement in 3/4 format instructions
includes program counter relative's 2's complement
===================
*/
int Addressing( int* target, bool* flag )
{
	if ( flag[F_X] )
	{
		*target += reg_g[R_X];
	}

	if ( flag[F_B] )
	{
		*target += reg_g[R_B];
	}

	if ( flag[F_P] )
	{
		char binTarget[13] = { '\0' }, temp[2] = { '\0' };

		strcpy( binTarget, DecimalToBinary( *target, 12 ) );
		memcpy( temp, &binTarget[0], 1 );

		if ( strcmp( temp, "1" ) == 0 ) // check complement of 2
		{
			for ( int i = 1; i < 12; i++ )
			{
				if ( binTarget[i] == '1' )
				{
					binTarget[i] = '0';
				} else {
					binTarget[i] = '1';
				}
			}

			*target = ( BinaryToDecimal( &binTarget[1], 11 ) ) + 1;
			*target = 0 - *target;
		}

		*target += reg_g[R_PC];
	}

	return *target;
}

/*
===================
AlolanExeggcutor
-------------------
Main execution function, resovles every implemented
instruction according to their definitions
===================
*/
void AlolanExeggcutor( int opCodeInt, int* value, int* r, mem_t* memory )
{
	char rightByte[3] = { '\0' }, temp[7] = { '\0' };
	char dummy[25] = { '\0' }, dummy2[25] = { '\0' };
	int amount, valueHolder;

	if ( opCodeInt == 12 || opCodeInt == 120 || opCodeInt == 84 || opCodeInt == 20 || opCodeInt == 124 || opCodeInt == 132 || opCodeInt == 16 )
	{
		valueHolder = *value;
		if ( memory[current_g].size == 3 )
		{
			valueHolder = HexToDecimal( memory[current_g].lastBits, 3 );
		} else {
			valueHolder = HexToDecimal( memory[current_g].lastBits, 5 );
		}
		Addressing( &valueHolder, memory[current_g].flag );

		switch ( opCodeInt ) // store
		{
			case 12: // STA
				memory[valueHolder].opCodeInt = reg_g[R_A];
				break;

			case 120: // STB
				memory[valueHolder].opCodeInt = reg_g[R_B];
				break;

			case 84: // STCH
				strcpy( temp, DecimalToHex( reg_g[R_A] ) );
				Filler( temp, 6 );
				memcpy( rightByte, &temp[4], 2 );
				memory[valueHolder].opCodeInt = HexToDecimal( rightByte, 2 );
				break;

			case 20: // STL
				memory[valueHolder].opCodeInt = reg_g[R_L];
				break;

			case 124: // STS
				memory[valueHolder].opCodeInt = reg_g[R_S];
				break;

			case 132: // STT
				memory[valueHolder].opCodeInt = reg_g[R_T];
				break;

			case 16: // STX
				memory[valueHolder].opCodeInt = reg_g[R_X];
				break;
		
		}
		return;
	}

	if ( opCodeInt == 60 || opCodeInt == 48 || opCodeInt == 52 || opCodeInt == 56 || opCodeInt == 72 )
	{
		valueHolder = *value;
		if ( memory[current_g].size == 3 )
		{
			valueHolder = HexToDecimal( memory[current_g].lastBits, 3 );
		} else {
			valueHolder = HexToDecimal( memory[current_g].lastBits, 5 );
		}
		Addressing( &valueHolder, memory[current_g].flag );

		switch ( opCodeInt )
		{
			case 60: // J
				reg_g[R_PC] = valueHolder; 
				break;

			case 48: // JEQ
				if ( reg_g[R_SW] == EQUAL )
				{
					reg_g[R_PC] = valueHolder;
				}
				break;

			case 52: // JGT
				if ( reg_g[R_SW] == GREATER )
				{
					reg_g[R_PC] = valueHolder;
				}
				break;

			case 56: // JLT
				if ( reg_g[R_SW] == LESSER )
				{
					reg_g[R_PC] = valueHolder;
				}
				break;

			case 72: // JSUB
				reg_g[R_L] = reg_g[R_PC];
				reg_g[R_PC] = valueHolder; 
				break;

		}
		return;
	}

	switch ( opCodeInt ) // r1 S r2 T
	{
		case 24: // ADD
			reg_g[R_A] = reg_g[R_A] + *value;
			break;

		case 144: // ADDR
			reg_g[r[1]] = reg_g[r[1]] + reg_g[r[0]];
			break;

		case 64: // AND
			reg_g[R_A] = reg_g[R_A] & *value;
			break;

		case 180: // CLEAR
			reg_g[r[0]] = 0;
			break;

		case 40: // COMP
			if ( reg_g[R_A] == *value )
			{
				reg_g[R_SW] = EQUAL;
			} else if ( reg_g[R_A] >= *value ) {
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
			reg_g[R_A] = reg_g[R_A] / *value;
			break;

		case 156: // DIVR
			reg_g[r[1]] = reg_g[r[1]] / reg_g[r[0]];
			break;

		case 0: // LDA
			reg_g[R_A] = *value;
			break;

		case 104: // LDB
			reg_g[R_B] = *value;
			break;

		case 80: // LDCH
			strcpy( temp, DecimalToHex( *value ) );
			Filler( temp, 6 );
			memcpy( rightByte, &temp[4], 2 );
			reg_g[R_A] = HexToDecimal( rightByte, 2 );
			break;

		case 8: // LDL
			reg_g[R_L] = *value;
			break;

		case 108: // LDS
			reg_g[R_S] = *value;
			break;

		case 116: // LDT
			reg_g[R_T] = *value;
			break;

		case 4: // LDX
			reg_g[R_X] = *value;
			break;

		case 32: // MUL
			reg_g[R_A] = reg_g[R_A] * *value;
			break;

		case 152: // MULR
			reg_g[r[1]] = reg_g[r[1]] * reg_g[r[0]];
			break;

		case 68: // OR
			reg_g[R_A] = reg_g[R_A] | *value;
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

		case 28: // SUB
			reg_g[R_A] = reg_g[R_A] - *value;
			break;

		case 148: // SUBR
			reg_g[r[1]] = reg_g[r[1]] - reg_g[r[0]];
			break;

		case 44: // TIX
			reg_g[R_X] = reg_g[R_X] + 1;
			if ( reg_g[R_X] == *value )
			{
				reg_g[R_SW] = EQUAL;
			} else if ( reg_g[R_X] >= *value ) {
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

		case 220: // WD
			char out[16] = { '\0' };
			itoaB( *value, out, 10 );
			printf( "\n------------- \n" );
			printf( "[OUTPUT]: %s", out );
			break;
		
		default: // yeah
			fputs( "\nNOT IMPLEMENTED ", stderr );
			break;
			
	}
	return;
}
