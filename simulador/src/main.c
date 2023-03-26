/*
	3KB memory ( 2^10 sets of 24 bits )
*/

#include <stdbool.h>
#include "execute.h"

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


void SixBitOp( char* obj, char* opCode, bool* flag, char* lastBits, int* opCodeInt );
void Addressing( int* target, bool* flag , registers_t* reg );
int SIC( char* flagString, bool* flag, int regX, char* lastBits );
void SixBitAddressing( bool* flag, int* target, registers_t* reg );

int main()
{
	registers_t reg = { 0 };
	mem_t memory[1024] = { 0 };

	FILE* inputFile = fopen( "cod-obj.txt", "r" );

	int instSize, target, i = 0;

	char temp[2] = { '\0' };

	if( !inputFile )
	{
			fputs( "\n=!=!=!=!=!=!= \n\nINPUT FILE ERROR \n\n=!=!=!=!=!=!= \n\n", stderr );
			return EXIT_FAILURE;
	}

	printf( "\n============= \n" );

	while ( 1 ) // memory setting
	{
		fscanf( inputFile, "%s", memory[i].objCode );
		if ( feof( inputFile ) )
		{
			break;
		}

	// checks the string size to determine the instruction format
		instSize = ( ( int ) strlen( memory[i].objCode ) ) / 2;

		memory[i].size = instSize;

		/* if ( memory[i].loca > 1024 || memory[i].loca < 0 ) // pc out of memory range
		{
			fputs( "\n=!=!=!=!=!=!= \n\nMEMORY FULL \n\n=!=!=!=!=!=!= \n", stderr );
			return EXIT_FAILURE;
		} */

		switch ( instSize )
		{
			case 1: // there are no 1 byte instructions on our table
			case 2:
				memcpy( memory[i].opCode, memory[i].objCode, 2 ); // fetches opcode
				memory[i].opCodeInt = HexToDecimal( memory[i].opCode, 2 );
				break;

			case 3:
				char test[3] = { '\0' };
				memcpy( test, HexDigitToBinary4bit( memcpy( temp, &( memory[i].objCode[1] ), 1 ) ), 2 );
				if ( strcmp( test, "00" ) == 0 ) // checks ni flags
				{
					// standard SIC
					printf( "STANDARD \n" ); // nothing
				} else {
					memory[i].flag[F_E] = false;
					SixBitOp( memory[i].objCode, memory[i].opCode, memory[i].flag, memory[i].lastBits, &( memory[i].opCodeInt ) );
				}
				break;

			case 4:
				memory[i].flag[F_E] = true;
				SixBitOp( memory[i].objCode, memory[i].opCode, memory[i].flag, memory[i].lastBits, &( memory[i].opCodeInt ) );
				break;

			default:
				fputs( "SIZE ERROR \n" , stderr );
				break;

		}

		i += instSize;
	}
	printf( "Memory used: %d Bytes \n", i );
	fclose( inputFile );

	reg.pc = 0;
	
	printf( "\n============= \n" );

	while ( 1 ) // actual executing
	{
		int current = reg.pc;
		instSize = memory[current].size;
		reg.pc += instSize;

		printf( "%X | %s \n%s \n", ( reg.pc - instSize ), memory[current].objCode, memory[current].opCode );
		if ( instSize > 2 )
		{
			printf( "%s \n", memory[current].lastBits );
		}
		
		switch ( instSize )
		{
			case 1:
				target = reg.pc;
				break;
				
			case 2:
				memcpy( temp, &( memory[current].objCode[2] ), 1 ); // fetches r1
				reg.s = atoi( temp );

				memcpy( temp, &( memory[current].objCode[3] ), 1 ); // fetches r2
				reg.t = atoi( temp );

				target = reg.pc;
				break;

			case 3:
				target = HexToDecimal( memory[current].lastBits, 3 );
				SixBitAddressing( memory[current].flag, &target, &reg );
				break;

			case 4:
				target = HexToDecimal( memory[current].lastBits, 5 );
				SixBitAddressing( memory[current].flag, &target, &reg );
				break;

			default:
				break;

		}
		
		InstructionTable( memory[current].opCode );
		AlolanExeggcutor( memory[current].opCodeInt, &reg, &target );
		printf( "============= \n" );
	}
	

	return 0;
}

/*
===================
SixBitOp
-------------------
Treats the object code for 3/4 formats
extracting its instruction's opCode and flags
===================
*/
void SixBitOp( char* obj, char* opCode, bool* flag, char* lastBits, int* opCodeInt )
{
	char op[5] = { '\0' }, flagString[7] = { '\0' };
	char midBin[5] = { '\0' }, temp[3] = { '\0' };

	memset( opCode, '\0', 3 );
	memset( lastBits, '\0', 6 );

	for ( int i = 0; i < 3; i++ )
	{
		switch ( i )
		{
			case 0: // 1st-4th bits
				memcpy( opCode, &obj[i], 1 );
				break;

			case 1: // 5th-8th bits
				strcpy( midBin, HexDigitToBinary4bit( memcpy( temp, &obj[i], 1 ) ) );
				memcpy( temp, midBin, 2 ); // first half to opcode
				strcpy( op, temp );
				strcat( op, "00" );
				strcat( opCode, Binary4bitToHexDigit( op ) ); // full opcode

				*opCodeInt = HexToDecimal( opCode, 2 );

				memcpy( flagString, &midBin[2], 2 ); // second half to flags
				break;

			case 2: // 9th-12th bits
				strcat( flagString, HexDigitToBinary4bit( &obj[i] ) ); // full flags
				break;

			default:
				break;

		}
	}
	
	memset( temp, '\0', 3 );

	for ( int i = 0; i < 5; i++ ) // sets flags
	{
		memcpy( temp, &flagString[i], 1 );
		if ( strcmp( temp, "1" ) == 0 )
		{
			flag[i] = true;
		} else {
			flag[i] = false;
		}
	}

	strcpy( lastBits, &obj[3] ); // stores address/displacement
	
	return;
}

void SixBitAddressing( bool* flag, int* target, registers_t* reg )
{
	if( flag[F_N] && flag[F_I] ) //11
	{	
		Addressing( target, flag, reg ); // address
		return;
	}

	if( flag[F_N] && !flag[F_I] ) //10
	{
		Addressing( target, flag, reg ); // pointer to address
		return;
	}

	if( !flag[F_N] && flag[F_I] ) //01
	{
		Addressing( target, flag, reg ); // value
		return;
	}

	return;
}

void Addressing( int* target, bool* flag , registers_t* reg )
{
	if ( flag[F_X] )
	{
		*target += reg->x;
	}

	if ( flag[F_B] )
	{
		*target += reg->b;
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

		*target += reg->pc;
	}

	return;
}

int SIC( char* flagString, bool* flag, int regX, char* lastBits )
{
	char temp2[4] = "0";
	int target;
	strcat( temp2, &flagString[3] );
	if ( flag[F_E] == true )
	{
		target = ( BinaryToDecimal( temp2, 3 ) * 5 ) + HexToDecimal( lastBits, 5 );
	} else {
		target = ( BinaryToDecimal( temp2, 3 ) * 3 ) + HexToDecimal( lastBits, 3 );
	}
	
	if( flag[F_X] )
	{
		target += regX;
	}

	return target;
}

