/*
 * Sudoku Solver to solve a 9x9 sudoku grid using brute force backtracking
 * algorithm.
 * Author: Karthik Raj
 * Date: 08-18-2013
 */

#include<stdio.h>
#include <stdbool.h>

/*
 * Grid params
 */
#define UNASSIGNED 0
#define num_rows 9
#define num_cols 9
int num_boxes_along_row = 3;
int num_boxes_along_col = 3;
int num_rows_in_box;
int num_cols_in_box;

void print_board(int grid[][num_cols])
{
    int row;
    int col;
    printf("\n------------------------------------------------------\n");
    for (row = 0; row < num_rows; row++) {
        for (col = 0; col < num_cols; col++) {
            printf("  %d  |", grid[row][col]);
        }
        printf("\n------------------------------------------------------\n");
    }
}

/*
 * Find the next available cell in the grid i.e the next cell which is still
 * UNASSIGNED.
 * returns true if an unassigned cell is found, else returns false
 */
bool find_next_unassigned_loc(int grid[][num_cols], int *row, int *col)
{
    int i;
    int j;
    
    for (i = 0; i < num_rows; i++) {
        for (j = 0; j < num_cols; j++) {
            if(grid[i][j] == UNASSIGNED) {
                *row = i;
                *col = j;
                return true;
            }
        }
    }
    return false;
}

bool no_conflicts_along_row(int grid[][num_cols], int row, int num)
{
    int i;
    int j;
    
    for(i = 0; i < num_cols; i++) {
        if(grid[row][i] != UNASSIGNED) {
            if(grid[row][i] == UNASSIGNED) {
                continue;
            }
            if(grid[row][i] == num) {
                //printf("Conflict along row: %d\n", row);
                return false;
            } 
        }
    }
    return true;
}

bool no_conflicts_along_col(int grid[][num_cols], int col, int num)
{
    int i;
    int j;
    int found[9];
    memset(found, 0, sizeof(found));
    
    for(i = 0; i < num_rows; i++) {
        if(grid[i][col] != UNASSIGNED) {
            if(grid[i][col] == UNASSIGNED) {
                continue;
            }
            if(grid[i][col] == num) {
                //printf("Conflict along col: %d\n", col);
                return false;
            }
        }
    }
    return true;
}

bool no_conflicts_in_block(int grid[][num_cols], int row, int col, int num)
{
    int i;
    int j;
    int box_row_index;
    int box_col_index;
    int icount = 0;
    int jcount = 0;
    
    box_row_index = (row / num_boxes_along_row) * num_rows_in_box;
    box_col_index = (col / num_boxes_along_col) * num_cols_in_box;
    
    for(i = box_row_index, icount = 0; icount < num_rows_in_box; i++, icount++) {
        for(j = box_col_index, jcount = 0; jcount < num_cols_in_box; j++, jcount++) {
            if(grid[i][j] == UNASSIGNED) {
                continue;
            }
            //printf("cell[%d][%d] = %d\n", i, j, grid[i][j]);
            if(grid[i][j] == num) {
                //printf("Conflict in block[%d][%d]\n", box_row_index, box_col_index);
                return false;
            }
        }
    }
    return true;
}

bool no_conflicts_found(int grid[][num_cols], int row, int col, int num)
{
    return (no_conflicts_along_row(grid, row, num) &&
            no_conflicts_along_col(grid, col, num) &&
            no_conflicts_in_block(grid, row, col, num));
}

bool is_valid_sudoku(int grid[][num_cols])
{
    int i;
    int j;
    static bool rows[num_rows][num_cols];
    static bool cols[num_rows][num_cols];
    static bool blocks[num_rows][num_cols];
    
    for (i = 0; i < num_rows; i++) {
        for (j = 0; j < num_cols; j++) {
            if (grid[i][j] == UNASSIGNED) {
                continue;
            }
            int pos = grid[i][j] - 1;
            if (rows[i][pos] || cols[j][pos] || blocks[i - i % 3 + j / 3][pos]) {
                return false;
            }
            rows[i][pos] = cols[j][pos] = blocks[i - i % 3 + j / 3][pos] = true;
        }
    }
    return true;
}

bool solve_sudoku(int grid[][num_cols])
{
    int row;
    int col;
    int num;
    
    /*
     * If there are no more unassigned cells then the grid is solved!
     */
    if(!find_next_unassigned_loc(grid, &row, &col)) {
        print_board(grid);
        return true;
    }

    /*
     * try to place numbers 1-9 for a given cell identified by row and col
     */
    for(num = 1; num <= 9; num++) {
        /*
         * If the number does not conflict with the given constraints for the
         * current state of the grid assume it to be optimal and assign the 
         * number to the cell
         */
        if(no_conflicts_found(grid, row, col, num)) {
            grid[row][col] = num;
            /*
             * Solve the rest of the grid with the current cell assigned to num
             */
            if(solve_sudoku(grid)) {
                return true;
            }
            /*
             * If the grid is not solveable with the given choice of the number 
             * for the cell, try the next possible number
             */
            grid[row][col] = UNASSIGNED;
        } else {
            printf("cell[%d][%d] = %d conflicts criteria\n", row, col, num);
        }
    }
    /*
     * triggers backtracking for earlier decisions when a cell cannot be 
     * assigned any valid digit without violating the constraints
     */
    return false;
}

int main(int argc, char **argv)
{
    int grid[num_rows][num_cols] =
       {{0, 0, 3, 0, 0, 8, 9, 0, 0},
        {0, 0, 0, 9, 0, 0, 6, 0, 0},
        {0, 0, 8, 5, 0, 4, 0, 3, 0},
        {8, 0, 2, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 3, 0, 7, 0, 8, 0},
        {0, 0, 0, 0, 0, 0, 4, 0, 6},
        {0, 6, 0, 4, 0, 5, 1, 0, 0},
        {0, 0, 4, 0, 0, 3, 0, 0, 0},
        {0, 0, 9, 1, 0, 0, 3, 0, 0}};
    num_rows_in_box = num_rows/num_boxes_along_col;
    num_cols_in_box = num_cols/num_boxes_along_row;
    
    if(is_valid_sudoku(grid)) {
        solve_sudoku(grid);
    } else {
        printf("Input board is invalid\n");
    }
}
