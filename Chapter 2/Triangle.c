#include <cs50.h>
#include <stdio.h>

int main (void){

     int height = get_int("Height:");

    int j = 0;
    for(int i = 1; i <= height; i++){
        for(int space = height - i; space > 0; space--){
            printf(" ");
        }
        for(int hash = 0; hash < i; hash++){
            printf("*");
        }

         for(int hash2 = 0; hash2 < j; hash2++){
            printf("*");
        }
        j = j + 1;
        printf("\n");
    }

    int j2 = 0;

    for(int i2 = 1; i2 <= height; i2++){
        for(int space2 = height; space2 < i2 + height; space2++){
            printf(" ");
        }
        for(int hash2 = height; hash2 > i2; hash2--){
            printf("*");
        }

         for(int hash3 = height - 2; hash3 > j2; hash3--){
            printf("*");
        }
        j2 = j2 + 1;
        printf("\n");
    }
}
