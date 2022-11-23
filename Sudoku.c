// Implements the game of Sudoku

#include <ctype.h>
#include <ncurses.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sudoku.h"
#include <time.h>
#include <unistd.h>

// Macro for processing control characters
#define CTRL(x) ((x) & ~0140)

// Size of each int (in bytes) in *.bin files
#define INTSIZE 4

// Wrapper for our game's globals
struct
{
    // The current level
    char *level;

    // The game's board
    int board[9][9];

    // The game's original board
    int oboard[9][9];

    // The board's number
    int number;

    // The board's top-left coordinates
    int top, left;

    // The cursor's current location between (0,0) and (8,8)
    int y, x;
    //add a previous num previoius x and y... restore using backspace
} g;

// Prototypes
void draw_grid(void); //
void draw_borders(void); //
void draw_logo(void); //
void draw_numbers(void);
void hide_banner(void); //
bool load_board(void); //
void handle_signal(int signum);
void log_move(int ch);
void redraw_all(void); //
bool restart_game(void); //
void show_banner(char *b); //
void show_cursor(void);
void shutdown(void); //
bool startup(void); //

// Newly-added prototypes
bool check_ind_row(int val);
bool check_ind_col(int val);
bool check_ind_box(int val);
bool checkwinrows(void);
bool checkwincols(void);
bool checkwinboxes(void);
void update_numbers(int ch, bool zero);
void undo_move(void);

int main(int argc, char *argv[])
{
    // Ensure that number of arguments is as expected
    if (argc != 2 && argc != 3)
    {
        fprintf(stderr, "Usage: sudoku n00b|l33t [#]\n");
        return 1;
    }

    // Ensure that level is valid
    if (strcmp(argv[1], "debug") == 0)
    {
        g.level = "debug";
    }
    else if (strcmp(argv[1], "n00b") == 0)
    {
        g.level = "n00b";
    }
    else if (strcmp(argv[1], "l33t") == 0)
    {
        g.level = "l33t";
    }
    else
    {
        fprintf(stderr, "Usage: sudoku n00b|l33t [#]\n");
        return 2;
    }

    // n00b and l33t levels have 1024 boards; debug level has 9
    int max = (strcmp(g.level, "debug") == 0) ? 9 : 1024;

    // Ensure that #, if provided, is in [1, max]
    if (argc == 3)
    {
        // Ensure n is integral
        char c;
        if (sscanf(argv[2], " %d %c", &g.number, &c) != 1)
        {
            fprintf(stderr, "Usage: sudoku n00b|l33t [#]\n");
            return 3;
        }

        // Ensure n is in [1, max]
        if (g.number < 1 || g.number > max)
        {
            fprintf(stderr, "That board # does not exist!\n");
            return 4;
        }

        // Seed PRNG with # so that we get same sequence of boards
        srand(g.number);
    }
    else
    {
        // Seed PRNG with current time so that we get any sequence of boards
        srand(time(NULL));

        // Choose a random n in [1, max]
        g.number = rand() % max + 1;
    }

    // Start up ncurses
    if (!startup())
    {
        fprintf(stderr, "Error starting up ncurses!\n");
        return 5;
    }

    // Register handler for SIGWINCH (SIGnal WINdow CHanged)
    signal(SIGWINCH, (void (*)(int)) handle_signal);

    // Start the first game
    if (!restart_game())
    {
        shutdown();
        fprintf(stderr, "Could not load board from disk!\n");
        return 6;
    }
    redraw_all();
    // Let the user play
    int ch;
    int undo[3];
    do
    {
        // Refresh the screen
        refresh();

        // Get user's input
        ch = getch();

        // Capitalize input to simplify cases
        ch = toupper(ch);
        //reposition(ch);
        // Process user's input
        switch (ch)
        {
            // Start a new game
            case 'N':
                g.number = rand() % max + 1;
                if (!restart_game())
                {
                    shutdown();
                    fprintf(stderr, "Could not load board from disk!\n");
                    return 6;
                }
                break;

            // Restart current game
            case 'R':
                if (!restart_game())
                {
                    shutdown();
                    fprintf(stderr, "Could not load board from disk!\n");
                    return 6;
                }
                break;

            // Let user manually redraw screen with ctrl-L
            case CTRL('l'):
                redraw_all();
                break;

            case KEY_UP:
                if(g.y == 0){
                    g.y = 8;
                }
                else{
                    g.y = g.y - 1;
                }
                show_cursor();
            break;

            case KEY_DOWN:
                if(g.y == 8){
                    g.y = 0;
                }
                else{
                    g.y = g.y + 1;
                }
                show_cursor();
            break;

            case KEY_LEFT:
                if(g.x == 0){
                    g.x = 8;
                }
                else{
                    g.x = g.x - 1;
                }
                show_cursor();
            break;

            case KEY_RIGHT:
                if(g.x == 8){
                    g.x = 0;
                }
                else{
                    g.x = g.x + 1;
                }
                show_cursor();
            break;

            case KEY_BACKSPACE:
            case KEY_DC:
            case '0':
            case '.':
                if(g.oboard[g.y][g.x] == 0){
                    undo[0] = g.y;
                    undo[1] = g.x;
                    undo[2] = g.board[g.y][g.x];
                    g.board[g.y][g.x] = 0;
                    draw_numbers();
                    hide_banner();
                }
                show_cursor();
            break;

            case 'U':
                g.board[undo[0]][undo[1]] = undo[2];
                draw_numbers();
                hide_banner();
                show_cursor();
            break;

            case '1'...'9':
            if(g.oboard[g.y][g.x] == 0){
                undo[0] = g.y;
                undo[1] = g.x;
                undo[2] = g.board[g.y][g.x];
                update_numbers(ch, true);
                if(check_ind_row(ch) == false){
                    break;
                }
                if(check_ind_col(ch) == false){
                    break;
                }
                if(checkwinrows() == true && checkwincols() == true && checkwinboxes() == true){
                    hide_banner();
                    show_banner("Congratulations, You Won!!!");
                    show_cursor();
                    break;
                }
                check_ind_box(ch);
            break;
            }
        }

        // Log input (and board's state) if any was received this iteration
        // if (ch != ERR)
        //     log_move(ch);
    }
    while (ch != 'Q');
    // Shut down ncurses
    shutdown();

    // Tidy up the screen (using ANSI escape sequences)
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);

    // That's all folks
    printf("\nkthxbai!\n\n");
    return 0;
}


// Draw's the game's board
void draw_grid(void)
{
    // Get window's dimensions
    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);

    // Determine where top-left corner of board belongs
    g.top = maxy/2 - 7;
    g.left = maxx/2 - 30;

    // Enable color if possible
    if (has_colors())
        attron(COLOR_PAIR(PAIR_GRID));

    // Print grid
    for (int i = 0 ; i < 3 ; ++i )
    {
        mvaddstr(g.top + 0 + 4 * i, g.left, "+-------+-------+-------+");
        mvaddstr(g.top + 1 + 4 * i, g.left, "|       |       |       |");
        mvaddstr(g.top + 2 + 4 * i, g.left, "|       |       |       |");
        mvaddstr(g.top + 3 + 4 * i, g.left, "|       |       |       |");
    }
    mvaddstr(g.top + 4 * 3, g.left, "+-------+-------+-------+" );

    // Remind user of level and #
    char reminder[maxx+1];
    sprintf(reminder, "   playing %s #%d", g.level, g.number);
    mvaddstr(g.top + 14, g.left + 25 - strlen(reminder), reminder);

    // Disable color if possible
    if (has_colors())
    {
        attroff(COLOR_PAIR(PAIR_GRID));
    }
}


// Draws game's borders.
void draw_borders(void)
{
    // Get window's dimensions
    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);

    // Enable color if possible (else b&w highlighting)
    if (has_colors())
    {
        attron(A_PROTECT);
        attron(COLOR_PAIR(PAIR_BORDER));
    }
    else
    {
        attron(A_REVERSE);
    }

    // Draw borders
    for (int i = 0; i < maxx; i++)
    {
        mvaddch(0, i, ' ');
        mvaddch(maxy-1, i, ' ');
    }

    // Draw header
    char header[maxx+1];
    sprintf(header, "%s by %s", TITLE, AUTHOR);
    mvaddstr(0, (maxx - strlen(header)) / 2, header);

    // Draw footer
    mvaddstr(maxy-1, 1, "[N]ew Game   [R]estart Game");
    mvaddstr(maxy-1, maxx-28, "[U]ndo Button   [Q]uit Game");

    // Disable color if possible (else b&w highlighting)
    if (has_colors())
    {
        attroff(COLOR_PAIR(PAIR_BORDER));
    }
    else
    {
        attroff(A_REVERSE);
    }
}


// Draws game's logo (Must be called after draw_grid has been called at least once)
void draw_logo(void)
{
    // Determine top-left coordinates of logo
    int top = g.top + 2;
    int left = g.left + 30;

    // Enable color if possible
    if (has_colors())
    {
        attron(COLOR_PAIR(PAIR_LOGO));
    }

    // Draw logo

    // Sign logo
    char signature[3+strlen(AUTHOR)+1];
    sprintf(signature, "by %s", AUTHOR);
    mvaddstr(top + 7, left + 35 - strlen(signature) - 1, signature);

    // Disable color if possible
    if (has_colors())
    {
        attroff(COLOR_PAIR(PAIR_LOGO));
    }
}


//Draw's game's numbers (Must be called after draw_grid has been called at least once)

void draw_numbers(void)
{
    // Enable color if possible
    // Iterate over board's numbers
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            // Determine char
            if(has_colors()){
                attron(COLOR_PAIR(PAIR_DIGITS));
            }
            if(g.board[i][j] == g.oboard[i][j]){
                attron(COLOR_PAIR(PAIR_DIGITS));
            }
            if(g.board[i][j] != g.oboard[i][j]){
                attron(COLOR_PAIR(PAIR_NUMBERS));
            }
            if(checkwinrows() == true && checkwincols() == true && checkwinboxes() == true){
                attron(COLOR_PAIR(PAIR_WIN));
            }
            char c = (g.board[i][j] == 0) ? '.' : g.board[i][j] + '0';
            mvaddch(g.top + i + 1 + i/3, g.left + 2 + 2*(j + j/3), c);
            refresh();
        }
    }
    // Disable color if possible
    if (has_colors())
    {
        attroff(COLOR_PAIR(PAIR_BANNER));
    }
}


// Designed to handles signals (e.g., SIGWINCH)

void handle_signal(int signum)
{
    // Handle a change in the window (i.e., a resizing)
    if (signum == SIGWINCH)
    {
        redraw_all();
    }

    // Re-register myself so this signal gets handled in future too
    signal(signum, (void (*)(int)) handle_signal);
}


// Hides banner
void hide_banner(void)
{
    // Get window's dimensions
    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);

    // Overwrite banner with spaces
    for (int i = 0; i < maxx; i++)
    {
        mvaddch(g.top + 16, i, ' ');
    }
}


// Loads current board from disk, returning true iff successful
bool load_board(void)
{
    // Open file with boards of specified level
    char filename[strlen(g.level) + 5];
    sprintf(filename, "%s.bin", g.level);
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        return false;
    }

    // Determine file's size
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);

    // Ensure file is of expected size
    if (size % (81 * INTSIZE) != 0)
    {
        fclose(fp);
        return false;
    }

    // Compute offset of specified board
    int offset = ((g.number - 1) * 81 * INTSIZE);

    // Seek to specified board
    fseek(fp, offset, SEEK_SET);

    // Read board into memory
    if (fread(g.board, 81 * INTSIZE, 1, fp) != 1)
    {
        fclose(fp);
        return false;
    }
//load oboard with everything thats in gboard
    // w00t
    fclose(fp);

    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            g.oboard[i][j] = g.board[i][j];
        }
    }
    return true;
}

// Logs input and board's state to log.txt to facilitate automated tests
// void log_move(int ch)
// {
//     // Open log
//     FILE *fp = fopen("log.txt", "a");
//     if (fp == NULL)
//     {
//         return;
//     }

//     // Log input
//     fprintf(fp, "%d\n", ch);

//     // Log board
//     for (int i = 0; i < 9; i++)
//     {
//         for (int j = 0; j < 9; j++)
//         {
//             fprintf(fp, "%d", g.board[i][j]);
//         }
//         fprintf(fp, "\n");
//     }

//     // That's it
//     fclose(fp);
// }


// (Re)draws everything on the screen
void redraw_all(void)
{
    // Reset ncurses
    endwin();
    refresh();

    // Clear screen
    clear();

    // Re-draw everything
    draw_borders();
    draw_grid();
    draw_logo();
    draw_numbers();

    // Show cursor
    show_cursor();
}


// (Re)starts current game, returning true iff succesful
bool restart_game(void)
{
    // Reload current game
    if (!load_board())
    {
        return false;
    }

    // Redraw board
    draw_grid();
    draw_numbers();

    // Get window's dimensions
    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);

    // Move cursor to board's center
    g.y = g.x = 4;
    show_cursor();

    // Remove log, if any
    remove("log.txt");

    // w00t
    return true;
}


// Shows cursor at (g.y, g.x)
void show_cursor(void)
{
    // Restore cursor's location
    move(g.top + g.y + 1 + g.y/3, g.left + 2 + 2*(g.x + g.x/3));
}


// Shows a banner (Must be called after show_grid has been called at least once)

void show_banner(char *b)
{
    // Enable color if possible
    if (has_colors())
    {
        attron(COLOR_PAIR(PAIR_BANNER));
    }

    // Determine where top-left corner of board belongs
    mvaddstr(g.top + 16, g.left + 64 - strlen(b), b);

    // Disable color if possible
    if (has_colors())
    {
        attroff(COLOR_PAIR(PAIR_BANNER));
    }
}


// Shuts down ncurses
void shutdown(void)
{
    endwin();
}


// Starts up ncurses and returns true iff successful
bool startup(void)
{
    // Initialize ncurses
    if (initscr() == NULL)
    {
        return false;
    }

    // Prepare for color if possible
    if (has_colors())
    {
        // Enable color
        if (start_color() == ERR || attron(A_PROTECT) == ERR)
        {
            endwin();
            return false;
        }

        // Initialize pairs of colors
        if (init_pair(PAIR_BANNER, FG_BANNER, BG_BANNER) == ERR ||
            init_pair(PAIR_GRID, FG_GRID, BG_GRID) == ERR ||
            init_pair(PAIR_BORDER, FG_BORDER, BG_BORDER) == ERR ||
            init_pair(PAIR_LOGO, FG_LOGO, BG_LOGO) == ERR ||
            init_pair(PAIR_DIGITS, FG_DIGITS, BG_DIGITS) == ERR ||
            init_pair(PAIR_NUMBERS, FG_NUMBERS, BG_NUMBERS) == ERR ||
            init_pair(PAIR_WIN, FG_WIN, BG_WIN == ERR))
        {
            endwin();
            return false;
        }
    }

    // Don't echo keyboard input
    if (noecho() == ERR)
    {
        endwin();
        return false;
    }

    // Disable line buffering and certain signals
    if (raw() == ERR)
    {
        endwin();
        return false;
    }

    // Enable arrow keys
    if (keypad(stdscr, true) == ERR)
    {
        endwin();
        return false;
    }

    // Wait 1000 ms at a time for input
    timeout(1000);

    // w00t
    return true;
}

void update_numbers(int ch, bool zero){
    g.board[g.y][g.x] = ch - '0';
    draw_numbers();
    show_cursor();
}

bool check_ind_row(int val){
    for(int i = 0; i < 9; i++){
        if(g.board[g.y][i] == (val - '0') && i != g.x){
            hide_banner();
            show_banner("Row Check Failed");
            show_cursor();
            return false;
        }
    }
    hide_banner();
    show_cursor();
    return true;
}

bool check_ind_col(int val){
    for(int i = 0; i < 9; i++){
        if(g.board[i][g.x] == (val - '0') && i != g.y){
            hide_banner();
            show_banner("Column Check Failed");
            show_cursor();
            return false;
        }
    }
    hide_banner();
    show_cursor();
    return true;
}

bool check_ind_box(int val){
    int a = g.y;
    int b = g.x;
    int c = g.y % 3;
    int d = g.x % 3;
    for(int i = a; i < a + 3; i++){
        for(int j = b; j < b + 3; j++){
            if(g.board[i - c][j - d] == (val - '0') && i - c != g.y){
                hide_banner();
                show_banner("Box Check Failed");
                show_cursor();
                return false;
            }
        }
    }
    hide_banner();
    show_cursor();
    return true;
}

bool checkwinrows(){
    int score = 0;
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            score = score + g.board[i][j];
        }
         if(score == 45){
            score = 0;
        }
        else{
            return false;
        }
    }
    return true;
}

bool checkwincols(){
    int score = 0;
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            score = score + g.board[i][j];
        }
         if(score == 45){
            score = 0;
        }
        else{
            return false;
        }
    }
    return true;
}

bool checkwinboxes(){
    int score = 0;
    int a = 0;
    int b = 0;
    for(int l = 0; l < 3; l++){
        for(int k = 0; k < 3; k++){
            for(int i = a; i < a + 3; i++){
                for(int j = b; j < b + 3; j++){
                    score = score + g.board[i][j];
                }
            }
            if(score == 45){
                score = 0;
                a = a + 3;
            }
            else{
                return false;
            }
        }

        a = 0;
        b = b + 3;
    }
    return true;
}

//move cursor - done
//input numbers - done
//regulate which numbers can be input - done
//change colors of inputted numbers - done
//use backspace key/period/dc/0 to return to black space - done
//check ind row - done
//check ind column - done
//check ind boxes - done
//check all rows - done
//check all columns - done
//check all boxes - done
//check win - done
//change colors if won - done
//create an undo button - done