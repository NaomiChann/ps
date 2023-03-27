#include <stdbool.h>
#include "execute.h"

// 3KB memory ( 2^10 sets of 24 bits )
#define MEMORY_MAX_ADDRESS 1024

int reg_g[10] = { '\0' };

int current = 0; // oh

void SixBitOp( char* obj, char* opCode, bool* flag, char* lastBits, int* opCodeInt );
int Addressing( int* target, bool* flag );
void SixBitAddressing( bool* flag, int* target, int opCodeInt, int* helper, mem_t* memory );

int main()
{
	mem_t memory[MEMORY_MAX_ADDRESS] = { 0 };

	FILE* inputFile = fopen( "cod-obj.txt", "r" );

	int instSize, target, helper[2] = { 0 };

	char temp[2] = { '\0' };

	if( !inputFile )
	{
			fputs( "\n=!=!=!=!=!=!= \n\nINPUT FILE ERROR \n\n=!=!=!=!=!=!= \n\n", stderr );
			return EXIT_FAILURE;
	}

	printf( "\n============= \n" );

	while ( 1 ) // memory mapping loop
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

		char test[3] = { '\0' };

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
				memset( temp, '\0', 2 );
				memcpy( test, &( HexDigitToBinary4bit( memcpy( temp, &( memory[i].objCode[1] ), 1 ) )[2] ), 2 );
				
				if ( strcmp( test, "00" ) == 0 ) // standard sic
				{
					// | opcode  | ni | x | address				|
					// | xxxx xx | 00 | x | xxx xxxx xxxx xxxx 	|
					char bin[25] = { '\0' }, holder[7] = { '\0' }, address[16] = { '\0' };

					memory[i].flag[F_N] = false;
					memory[i].flag[F_I] = false;
					
					strcpy( bin, DecimalToBinary( HexToDecimal( memory[i].objCode, 6 ) , 24 ) );
					memcpy( holder, bin, 6 );
					memory[i].opCodeInt = BinaryToDecimal( holder, 6 );
					strcpy( memory[i].opCode, DecimalToHex( memory[i].opCodeInt ) );

					if ( bin[8] == '0' )
					{
						memory[i].flag[F_X] = false;
					} else {
						memory[i].flag[F_X] = true;
					}

					strcpy( address, &bin[9] );
					strcpy( memory[i].lastBits, Filler( DecimalToHex( BinaryToDecimal( address, 15 ) ), 4 ) );
				} else { // regular format 3
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

	reg_g[R_PC] = 0;
	

////////////////////////////

//	DONE MEMORY MAPPING   //

////////////////////////////


	while ( 1 ) // main executiion loop
	{
		current = reg_g[R_PC];
		instSize = memory[current].size;
		reg_g[R_PC] += instSize;
		
		printf( "\n============= \n" );
		printf( "%X | %s \n%s ", ( reg_g[R_PC] - instSize ), memory[current].objCode, memory[current].opCode );
		if ( strcmp( memory[current].objCode, "BABABABE" ) == 0 ) {
			printf( " END READING \n" );
			printf( "A: %d X: %d L: %d \nB: %d S: %d T: %d \nPC: %d SW: %d \n", reg_g[R_A], reg_g[R_X], reg_g[R_L], reg_g[R_B], reg_g[R_S], reg_g[R_T], reg_g[R_PC], reg_g[R_SW] );
			return EXIT_SUCCESS;
		}
		if ( memory[current].size < 1 || memory[current].size > 4 ) {
			fputs( "\n=!=!=!=!=!=!= \n\nSEGMENTATION FAULT \n\n=!=!=!=!=!=!= \n", stderr );
			return EXIT_FAILURE;
		}
		
	// pretty formatting stuff, useless since there will be a gui
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
				target = reg_g[R_PC];
				break;

			case 2:
				memcpy( temp, &( memory[current].objCode[2] ), 1 ); // fetches r1
				helper[0] = atoi( temp );
				printf( "r1 %d ", helper[0] );

				memcpy( temp, &( memory[current].objCode[3] ), 1 ); // fetches r2
				helper[1] = atoi( temp );
				printf( "r2 %d", helper[1] );

				target = reg_g[R_PC];
				AlolanExeggcutor( memory[current].opCodeInt, NULL, helper, memory );
				break;

			case 3:
				target = HexToDecimal( memory[current].lastBits, 3 );

				if ( !memory[current].flag[F_N] && !memory[current].flag[F_I] ) // standard sic
				{
					target = HexToDecimal( memory[current].lastBits, 4 );
				}
				SixBitAddressing( memory[current].flag, &target, memory[current].opCodeInt, helper, memory );
				break;

			case 4:
				target = HexToDecimal( memory[current].lastBits, 5 );
				SixBitAddressing( memory[current].flag, &target, memory[current].opCodeInt, helper, memory );
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
as well as displacement/address
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

/*
===================
SixBitAddressing
-------------------
Divides 3/4 format execution
into its proper addressing types
===================
*/
void SixBitAddressing( bool* flag, int* target, int opCodeInt, int* helper, mem_t* memory )
{
	if ( flag[F_N] && flag[F_I] ) // ni 11
	{	
		Addressing( target, flag ); // address
		AlolanExeggcutor( opCodeInt, target, helper, memory );
		return;
	}

	if ( flag[F_N] && !flag[F_I] ) // ni 10
	{
		Addressing( target, flag ); // pointer to address
		AlolanExeggcutor( opCodeInt, &memory[*target].opCodeInt, helper, memory );
		return;
	}

	if ( !flag[F_N] && flag[F_I] ) // ni 01
	{
		int value;
		value = Addressing( target, flag ); // value
		AlolanExeggcutor( opCodeInt, &value, helper, memory );
		return;
	}

	if ( !flag[F_N] && !flag[F_I] ) // ni 00
	{
		Addressing( target, flag ); // address
		AlolanExeggcutor( opCodeInt, target, helper, memory );
		return;
	}

	return;
}

/*
===================
Addressing
-------------------
Adds the remaining flags displacement to
3/4 format instructions
includes program counter relative's
2's complement
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
