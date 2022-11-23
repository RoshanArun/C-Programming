#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void){
    int i = 0;
    string s = get_string("");
    printf("%c", toupper (s[0]));
    for(i = 1; i < strlen(s); i++){
        if(s[i] == ' '){
            printf("%c", toupper (s[i + 1]));
        }
    }
    printf("\n");
}