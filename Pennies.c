#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void){
    int days = get_int ("Days in month:");
    bool x = true;

    while (x==true){
    if (days <= 31 && days >= 28){
        x=false;
        } else {
        days = get_int ("Days in month:");
        }
    }

    double firstpennies = get_double ("Pennies on first day:");
    double newpennies = 0;

    while (x==false){
        if(firstpennies<=30 && firstpennies >= 1){
            x = true;
        } else {
            firstpennies = get_int ("Pennies on first day:");
        }
    }

    for (int i = 0; i < days; i++){
        newpennies = newpennies + firstpennies;
        firstpennies = firstpennies * 2;
    }
    printf("$%.2f\n", newpennies/100.0);
}