/*
	3KB memory ( 2^10 sets of 24 bits )
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "converters.h"

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

char* SixBitOp( char* obj, int* opCodeInt, char* flagString, char* lastBits );
char* InstructionTable( const char* s );
void AlolanExeggcutor( int iopCode, registers_t* reg, int* addressM );
int Addressing( int target, bool* flag , registers_t* reg );
int SIC( char* flagString, bool* flag, int regX, char* lastBits );

int main()
{
	registers_t reg = { 0 };

	FILE* inputFile = fopen( "cod-obj.txt", "r" );
	char objCode[9] = { '\0' };
	char opCode[3] = { '\0' }, flagString[7] = { '\0' }, lastBits[6] = { '\0' };
	bool flag[6];

	int instSize, opCodeInt, target;

	char temp[2] = { '\0' };
	int tempI;

	if( !inputFile )
	{
			fputs( "\n=!=!=!=!=!=!= \n\nINPUT FILE ERROR \n\n=!=!=!=!=!=!= \n\n", stderr );
			return EXIT_FAILURE;
	}

	while ( 1 ) // main loop
	{
		fscanf( inputFile, "%s", objCode );
		if ( feof( inputFile ) )
		{
			break;
		}

	// checks the string size to determine the instruction format
		instSize = ( ( int ) strlen( objCode ) );
		printf( "%X | %s ", reg.pc, objCode ); // current address

		reg.pc += instSize / 2; // pc updates after fetching and before executing

		if ( reg.pc > 1024 || reg.pc < 0 ) // pc out of memory range
		{
			fputs( "\n=!=!=!=!=!=!= \n\nOUT OF BOUNDS \n\n=!=!=!=!=!=!= \n", stderr );
			return EXIT_FAILURE;
		}

		switch ( instSize / 2 )
		{
			case 1: // there are no 1 byte instructions on our table
				printf( "\n" );
				break;

			case 2:
	/*
		| 8 bit	| 4 bit	| 4 bit	|
		| op	| r1	| r2	|
	*/
				char temp2[3];
				memset( temp, '\0', 2 );
				memset( temp2, '\0', 3 );
				memcpy( temp2, objCode, 2 );
				printf( "\nopCode %s \n", temp2 ); // temp2 = opCode

				memcpy( temp, &objCode[2], 1 );
				printf( "r1 %s ", temp ); // temp = r1
				reg.s = atoi( temp );

				memcpy( temp, &objCode[3], 1 );
				printf( "r2 %s \n", temp ); // temp = r2
				reg.t = atoi( temp );

				InstructionTable( temp2 );
				break;
			case 3:
	/*
		| 6 bit	| 6 bit	| 12 bit		|
		| op	| flags	| displacement	|

		for standard SIC

		| 6 bit	| 3 bit	| 15 bit	|
		| op	| flags	| address	|
	*/
				char test[3] = {'\0'};
				memcpy( test, HexDigitToBinary4bit( &objCode[2] ), 2 );
				if ( strcmp( test, "00" ) == 0 )
				{
					// standard SIC
				} else {
					flag[F_E] = false;
					strcpy( opCode, SixBitOp( objCode, &opCodeInt, flagString, lastBits ) );
					InstructionTable( opCode );
					tempI = HexToDecimal( lastBits, 3 );

					for( int i = 0; i < 5; i++ )
					{
						memcpy( temp, &flagString[i], 1 );
						if ( strcmp( temp, "1" ) == 0 )
						{
							flag[i] = true;
						} else {
							flag[i] = false;
						}
					}
				}
				break;
			case 4:
	/*
		| 6 bit	| 6 bit	| 20 bit	|
		| op	| flags	| address	|
	*/
				flag[F_E] = true;
				strcpy( opCode, SixBitOp( objCode, &opCodeInt, flagString, lastBits ) );
				InstructionTable( opCode );
				tempI = HexToDecimal( lastBits, 5 );

				for( int i = 0; i < 5; i++ )
				{
					memcpy( temp, &flagString[i], 1 );
					if ( strcmp( temp, "1" ) == 0 ) {
						flag[i] = true;
					} else {
						flag[i] = false;
					}
				}
				break;
			default:
				fputs( "SIZE ERROR \n" , stderr );
				break;
		}

		if ( ( ( instSize / 2 ) == 3  || ( instSize / 2 ) == 4 ) )
		{
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

			if ( !flag[F_N] && !flag[F_I] ) //00
			{
				target = SIC( flagString, flag, reg.x, lastBits );
			} else {
				if( flag[F_N] && flag[F_I] ) //11
				{	
					target = Addressing( tempI, flag, &reg ); // address
				}

				if( flag[F_N] && !flag[F_I] ) //10
				{
					target = Addressing( tempI, flag, &reg ); // pointer
				}

				if( !flag[F_N] && flag[F_I] ) //01
				{
					target = Addressing( tempI, flag, &reg ); // value
				}

				
			}
		}

		AlolanExeggcutor( opCodeInt, &reg, &target );
		printf( "\nnext address %X", reg.pc );
		
		printf( "\n============= \n" );

	}
	printf( "Memory used: %d Bytes \n", reg.pc );
	fclose( inputFile );

	return 0;
}

/*
===================
SixBitOp
-------------------
Treats the object code for 3 and 4 byte formats
extracting its instruction opCode and flags
===================
*/
char* SixBitOp( char* obj, int* opCodeInt, char* flagString, char* lastBits )
{
	static char opCode[3];
	char holder[5];
	int decimal = 0;
	char temp[3], sup[3], op[5];
	memset( opCode, '\0', 3 );
	memset( lastBits, '\0', 6 );
	memset( flagString, '\0', 7 );
	memset( temp, '\0', 3 );
	memset( sup, '\0', 3 );
	for ( int i = 0; i < 3; i++ )
	{
		switch ( i )
		{
			case 0: // first 4
				memcpy( opCode, &obj[i], 1 );
				break;
			case 1: // second 4
				strcpy( holder, HexDigitToBinary4bit( &obj[i] ) );
				memcpy( temp, holder, 2 );
				strcpy( op, temp );
// adds two bits 0 at the end for a total of 4 bits
// and translates it back to hex
				strcat( op, "00" );
				strcat( opCode, Binary4bitToHexDigit( op ) );

// capitalizes any characters in the hex string
				for ( int j = 0; j < 2; j++ )
				{ 
					sup[j] = toupper( opCode[j] ); 
				}
				strcpy( opCode, sup );
				printf( "\nopCode %s \n", opCode );

				memcpy( flagString, &holder[2], 2 );
				break;
			case 2: // byte 3
				strcat( flagString, HexDigitToBinary4bit( &obj[i] ) );
				//printf( "flags %s \n", flagString );
				break;
			default:
				break;
		}
	}

	strcpy( lastBits, &obj[3] );

	if ( strlen( obj ) == 8 )
	{
		printf( "address %s \n", lastBits );
	} else {
		printf( "displacement %s \n", lastBits );
	}

// changes opCodeInt through pointers so function
// can still return opCode's string for printing
	*opCodeInt = HexToDecimal( opCode, 2 );;
	return opCode;
}

/*
===================
InstructionTable
-------------------
Checks the opCode in hexadecimal format 
and returns the name of the instruction
===================
*/
char* InstructionTable( const char* s )
{
	char line[10], temp[4] = " ";
	static char instructions[10];
	bool test = false;
	FILE* table = fopen( "InstructionTable.txt", "r" );
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
		fputs( "Instruction not found \n", stderr );
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
void AlolanExeggcutor( int iopCode, registers_t* reg, int* addressM )
{
		switch ( iopCode ) // r1 S r2 T
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
			char temp[25];
			// m as hex into temp
			// regA to string of hex
			// memcpy( &regA[5], *addressM, 1 );
			break;
		case 8: // LDL
			reg->l = addressM;
			break;
		case 108: // LDS
			reg->s = addressM;
			break;
		case 116: // LDT
			reg->t = addressM;
			break;
		case 4: // LDX
			reg->x = addressM;
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
			addressM = reg->a;
			break;
		case 120: // STB
			addressM = reg->b;
			break;
		case 84: // STCH
			// both regA and m as strings
			char AString[6], MString;
			memcpy( addressM, &AString[5], 1 );
			break;
		case 20: // STL
			addressM = reg->l;
			break;
		case 124: // STS
			addressM = reg->s;
			break;
		case 132: // STT
			addressM = reg->t;
			break;
		case 16: // STX
			addressM = reg->x;
			break;
		case 28: // SUB
			reg->a = reg->a - *addressM;
			break;
		case 148: // SUBR
			reg->t = ( reg->t ) - ( reg->s );
			break;
		case 44: // TIX
			reg->x = ( reg->x ) + 1;
			if ( reg->x == addressM )
			{
				reg->sw = EQUAL;
			} else if ( reg->x >= addressM ) {
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

int Addressing( int target, bool* flag , registers_t* reg )
{
	if ( flag[F_X] )
	{
		target += reg->x;
	}

	if ( flag[F_B] )
	{
		target += reg->b;
	}

	if ( flag[F_P] )
	{
		target += reg->pc;
	}

	return target;
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