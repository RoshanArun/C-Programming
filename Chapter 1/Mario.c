#include <stdio.h>
#include <cs50.h>

int main(void){
    int height = get_int("Height:"); //gets height of pyramid
    bool x = true;

    while( x == true){
    if(height <= 23 && height > 0){ //while x equals true, and the height is between 0 and 23, moves to the for loops
        x = false;
        }else{ //if height is not between 0 and 23, it will prompt the user for height again
        height = get_int("Height:");
        }
    }
    for(int i = 1; i <= height; i++){
        for(int space = height - i; space > 0; space--){
            printf(" ");//prints the number of spaces
        }

        for(int left = 2; left <= i + 1; left++){
            printf("#");//prints the hashies making the left side of pyramid
        }

        printf("  "); //prints the double space in between the left and right side of the pyramid

        for(int right = 2; right <= i + 1; right++){
            printf("#");//prints the hashes making the right side of the pyramid
        }

        printf("\n"); //prints next line
    }
}