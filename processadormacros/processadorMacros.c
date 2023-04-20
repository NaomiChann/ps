#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
===================
FindMacro
-------------------
Searchs for macros in the assembly file,
and creates another file with the name of the macro.
===================
*/
void FindMacro ()
{
    char line[256], *result, *name, *fp;
    FILE* inputFile = fopen( "assembly.asm", "r" );

    
	while(fgets( line, sizeof( line ), inputFile ) )
	{
        if( strcmp( line,"END" ) == 0 )
        {
            break;
        } 
        else
        {
            result = strstr(line, "MACRO");
            if (result != NULL) 
            {
            int i = 0;
            while (isspace(line[i])) 
            {
                i++;
            }

            int j = i;
            while (line[j] != '\0' && !isspace(line[j]))
            {
            j++;
            }
            strncpy(name, line + i, j - i);
            name[j - i] = '\0';
            fp = strcat( name, ".asm" );
            FILE* outputFile = fopen( fp, "w" );
            fprintf(outputFile, "%s\n", line);
            fclose(outputFile);
            }
        }

    }
    fclose( inputFile );
}

/*
===================
ExpandMacro
-------------------
Second part of the process, expands
the macros.
===================
*/
void ExpandMacro( const char* fileName)
{
    char line[256], *result, *name, *fp;
    FILE* inputFile = fopen( "assembly.asm", "r" );

    while(fgets( line, sizeof( line ), inputFile ) )
	{
        FILE* outputFile = fopen( fileName, "w" );
        fscanf(fileName, "%s", "$MACRO");
         if( strcmp( line,"END" ) == 0 )
        {
            break;
        } 
        fprintf(outputFile, "%s\n", line);
        fscanf(fileName, "%s", "MEND");
        fclose(outputFile);
    }
    fclose(inputFile);
}

int main()
{
    FindMacro();
}