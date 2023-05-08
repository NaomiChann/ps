#include "assembler-first.h"

FILE* fileInput_g;
FILE* fileOutput_g;
FILE* fileDeftab_g;

line_t input_g = { 0 };
char lineInput_g[256] = { '\0' };
char namTab_g[128][8] = { '\0' };
bool expanding_g = false;

int indexMacro_g = 0;
int levelExpansion_g = 0;

void Nanami( line_t process );
void ExpandOmori( char* macro );
void DelfinoPlaza();
void GetItTogether();
void RustRemover( char* removee );

int main()
{
	ClearFile( "tables/name.txt" );
	ClearFile( "tables/definition.txt" );

	fileInput_g = fopen( "program/input.asm", "r" );
	fileOutput_g = fopen( "program/assembly.asm", "w" );
	fileDeftab_g = fopen( "tables/definition.txt", "a+" );

	while ( 1 )
	{
		GetItTogether();
		Fetcher( lineInput_g, &input_g );

		if ( input_g.label[0] == '.' )
		{
			continue;
		}

		if ( strcmp( input_g.operation, "END" ) == 0 )
		{
			fprintf( fileOutput_g, "%s\t%s\t%s\n", input_g.label, input_g.operation, input_g.operand );
			break;
		}

		Nanami( input_g );
	}

	fclose( fileInput_g );
	fclose( fileOutput_g );
	fclose( fileDeftab_g );

	return 0;
}

void GetItTogether()
{
	if ( !expanding_g )
	{
		fgets( lineInput_g, sizeof( lineInput_g ), fileInput_g );
	}
}

void Nanami( line_t process )
{
	for ( int i = 0; i < indexMacro_g; ++i )
	{
		if ( strcmp( process.operation, namTab_g[i] ) == 0 )
		{
			printf( "\nEXPANDING %s. . .\n", namTab_g[i] );
			/*
			char guiout[100] = { '\0' };
			strcat( guiout, "\nEXPANDING " );
			strcat( guiout, namTab_g[i] );
			strcat( guiout, ". . .\n" );
			appendTextToOutput( guiout );
			*/
			ExpandOmori( namTab_g[i] );
			expanding_g = false;
			return;
		}
	}

	if ( strcmp( process.operation, "MACRO" ) == 0 )
	{
		strcpy( namTab_g[indexMacro_g], process.label );
		++indexMacro_g;
		fseek( fileDeftab_g, 0, SEEK_END );
		fprintf( fileDeftab_g, "%s\t%s\t%s\n", process.label, process.operation, process.operand );
		printf( "\nDEFINING %s. . .\n", process.label );
		DelfinoPlaza();
	} else {
		fprintf( fileOutput_g, "%s\t%s", process.label, process.operation );
		if ( strcmp( process.operation, "MEND" ) != 0 )
		{
			fprintf( fileOutput_g, "\t%s\n", process.operand );
		} else {
			fprintf( fileOutput_g, "\n" );
		}
	}
}

void ExpandOmori( char* macro )
{
	line_t expand = { 0 };
	char lineExpand[256] = { '\0' }, dummy[256] = { '\0' };
	char argtab[32][8] = { '\0' };
	int indexExpansion = 0, argCount = 0;

	expanding_g = true;
	++levelExpansion_g;

	rewind( fileDeftab_g );
	fgets( lineExpand, sizeof( lineExpand ), fileDeftab_g );
	strcpy( dummy, lineExpand );
	Fetcher( dummy, &expand );

	while ( strcmp( expand.label, macro ) != 0 )
	{
		fgets( lineExpand, sizeof( lineExpand ), fileDeftab_g );
		strcpy( dummy, lineExpand );
		Fetcher( dummy, &expand );
	}

	/* for ( int i = 0; i < argCount; ++i )
	{
		argtab[i];
	} */

	if ( strstr( expand.operand, "&" ) != NULL )
	{
		int tracker = strlen( expand.operand );

		for ( int i = 0; i < tracker; ++i )
		{
			if ( expand.operand[i] == '&' )
			{
				++argCount;
			}
		}
	}

	RustRemover( expand.operand );

	for ( int i = 0; i < argCount; ++i )
	{
		if ( i == 0 && argCount != 1 )
		{
			strcpy( argtab[i], strtok( expand.operand, ", " ) );
			continue;
		}
		
		if ( argCount == 1 )
		{
			strcpy( argtab[i], strtok( expand.operand, " \0" ) );

		} else if ( i == ( argCount - 1 ) ) {
			strcpy( argtab[i], strtok( NULL, " \0" ) );

		} else {
			strcpy( argtab[i], strtok( NULL, ", " ) );
		}
	}
	

	strcpy( dummy, lineExpand );
	Fetcher( dummy, &expand );

	while ( fgets( lineExpand, sizeof( lineExpand ), fileDeftab_g ) )
	{
		++indexExpansion;
		strcpy( dummy, lineExpand );
		Fetcher( dummy, &expand );

		for ( int i = 0; i < indexMacro_g; ++i )
		{
			if ( strcmp( expand.operation, namTab_g[i] ) == 0 )
			{
				printf( "\nEXPANDING %s. . .\n", namTab_g[i] );
				/*
				char guiout[100] = { '\0' };
				for ( int j = 0; j < levelExpansion_g; ++j )
				{
					strcat( guiout, "\t" );
				}
				strcat( guiout, "\nEXPANDING " );
				strcat( guiout, namTab_g[i] );
				strcat( guiout, ". . .\n" );
				appendTextToOutput( guiout );
				*/
				ExpandOmori( namTab_g[i] );
				rewind( fileDeftab_g );
				strcpy( dummy, lineExpand );

				while ( strcmp( expand.label, macro ) != 0 )
				{
					fgets( lineExpand, sizeof( lineExpand ), fileDeftab_g );
					strcpy( dummy, lineExpand );
					Fetcher( dummy, &expand );
				}

				for ( int i = 0; i < indexExpansion; ++i )
				{
					fgets( lineExpand, sizeof( lineExpand ), fileDeftab_g );
				}
				
				fgets( lineExpand, sizeof( lineExpand ), fileDeftab_g );
				Fetcher( lineExpand, &expand );
				++indexExpansion;
				break;
			}
		}

		if ( strcmp( expand.operation, "MEND" ) != 0 )
		{
			if ( strstr( expand.operand, "&" ) != NULL )
			{
				char dummyCat[256] = { '\0' };
				for ( int i = 0; i < argCount; ++i )
				{
					strcat( dummyCat, argtab[i] );
					if ( i < ( argCount - 1 ) )
					{
						strcat( dummyCat, ", " );
					}
				}
				printf( "%s\t%s\t%s\n", expand.label, expand.operation, dummyCat );
				fprintf( fileOutput_g, "%s\t%s\t%s\n", expand.label, expand.operation, dummyCat );
				continue;
			}
			printf( "%s\t%s\t%s\n", expand.label, expand.operation, expand.operand );
			fprintf( fileOutput_g, "%s\t%s\t%s\n", expand.label, expand.operation, expand.operand );
		} else {
			printf( "EXPANDED\n\n" );
			/*
			char guiout[100] = { '\0' };
			for ( int j = 0; j < levelExpansion_g - 1; ++j )
			{
				strcat( guiout, "\t" );
			}
			strcat( guiout, "EXPANDED" );
			appendTextToOutput( guiout );
			*/
			--levelExpansion_g;
			return;
		}
	}
}

void DelfinoPlaza()
{
	line_t define = { 0 };
	int level = 1;

	while ( level > 0 )
	{
		GetItTogether();
		Fetcher( lineInput_g, &define );

		if ( define.label[0] == '.' )
		{
			continue;
		}

		
		if ( strcmp( define.operation, "MACRO" ) == 0 )
		{
			++level;
		} else if ( strcmp( define.operation, "MEND" ) == 0 )
		{
			--level;
			fseek( fileDeftab_g, 0, SEEK_END );
			fprintf( fileDeftab_g, "\tMEND\n" );
			continue;
		}
		fseek( fileDeftab_g, 0, SEEK_END );
		fprintf( fileDeftab_g, "%s\t%s\t%s\n", define.label, define.operation, define.operand );
	}
	printf( "DEFINED\n\n" );
}

void RustRemover( char* removee )
{
	if ( strstr( removee, "&" ) != NULL )
	{
		int tracker = strlen( removee );
		char single[2] = { '\0' };
		char* holder = ( char* ) malloc( sizeof( char ) * ( tracker + 1 ) );
		memset( holder, '\0', ( tracker + 1 ) );

		for ( int i = 0; i < tracker; ++i )
		{
			if ( removee[i] == '&' )
			{
				memcpy( single, &removee[i + 1], 1 );
				++i;
			} else {
				memcpy( single, &removee[i], 1 );
			}

			strcat( holder, single );
		}

		strcpy( removee, holder );

		free( holder );
	}
}