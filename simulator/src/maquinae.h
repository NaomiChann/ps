#include <stdbool.h>
#include "execution.h"

// 3KB memory ( 2^10 sets of 24 bits )
#define MEMORY_MAX_ADDRESS 1024

// globally accessible registers
int reg_g[10] = { '\0' };
int current = 0; // oh

// prototypes
void AmnesiaTreatment();
void SixBitOp( char* obj, char* opCode, bool* flag, char* lastBits, int* opCodeInt );
void SixBitAddressing( bool* flag, int* target, int opCodeInt, int* helper, mem_t* memory );
void RegUIgas();

/*
===================
AmnesiaTreatment
-------------------
Does memory mapping for the vm then
manages it during the execution
===================
*/
void AmnesiaTreatment()
{ // todo: needs something to recognize allocation
	mem_t memory[MEMORY_MAX_ADDRESS] = { 0 };
	FILE* fileInput = fopen( "program/assembled.txt", "r" );

	int instSize, startAdd, target, helper[2] = { 0 };
	char temp[2] = { '\0' }, holding[9] = { '\0' }, test[3] = { '\0' };
	char line[256] = { '\0' };

	fgets( line, sizeof( line ), fileInput );
	strcpy( holding, strtok( line, "\n" ) );
	startAdd = HexToDecimal( holding, 8 );

	printf( "\nMEMORY MAPPING STARTED\n" );

	while ( fgets( line, sizeof( line ), fileInput ) )
	{
		static int i = 0;

		if ( line[0] == 'R' )
		{
			fputs( "\n=!=!=!=!=!=!= \n\nEXTERNAL REFFERENCE \n\n=!=!=!=!=!=!= \n\n" , stderr );
			return;
		}

		if ( i == 0 )
		{
			i = startAdd;
		}
		
		strcpy( holding, strtok( line, "\n" ) );

		// checks the string size to determine the instruction format
		instSize = strlen( holding ) / 2;
		memory[i].size = instSize;
		strcpy( memory[i].objCode, holding );

		switch ( instSize )
		{
			case 1: // there are no 1 byte instructions on our table yet
				strcpy( memory[i].opCode, memory[i].objCode ); // fetches opcode
				memory[i].opCodeInt = HexToDecimal( memory[i].opCode, 2 );
				break;

			case 2:
				memcpy( memory[i].opCode, memory[i].objCode, 2 ); // fetches opcode
				memory[i].opCodeInt = HexToDecimal( memory[i].opCode, 2 );
				break;

			case 3:
				memset( temp, '\0', 2 ); // clears temp
				// takes first byte from obj code, since these are all from opcode
				memcpy( temp, &( memory[i].objCode[1] ), 1 );
				memcpy( test, &( HexDigitToBinary4bit( temp )[2] ), 2 );
				
				if ( strcmp( test, "00" ) != 0 ) // regular format 3
				{
					memory[i].flag[F_E] = false; // flag e is guaranteed to be false
					SixBitOp( memory[i].objCode, memory[i].opCode, memory[i].flag, memory[i].lastBits, &( memory[i].opCodeInt ) );
				} else { // standard sic ( actually should never be assembled, but the executor is able to run it regardless )
					// | opcode  | ni | x | address				|
					// | xxxx xx | 00 | x | xxx xxxx xxxx xxxx 	|
					char bin[25] = { '\0' }, holder[9] = { '\0' }, address[16] = { '\0' };

					// guaranteed flags
					memory[i].flag[F_N] = false;
					memory[i].flag[F_I] = false;
					
					// fully treats the obj code accoring to the standard sic format
					strcpy( bin, DecimalToBinary( HexToDecimal( memory[i].objCode, 6 ) , 24 ) );
					memcpy( holder, bin, 6 );
					strcat( holder, "00" );
					memory[i].opCodeInt = BinaryToDecimal( holder, 8 );
					strcpy( memory[i].opCode, Filler( DecimalToHex( memory[i].opCodeInt ), 2 ) );

					// fetches flag x
					if ( bin[8] == '0' )
					{
						memory[i].flag[F_X] = false;
					} else {
						memory[i].flag[F_X] = true;
					}

					// fetches address
					strcpy( address, &bin[9] );
					strcpy( memory[i].lastBits, Filler( DecimalToHex( BinaryToDecimal( address, 15 ) ), 4 ) );
				}
				break;

			case 4:
				memory[i].flag[F_E] = true; // flag e guaranteed to be true
				SixBitOp( memory[i].objCode, memory[i].opCode, memory[i].flag, memory[i].lastBits, &( memory[i].opCodeInt ) );
				break;

			default:
				fputs( "SIZE ERROR \n" , stderr );
				break;

		}

		for ( int j = 0; j < memory[i].size; ++j )
		{
			char out[3] = { '\0' };
			memcpy( out, &memory[i].objCode[j * 2], 2 );
			//addDataGridItem( out );
		}
		
		i += instSize;
	}


	fclose( fileInput );

	// sets pc to start addrest
	reg_g[R_PC] = startAdd;
	
	printf( "CONCLUDED\n" );

////////////////////////////

//	DONE MEMORY MAPPING   //

////////////////////////////
	
	printf( "\n\nSTARTING EXECUTION. . .\n" );

	while ( 1 ) // main executiion loop
	{
		//Sleep( posUI_g * 100 );
		current = reg_g[R_PC]; // sets current line as pc
		instSize = memory[current].size;
		reg_g[R_PC] += instSize; // increments pc to next location
		if ( instSize == 0 )
		{
			fputs( "\n=!=!=!=!=!=!= \n\nSEGMENTATION FAULT \n\n=!=!=!=!=!=!= \n\n" , stderr );
			return;
		}
		
		printf( "\n============= \n" );
		printf( "%X | %s \n%s ", ( reg_g[R_PC] - instSize ), memory[current].objCode, memory[current].opCode );
		//RegUIgas();

		// checks for end reading keyword BABABABE
		if ( strcmp( memory[current].objCode, "BABABABE" ) == 0 ) {
			printf( "END READING" );
			printf( "\n============= \n\n" );
			printf( "A: %d X: %d L: %d \nB: %d S: %d T: %d \nPC: %d SW: %d \n\n", reg_g[R_A], reg_g[R_X], reg_g[R_L], reg_g[R_B], reg_g[R_S], reg_g[R_T], reg_g[R_PC], reg_g[R_SW] );
			return;
		}

		// used to work for checking any space out of the format sizes, will not work with allocated spaces
		// probably some special checking for allocated spaces and rejecting anything other than "populated locations"
		/*
		if ( memory[current].size < 1 || memory[current].size > 4 ) {
			fputs( "\n=!=!=!=!=!=!= \n\nSEGMENTATION FAULT \n\n=!=!=!=!=!=!= \n", stderr );
			return EXIT_FAILURE;
		}
		*/
		
		if ( instSize > 2 )
		{
			printf( "%s ", memory[current].lastBits );
		}
		printf( "\n" );
		
		InstructionTable( memory[current].opCode );

		if ( instSize > 2 )
		{
			for ( int i = 0; i < 6; i++ )
			{
				printf( "%d", ( int ) memory[current].flag[i] );
			}
		}
		
		
		switch ( instSize ) // format detection
		{
			case 1:
				// works for now, will break with allocation
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

	return;
}
/*
void RegUIgas()
{
	char guito[10] = { '\0' };
	itoaB( reg_g[R_A], guito, 10 );
	setACC( guito );
	itoaB( reg_g[R_X], guito, 10 );
	setRX( guito );
	itoaB( reg_g[R_L], guito, 10 );
	setRL( guito );
	itoaB( reg_g[R_B], guito, 10 );
	setRB( guito );
	itoaB( reg_g[R_S], guito, 10 );
	setRS( guito );
	itoaB( reg_g[R_T], guito, 10 );
	setRT( guito );
	itoaB( reg_g[R_PC], guito, 10 );
	setPC( guito );
	itoaB( reg_g[R_SW], guito, 10 );
	setSW( guito );
}
*/

/*
===================
SixBitOp
-------------------
Treats the object code for 3/4 formats,
extracting its instruction's opCode and flags
as well as displacement/address through string
manipulation and hexadecimal to binary conversion
===================
*/
void SixBitOp( char* obj, char* opCode, bool* flag, char* lastBits, int* opCodeInt )
{
	char op[5] = { '\0' }, flagString[7] = { '\0' };
	char midBin[5] = { '\0' }, temp[3] = { '\0' };

	// clears any leftovers from the strings
	memset( opCode, '\0', 3 );
	memset( lastBits, '\0', 6 );

	for ( int i = 0; i < 3; i++ ) // one loop for each hex digit
	{
		switch ( i )
		{
			case 0: // 1-4th bits
				// first digit is entirely opcode
				memcpy( opCode, &obj[i], 1 );
				break;

			case 1: // 5-8th bits
				// first 2 bits from the second digit are opcode, then flag n and i
				memcpy( temp, &obj[i], 1 );
				strcpy( midBin, HexDigitToBinary4bit( temp ) );
				memcpy( temp, midBin, 2 ); // first half to opcode
				strcpy( op, temp );
				strcat( op, "00" ); // fills the right with zeros to complete a byte
				strcat( opCode, Binary4bitToHexDigit( op ) ); // full opcode

				*opCodeInt = HexToDecimal( opCode, 2 ); // translation to decimal for easier use

				memcpy( flagString, &midBin[2], 2 ); // second half to flags
				break;

			case 2: // 9-12th bits
				// last digit is entirely flags
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
Divides 3/4 format execution into its proper addressing types
so that the proper target usage is sent to the execution
( in most cases, jumps and stores recieve special treatment )
===================
*/
void SixBitAddressing( bool* flag, int* target, int opCodeInt, int* helper, mem_t* memory )
{
	if ( flag[F_N] && flag[F_I] ) // ni 11
	{	
		Addressing( target, flag ); // address
		AlolanExeggcutor( opCodeInt, &memory[*target].opCodeInt, helper, memory );
		return;
	}

	if ( flag[F_N] && !flag[F_I] ) // ni 10
	{
		Addressing( target, flag ); // pointer to address
		AlolanExeggcutor( opCodeInt, &memory[memory[*target].opCodeInt].opCodeInt, helper, memory );
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
