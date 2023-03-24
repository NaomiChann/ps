#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int globalMem;

void memory ( int numBytes ) ;

int main() //
{
	FILE *inputFile = fopen( "cod-obj.txt", "r" );
	char opcode[9];
	int instSize;

	while ( 1 )
	{
    
		fscanf( inputFile, "%s", opcode );

		if ( feof( inputFile ) )
		{
			break;
		}

		instSize = ( ( int ) strlen( opcode ) ) / 2;
		printf( "%s ", opcode );
		printf( "%d bytes format, ", instSize );

		switch ( instSize )
		{
			case 1:
				printf( "8 bits op \n" );
				memory(1);
				break;
			case 2:
				printf( "8 bits op, 4 bits r1, 4 bits r2 \n" );
				memory(2);
				break;
			case 3:
				printf( "6 bits op, 6 bits nixbpe, 12 bits displacement \n" );
				memory(3);
				break;
			case 4:
				printf( "6 bits op, 6 bits nixbpe, 20 bits address  \n" );
				break;
			default:
				printf( "SIZE ERROR \n" );
				break;
		}      
	}
	printf( "Memory used: %d Bytes\n", globalMem);
	fclose( inputFile );

	return 0;
}

void memory ( int numBytes ) 
{
	globalMem += numBytes;

	if( globalMem > 1000 )
	{
		printf( "You have exceeded the memory size that is 1K" );
		exit( 1 );
	}
}
