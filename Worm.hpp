/*******************************************************************************
* WORM CLASS DECLARATION                                                       *
*                                                                              *
* Project: Worm Project                                                        *
* Name: Darien Labbe                                                           *
* Date: 2 November 2022                                                        *
*                                                                              *
*******************************************************************************/

#ifndef WORM_HPP
#define WORM_HPP
#include <iostream>
#include "Cell.hpp"

class Worm {
    public:
        // Default Constructor
        Worm();

        // Default Destructor
        ~Worm();

        // Insert the specified int at headPos of queue unless it's full
        void push_head(Cell cellToBeAdded);

        // Delete and return the last element, exit with error if empty
        void pop_tail();

        // Return the cell representing the position of the worm’s head without modifying the worm
        Cell headPos();

        // Return the cell representing the position of the worm’s tail without modifying the worm
        Cell tailPos();

        // Return the number of elements
        int NumElements() { return size; }

        // Return true if and only if queue is empty
        bool empty() { return (size == 0); }

        // Return true if and only if queue is full
        bool full() { return (size == MAX_ROWS * MAX_COLS); }

        // Returns the cell of the next segment of worm
        Cell nextWormSegment(int j);

    private:
        // Exits the queue with error message if empty
        void exitIfEmpty() {
            if(empty()) {
                std::cerr << "Can't do this operation on an empty queue\n";
                exit(1);
            }
        }

        // Keeps track of queue size
        int size;

        // Node class for each node in the linked list
        class LLNode {
            public:
                //Value of cell at node
                Cell cellValue;

                //Pointer to next node
                LLNode* next;

                // Default and parameterized constructors
                LLNode() { next = nullptr; }
                LLNode(Cell cellData) {
                    cellValue.setRowCol(cellData.getRow(), cellData.getCol());
                    next = nullptr;
                }
        };

        //Instance of a head pointer to head of worm and tail pointer to tail of worm
        LLNode *head, *tail;
};

#endif
