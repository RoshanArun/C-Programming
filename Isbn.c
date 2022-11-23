#include <cs50.h>
#include <stdio.h>

int main(void){
    long isbn = get_long ("ISBN:"); //gets the isbn n# from user
    int final = 0;
    for(int i = 1; i <= 10; i++){
        int tenth = isbn % 10; //gets last number
        tenth = tenth * i; //Multiplies last number by i (i is increasing)
        isbn = isbn / 10; //get rid of last number
        final = final + tenth; //adds all the of last numbers
    }
     if(final % 11 == 0){  //if all last numbers added mod 11 equals 0 then its a valid isbn
        printf("YES\n");
     }
     else{ //if all last numbers added mod 11 does not equal 0, its a invalid isbn
        printf("NO\n");
    }
}