#include <stdbool.h>
#include "execute.h"

	// 3KB memory ( 2^10 sets of 24 bits )
#define MEMORY_MAX_ADDRESS 1024

int current = 0;

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

void SixBitOp( char* obj, char* opCode, bool* flag, char* lastBits, int* opCodeInt );
int Addressing( int* target, bool* flag , registers_t* reg );
int SIC( char* flagString, bool* flag, int regX, char* lastBits );
void SixBitAddressing( bool* flag, int* target, registers_t* reg, int opCodeInt, mem_t* memory );

int main()
{
	registers_t reg = { 0 };
	mem_t memory[MEMORY_MAX_ADDRESS] = { 0 };

	FILE* inputFile = fopen( "cod-obj.txt", "r" );

	int instSize, target;

	char temp[2] = { '\0' };

	if( !inputFile )
	{
			fputs( "\n=!=!=!=!=!=!= \n\nINPUT FILE ERROR \n\n=!=!=!=!=!=!= \n\n", stderr );
			return EXIT_FAILURE;
	}

	printf( "\n============= \n" );

	while ( 1 ) // memory setting
	{
		static int i = 0;
		fscanf( inputFile, "%s", memory[i].objCode );
		if ( feof( inputFile ) )
		{
			break;
		}

	// checks the string size to determine the instruction format
		instSize = ( ( int ) strlen( memory[i].objCode ) ) / 2;

		memory[i].size = instSize;

		if ( i > MEMORY_MAX_ADDRESS ) // out of memory
		{
			fputs( "\n=!=!=!=!=!=!= \n\nMEMORY FULL \n\n=!=!=!=!=!=!= \n", stderr );
			return EXIT_FAILURE;
		}

		switch ( instSize )
		{
			case 1: // there are no 1 byte instructions on our table
				printf( "%d - L A B E L \n", i );
				memcpy( memory[i].opCode, memory[i].objCode, 2 ); // fetches opcode
				memory[i].opCodeInt = HexToDecimal( memory[i].opCode, 2 );
				break;
			case 2:
				memcpy( memory[i].opCode, memory[i].objCode, 2 ); // fetches opcode
				memory[i].opCodeInt = HexToDecimal( memory[i].opCode, 2 );
				break;

			case 3:
				char test[3] = { '\0' };
				memcpy( test, &( HexDigitToBinary4bit( memcpy( temp, &( memory[i].objCode[1] ), 1 ) )[2] ), 2 );
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
		target = i;
	}
	fclose( inputFile );
	printf( "Memory used: %d Bytes \n", target );

	reg.pc = 0;
	
	

	while ( 1 ) // actual executing
	{
		current = reg.pc;
		instSize = memory[current].size;
		reg.pc += instSize;
		
		printf( "\n============= \n" );
		printf( "%X | %s \n%s ", ( reg.pc - instSize ), memory[current].objCode, memory[current].opCode );
		if ( strcmp( memory[current].objCode, "BABABABE" ) == 0 ) {
			printf( " END READING \n" );
			printf( "A: %d X: %d L: %d \nB: %d S: %d T: %d \nPC: %d SW: %d \n", reg.a, reg.x, reg.l, reg.b, reg.s, reg.t, reg.pc, reg.sw );
			return EXIT_SUCCESS;
		}
		if ( memory[current].size < 1 || memory[current].size > 4 ) {
			fputs( "\n=!=!=!=!=!=!= \n\nSEGMENTATION FAULT \n\n=!=!=!=!=!=!= \n", stderr );
			return EXIT_FAILURE;
		}
		
		if ( instSize > 2 )
		{
			printf( "%s \n", memory[current].lastBits );
		}
		
		InstructionTable( memory[current].opCode );

		if ( instSize > 2 )
		{
			for ( int i = 0; i < 6; i++ )
			{
				printf( "%d", ( int ) memory[current].flag[i] );
			}
		}
		
		switch ( instSize )
		{
			case 1:
				target = reg.pc;
				break;

			case 2:
				memcpy( temp, &( memory[current].objCode[2] ), 1 ); // fetches r1
				reg.s = atoi( temp );
				printf( "r1 %d ", reg.s );

				memcpy( temp, &( memory[current].objCode[3] ), 1 ); // fetches r2
				reg.t = atoi( temp );
				printf( "r2 %d", reg.t );

				target = reg.pc;
				break;

			case 3:
				target = HexToDecimal( memory[current].lastBits, 3 );
				SixBitAddressing( memory[current].flag, &target, &reg, memory[current].opCodeInt, memory );
				break;

			case 4:
				target = HexToDecimal( memory[current].lastBits, 5 );
				SixBitAddressing( memory[current].flag, &target, &reg, memory[current].opCodeInt, memory );
				break;

			default:
				break;

		}
	}

	printf( "============= \n" );

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

void SixBitAddressing( bool* flag, int* target, registers_t* reg, int opCodeInt, mem_t* memory )
{
	if ( flag[F_N] && flag[F_I] ) //11
	{	
		Addressing( target, flag, reg ); // address
		AlolanExeggcutor( opCodeInt, reg, target, memory );
		return;
	}

	if ( flag[F_N] && !flag[F_I] ) //10
	{
		Addressing( target, flag, reg ); // pointer to address
		AlolanExeggcutor( opCodeInt, reg, &memory[*target].opCodeInt, memory );
		return;
	}

	if ( !flag[F_N] && flag[F_I] ) //01
	{
		int value;
		value = Addressing( target, flag, reg ); // value
		AlolanExeggcutor( opCodeInt, reg, &value, memory );
		return;
	}

	return;
}

int Addressing( int* target, bool* flag , registers_t* reg )
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

	return *target;
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

