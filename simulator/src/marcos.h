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
int expansionCounter_g = 0;

void Nanami( line_t process );
void ExpandOmori( char* macro, char* argInput );
void DelfinoPlaza();
void GetItTogether();
void RustRemover( char* removee );
void Arganizer( int argCount, char argtab[32][8], char* argline );
void Exchanger( char* exchangee, int argCount, char argtab[32][8], char argOriginal[32][8] );

int Mamma()
{
	printf( "\nINITIALIZING. . .\n" );
	ClearFile( "tables/name.txt" );
	ClearFile( "tables/definition.txt" );

	fileInput_g = fopen( "program/input.asm", "r" );
	fileOutput_g = fopen( "program/MASMAPRG.asm", "w" );
	fileDeftab_g = fopen( "tables/definition.txt", "a+" );

	
	printf( "MACRO PROCESSING STARTED\n" );

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

	printf( "MACRO PROCESSING CONCLUDED\n\n" );
	
	Mounty();

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
			ExpandOmori( namTab_g[i], process.operand );
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

void Arganizer( int argCount, char argtab[32][8], char* argline )
{
	for ( int i = 0; i < argCount; ++i )
	{
		if ( i == 0 && argCount != 1 )
		{
			strcpy( argtab[i], strtok( argline, ", " ) );
			continue;
		}
		
		if ( argCount == 1 )
		{
			strcpy( argtab[i], strtok( argline, " \0" ) );

		} else if ( i == ( argCount - 1 ) ) {
			strcpy( argtab[i], strtok( NULL, " \0" ) );

		} else {
			strcpy( argtab[i], strtok( NULL, ", " ) );
		}
	}
}

void Exchanger( char* exchangee, int argCount, char argtab[32][8], char argOriginal[32][8] )
{
	if ( exchangee != NULL )
	{
		for ( int i = 0; i < argCount; ++i )
		{
			if ( strstr( exchangee, "&" ) != NULL )
			{
				int tracker = strlen( exchangee );
				char single[2] = { '\0' };
				char* holder = ( char* ) malloc( sizeof( char ) * ( tracker + 1 ) );
				char* replaced = ( char* ) malloc( sizeof( char ) * ( tracker + 1 ) );
				memset( holder, '\0', ( tracker + 1 ) );
				memset( replaced, '\0', ( tracker + 1 ) );

				for ( int i = 0; i <= tracker; ++i )
				{
					if ( exchangee[i] == '&' )
					{
						memcpy( single, &exchangee[i + 1], 1 );
						++i;
					} else if ( exchangee[i] == ',' || exchangee[i] == '\0' ) {
						for ( int j = 0; j < argCount; ++j )
						{
							if ( strcmp( holder, argtab[j] ) == 0 )
							{
								strcat( replaced, argOriginal[j] );
								memset( holder, '\0', ( tracker + 1 ) );
								memset( single, '\0', 1 );
								++i;
							}
						}
					} else {
						memcpy( single, &exchangee[i], 1 );
					}

					strcat( holder, single );
				}

				strcpy( exchangee, replaced );

				free( holder );
			}
		}
	}
}

void Alphies( char* line, char* alpha )
{
	char holder[256] = { '\0' };

	char single[2] = { '\0' };
	if ( strstr( line, "$" ) != NULL )
	{
		for ( int i = 0; i < strlen( line ); ++i )
		{
			memcpy( single, &line[i], 1 );
			strcat( holder, single );

			if ( line[i] == '$' )
			{
				strcat( holder, alpha );
			}
		}

		strcpy( line, holder );
	}
}

void ExpandOmori( char* macro, char* argInput )
{
	line_t expand = { 0 };
	char lineExpand[256] = { '\0' }, dummy[256] = { '\0' };
	char argtab[32][8] = { '\0' }, argOriginal[32][8] = { '\0' };
	char alpha[3];
	int indexExpansion = 0, argCount = 0, expansionId = 0;

	expanding_g = true;
	++levelExpansion_g;
	expansionId = expansionCounter_g;
	strcpy( alpha, DecimalToAlphanumeric( expansionId ) );
	++expansionCounter_g;

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
	Arganizer( argCount, argtab, expand.operand );
	Arganizer( argCount, argOriginal, argInput );
	
	strcpy( dummy, lineExpand );
	Fetcher( dummy, &expand );

	while ( fgets( lineExpand, sizeof( lineExpand ), fileDeftab_g ) )
	{
		++indexExpansion;
		
		strcpy( dummy, lineExpand );
		Fetcher( dummy, &expand );

		Exchanger( expand.operand, argCount, argtab, argOriginal );

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
				ExpandOmori( namTab_g[i], expand.operand );
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
				
				//Fetcher( lineExpand, &expand );
				++indexExpansion;
				break;
			}
		}

		if ( strcmp( expand.operation, "MEND" ) != 0 )
		{
			Alphies( lineExpand, alpha );
			Fetcher( lineExpand, &expand );

			if ( strstr( expand.operand, "&" ) != NULL )
			{
				Exchanger( expand.operand, argCount, argtab, argOriginal );
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
	printf( "DEFINED\n" );
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