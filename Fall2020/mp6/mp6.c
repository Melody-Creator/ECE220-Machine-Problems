/*									tab:8
 *
 * mp6.c - implementation file for ECE220 "Blocky" game
 *
 * "Copyright (c) 2017-2020 by Steven S. Lumetta."
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE AUTHOR OR THE UNIVERSITY OF ILLINOIS BE LIABLE TO 
 * ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL 
 * DAMAGES ARISING OUT  OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, 
 * EVEN IF THE AUTHOR AND/OR THE UNIVERSITY OF ILLINOIS HAS BEEN ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE AUTHOR AND THE UNIVERSITY OF ILLINOIS SPECIFICALLY DISCLAIM ANY 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE 
 * PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND NEITHER THE AUTHOR NOR
 * THE UNIVERSITY OF ILLINOIS HAS ANY OBLIGATION TO PROVIDE MAINTENANCE, 
 * SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Author:	    Steve Lumetta
 * Version:	    6
 * Creation Date:   7 November 2017
 * Filename:	    mp6.c
 * History:
 *	SL	1	7 November 2017
 *		First written.
 *      JXG     2	8 November 2017
 *		Added ncurses support.
 *	SL	3	30 November 2017
 *		Polished for MP distribution.
 *	SL	4	18 February 2018
 *		Edited slightly for ECE220H S18 distribution.
 *	SL	5	6 October 2020
 *		Edited slightly for ECE220 F20 distribution.
 *	SL	6	6 October 2020
 *		Stripped down from gold version.
 */


#include <stdint.h>
#include <stdio.h>

#include "mp6.h"


/* 
 * The maximum number of blocks in one piece.  Do not change for
 * this assignment (you can do as you like with the code afterward).
 */
#define MAX_PIECE_SIZE 4

/*
 * Pieces are defined by an array of offsets from the center point.
 * Each piece consists of a set of adjacent blocks that together form
 * the piece.  The first index of the array is the piece type, the 
 * second is the orientation (0-3), the third is block index within 
 * the piece (0 to MAX_PIECE_SIZE - 1), and the fourth gives the 
 * x (0) and y (1) offsets of the block relative to the center point.
 * Note that the first block is always the center point of the piece.
 * 
 * Offsets may be replicated if a piece contains fewer than MAX_PIECE_SIZE
 * blocks.
 *
 * Clockwise rotation changes the orientation in the positive (mod 4)
 * direction, and counter-clockwise rotation changes the orientation in
 * the negative (mod 4) direction.
 */
static const int32_t piece_def[NUM_PIECE_TYPES][4][MAX_PIECE_SIZE][2] = {
    {   /* PIECE_TYPE_ELL */
	{{0, 0}, { 1,  0}, { 2,  0}, { 0,  1}},
	{{0, 0}, { 0,  1}, { 0,  2}, {-1,  0}},
	{{0, 0}, {-1,  0}, {-2,  0}, { 0, -1}},
	{{0, 0}, { 0, -1}, { 0, -2}, { 1,  0}}
    },
    {   /* PIECE_TYPE_REV_ELL */
	{{0, 0}, { 1,  0}, { 2,  0}, { 0, -1}},
	{{0, 0}, { 0,  1}, { 0,  2}, { 1,  0}},
	{{0, 0}, {-1,  0}, {-2,  0}, { 0,  1}},
	{{0, 0}, { 0, -1}, { 0, -2}, {-1,  0}}
    },
    {   /* PIECE_TYPE_LINE */
	{{0, 0}, {-1,  0}, { 1,  0}, { 2,  0}},
	{{0, 0}, { 0, -1}, { 0,  1}, { 0,  2}},
	{{0, 0}, { 1,  0}, {-1,  0}, {-2,  0}},
	{{0, 0}, { 0,  1}, { 0, -1}, { 0, -2}}
    },
    {   /* PIECE_TYPE_TEE */
	{{0, 0}, {-1,  0}, { 0, -1}, { 1,  0}},
	{{0, 0}, { 0, -1}, { 1,  0}, { 0,  1}},
	{{0, 0}, { 1,  0}, { 0,  1}, {-1,  0}},
	{{0, 0}, { 0,  1}, {-1,  0}, { 0, -1}}
    },
    {   /* PIECE_TYPE_ESS */
	{{0, 0}, { 0, -1}, { 1,  0}, { 1,  1}},
	{{0, 0}, { 1,  0}, { 0,  1}, {-1,  1}},
	{{0, 0}, { 0,  1}, {-1,  0}, {-1, -1}},
	{{0, 0}, {-1,  0}, { 0, -1}, { 1, -1}}
    },
    {   /* PIECE_TYPE_REV_ESS */
	{{0, 0}, { 0, -1}, {-1,  0}, {-1,  1}},
	{{0, 0}, { 1,  0}, { 0, -1}, {-1, -1}},
	{{0, 0}, { 0,  1}, { 1,  0}, { 1, -1}},
	{{0, 0}, {-1,  0}, { 0,  1}, { 1,  1}}
    }
};


/* 
 * empty_board -- fills a board b with empty spaces (SPACE_EMPTY)
 * INPUTS: b -- the board
 * OUTPUTS: b -- the board, filled with empty spaces
 * RETURN VALUE: 1 on success, or 0 on failure
 * SIDE EFFECTS: none
 */
int32_t
empty_board (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH])
{
    for(int32_t i = 0; i < BOARD_HEIGHT; i++)
        for(int32_t j = 0; j < BOARD_WIDTH; j++)
            b[i][j] = SPACE_EMPTY;      //empty each space
    return 1;
    return 0;
}


/* 
 * mark_piece -- forcibly writes a piece into a board, ignoring the 
 *               board's current contents; you will need to understand
 *               the piece definition array (piece_def) in order to
 *               implement this function
 * INPUTS: b -- the board
 *         p -- the type of piece
 *         orient -- the orientation of the piece
 *         x -- horizontal coordinate of the piece's center point
 *         y -- vertical coordinate of the piece's center point
 *         v -- type of space to write (in the shape of the piece)
 * OUTPUTS: b -- the board, with the piece filled in with value v
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void
mark_piece (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], 
            piece_type_t p, int32_t orient, int32_t x, int32_t y,
	    space_type_t v)
{
    for(int32_t i = 0; i < 4; i++){
        int32_t nx = x + piece_def[p][orient][i][0];
        int32_t ny = y + piece_def[p][orient][i][1];    //get the new coordinate
        b[ny][nx] = v;
    }
}


/* 
 * test_piece_fit -- checks whether a piece fits into a board; you will 
 *		     need to understand the piece definition array 
 *		     (piece_def) in order to implement this function
 * INPUTS: b -- the board
 *         p -- the type of piece
 *         orient -- the orientation of the piece
 *         x -- horizontal coordinate of the piece's center point
 *         y -- vertical coordinate of the piece's center point
 * OUTPUTS: none
 * RETURN VALUE: if the piece does not fit within the board, returns 
 *		 FIT_OUT_OF_BOARD; otherwise, if the piece would overwrite 
 *		 non-empty board spaces, returns FIT_NO_ROOM_THERE; 
 *		 otherwise, returns FIT_SUCCESS
 * SIDE EFFECTS: none
 */
fit_result_t
test_piece_fit (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], 
                piece_type_t p, int32_t orient, int32_t x, int32_t y)
{
    int32_t flag = 0;
    for(int32_t i = 0; i < 4; i++){
        int32_t nx = x + piece_def[p][orient][i][0];
        int32_t ny = y + piece_def[p][orient][i][1];    //get the new coordinate
        if(ny < 0 || ny >= BOARD_HEIGHT || nx < 0 || nx >= BOARD_WIDTH){
            flag = 1;   //check if it's out of boundary
            break;
        }
        if(b[ny][nx] != SPACE_EMPTY){
            flag = -1;  //check if it's occupied by other pieces
            break;
        }
    }
    if(flag == 0)  return FIT_SUCCESS;
    if(flag == 1)  return FIT_OUT_OF_BOARD;
    return FIT_NO_ROOM_THERE;
}


/* 
 * print_board -- prints a board to the screen using period ('.') to 
 *		  represent empty spaces, percent ('%') to represent 
 *		  old blocks, and asterisk ('*') to represent the current 
 *		  piece (SPACE_BLOCK); see specification for example
 * 		  of board appearance
 * INPUTS: b -- the board
 * OUTPUTS: none
 * RETURN VALUE: none
 * SIDE EFFECTS: prints to the monitor
 */
void
print_board (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH])
{
    for(int32_t i = 0; i < BOARD_HEIGHT; i++){
        for(int32_t j = 0; j < BOARD_WIDTH; j++){
            if(b[i][j] == SPACE_EMPTY)  putchar('.');
            if(b[i][j] == SPACE_FULL)   putchar('%');
            if(b[i][j] == SPACE_BLOCK)  putchar('*');   //print the corresponding characters
        }
        puts("");   //newline
    }
}


/* 
 * try_to_move_down -- tries to move the current piece down one space in 
 *                     the board; assumes that the piece is in the board
 *		       and must thus be removed before it can be put into
 *                     its new position (or back into its old position
 *                     on failure)
 * INPUTS: b -- the board
 *         p -- the type of piece
 *         orient -- the current orientation of the piece
 *         x -- current horizontal coordinate of the piece's center point
 *         y -- current vertical coordinate of the piece's center point
 * OUTPUTS: b -- the board, with the piece moved down (on success), or
 *               unchanged on failure
 * RETURN VALUE: 1 on success, 0 on failure
 * SIDE EFFECTS: none
 */
int32_t
try_to_move_down (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], 
                  piece_type_t p, int32_t orient, int32_t x, int32_t y)
{
    int32_t flag;
    remove_piece(b, p, orient, x, y);
    if(test_piece_fit(b, p, orient, x, y + 1) == FIT_SUCCESS){
        add_piece(b, p, orient, x, y + 1);  //put into new position
        flag = 1;
    }
    else{
        add_piece(b, p, orient, x, y);  //put into old position
        flag = 0;
    }
    if(flag)  return 1;
    return 0;
}


/* 
 * try_to_move_left -- tries to move the current piece one space left in
 *                     the board; assumes that the piece is in the board
 *		       and must thus be removed before it can be put into
 *                     its new position (or back into its old position
 *                     on failure)
 * INPUTS: b -- the board
 *         p -- the type of piece
 *         orient -- the current orientation of the piece
 *         x -- current horizontal coordinate of the piece's center point
 *         y -- current vertical coordinate of the piece's center point
 * OUTPUTS: b -- the board, with the piece moved left (on success), or
 *               unchanged on failure
 * RETURN VALUE: 1 on success, 0 on failure
 * SIDE EFFECTS: none
 */
int32_t
try_to_move_left (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], 
                  piece_type_t p, int32_t orient, int32_t x, int32_t y)
{
    int32_t flag;
    remove_piece(b, p, orient, x, y);
    if(test_piece_fit(b, p, orient, x - 1, y) == FIT_SUCCESS){
        add_piece(b, p, orient, x - 1, y);  //put into new position
        flag = 1;
    }
    else{
        add_piece(b, p, orient, x, y);  //put into old position
        flag = 0;
    }
    if(flag)  return 1;
    return 0;
}


/* 
 * try_to_move_right -- tries to move the current piece one space right in
 *                     the board; assumes that the piece is in the board
 *		       and must thus be removed before it can be put into
 *                     its new position (or back into its old position
 *                     on failure)
 * INPUTS: b -- the board
 *         p -- the type of piece
 *         orient -- the current orientation of the piece
 *         x -- current horizontal coordinate of the piece's center point
 *         y -- current vertical coordinate of the piece's center point
 * OUTPUTS: b -- the board, with the piece moved right (on success), or
 *               unchanged on failure
 * RETURN VALUE: 1 on success, 0 on failure
 * SIDE EFFECTS: none
 */
int32_t
try_to_move_right (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], 
                   piece_type_t p, int32_t orient, int32_t x, int32_t y)
{
    int32_t flag;
    remove_piece(b, p, orient, x, y);
    if(test_piece_fit(b, p, orient, x + 1, y) == FIT_SUCCESS){
        add_piece(b, p, orient, x + 1, y);  //put into new position
        flag = 1;
    }
    else{
        add_piece(b, p, orient, x, y);  //put into old position
        flag = 0;
    }
    if(flag)  return 1;
    return 0;
}


/* 
 * try_to_rotate_clockwise -- 
 *	tries to rotate the current piece clockwise in the board; assumes 
 *	that the piece is in the board and must thus be removed before it 
 *	can be put into its new position (or back into its old position on
 *	failure)
 * INPUTS: b -- the board
 *         p -- the type of piece
 *         orient -- the current orientation of the piece
 *         x -- current horizontal coordinate of the piece's center point
 *         y -- current vertical coordinate of the piece's center point
 * OUTPUTS: b -- the board, with the piece rotated clockwise (on success), 
 *		 or unchanged on failure
 * RETURN VALUE: 1 on success, 0 on failure
 * SIDE EFFECTS: none
 */
int32_t
try_to_rotate_clockwise (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], 
                         piece_type_t p, int32_t orient, int32_t x, int32_t y)
{
    int32_t flag, n_orient = (orient + 1) % 4;  //positive direction (mod 4)
    remove_piece(b, p, orient, x, y);
    if(test_piece_fit(b, p, n_orient, x, y) == FIT_SUCCESS){
        add_piece(b, p, n_orient, x, y);    //put into new position
        flag = 1;
    }
    else{
        add_piece(b, p, orient, x, y);  //put into old position
        flag = 0;
    }
    if(flag)  return 1;
    return 0;
}


/* 
 * try_to_rotate_cc -- 
 *	tries to rotate the current piece counter-clockwise in the board; 
 *	assumes that the piece is in the board and must thus be removed 
 *	before it can be put into its new position (or back into its old 
 *	position on failure)
 * INPUTS: b -- the board
 *         p -- the type of piece
 *         orient -- the current orientation of the piece
 *         x -- current horizontal coordinate of the piece's center point
 *         y -- current vertical coordinate of the piece's center point
 * OUTPUTS: b -- the board, with the piece rotated counter-clockwise (on 
 *		 success), or unchanged on failure
 * RETURN VALUE: 1 on success, 0 on failure
 * SIDE EFFECTS: none
 */
int32_t
try_to_rotate_cc (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], 
                  piece_type_t p, int32_t orient, int32_t x, int32_t y)
{
    int32_t flag, n_orient = (orient - 1 + 4) % 4; //negative direction (mod 4)
    remove_piece(b, p, orient, x, y);
    if(test_piece_fit(b, p, n_orient, x, y) == FIT_SUCCESS){
        add_piece(b, p, n_orient, x, y);    //put into new position
        flag = 1;
    }
    else{
        add_piece(b, p, orient, x, y);  //put into old position
        flag = 0;
    }
    if(flag)  return 1;
    return 0;
}


/* 
 * is_row_full -- checks whether a row in a board is filled with SPACE_FULL 
 * INPUTS: b -- the board
 *	   row -- which row to check
 * OUTPUTS: none
 * RETURN VALUE: 1 if the specified row is full, or 0 otherwise
 * SIDE EFFECTS: none
 */
int32_t
is_row_full (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH], int row)
{
    int32_t flag = 1;
    for(int32_t i = 0; i < BOARD_WIDTH; i++)
        if(b[row][i] != SPACE_FULL){
            flag = 0;
            break;      //break when we know it's not a full row
        }
    if(flag)  return 1;
    return 0;
}


/* 
 * remove_full_rows -- removes all rows filled with SPACE_FULL from a 
 *                     board, moving rows above them downward and adding 
 *                     new empty rows at the top of the board
 * INPUTS: b -- the board
 * OUTPUTS: b -- the board, with full rows removed
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void
remove_full_rows (space_type_t b[BOARD_HEIGHT][BOARD_WIDTH])
{
    for(int32_t row = BOARD_HEIGHT-1; row >= 0; row--){ //enumerate the possible full row
        if(is_row_full(b, row) == 0)  continue; //if this row is not full, skip it
        for(int32_t i = row; i > 0; i--)
            for(int32_t j = 0; j < BOARD_WIDTH; j++)
                b[i][j] = b[i-1][j];    //copy the row above
        for(int32_t j = 0; j < BOARD_WIDTH; j++)
            b[0][j] = SPACE_EMPTY;  //empty the top row
        row ++;     //re-check the same row
    }
}

