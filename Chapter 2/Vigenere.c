#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char *argv[]){
    //checks # of arguments
    if (argc != 2){
        printf("Usage: ./vigenere key");
        return 1;
    }

    string key = (argv[1]);
    int length = strlen(key);
    int letter = 0;

    for (int i = 0; i < length; i++){
        if(!isalpha(key[i])){
            printf("Key can only be characters!\n");
            return 1;
        }
    }

   // gets Plaintext and prints ciphertext
    string words = get_string("Plaintext: ");
    printf("ciphertext: ");

    //main encodes plaintext
    for(int i = 0; i < strlen(words); i++){
        if isupper(words[i]){
            words[i] = (words[i] - 'A' + toupper(key[letter]) - 'A') % 26 + 'A';
            printf("%c", words[i]);
            letter = (letter + 1) % length;
        }

        else if(islower(words[i])){
            words[i] = (words[i] - 'a' + toupper(key[counter]) - 'A') % 26 + 'a';
            printf("%c", words[i]);
            letter = (letter + 1) % length;
        }

        else{
            printf("%c", words[i]);
        }
    }
    printf("\n");
}