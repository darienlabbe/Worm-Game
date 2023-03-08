/*******************************************************************************
* SCREEN CLASS DECLARATION                                                     *
*                                                                              *
* Project: Worm Project                                                        *
* Name: Darien Labbe                                                           *
* Date: 2 November 2022                                                        *
*                                                                              *
*******************************************************************************/

#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <vector>
#include "Worm.hpp"

class Screen {
    public:
        // Default Constructor
        Screen();

        // Parameterized Constructor
        Screen(int rowC, int newCol);

        // Create worm at center of screen
        void initializeWorm();

        // Make cellToBeFree empty
        void makeFree(Cell cellToBeFree, bool initialize);

        // Make cellToBeOccupied occupied
        void occupyScreen(Cell cellToBeOccupied, bool isWorm);

        // Checks if cellToCheck is free, returns true if it is and false if
        // occupied
        bool isFree(Cell cellToCheck);

        // Randomly selects placement for snack
        Cell getRandomFreeCell();

        // Calculate the center of the screen
        Cell calculateCenter();

        // Displays the title, score, game board, and worm using curses
        void displayStartOfGame(int score);

        // If the coordinates (x,y) are at the head of the worm then return
        // true, otherwise return false
        bool atHead(int x, int y);

        // Return the cell of the head
        Cell headPosition();

        // Displays the worm
        void displayWorm();

        // Gets a random cell, occupies it, and returns it
        Cell placeFood();

        // Moves the worm up
        void moveUp(bool growing);

        // Moves the worm down
        void moveDown(bool growing);

        // Moves the worm right
        void moveRight(bool growing);

        // Moves the worm left
        void moveLeft(bool growing);

        // Returns true if the neighboring cell in the nextPos direction is a
        // snack and false if it is not
        bool foodFound(Direction nextPos);

        // Returns true if the cell the worms head is going to go into next is
        // a wall, or itself and returns false if not
        bool collision(Direction nextPos);

    private:
        // Total number of rows and columns
        int rowCount, columnCount;
        unsigned long freePoolSize;

        // The worm for the screen
        Worm worm;

        // 2D vector for screen and positioning
        std::vector<std::vector<int>> playingField;

        // 1D vector for list of available free cells
        std::vector<Cell> freePool;
};

#endif
