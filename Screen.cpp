/*******************************************************************************
* SCREEN CLASS IMPLEMENTATION                                                  *
*                                                                              *
* Project: Worm Project                                                        *
* Name: Darien Labbe                                                           *
* Date: 2 November 2022                                                        *
*                                                                              *
*******************************************************************************/

#include <cstdlib>
#include <curses.h>
#include <ctime>
#include "Screen.hpp"
using namespace std;

// Function: Default Constructor
// Input: None
// Output: None
// Description: Creates a default screen with minimum dimensions.
Screen::Screen() {
    int screenNum = 0;
    freePoolSize = rowCount = MIN_ROWS + 1;
    columnCount = MIN_COLS + 1;

    // Initialize the screen and the free pool
    for (int i = 0; i <= rowCount; i++) {
        vector<int> row;
        for (int j = 0; j <= columnCount; j++) {
            if (i == 0 || i == rowCount || j == 0 || j == columnCount) // If boarder, insert -1
                row.push_back(-1);
            else {
                row.push_back(screenNum++);
                Cell screenCell(i, j);
                makeFree(screenCell, true);
            }
        }playingField.push_back(row); //Push back a completed row
    }
    freePoolSize = freePool.size();
    initializeWorm(); // Initialize worm
}

// Function: Parameterized Constructor
// Input: newRow, newCol
// Output: None
// Description: Creates a screen with the dimensions passed as parameters.
Screen::Screen(int newRow, int newCol) {
    int screenNum = 0;
    rowCount = newRow;
    columnCount = newCol;

    // Initialize the screen and the free pool
    for (int i = 0; i <= rowCount + 1; i++) {
        vector<int> row;
        for (int j = 0; j <= columnCount + 1; j++) {
            if (i == 0 || i == rowCount + 1 || j == 0 || j == columnCount + 1) // If boarder, insert -1
                row.push_back(-1);
            else {
                row.push_back(screenNum++);
                Cell screenCell(i, j);
                makeFree(screenCell, true);
            }
        }playingField.push_back(row); //Push back a completed row
    }
    freePoolSize = freePool.size();
    initializeWorm(); // Initialize worm
}

// Function: initializeWorm
// Input: None
// Output: None
// Description: Creates the worm in the screen 2D vector.
void Screen::initializeWorm() {
    Cell wormPlacementCell = calculateCenter();
    wormPlacementCell.setRowCol(calculateCenter().getRow(), calculateCenter().getCol());
    for (int i = 0; i < 8; i++) {
        occupyScreen(wormPlacementCell, true);
        worm.push_head(wormPlacementCell);
        wormPlacementCell = wormPlacementCell.neighbor(RIGHT);
    }
}

// Function: makeFree
// Input: cellToBeFree, initialize
// Output: None
// Description: Makes the cellToBeFree empty. Creates the vector by pushing
//      back new values if initializing.
void Screen::makeFree(Cell cellToBeFree, bool initialize) {
    if (initialize) {
        freePool.push_back(cellToBeFree);
        freePoolSize++;
    }else {
        playingField[cellToBeFree.getRow()][cellToBeFree.getCol()] = freePoolSize++;
        freePool[playingField[cellToBeFree.getRow()][cellToBeFree.getCol()]] = cellToBeFree;
    }
}

// Function: placeFood
// Input: cellToBeFree, initialize
// Output: foodCell
// Description: Gets a random cell to place the food, occupies that cell and
//      returns it.
Cell Screen::placeFood() {
    Cell foodCell = getRandomFreeCell();
    occupyScreen(foodCell, false);
    return foodCell;
}

// Function: occupyScreen
// Input: cellToBeOccupied, isWorm
// Output: None
// Description: Makes the cellToBeOccupied occupied by either the worm or food
//      depending on if isWorm is true or not.
void Screen::occupyScreen(Cell cellToBeOccupied, bool isWorm){
    int count = 1;
    freePool[playingField[cellToBeOccupied.getRow()][cellToBeOccupied.getCol()]] = freePool[freePoolSize - 1];
    while (playingField[freePool[freePoolSize - count].getRow()][freePool[freePoolSize - count].getCol()] <= -2) count++;
        playingField[freePool[freePoolSize - count].getRow()][freePool[freePoolSize - count].getCol()] = playingField[cellToBeOccupied.getRow()][cellToBeOccupied.getCol()];
    if (isWorm) playingField[cellToBeOccupied.getRow()][cellToBeOccupied.getCol()] = -2;
    else playingField[cellToBeOccupied.getRow()][cellToBeOccupied.getCol()] = -3; // Food
    freePoolSize--;
}

// Function: isFree
// Input: cellToCheck
// Output: true or false
// Description: Checks if the cellToCheck is free or not.
bool Screen::isFree(Cell cellToCheck) {
    return playingField[cellToCheck.getRow()][cellToCheck.getCol()] >= 0;
}

// Function: getRandomFreeCell
// Input: None
// Output: None
// Description: Using srand and rand, function randomly selects placement for
//      snack.
Cell Screen::getRandomFreeCell() {
    srand(time(nullptr));
    Cell randomCell(freePool[(rand() % freePoolSize) + 1]);

    return randomCell;
}

// Function: calculateCenter
// Input: None
// Output: None
// Description: Function calculates the center of the screen.
Cell Screen::calculateCenter() {
    Cell centerCell(rowCount / 2,1); // If smaller screen worm doesn't fit so make col 1
    
    //If larger screen
    if (columnCount > 17) centerCell.setRowCol(rowCount / 2, columnCount / 2);

    return centerCell;
}

// Function: displayStartOfGame
// Input: score
// Output: None
// Description: Function uses curses to display the title, score, and worm.
void Screen::displayStartOfGame(int score) {
    // Display initial title and score
    if (columnCount <= 17) {
        mvaddstr(0, 0, "Worm");
        mvaddstr(0, 5, "Score: ");
    }
    else {
        mvaddstr(0, 3, "Worm");
        mvaddstr(0, columnCount - 8, "Score: ");
    }addch((char)score + '0');

    // Display Border
    move(1, 0);
    for (int i = 0; i <= rowCount + 1; i++) {
        for (int j = 0; j <= columnCount + 1; j++) {
            if (playingField[i][j] == -1) mvaddch(i + 1, j,'*');
        }
    }
    // Display worm
    displayWorm();
    refresh();
}

// Function: displayWorm
// Input: None
// Output: None
// Description: Function displays the worm using curses.
void Screen::displayWorm() {
    int count = 0;
    Cell wormSegment = worm.tailPos();
    for (int i = 0; i < worm.NumElements(); i++) {
        if (atHead(wormSegment.getRow(), wormSegment.getCol()))
            mvaddch(wormSegment.getRow() + 1, wormSegment.getCol(), '@');
        else if (playingField[wormSegment.getRow()][wormSegment.getCol()] == -2) {
            mvaddch(wormSegment.getRow() + 1, wormSegment.getCol(), 'o');
            wormSegment = worm.nextWormSegment(count++);
        }
    }
}

// Function: atHead
// Input: y, x
// Output: true or false
// Description: Checks if the coordinates (x, y) are at the head of the worm.
bool Screen::atHead(int y, int x) {
    return worm.headPos().getRow() == y && worm.headPos().getCol() == x;
}

// Function: headPosition
// Input: None
// Output: None
// Description: Function returns the cell that the head is in.
Cell Screen::headPosition() {
    return worm.headPos();
}

// Function: moveUp
// Input: growing
// Output: None
// Description: Function moves the worm head up one and if it is not growing,
//      the tail is popped in order to move the worm.
void Screen::moveUp(bool growing) {
    Cell newHead(worm.headPos().getRow() - 1, worm.headPos().getCol());
    worm.push_head(newHead);
    occupyScreen(newHead, true);
    if(!growing) {
        makeFree(worm.tailPos(), false);
        mvaddch(worm.tailPos().getRow() + 1, worm.tailPos().getCol(), ' ');
        worm.pop_tail();
    }
}

// Function: moveDown
// Input: growing
// Output: None
// Description: Function moves the worm head down one and if it is not growing,
//      the tail is popped in order to move the worm.
void Screen::moveDown(bool growing) {
    Cell newHead(worm.headPos().getRow() + 1, worm.headPos().getCol());
    worm.push_head(newHead);
    occupyScreen(newHead, true);
    if(!growing) {
        makeFree(worm.tailPos(), false);
        mvaddch(worm.tailPos().getRow() + 1, worm.tailPos().getCol(), ' ');
        worm.pop_tail();
    }
}

// Function: moveRight
// Input: growing
// Output: None
// Description: Function moves the worm head right one and if it is not growing,
//      the tail is popped in order to move the worm.
void Screen::moveRight(bool growing) {
    Cell newHead(worm.headPos().getRow(), worm.headPos().getCol() + 1);
    worm.push_head(newHead);
    occupyScreen(newHead, true);
    if(!growing) {
        makeFree(worm.tailPos(), false);
        mvaddch(worm.tailPos().getRow() + 1, worm.tailPos().getCol(), ' ');
        worm.pop_tail();
    }
}

// Function: moveLeft
// Input: growing
// Output: None
// Description: Function moves the worm head left one and if it is not growing,
//      the tail is popped in order to move the worm.
void Screen::moveLeft(bool growing) {
    Cell newHead(worm.headPos().getRow(), worm.headPos().getCol() - 1);
    worm.push_head(newHead);
    occupyScreen(newHead, true);
    if(!growing) {
        makeFree(worm.tailPos(), false);
        mvaddch(worm.tailPos().getRow() + 1, worm.tailPos().getCol(), ' ');
        worm.pop_tail();
    }
}

// Function: foodFound
// Input: nextPos
// Output: None
// Description: Function returns true if food exists in the neighbor in the
//      direction of nextPos to the head or the worm.
bool Screen::foodFound(Direction nextPos){
    return playingField[worm.headPos().neighbor(nextPos).getRow()][worm.headPos().neighbor(nextPos).getCol()] == -3;
}

// Function: collision
// Input: nextPos
// Output: None
// Description: Function returns true if the worm will go into a wall or itself
//       in the neighbor in the direction of nextPos to the head or the worm.
bool Screen::collision(Direction nextPos) {
    Cell temp = worm.headPos().neighbor(nextPos);
    if (playingField[temp.getRow()][temp.getCol()] == -1) return true; // If it runs into a wall
    if (playingField[temp.getRow()][temp.getCol()] == -2) return true; // If it runs into itself
    return false;
}
