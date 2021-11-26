#pragma once

#include "common.h"

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

#include "file.h"

/*
 * Each cell can either be 'unknown' (value == 0), or 'known' (value != 0).
 * For example, a cell which has the number 3 in it, will have 3 as its value.
 * Possible determines which are possible (assuming the cell is unknown).
 * NOTE: possible starts at zero, while the value starts at one; so to check
 * if 3 is possible, you check (cell.possible[3 - 1] == true).
*/
struct Cell {
	uint value;
	bool possible[9];
};

struct Board {
	// NOTE: Cells in the format of [y][x]
	struct Cell cell[9][9];
};

int				Cell_initialize(struct Cell* cell);

int				Board_initialize(struct Board* board);
void			Board_print(struct Board* board);
int				Board_solve(struct Board* board);
bool			Board_valid(struct Board* board);
void			Board_set_cell(struct Board* board, uint x, uint y, uint value);
int				Board_load_from_file(struct Board* board, const char* path);