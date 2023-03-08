/*******************************************************************************
* CELL CLASS DECLARATION                                                       *
*                                                                              *
* Project: Worm Project                                                        *
* Name: Darien Labbe                                                           *
* Date: 2 November 2022                                                        *
*                                                                              *
*******************************************************************************/

#ifndef CELL_HPP
#define CELL_HPP
#include <iostream> // For output operator
#include <sstream>
#include "Constants.hpp"

class Cell{
    public:
        // Default Constructor
        // Sets the row and col values to 0 for a default Cell data type.
        Cell();

        // Copy Constructor
        // Sets the row to other.row and col to other.col.
        Cell(const Cell &other);

        // Parameterized Constructor
        // Sets the row to inputRow and col to inputCol.
        Cell(int inputRow, int inputCol);

        // Assignment Operator
        // Assigns the value of other.row to row and other.col to col.
        void operator = (const Cell other);

        // Sets row to otherRow and col to otherCol. Function used to reassign
        // new values to row and col.
        void setRowCol(int otherRow, int otherCol);

        // Returns the row int value of a provided cell.
	    int getRow();

        // Returns the col int value of a provided cell.
	    int getCol();

        // Function finds the cell that is neighbors to the provided cell, and
        // it selects which neighbor to return based on the enum direction
        // variable passed as an argument.
	    Cell neighbor (Direction direction);

        // This function overloads the output operator.
        friend std::ostream& operator << (std::ostream& outs, const Cell& cell);

    private:
        // Represent the value of the row and column of a cell
        int row, col;
};

#endif
