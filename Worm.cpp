/*******************************************************************************
* WORM CLASS IMPLEMENTATION                                                    *
*                                                                              *
* Project: Worm Project                                                        *
* Name: Darien Labbe                                                           *
* Date: 2 November 2022                                                        *
*                                                                              *
*******************************************************************************/

#include <cassert>
#include "Worm.hpp"

// Function: Default Constructor
// Input: None
// Output: None
// Description: Creates a default worm of size 0.
Worm::Worm() {
    head = tail = nullptr;
    size = 0;
}

// Function: Default Destructor
// Input: None
// Output: None
// Description: Destructs the linked list worm
Worm::~Worm() {
    LLNode* current = head;
    //LLNode* previous;
    while (current != nullptr) {
        LLNode* previous = current;
        current = current->next;
        delete previous;
    }
}

// Function: push_head
// Input: cellToBeAdded
// Output: None
// Description: If queue is full, return and don't continue, add item to queue,
//       adjust pointers accordingly.
void Worm::push_head(Cell cellToBeAdded) {
    // Check not full
    if (full()) return;

    // Allocate the new Node and check for false add
    LLNode* newNode = new LLNode(cellToBeAdded);
    //if (newNode == nullptr) return false;

    // Make the current tail element point to newNode
    if (head != nullptr) {
        head->next = newNode;
    }

    // Adjust head and tail as needed and increment size
    if (tail == nullptr) tail = head = newNode; // If empty add to both
    else head = newNode; // If not empty only add to head
    size++;
}

// Function: pop_tail
// Input: None
// Output: None
// Description: If empty exit, and assert that tail is not null, then make tail
//      next and delete the node that was tail.
void Worm::pop_tail() {
    exitIfEmpty(); // call parent function to exit w/ error from empty queue
    assert (tail != nullptr);

    LLNode* doomedNode = tail;
    tail = tail->next;
    size--;
    delete doomedNode;
}

// Function: headPos
// Input: None
// Output: None
// Description: Returns the cell at the head
Cell Worm::headPos() {
    // Check not empty
    if(empty()) {
        std::cerr << "Can't do this operation on an empty queue\n";
        exit(1);
    } return head->cellValue;
}

// Function: tailPos
// Input: None
// Output: None
// Description: Returns the cell at the tail
Cell Worm::tailPos() {
    // Check not empty
    if(empty()) {
        std::cerr << "Can't do this operation on an empty queue\n";
        exit(1);
    } return tail->cellValue;
}

// Function: nextWormSegment
// Input: listPlacement
// Output: None
// Description: Returns the cell of the next worm segment
Cell Worm::nextWormSegment(int listPlacement) {
    // Check not empty
    if(empty()) {
        std::cerr << "Can't do this operation on an empty queue\n";
        exit(1);
    }
    // Create the next node to be returned
    LLNode* nextNode = tail->next;

    // Loop until reached desired segment
    for (int i = 0; i < listPlacement; i++)
        nextNode = nextNode->next;
    return nextNode->cellValue;
}

