#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void){
    int count = 0;
    float cash = get_float ("O hai! How much change is owed? ");
    while (cash < 0.0){
    cash = get_float ("O hai! How much change is owed? ");
    }
    cash = cash * 100.0;
    cash = roundf(cash);
    while (cash > 0) {

         if(cash >= 25){
        cash = cash - 25.0;
        count = count + 1;

          }else if (cash >= 10.0) {
        cash = cash - 10.0;
        count = count + 1;

            }else if (cash >= 5.0) {
        cash = cash - 5.0;
        count = count + 1;

              }else if (cash >= 1.0) {
        cash = cash - 1.0;
        count = count + 1;
                }
    }
        printf("%d\n", count);
}
