/*******************************************************************************
* MAIN FUNCTION                                                                *
*                                                                              *
* Project: Worm Project                                                        *
* Name: Darien Labbe                                                           *
* Date: 2 November 2022                                                        *
*                                                                              *
*******************************************************************************/

#include <unistd.h>
#include <cstdlib>
#include <sys/time.h>
#include <csignal>
#include <curses.h>
#include <iostream>
#include "Screen.hpp"
using namespace std;

const int SEC_DELAY = 0;
const int USEC_DELAY = 800000;
static char lastc = ' ';

// Function Declarations
void startup();
void terminateScreen();
char get_char();
void die();
Direction inputDirection(Direction curDir, char c);

// Function: main
// Input: argc, argv[]
// Output: 0
// Description: This is the main function for the worm project.
int main(int argc, char *argv[]) {
    // If no input at argv then cerr and exit
    if (argc != 3) {
        cerr << "Not enough arguments for playing field dimensions" << endl;
        exit(1);
    }

    // Set size of play area based on the user input
    int rowCount = stoi(argv[1]);
    int columnCount = stoi(argv[2]);

    // Check that user input is within accepted dimensions
    if (rowCount > MAX_ROWS || rowCount < MIN_ROWS) {
        cerr << "Number of rows out of bounds" << endl;
        exit(1);
    }
    if (columnCount > MAX_COLS || columnCount < MIN_COLS) {
        cerr << "Number of columns out of bounds" << endl;
        exit(1);
    }

    // Create a screen
    Screen newGame(rowCount, columnCount);

    // Initialize variables
    srand(time(NULL));
    int foodValue = (rand() % ('9' - '1' + 1)) + '1'; // Random foodValue value
    int score = 0, amountLeftToGrow = 0; // Score starts at 0
    bool notDead = true, foodWasFound = false, isGrowing = false;
    Direction currentDirection;

    // Display initial information to begin game
    startup();
    newGame.displayStartOfGame(score);
    Cell foodPlacement = newGame.placeFood();
    mvaddch(foodPlacement.getRow() + 1, foodPlacement.getCol(), foodValue);
    refresh();

    // Get controls to start by waiting for input to begin
    currentDirection = inputDirection(currentDirection, getch());
    // Start Game Loop
    while(notDead) {
        // If next head position is foodValue then set true to foodFound and
        // isGrowing and make the position at the foodValue free
        if (newGame.foodFound(currentDirection)) {
            isGrowing = foodWasFound = true;
            newGame.makeFree(newGame.headPosition().neighbor(currentDirection), false);
        }

        // Using the current direction, detect if there was a collision, move the
        // worm and grow the worm if the worm needs to be grown
        switch (currentDirection) {
            case UP:
                if (newGame.collision(currentDirection)) notDead = false;
                else {
                    if(isGrowing || amountLeftToGrow > 0) {
                        newGame.moveUp(true);
                        amountLeftToGrow--;
                    }else newGame.moveUp(isGrowing);
                }break;
            case DOWN:
                if (newGame.collision(currentDirection)) notDead = false;
                else {
                    if(isGrowing || amountLeftToGrow > 0) {
                        newGame.moveDown(true);
                        amountLeftToGrow--;
                    }else newGame.moveDown(isGrowing);
                }break;
            case RIGHT:
                if (newGame.collision(currentDirection)) notDead = false;
                else {
                    if(isGrowing || amountLeftToGrow > 0) {
                        newGame.moveRight(true);
                        amountLeftToGrow--;
                    }else newGame.moveRight(isGrowing);
                }break;
            case LEFT:
                if (newGame.collision(currentDirection)) notDead = false;
                else {
                    if(isGrowing || amountLeftToGrow > 0) {
                        newGame.moveLeft(true);
                        amountLeftToGrow--;
                    }else newGame.moveLeft(isGrowing);
                }break;
            default:break;
        }
        // Update worm and refresh screen
        newGame.displayWorm();
        refresh();
        
        // If foodValue was found then add to score and make a new foodValue
        // and place
        if (foodWasFound) {
            score += foodValue - '0'; // Add to score
            amountLeftToGrow += foodValue - '0';
            // Replace score display
            if (columnCount <= 17) mvaddstr(0, 12, to_string(score).c_str());
            else mvaddstr(0, columnCount - 1, to_string(score).c_str());

            // Make new random foodValue at random free place and display on screen
            foodValue = (rand() % ('9' - '1' + 1)) + '1';
            foodPlacement = newGame.placeFood();
            mvaddch(foodPlacement.getRow() + 1, foodPlacement.getCol(), foodValue);
            isGrowing = foodWasFound = false;
            refresh();
        }
        // Get next control
        currentDirection = inputDirection(currentDirection, get_char());
    }

    // End curses
    terminateScreen();

    // Display end state game output
    cout << "The worm died since it ran into something!" << endl;
    cout << "Your final score was: " << score << endl;

    return 0;
}

void startup() {
     initscr();	    // Activate the curses.
     curs_set(0);   // Do not display the cursor.
     clear();	    // Clear the screen that curses provides.
     noecho();	    // Prevent the input chars to be echoed to the screen.
     cbreak();	    // Change the stty so that characters are delivered to the
	                // program as they are typed--no need to hit the return key!
}

void terminateScreen() {
     mvcur( 0, COLS - 1, LINES - 1, 0 );    // Move cursor
     clear();                               // Clear what curses displayed
     refresh();                             // Refresh to show clean
     endwin();                              // End curses use
}

Direction inputDirection(Direction curDir, char c) {
    Direction dir;
    switch (c) {
        case 'w':
            dir = UP;
            break;
        case 'a':
            dir = LEFT;
            break;
        case 's':
            dir = DOWN;
            break;
        case 'd':
            dir = RIGHT;
            break;
        default:
            dir = curDir;
            break;
    } return dir;
}

char get_char() {
    char input;
    struct timeval tvec;
    fd_set rfds;

    tvec.tv_sec = SEC_DELAY;
    tvec.tv_usec = USEC_DELAY;
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);

    int retval = select(1, &rfds, NULL, NULL, &tvec);

    if ( retval < 0 )
        die();

    if ( retval ) {
        if (read(0, &input, 1) < 0)
            die();
        lastc = input;
    }

    return( lastc );
}

void die() {
    signal( SIGINT, SIG_IGN );
    mvcur( 0, COLS - 1, LINES - 1, 0 );
    endwin();
    exit( 1 );
}
