#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <crypt.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    if (argc != 2 ){
        printf("Usage: ./crack <hash>\n");
        return 1;
    }

    string all = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    bool found = false;

    for (int i = 0; i < strlen(all) && found == false; i++) {
        char tmpstr1[2] = "";
        tmpstr1[0] = all[i];
        tmpstr1[1] = '\0';
        string h1 = crypt(tmpstr1, "50");
        if (strcmp(h1, argv[1]) == 0) {
            printf("The decrypt string is: %s\n", tmpstr1);
            found = true;
        }
    }

    for (int i = 0; i < strlen(all) && found == false; i++) {
        for (int j = 0; j < strlen(all) && found == false; j++) {
            char tmpstr2[3] = "";
            tmpstr2[0] = all[i];
            tmpstr2[1] = all[j];
            tmpstr2[2] = '\0';
            string h2 = crypt(tmpstr2, "50");
            if (strcmp(h2, argv[1]) == 0) {
                printf("The decrypt string is: %s\n", tmpstr2);
                found = true;
            }
        }
    }

    for (int i = 0; i < strlen(all) && found == false; i++) {
        for (int j = 0; j < strlen(all) && found == false; j++) {
            for (int k = 0; k < strlen(all) && found == false; k++) {
                char tmpstr3[4] = "";
                tmpstr3[0] = all[i];
                tmpstr3[1] = all[j];
                tmpstr3[2] = all[k];
                tmpstr3[3] = '\0';
                string h3 = crypt(tmpstr3, "50");
                if (strcmp(h3, argv[1]) == 0) {
                    printf("The decrypt string is: %s\n", tmpstr3);
                    found = true;
                }
            }
        }
    }

    for (int i = 0; i < strlen(all) && found == false; i++) {
        for (int j = 0; j < strlen(all) && found == false; j++) {
            for (int k = 0; k < strlen(all) && found == false; k++) {
                for (int l = 0; l < strlen(all) && found == false; l++) {
                    char tmpstr4[5] = "";
                    tmpstr4[0] = all[i];
                    tmpstr4[1] = all[j];
                    tmpstr4[2] = all[k];
                    tmpstr4[3] = all[l];
                    tmpstr4[4] = '\0';
                    string h4 = crypt(tmpstr4, "50");
                    if (strcmp(h4, argv[1]) == 0) {
                        printf("The decrypt string is: %s\n", tmpstr4);
                        found = true;
                    }
                }
            }
        }
    }

    for (int i = 0; i < strlen(all) && found == false; i++) {
        for (int j = 0; j < strlen(all) && found == false; j++) {
            for (int k = 0; k < strlen(all) && found == false; k++) {
                for (int l = 0; l < strlen(all) && found == false; l++) {
                    for (int m = 0; m < strlen(all) && found == false; m++) {
                        char tmpstr5[6] = "";
                        tmpstr5[0] = all[i];
                        tmpstr5[1] = all[j];
                        tmpstr5[2] = all[k];
                        tmpstr5[3] = all[l];
                        tmpstr5[4] = all[m];
                        tmpstr5[5] = '\0';
                        string h5 = crypt(tmpstr5, "50");
                        if (strcmp(h5, argv[1]) == 0) {
                            printf("The decrypt string is: %s\n", tmpstr5);
                            found = true;
                        }
                    }
                }
            }
        }
    }

    if (found == false) {
        printf("Sorry...no match found.\n");
    }
}