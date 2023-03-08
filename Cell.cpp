/*******************************************************************************
* CELL CLASS IMPLEMENTATION                                                    *
*                                                                              *
* Project: Worm Project                                                        *
* Name: Darien Labbe                                                           *
* Date: 2 November 2022                                                        *
*                                                                              *
*******************************************************************************/

#include <iostream> // For output operator
#include "Cell.hpp"
using namespace std;

// Function: Default Constructor
// Input: None
// Output: None
// Description: Sets the row and col values to 0 for a default Cell data type.
Cell::Cell() {
    row = 0;
    col = 0;
}

// Function: Copy Constructor
// Input: other
// Output: None
// Description: Sets the row to other.row and col to other.col.
Cell::Cell(const Cell &other) {
    row = other.row;
    col = other.col;
}

// Function: Parameterized Constructor
// Input: inputRow, inputCol
// Output: None
// Description: Sets the row to inputRow and col to inputCol.
Cell::Cell(int inputRow, int inputCol) {
    row = inputRow;
    col = inputCol;
}

// Function: Assignment Operator
// Input: other
// Output: None
// Description: Assigns the value of other.row to row and other.col to col.
void Cell::operator = (const Cell other) {
    row = other.row;
    col = other.col;
}

// Function: setRowCol
// Input: otherRow, otherCol
// Output: None
// Description: Sets row to otherRow and col to otherCol. Function used to
//      reassign new values to row and col.
void Cell::setRowCol(int otherRow, int otherCol) {
    row = otherRow;
    col = otherCol;
}

// Function: getRow
// Input: None
// Output: row
// Description: Returns the row int value of a provided cell.
int Cell::getRow() {
    return row;
}

// Function: getCol
// Input: None
// Output: col
// Description: Returns the col int value of a provided cell.
int Cell::getCol() {
    return col;
}

// Function: neighbor
// Input: direction
// Output: neighborCell
// Description: Function finds the cell that is neighbors to the provided cell,
//      and it selects which neighbor to return based on the enum direction
//      variable passed as an argument.
Cell Cell::neighbor(Direction direction) {
    Cell neighborCell(row, col);
    switch (direction) {
        case UP:
            neighborCell.setRowCol(row - 1, col);
            return neighborCell;
        case DOWN:
            neighborCell.setRowCol(row + 1, col);
            return neighborCell;
        case LEFT:
            neighborCell.setRowCol(row, col - 1);
            return neighborCell;
        case RIGHT:
            neighborCell.setRowCol(row, col + 1);
            return neighborCell;
        case INVALID:
            neighborCell.setRowCol(0,0);
            break;
        default:
            std::cerr << "ERROR" << std::endl;
            neighborCell.setRowCol(0,0);
    } return neighborCell;
}

// Overloaded output operator
// Function: Output operator
// Input: outs, cellToBeOutput
// Output: Output stream
// Description: This function overloads the output operator.
std::ostream& operator << (std::ostream& outs, const Cell& cellToBeOutput) {
    return outs << '(' << cellToBeOutput.row << ", " << cellToBeOutput.col << ')';
}