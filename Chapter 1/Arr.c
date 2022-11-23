#include <cs50.h>
#include <stdlib.h>
#include <stdio.h>
#include <crypt.h>
#include <string.h>
#include <time.h>

void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Stroing start time
    clock_t start_time = clock();

    // looping till required time is not acheived
    while (clock() < start_time + milli_seconds)
        ;
}


int main(int argc, char* argv[]) {

    int delayTime = 50;
    string alphas = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (int i = 0; i < strlen(alphas); i++) {
        printf("In the 1st \"for loop\" using %c\n", alphas[i]);
        delay(delayTime);

        for (int j = 0; j < strlen(alphas); j++) {
            printf("    In the 2nd \"for loop\" using %c\n", alphas[j]);
            delay(delayTime);

            for (int k = 0; k < strlen(alphas); k++) {
                printf("        In the 3rd \"for loop\" using %c\n", alphas[k]);
                delay(delayTime);

                for (int l = 0; l < strlen(alphas); l++) {
                    printf("            In the 4th \"for loop\" using %c\n", alphas[l]);
                    delay(delayTime);

                    for (int m = 0; m < strlen(alphas); m++) {
                        printf("                In the 5th \"for loop\" using %c\n", alphas[m]);
                        delay(delayTime);
                        if (m == 51) {
                            //return 0;
                        }

                    }
                }
            }
        }

    }
}