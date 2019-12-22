#include <stdio.h>
#include <string.h>
#include "zlib.h"

#define B_SIZE 2014*1024

// Tool to compress input data with ZLIB and output, to a file, the compressed data.
// Compile: gcc deflate.c -lz -o deflate
int main(int argc, char* argv[])
{
	char buffer[B_SIZE];
	char deflated[B_SIZE];
	char* infile;
	char* outfile;
	
	FILE *inFptr;
	FILE *outFptr;
	
	size_t totInBytes = 0;

	if (argc != 3)
	{
		printf("Usage: [%s] input_file output_file\n\n", argv[0]);

	}
	else
	{
		printf("Input file:  [%s]\n", argv[1]);
		printf("Output file: [%s]\n", argv[2]);

		infile = argv[1];
		outfile = argv[2];

		inFptr = fopen(infile, "r"); //	Open input file for reading.
		
		if (inFptr == NULL)
		{
			printf("Failed to open file input file: [%s].\nExiting.\n", infile);
			return 1;
		}

		// Read the whole contents of the input file and store in memory.
		size_t inBytes = 0;
		do 
		{
			inBytes = fread(buffer, 1, B_SIZE - totInBytes, inFptr);
			totInBytes += inBytes;

			printf("Read [%d] bytes, for a total of [%d] bytes.\n", inBytes, totInBytes);

		} while (inBytes > 0);

		// Release resources.
		fclose(inFptr);
	
		
		// Read buffer from STDIN
		//fgets(buffer, B_SIZE, stdin);
		//printf("Read: %s", buffer);

		// Compress using ZLIB
		z_stream defstream;
		defstream.zalloc = Z_NULL;
		defstream.zfree = Z_NULL;
		defstream.opaque = Z_NULL;
		defstream.avail_in = (uInt)totInBytes; // size of input, string + terminator
		defstream.next_in = (Bytef *)buffer; // input char array
		defstream.avail_out = (uInt)sizeof(deflated); // size of output
		defstream.next_out = (Bytef *)deflated; // output char array
		
		// the actual compression work.
		deflateInit(&defstream, Z_BEST_COMPRESSION);
		deflate(&defstream, Z_FINISH);
		deflateEnd(&defstream);
	
    printf("Compressed size is [%lu] bytes.\n", defstream.total_out);	

		printf("Opening file [%s] for output...\n", outfile);
		outFptr = fopen(outfile, "w"); //	Open input file for reading.
		
		if (outFptr == NULL)
		{
			printf("Failed to open output file: [%s].\nExiting.\n", outfile);
			return 1;
		}

		size_t outBytes = fwrite(deflated, 1, defstream.total_out, outFptr);
		fclose(outFptr);
	
		printf("Wrote a total of [%d] bytes to output file [%s].\n", outBytes, outfile);
	} 



	return 0;

}
