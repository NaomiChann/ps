#include <stdio.h>
#include <string.h>

int main() //int argc, char const *argv[])
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
				break;
			case 2:
				printf( "8 bits op, 4 bits r1, 4 bits r2 \n" );
				break;
			case 3:
				printf( "6 bits op, 6 bits nixbpe, 12 bits displacement \n" );
				break;
			case 4:
				printf( "6 bits op, 6 bits nixbpe, 20 bits address  \n" );
				break;
			default:
				printf( "SIZE ERROR \n" );
				break;
		}
        
	}

	fclose( inputFile );

	return 0;
}
