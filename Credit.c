#include <cs50.h>
#include <stdio.h>

int main(void){
    long number = get_long ("Number: ");
    int last = 0;
    int final = 0;
    int storing = 0;
    int firstodd = 0;
    int storing2 = 0;
    int holdslast = 0;
    for(int i = 0; i < number; i++){
        last = number % 10; //gets last number
        storing = storing + last; //adds last number to storing
        number = number / 10; //gets rid of last number of credit card
        last = number % 10; //gets last number
        if(number < 100){
            firstodd = number % 10; //if number is less than hundred, stores last number
            printf("num%d\n", firstodd);
        }

        last = last * 2; //multiplies last number by two
        holdslast = last; //holds the last number
        if(last >= 10){ //splits any number greater than 10 into two digits, then adds the two digits induvidually
            last = last % 10; //gets last number
            storing2 = storing2 + last; //stores last number
            last = holdslast; //resets last
            last = last / 10; //gets rid of last number
            last = last % 10; //gets last number
            last = last + storing2; //adds the stored and "last" number
        }

        number = number / 10; //divides number by ten, gets rid of last digit
        final = final + last + storing; //adds the split num, and the stored number
        last = 0; //resets last to 0
        storing = 0; //resets storing to 0
        if(number < 10){
            last = number % 10; //if number is less than ten, adds that number to the final
            final = final + last;
        }
    }
    if(final % 10 != 0){
        printf("INVALID\n"); //if final mod 10 doesnt equal 0, prints invalid

    }else if (firstodd == 5){
        printf("MASTERCARD\n"); //if last digit ends in 5, prints mastercard

    }else if (last == 3 && firstodd == 4){
        printf("AMEX\n"); //if last digits are 34, prints amex

    }else if (last == 3 && firstodd == 7){
        printf("AMEX\n"); //if last digits are 37, prints amex

    }else if (last == 4 || firstodd == 4){
        printf("VISA\n"); //if last number ends in 4, prints visa

    }else{
        printf("INVALID\n"); //if none of the other print statements are valid, print invalid
    }
}