#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef uint8_t BYTE;


int main(int argc, char *argv[])
{
    //check the numer of arguments to be at least one
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    //open raw file
    FILE *raw_file = fopen(argv[1], "r");
    if (raw_file == NULL)
    {
        printf("File didn't read");
        return 2;
    }

    FILE *image = NULL;

    // variables to be used in loop
    int image_num = 0;
    int JPEG_num = 0;
    BYTE buffer[512];
    char filename[8];


    // loop until end of file is reached
    while (fread(buffer, sizeof(BYTE), 512*sizeof(BYTE), raw_file) > 0)
    {
        // condition to check for JPEG format
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0)==0xe0)
        {
            //condition for name if it is not first JPEG 
            if (JPEG_num > 0)
            {
                fclose(image);
                sprintf(filename, "%03i.jpg", JPEG_num);
                image = fopen(filename, "w");
                JPEG_num++;
            }

            //condition for first JPEG file name
            else
            {
                sprintf(filename, "%03i.jpg", JPEG_num);
                image = fopen(filename, "w");
                JPEG_num++;
            }



        }
        // write data to a file to create JPEG
        if (JPEG_num > 0)
            {
                fwrite(buffer, sizeof(BYTE), 512*sizeof(BYTE), image);
            }


    }

    // close all files
    fclose(raw_file);
    fclose(image);
    return 0;

}
