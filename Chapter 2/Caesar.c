#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <cs50.h>

int main(int argc, string argv[]){
    if (argc != 2){
        printf("Usage: ./caesar <shift#>\n");
        return 1;
    }
    int change = atoi(argv[1]);
    string words = get_string("Plaintext: ");
    printf("ciphertext: ");
    for (int i = 0; i < strlen(words); i++){
        if isupper(words[i]){
            words[i] = (((words[i] - 64) + change) % 26) + 64;
            printf("%c", words[i]);
        }
        else if islower (words[i]){
            words[i] = (((words[i] - 96) + change) % 26) + 96;
            printf("%c", words[i]);
        }
        else{
            printf("%c", words[i]);
        }
    }
    printf("\n");
}