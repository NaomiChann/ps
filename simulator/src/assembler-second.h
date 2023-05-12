#include "maquinae.h"

// GLOBAL
extern const char* directives[];
bool lebo_g = false, here_g = false;

typedef struct l
{
	char label[8];
	char* operation;
	char* operand;
	int loc;
	char* flags;
} line_t;

void FetcherTheEvilTwink( char* line, line_t* programLine );
void CheckInstruction( line_t* programLine, char* op );
bool FlagIntegrity( char* flags );
void ValueYourself( line_t* programLine, char format );
void TirapTchurum( int* target );
bool Jumper( char* operation );

/*
===================
MountyTheEvilTwin
-------------------
Performs the second pass and outputs the
assembled file for execution
===================
*/
void MountyTheEvilTwin()
{
	FILE* fileOutput = fopen( "program/assembled.txt", "w" );
	FILE* fileInput = fopen( "program/flagged.txt", "r" );
	char line[256] = { '\0' }, opCodeF[13] = { '\0' };

	printf( "SECOND PASS. . .\n" );

	while ( fgets( line, sizeof( line ), fileInput ) )
	{
		line_t programLine = { 0 };
		memset( opCodeF, '\0', 13 );
		FetcherTheEvilTwink( line, &programLine );
		lebo_g = false;
		here_g = false;

		if ( strcmp( programLine.operation, "END" ) == 0 )
		{
			fprintf( fileOutput, "BABABABE\n" );
			break;
		}

		if ( strcmp( programLine.operation, "START" ) == 0 )
		{
			fprintf( fileOutput, "%s\n", Filler( DecimalToHex( atoi( programLine.operand ) ), 8 ) );
			continue;
		}

		if ( programLine.operand[0] == '[' )
		{
			programLine.operand = &programLine.operand[1];
			lebo_g = true;
		}

		CheckInstruction( &programLine, opCodeF );

		if ( strcmp( programLine.operation, "RESB" ) == 0 )
		{
			for ( int i = 0; i < atoi( programLine.operand ); ++i )
			{
				fprintf( fileOutput, "00\n" );
			}
		} else if ( strcmp( programLine.operation, "RESW" ) == 0 ) {
			for ( int i = 0; i < atoi( programLine.operand ); ++i )
			{
				fprintf( fileOutput, "000000\n" );
			}
		} else {
			if ( lebo_g && !here_g )
			{
				fprintf( fileOutput, "R" );
			}
			fprintf( fileOutput, "%s", programLine.operation );
			fprintf( fileOutput, "%s", programLine.operand );
			fprintf( fileOutput, "\n" );
		}
	}

	fclose( fileInput );
	fclose( fileOutput );
	
	printf( "CONCLUDED\n\n" );
	printf( "PROGRAM ASSEMBLED\n\n" );

	AmnesiaTreatment();
}

/*
===================
FetcherTheEvilTwink
-------------------
Segments the currently read line and
stores the values
===================
*/
void FetcherTheEvilTwink( char* line, line_t* programLine )
{
	char* field[4] = { '\0' };

	programLine->loc = atoi( strtok( line, "\t" ) );

	for ( int i = 0; i < 4; ++i )
	{
		field[i] = strtok( NULL, "\t" );

		if ( field[i] == NULL )
		{
			int count = 0;

			for ( int j = i - 1; j >= 0; --j )
			{
				switch ( count )
				{
					case 0:
						field[j] = strtok( field[j], "\n" );
						programLine->operand = field[j];
						break;

					case 1:
						programLine->operation = field[j];
						break;
					
					case 2:
						if ( FlagIntegrity( field[j] ) )
						{
							++j;
						}
						strcpy( programLine->label, field[j] );
						break;

					case 3:
						programLine->flags = field[j];
						break;
					
					default:
						break;
				}
				++count;
			}
			break;
		} else if ( i == 3 ) {
			programLine->flags = field[0];
			strcpy( programLine->label, field[1] );
			programLine->operation = field[2];
			field[3] = strtok( field[3], "\n" );
			programLine->operand = field[3];
			break;
		}
	}
}

/*
===================
CheckInstruction
-------------------
Fetches opcode and (kind of) treats directives
===================
*/
void CheckInstruction( line_t* programLine, char* op )
{
	FILE* fileInstruction = fopen( "tables/instruction.txt", "r" );

	char* instruction, * opCode, * format, lineInst[256] = { '\0' };
	char holder[7] = { '\0' };

	while ( fgets( lineInst, sizeof( lineInst ), fileInstruction ) )
	{
		instruction = strtok( lineInst, " " );
		opCode = strtok( NULL, " " );
		format = strtok( NULL, "\n" );

		if ( strcmp( programLine->operation, instruction ) == 0 )
		{
			strcpy( op, DecimalToBinary( HexToDecimal( opCode, 2 ), 8 ) );
			break;
		}
	}

	if ( strlen( op ) == 0 )
	{
		for ( int i = 0; i < 5; ++i )
		{
			if ( strcmp( programLine->operation, directives[i] ) == 0 )
			{
				switch ( i )
				{
					case 2: // RESB
						break;

					case 3: // RESW
						break;

					case 4: // EQU
						break;

					default:
						strcpy( programLine->operation, "" );
						break;

				}
			}
		}
		fclose( fileInstruction );
		return;
	}

	ValueYourself( programLine, format[0] );

	if ( format[0] == '3' || format[0] == '4' )
	{
		memcpy( holder, op, 6 );
		strcpy( op, holder );
		strcat( op, programLine->flags );
	}

	strcpy( holder, Filler( DecimalToHex( BinaryToDecimal( op, atoi( format ) * 4 ) ), atoi( format ) ) );
	programLine->operation = holder;

	fclose( fileInstruction );
}

/*
===================
FlagIntegrity
-------------------
Checks if the input string is formatted
properly as a flag
===================
*/
bool FlagIntegrity( char* flags )
{
	if ( strlen( flags ) != 6 )
	{
		return false;
	}

	for ( int i = 0; i < 6; ++i )
	{
		if ( flags[i] != '0' && flags[i] != '1' )
		{
			return false;
		}
	}

	return true;
}

/*
===================
ValueYourself
-------------------
Obtains proper value for the operand,
getting both for format 2 instructions and
realizing 2's complement calcuation when
necessary

			value yourself ;)
===================
*/
void ValueYourself( line_t* programLine, char format )
{
	FILE* fileSymtab = fopen( "tables/symbol.txt", "r" );
	char operand1[9] = { '\0' }, operand2[2] = { '\0' };
	char line[256] = { '\0' }, value[18] = { '\0' }, tempValue[8] = { '\0' };
	bool multi = false, imme = false;

	strcpy( value, programLine->operand );

	if ( format == '2' )
	{
		if ( strstr( programLine->operand, "," ) != NULL )
		{
			multi = true;
			memcpy( operand1, &value[0], 1 );
			memcpy( operand2, &value[3], 1 );
		} else {
			strcpy( operand1, value );
			strcpy( operand2, "0" );
		}

		strcpy( value, operand1 );
	} else {
		if ( value[0] == '*' )
		{
			imme = true;
		}

		strcpy( operand1, value );
	}
// somewhere around here
	memset( programLine->operand, '\0', strlen( programLine->operand ) );

	while ( fgets( line, sizeof( line ), fileSymtab ) )
	{
		if ( strcmp( value, strtok( line, "\t" ) ) == 0 )
		{
			here_g = true;
			if ( format == '2' )
			{
				strcat( programLine->operand, strtok( NULL, "\n" ) );
				if ( multi )
				{
					multi = false;
					rewind( fileSymtab );
					strcpy( value, operand2 );
					
				} else if ( strlen( programLine->operand ) == 1 ) {
					strcat( programLine->operand, "0" );
					break;
				} else {
					break;
				}
			} else {
				strcpy( tempValue, strtok( NULL, "\n" ) );
				break;
			}
		}
	}

	if ( format == '3' ) {
		int temp = 3;

		if ( programLine->flags[F_E] == '1' )
		{
			format = '4';
			temp = 5;
		}

		if ( lebo_g && !here_g )
		{
			char fil[5] = { '\0' };
			strcat( programLine->operand, Filler( fil, temp ) );
			fclose( fileSymtab );
			return;
		}

		int destLoc;
		int currLoc;
		if ( imme )
		{
			currLoc = programLine->loc;
			if ( strlen( operand1 ) > 1  )
			{
				destLoc = currLoc + atoi( &operand1[1] );
			} else {
				destLoc = currLoc;
			}
		} else {
			currLoc = programLine->loc + atoi( &format );
			destLoc = atoi( tempValue );
		}
		
		if ( programLine->flags[F_N] == '0' && programLine->flags[F_I] == '1' )
		{
			strcat( programLine->operand, Filler( DecimalToHex( atoi( operand1 ) ), temp ) );
			fclose( fileSymtab );
			return;
		}

		if ( Jumper( programLine->operand ) || imme )
		{
			if ( destLoc < currLoc )
			{
				programLine->flags[F_P] = '1';
				currLoc -= destLoc + 1;
				TirapTchurum( &currLoc );
				strcat( programLine->operand, Filler( DecimalToHex( currLoc ), temp ) );
				fclose( fileSymtab );
				return;
			}
		}

		strcat( programLine->operand, Filler( DecimalToHex( destLoc ), temp ) );
		fclose( fileSymtab );
		return;
	}

	fclose( fileSymtab );
}

bool Jumper( char* operation )
{
	if ( strcmp( operation, "J" ) == 0 || strcmp( operation, "JEQ" ) == 0 || strcmp( operation, "JGT" ) == 0
	|| strcmp( operation, "JLT" ) == 0 || strcmp( operation, "JSUB" ) == 0 )
	{
		return true;
	}
	return false;
}

/*
===================
TirapTchurum
-------------------
2's complement from an integer

			OUA
===================
*/
void TirapTchurum( int* target )
{
	char binTarget[13] = { '\0' };

	strcpy( binTarget, DecimalToBinary( *target, 12 ) );

	for ( int i = 0; i < 12; ++i )
	{
		if ( binTarget[i] == '1' )
		{
			binTarget[i] = '0';
		} else {
			binTarget[i] = '1';
		}
	}

	*target = BinaryToDecimal( &binTarget[0], 12 );
}