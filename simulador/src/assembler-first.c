#include "assembler-second.h"

// GLOBAL
const char* directives[] = { "BYTE", "WORD", "RESB", "RESW", "BASE", "NOBASE", "END", "START" };

void Fetcher( char* line, line_t* programLine );
bool LabelDuplicates( char* label, int loc );
int LocUpdate( line_t* programLine );
int IWontByte( line_t* programLine );
void Printy( line_t* programLine, int length );
void DidYouComme( char* source );

int main()
{
	FILE* fileProgram = fopen( "program/assembly.asm", "r" );

	line_t programLine = { 0 };
	char line[256] = { '\0' };
	int locC = 0, startAddr = 0, operandInt = 0, length = 0;

	ClearFile( "tables/symbol.txt" );
	ClearFile( "program/flagged.txt" );

	while ( fgets( line, sizeof( line ), fileProgram ) )
	{
		Fetcher( line, &programLine );

		if ( programLine.label[0] == '.' )
		{
			continue;
		}

		programLine.loc = locC;

		if ( strlen( programLine.label ) > 0 && LabelDuplicates( programLine.label, programLine.loc ) )
		{
			// has duplicate label
			fputs( "\n=!=!=!=!=!=!= \n\nDUPLICATE LABEL \n\n=!=!=!=!=!=!= \n\n", stderr );
			fclose( fileProgram );
			return EXIT_FAILURE;
		}

		operandInt = atoi( programLine.operand );

		if ( strcmp( programLine.operation, "END" ) == 0 )
		{
			Printy( &programLine, length );
			break;
		}

		if ( strcmp( programLine.operation, "START" ) == 0 )
		{
			Printy( &programLine, length );
			startAddr = operandInt;
			locC = startAddr;
			continue;
		}

		length = LocUpdate( &programLine );

		Printy( &programLine, length );

		locC += length;

	}

	fclose( fileProgram );

	MountyTheEvilTwin();
	return 0;
}

/*
===================
Fetcher
-------------------
Divides contents from the file line and
stores in program line
===================
*/
void Fetcher( char* line, line_t* programLine )
{
	if ( isspace( line[0] ) ) // no label
	{
		LeftStrip( line );
		memset( programLine->label, '\0', 8 );
		programLine->operation = strtok( line, "\t" );
		programLine->operand = strtok( NULL, "\n" );

	} else { // labeled
		strcpy( programLine->label, strtok( line, "\t" ) );
		programLine->operation = strtok( NULL, "\t" );
		programLine->operand = strtok( NULL, "\n" );

	}
}

/*
===================
LabelDuplicates
-------------------
Checks if provided label is not yet present in symtab,
also declines any label that would conflict with register
names
===================
*/
bool LabelDuplicates( char* label, int loc )
{
	FILE* fileSymtab = fopen( "tables/symbol.txt", "a+" );
	char line[256] = { '\0' }, *labelLine;
	bool duplicate = false;

	while ( fgets( line, sizeof( line ), fileSymtab ) )
	{
		labelLine = strtok( line, "\t" );

		if ( strcmp( label, labelLine ) == 0 )
		{
			duplicate = true;
			fclose( fileSymtab );
			return duplicate;
		}
		
	}

	fprintf( fileSymtab, "%s\t%d\n", label, loc );
	fclose( fileSymtab );

	return duplicate;
}

/*
===================
LocUpdate
-------------------
Checks instruction table for instruction format, also
treats memory allocation directives and extended formats
===================
*/
int LocUpdate( line_t* programLine )
{
	FILE* fileInstruction = fopen( "tables/instruction.txt", "r" );

	char* instruction, * format, line[256] = { '\0' };
	int formatInt, startingPos = 0;
	bool plus = false;

	if ( programLine->operation[0] == '+' )
	{
		plus = true;
		startingPos = 1;
	}

	while( fgets( line, sizeof( line ), fileInstruction ) )
	{
		instruction = strtok( line, " " );
		strtok( NULL, " " );
		format = strtok( NULL, "\n" );
		formatInt = 0;

		if ( strcmp( &( programLine->operation[startingPos] ), instruction ) == 0 )
		{
			formatInt = atoi( format );
			if ( plus )
			{
				if ( formatInt == 3 )
				{
					++formatInt;
				} else {
					fputs( "\n=!=!=!=!=!=!= \n\nINVALID INSTRUCTION \n\n=!=!=!=!=!=!= \n\n", stderr );
				}
			}
			break;
		}
	}

	fclose( fileInstruction );

	if ( formatInt == 0 )
	{
		for ( int i = 0; i < 4; ++i )
		{
			if ( strcmp( programLine->operation, directives[i] ) == 0 )
			{
				switch ( i )
				{
					case 0: // BYTE
						formatInt = IWontByte( programLine );
						break;

					case 1: // WORD
						formatInt = 3;
						strcpy( programLine->operand, Filler( DecimalToHex( atoi( programLine->operand ) ), 6 ) );
						break;
						
					case 2: // RESB
						formatInt = atoi( programLine->operand );
						break;

					case 3: // RESW
						formatInt = 3 * atoi( programLine->operand );
						break;

					default:
						break;

				}
			}
		}
	}

	return formatInt;
}

/*
===================
IWontByte
-------------------
Treats the byte directive for both hex and char
===================
*/
int IWontByte( line_t* programLine )
{
	char* operandHex, * token, temp[3] = { '\0' };
	int length = 0;
	
	if ( strstr( programLine->operand, "X'" ) != NULL )
	{
		strtok( programLine->operand, "'" );
		programLine->operand =  strtok( NULL, "'" );

	} else if ( strstr( programLine->operand, "C'" ) != NULL ) {

		strtok( programLine->operand, "'" ); 
		programLine->operand =  strtok( NULL, "'" );

		operandHex = ( char* ) malloc( ( ( strlen( programLine->operand ) * 2 ) + 1 ) * sizeof( char ) );
		memset( operandHex, '\0', ( ( strlen( programLine->operand ) * 2 ) + 1 ) );

		token = strtok( programLine->operand, "" );

		while ( token != NULL )
		{
			printf( "\nC: " );
			// char by char
			for ( int i = 0; i < strlen( token ); ++i )
			{
				memcpy( temp, ASCIItoHex( &token[i] ), 2 );
				strcat( operandHex, temp );
			}
			printf( "\n" );

			token = strtok( NULL, "" );
		}
		
		strcpy( programLine->operand, operandHex );

		free( operandHex );
	}

	length = strlen( programLine->operand ) / 2;
	
	if ( strlen( programLine->operand ) % 2 != 0 )
	{
		++length;
	}

	Filler( programLine->operand, length * 2 );
	
	return length;
}

/*
===================
Printy
-------------------
Writes and formats intermediate file output
===================
*/
void Printy( line_t* programLine, int length )
{
	FILE* fileOut = fopen( "program/flagged.txt", "a+" );
	bool directive = false, flags[7] = { false };
	static bool base = false;

	if ( strcmp( programLine->operation, "BASE" ) == 0 )
	{
		base = true;
		return;
	} else if ( strcmp( programLine->operation, "NOBASE" ) == 0 )
	{
		base = false;
		return;
	}

	if ( programLine->operand != NULL )
	{
		DidYouComme( programLine->operand );
	}
	
	if ( strcmp( programLine->operation, "START" ) != 0 )
	{
		fprintf( fileOut, "%d\t", programLine->loc );
	}

	for ( int i = 0; i < 8; ++i )
	{
		if ( strcmp( programLine->operation, directives[i] ) == 0 )
		{
			directive = true;
			break;
		}
	}

	if ( length > 2 && !directive )
	{
		if ( programLine->operand[0] == '@' ) // indirect
		{
			flags[F_N] = true;
			flags[F_I] = false;
			programLine->operand = &(programLine->operand[1]);
		} else if ( programLine->operand[0] == '#' ) { // immediate
			flags[F_N] = false;
			flags[F_I] = true;
			programLine->operand = &(programLine->operand[1]);
		} else { // simple
			flags[F_N] = true;
			flags[F_I] = true;

			if ( strstr( programLine->operand, ", X" ) != NULL ) // indexed
			{
				flags[F_X] = true;
				char temp[8] = { '\0' };
				strcpy( temp, strtok( programLine->operand, "," ) );
				strcpy( programLine->operand, temp );
			}
		}

		flags[F_B] = base; // base relative

		// pc relative needs calculation to destination
		
		if ( programLine->operation[0] == '+' ) // extended
		{
			flags[F_E] = true;
			programLine->operation = &(programLine->operation[1]);
		}

		for ( int i = 0; i < 6; ++i )
		{
			if ( flags[i] == true ) {
				fprintf( fileOut, "1" );
			} else {
				fprintf( fileOut, "0" );
			}
		}
	}

	if ( strlen( programLine->label ) > 0 )
	{
		fprintf( fileOut, "\t%s\t", programLine->label );
	} else {
		fprintf( fileOut, "\t\t" );
	}

	fprintf( fileOut, "%s\t%s\n" , programLine->operation, programLine->operand );

	fclose( fileOut );
}

/*
===================
DidYouComme
-------------------
Removes comments
===================
*/
void DidYouComme( char* source )
{
	bool comment = false;

	if ( ( strstr( source, "." ) != NULL ) )
	{
		comment = true;
	}

	if ( comment )
	{
		source = strtok( source, "\t" );
	} else {
		source = strtok( source, "\n" );
	}
}