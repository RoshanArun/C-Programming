#include <ctype.h>
#include <ncurses.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const int row = 6;
const int col = 7;
bool whoseTurn = false; //if whoseTurn is false: player1 turn else player 2 turn

struct
{
    //defines the connect4 board  //pointer = it points to a location in memory
    char* board[6][7];
}game;

struct Player{
    char* token;
};

struct Player player1;
struct Player player2;
char * barrier1 = "|   |";
char * barrier2 = "|   ";

//Prototypes
int askUserInput(void);
void init_gameBoard(void);
void draw_grid(void);
void token1(void);
void token2(void);
void bounds(void);
void check_rows(void);
bool check_columns(int val);
bool check_diagonals(int val);
void PlayGame(void);
void WinCheck(struct Player player, int playerRow, int playerCol);
int PlacePiece(struct Player player, int input);

int main(int argc, char *argv[]){

    //correct number of arguments
    if(argc != 2){
        fprintf(stderr, "Usage connect4 2player\n");
        return 1;
    }

    init_gameBoard();
    system("clear");
    draw_grid();
    PlayGame();
}


int askUserInput(){
   int input = 0;
   scanf("%d", &input);
   return input;
}

//initializes the game board
void init_gameBoard(void){
    player1.token = "| X ";
    player2.token = "| O ";

    for(int j = 0; j < row; j++){
        for(int k = 0; k < col; k++){
            if(k == 6){
                game.board[j][k] = "|   |";
            }
            else{
                 game.board[j][k] = "|   ";
            }
        }
    }
}

//draw the game board
void draw_grid(void){
    printf("+---+---+---+---+---+---+---+\n");
    for(int j = 0; j < row; j++){
        for(int k = 0; k < col; k++){
            printf("%s", game.board[j][k]);
        }
         printf("\n");
         printf("+---+---+---+---+---+---+---+\n");
    }
    printf("  1   2   3   4   5   6   7\n");
}

void PlayGame(void){
    while(true){
        if(!whoseTurn){ //player1 turn
            int playerCol = askUserInput();

            if(playerCol >= 0 && playerCol <= 7){
                playerCol = playerCol - 1;
                int playerRow = PlacePiece(player1, playerCol);

                if(playerRow == -1){
                    whoseTurn = false;
                }
                else{
                    WinCheck(player1, playerRow, playerCol);
                    whoseTurn = true;
                }
            }
            else{

              printf("%s", "Invalid horizontal bounds \n");
              whoseTurn = false;
            }
        }
        else{
            int playerCol = askUserInput();

            if(playerCol >= 0 && playerCol <= 7){
                playerCol = playerCol - 1;
                int playerRow = PlacePiece(player2, playerCol);

                if(playerRow == -1){
                    whoseTurn = true;
                }
                else{
                    WinCheck(player2, playerRow, playerCol);
                    whoseTurn = false;
                }
            }

            else{
                printf("%s", "Invalid horizontal bounds \n");
                whoseTurn = true;
            }
        }
    }
}

int PlacePiece(struct Player player, int playerCol){
     int playerRow = -1;
     for(int i = row; i >= 0; i--){
         if(game.board[i][playerCol] == barrier1 || game.board[i][playerCol] == barrier2)
         {
             playerRow = i;
             game.board[i][playerCol] = player.token;
             break;
         }
     }

    if(playerRow == -1){
        printf("%s", "Invalid vertical bounds \n");
        return playerRow;
    }
    system("clear");
    draw_grid();
    return playerRow;
}

//checks for win by row/col
void WinCheck(struct Player player, int playerRow, int playerCol){

    // col win function
    if(game.board[playerRow][playerCol] == player.token && game.board[playerRow + 1][playerCol] == player.token && game.board[playerRow + 2][playerCol] == player.token && game.board[playerRow + 3][playerCol] == player.token){
        printf("Winner:%s\n", player.token);
        exit(1);
    }

    //row win function
    if(game.board[playerRow][playerCol] == player.token && game.board[playerRow][playerCol + 1] == player.token && game.board[playerRow][playerCol + 2] == player.token && game.board[playerRow][playerCol + 3] == player.token){
        printf("Winner:%s\n", player.token);
        exit(2);
    }

    if(game.board[playerRow][playerCol] == player.token && game.board[playerRow][playerCol - 1] == player.token && game.board[playerRow][playerCol - 2] == player.token && game.board[playerRow][playerCol - 3] == player.token){
        printf("Winner:%s\n", player.token);
        exit(3);
    }

    if(game.board[playerRow][playerCol] == player.token && game.board[playerRow][playerCol + 1] == player.token && game.board[playerRow][playerCol + 2] == player.token && game.board[playerRow][playerCol - 1] == player.token){
        printf("Winner:%s\n", player.token);
        exit(4);
    }

    if(playerCol != 6){
        if(game.board[playerRow][playerCol] == player.token && game.board[playerRow][playerCol + 1] == player.token && game.board[playerRow][playerCol - 1] == player.token && game.board[playerRow][playerCol - 2] == player.token){
        printf("Winner:%s\n", player.token);
        exit(5);
        }
    }

    //left diagonal win
    while(game.board[playerRow + 1][playerCol + 1] == player.token){
        playerRow = playerRow + 1;
        playerCol = playerCol + 1;
    }

    if(game.board[playerRow][playerCol] == player.token && game.board[playerRow - 1][playerCol - 1] == player.token && game.board[playerRow - 2][playerCol - 2] == player.token && game.board[playerRow - 3][playerCol - 3] == player.token){
        printf("Winner:%s\n", player.token);
        exit(6);
    }

    //right diagonal win
    while(game.board[playerRow + 1][playerCol - 1] == player.token){
        playerRow = playerRow + 1;
        playerCol = playerCol - 1;
    }

    if(game.board[playerRow][playerCol] == player.token && game.board[playerRow - 1][playerCol + 1] == player.token && game.board[playerRow - 2][playerCol + 2] == player.token && game.board[playerRow - 3][playerCol + 3] == player.token){
        printf("Winner:%s\n", player.token);
        exit(7);
    }
}