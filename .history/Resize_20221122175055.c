// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: copy infile outfile\n");
        return 1;
    }

    int size = atoi(argv[1]);

    // Remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // Open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // Open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // Read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // Read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // Ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    //new dimesions
    int oldWidth = bi.biWidth;
    int oldHeight = bi.biHeight;
    int width = oldWidth * size;
    int height = oldHeight * size;

    //new padding
    int oldpadding = (4 - (oldWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int newpadding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

    //headers
    bi.biWidth = width;
    bi.biHeight = height;
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * width) + newpadding) * abs(height);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


    // Write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // Write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    RGBTRIPLE padding[width * sizeof(RGBTRIPLE)];

    // Iterate over infile's scanlines
    for (int i = 0, biHeight = abs(oldHeight); i < biHeight; i++)
    {
            // Iterate over pixels in scanline
            for (int j = 0; j < oldWidth; j++)
            {
                // Temporary storage
                RGBTRIPLE triple;

                // Read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // Write RGB triple to outfile
                // fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);

                  for(int k = 0; k < size; k++)
                padding[(j * size) + k] = triple;
            }

        fseek(inptr, oldpadding, SEEK_CUR);

        for(int j = 0; j < size; j++){
            fwrite(padding, sizeof(RGBTRIPLE), width, outptr);

            //newpadding
            for(int g = 0; g < newpadding; g++){
                fputc(0x00, outptr);
            }
        }
    }

    // Close infile
    fclose(inptr);

    // Close outfile
    fclose(outptr);

    // Success
    return 0;
}