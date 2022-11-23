#include <cs50.h>
#include <stdio.h>

int main(int argc, string argv[]){
    if(argc != 4){
        printf("Usage: ./Calc <first #> <operator> <second #>\n");
        return 1;
    }
    float first = atof(argv[1]);
    float second = atof(argv[3]);
    float third = 0;
    float mod = 0;
    switch(argv[2][0]){
        case '+':
        first = first + second;
        printf("%f\n", first);
        break;

        case '-':
        first = first - second;
        printf("%f\n", first);
        break;

        case 'x':
        first = first * second;
        printf("%f\n", first);
        break;

        case '/':
        first = first / second;
        printf("%f\n", first);
        break;

        case '%':
        third = (int) (first / second);
        mod = first - (second * third);
        printf("%f\n", mod);
        break;

        default:;
        return 1;
    }
}