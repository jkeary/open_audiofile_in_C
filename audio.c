/****************************************************************************
 * audio.c
 * MPATE-GE 2618
 * Problem Set 4
 * 
 * Processes the beginning of an AIFF file.  
 * Reads in the the FORM id at the beginning of the file and then
 * reads the chunk size that follows it.  The chunk size is equal the
 * size of the entire AIFF file not including the 4-byte FORM id 
 * and the 4-byte chunk size value.
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "audio.h"

int main(int argc, char * argv[])
{
	
	/* DECLARE AND INITIALIZE VARIABLE */
	const char* infile = "mysteryAudio"; 
	const char* outfile1 = "f1.aiff";
	const char* outfile2 = "f2.aiff";
	const char* outfile3 = "f3.aiff";
	const char* outfile4 = "f4.aiff";
	const char* outfile5 = "f5.aiff";
	int i = 1;
	bool found = false;
	FILE *inptr, *outptr; 
	unsigned char chunkId[4];
	uint32_t chunkSize, convertedChunkSize, bytes_to_read;
	const int bsize = 512;
	unsigned char buffer[bsize];
	
//	/* DECLARE FUNCTIONS */
//	/* chunkSize and convertedChunkSize return 32 bit int values */
//	uint32_t chunkSize, convertedChunkSize;  
  
	/* OPEN FILES */ 
	inptr = fopen(infile, "rb");
	if (inptr == NULL) {
		printf("Could not open %s.\n", infile);
		return 1;
	}
	
	for ( i = 1 ; i <= 5 ; i++ ) { 
		if ( i == 1 ) {
			outptr = fopen( outfile1, "wb");
		}
		if ( i == 2 ) {
			outptr = fopen( outfile2, "wb");
		}
		if ( i == 3 ) {
			outptr = fopen( outfile3, "wb");
		}
		if ( i == 4 ) {
			outptr = fopen( outfile4, "wb");
		}
		if ( i == 5 ) {
			outptr = fopen( outfile5, "wb");	
		}
	
		/* Iterate through mysteryAudio looking for 'FORM' chunkIds */
		//while ( !feof(inptr) ) {
        found = false;
        printf("i: %d\n", i);
		while( !found ) {
			/* Read 4 Bytes */
			fread( chunkId, sizeof(unsigned char), 4, inptr ); 
//            printf("%c%c%c%c\n", chunkId[0], chunkId[1], chunkId[2], chunkId[3] );
			/* Check if the 4B are 'FORM' */
			if ( chunkId[0] == 'F' && chunkId[1] == 'O' && chunkId[2] == 'R' && chunkId[3] == 'M' ) {
			    printf("you found an aiff!!!\n");
				found = true;
				if ( found == true ) {
					/* write chunkID out*/
					fwrite( chunkId, sizeof(unsigned char), 4, outptr );
                    /* get chunkSize */
                    chunkSize = getChunkSize(inptr);
                    printf("ChunkSize: %#x\n", chunkSize);
                    /* write chunkSize out */
                    fwrite( &chunkSize, sizeof(uint32_t), 1, outptr );
                    /* chunkSize is in Big Endian so convert to Little */
                    convertedChunkSize = convertFromBigEndian(chunkSize);
                    printf("ConvertedChunkSize: %#x\n", convertedChunkSize);
                    /* read and write the aiff's COMM and SSND data chunk*/
                    /* While some bytes left, keep reading and writing */
                    while ( convertedChunkSize > 0 ) {
                        bytes_to_read = ( convertedChunkSize < bsize ) ? convertedChunkSize : bsize;
                        fread( buffer, sizeof(unsigned char), bytes_to_read, inptr );
                        fwrite( buffer, sizeof(unsigned char), bytes_to_read, outptr );
                        convertedChunkSize -= bytes_to_read;
                    }
                    /* Close your outptr */
                    fclose(outptr);
                    /* iterate to next outfile */
//						i++;
                }
            } 
            else { 
			    fseek( inptr, -3, SEEK_CUR );
			}
            chunkId[0] = 'C';
		}
			
		/* CLOSE */
		fclose(outptr);

	}
    /* Alright indeed, Uri */
	return 0;

}

/// FUNCTIONS CALLED BY MAIN FUNCTION ///

// Read in a 4-byte integer from the file that is the size of the aiff
uint32_t getChunkSize(FILE *inptr)
{
	uint32_t chunkSize;
	fread(&chunkSize, sizeof(uint32_t), 1, inptr);
	return chunkSize;
}

// Convert from a big endian to little endian format if necessary
// Note: you don't have understand this function (yet)
uint32_t convertFromBigEndian(uint32_t val)
{
	const int ONE = 1;
	uint32_t littleEndianVal = val;
																			 
	if ((*(char*)&ONE) != 0) { // means architecture is little endian
		littleEndianVal = (littleEndianVal >> 24) | (littleEndianVal >> 8 & 0x0000FF00) | (littleEndianVal << 8  &  0x00FF0000) | (littleEndianVal << 24);
		printf("Your machine is little endian.\n");
		return littleEndianVal;
	} else { // if architecture is big endian, just return the val as is.
		printf("Your machine is big endian.\n");
		return val;
	}
}
