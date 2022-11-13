//this code is just an experiment unrelated to a lab or problem set. Just challenging myself. this file
//is partly based off filter.c from problem set 4.
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "functions_nacci.c"

int main(int argc, char *argv[])
{
    //greet user


    printf("\n\n\nWELCOME TO THE FIBONACCI ART GENERATOR\n");
    printf("created by Bradley Liang\n");
    printf("🎨  🎨             🖼️  🖼️\n\n\n");


    //take in ./main_nacci [image(-i) or blank(-b)] infile.bmp outfile.bmp
    //./main_nacci -i blank.bmp nacci.bmp
    //2 modes for this program
    char *modes = "ib";

    //get mode selected by user and check if it works
    char selected_mode = getopt(argc, argv, modes);
    if (selected_mode != 'i' && selected_mode != 'b')
    {
        printf("hey moron, only 2 options exist. 'i', for images and 'b' for blank images. try again you neanderthal\n");
        return 1;
    }

    //if multiple modes were selected, throw an error at 'em
    if (getopt(argc, argv, modes) != -1)
    {
        printf("Hey genius, you can't run 2 modes at the same time\n");
        return 2;
    }

    //make sure this program is used right
    if (argc != 4)
    {
        printf("something went wrong when main_nacci.c tried to read your stupid incomprehensible command line\n");
        return 3;
    }

    //remember infile and outfile names
    char *intake_file_name = argv[2];
    char *output_file_name = argv[3];

    //open the inputed file
    FILE *input_file = fopen(intake_file_name, "r");
    if (input_file == NULL)
    {
        printf("I can't open the input file, it won't open for some reason\n");
        return 4;
    }

    //open a new file for output
    FILE *output_file = fopen(output_file_name, "w");
    if (output_file == NULL)
    {
        fclose(input_file);
        fclose(output_file);
        printf("I got your inputed file, but I couldn't create a new file to put my awesome drawing in\n");
        return 5;
    }

    // Read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, input_file);

    // Read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, input_file);

    // Ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(output_file);
        fclose(input_file);
        printf("Unsupported file format.\n");
        return 6;
    }

    // Get image's dimensions
    int height = abs(bi.biHeight);
    int width = bi.biWidth;
    //make sure image is square
    if (height != width)
    {
        printf("image isn't a square");
        fclose(output_file);
        fclose(input_file);
        return 7;
    }

    //make sure image is multiple of 43
    if ((height % 43) != 0)
    {
        printf("image dimensions aren't similar to a 43 by 43 square");
        fclose(output_file);
        fclose(input_file);
        return 7;
    }

    // Allocate memory for image
    RGBTRIPLE(*image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (image == NULL)
    {
        printf("you have basically no RAM left, I'd be pretty worried if I were you. Could not find memory for image\n");
        fclose(output_file);
        fclose(input_file);
        return 8;
    }

    //allocate memory for alpha matte
    bool(*alpha_matte)[width] = calloc(height, width * sizeof(bool));
    if (alpha_matte == NULL)
    {
        printf("you have basically no RAM left, I'd be pretty worried if I were you. Could not find image for Alpha Matte\n");
        fclose(output_file);
        fclose(input_file);
        return 9;
    }

    // Determine padding for scanlines
    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

    // Iterate over infile's scanlines
    for (int i = 0; i < height; i++)
    {
        // Read row into pixel array
        fread(image[i], sizeof(RGBTRIPLE), width, input_file);

        // Skip over padding
        fseek(input_file, padding, SEEK_CUR);
    }

    //

    //using mode, call different functions
    switch (selected_mode)
    {
        case 'i' :
            linemaker(height, width, image, alpha_matte, 0.8);
            shader(height, width, image, alpha_matte, 0.8);
            break;

        case 'b' :
            linemaker(height, width, image, alpha_matte, 0);
            shader(height, width, image, alpha_matte, 0);
            break;
    }
    // Write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, output_file);

    // Write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, output_file);

    // Write new pixels to outfile
    for (int i = 0; i < height; i++)
    {
        // Write row to outfile
        fwrite(image[i], sizeof(RGBTRIPLE), width, output_file);

        // Write padding at end of row
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, output_file);
        }
    }

    //at last, release the memory that the 2d array once stood
    free(image);
    free(alpha_matte);

    //also, close out of both input and output
    fclose(output_file);
    fclose(input_file);

    //Finally, we return 0 to exit our journey
    return 0;
}
