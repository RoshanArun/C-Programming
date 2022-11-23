#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if(argc != 2)
        {
            fprintf(stderr, "Usage: ./recover image\n");
            return 1;
        }

    //open memory card file
    char *infile = argv[1];
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
        {
            fprintf(stderr, "Could not open %s.\n", infile);
            return 2;
        }

    unsigned char* buffer = malloc(512);
    int number = 0;
    FILE* num = NULL;

//find the begining of JPEG
    //check each 512 byte block to see if it has a JPEG header
    //read blocks until it reaches another JPEG header
    //use a fread statement to read memory card
    //data-point to the struct that contains the bytes, inputr-file to read from
    while (fread(buffer, 1, 512, inptr)){

        //find a JPEG file
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0){

            //close previous file
           if(number > 0){
            fclose(num);
           }

           //create newfile
           char newfile[8];
           sprintf(newfile, "%03i.jpg", number);

           //open the new image
            //sprintf(filename, "%03i.jpg", number);
            //filename : character array to store resultant string
            //FILE num* = fopen(filename, "w")
           num = fopen(newfile, "w");
           number = number + 1;
        }
        //if the first jpeg isnt found then it skips it
        if(!number){
            continue;
        }

        //use a fwrite statement
        fwrite(buffer, 512, 1, num);

        //once the 50th file is recieved, stops the program
        if(number == 51){
            break;
        }
    }
    //close files
    fclose(inptr);
    fclose(num);
    return 0;
}
