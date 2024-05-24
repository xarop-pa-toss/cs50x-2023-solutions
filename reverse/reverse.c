#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    // TODO #1
    if (argc != 3)
    {
        printf("Please input input file followed by output file as arguments");
        return 1;
    }

    // Open input file for reading
    // TODO #2
    FILE *inWAV = fopen(argv[1], "r");
    if (inWAV == NULL)
    {
        printf("File not opened successfuly. Exiting program...");
        return 1;
    }

    // Read header into an array
    // TODO #3
    WAVHEADER header;
    fread(&header, 1, sizeof(header), inWAV);

    // Use check_format to ensure WAV format
    // TODO #4
    if (check_format(header) != 0)
    {
        printf("File is not WAV");
        return 1;
    }

    // Open output file for writing
    // TODO #5
    FILE *outWAV = fopen(argv[2], "w");

    // Write header to file
    // TODO #6
    fwrite(&header, 1, sizeof(header), outWAV);

    // Use get_block_size to calculate size of block
    // TODO #7
    int blockSize = get_block_size(header);

    // Write reversed audio to file
    // TODO #8

    // Size of audio (full file - header size)
    fseek(inWAV, 0, SEEK_END);
    int audioSize = ftell(inWAV) - sizeof(header);
    int channels = header.numChannels;

    // couldn't get it working reading without messing with the audioSize.
    // this implementation removes blocksize from total audioSize each time it reads a block

    fseek(inWAV, sizeof(header) + audioSize - blockSize, SEEK_SET);
    while (audioSize > 0)
    {
        char buffer[blockSize];
        fread(buffer, 1, blockSize, inWAV);
        fwrite(buffer, 1, blockSize, outWAV);
        fseek(inWAV, sizeof(header) + audioSize - (blockSize * 2), SEEK_SET);
        //doubling blockSize by two ensures the cursor moves to the correct position

        audioSize -= blockSize;

        //fseek(inWAV, -blockSize, SEEK_CUR);
        //fread(&buffer, blockSize, 1, inWAV);
        //fwrite(&buffer, blockSize, 1, outWAV);
        //fseek(inWAV, -blockSize, SEEK_CUR);
    }

    fclose(inWAV);
    fclose(outWAV);
}

int check_format(WAVHEADER header)
{
    // TODO #4
    // checking if header.format is "WAVE"
    int formatCheck[4];
    char *wave = "WAVE";

    for (int i = 0; i < 4; i++)
    {
        formatCheck[i] = header.format[i];
    }

    for (int k = 0; k < 4; k++)
    {
        if (formatCheck[k] != wave[k])
        {
            return 1;
        }
    }

    return 0;
}

int get_block_size(WAVHEADER header)
{
    // TODO #7
    return header.numChannels * (header.bitsPerSample / 8);
}