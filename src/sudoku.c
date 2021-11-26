#include "sudoku.h"

// Check if end-of-something character (line or file)
// used in Board_load_from_file()
static inline bool ending_character(char c) {
	return (c == 0) || (c == '\n') || (c == '\r');
}

/* -------------------------------------------------------------------- */

int Cell_initialize(struct Cell* cell) {
	if(cell == NULL) {
		return 1;
	}
	
	cell->value = 0;
	for(uint i = 0; i < 9; i++) {
		cell->possible[i] = true;
	}
	
	return 0;
}

int Board_initialize(struct Board* board) {
	if(board == NULL) {
		return 1;
	}
	
	for(uint y = 0; y < 9; y++) {
		for(uint x = 0; x < 9; x++) {
			Cell_initialize(&(board->cell[y][x]));
		}
	}
	
	return 0;
}

int Board_solve(struct Board* board) {
	/*
	 * The 'easy' algorithm just goes through the board and finds cells with
	 * only one possible value. It then will collapse that cell (which will
	 * update surrounding cells). It will repeat this process until there are
	 * no cells with only one possible. This serves to prevent the need for
	 * un-nessessary recursion, and will save memory usage.
	*/
	bool found_easy;
	uint solved_count;
	do {
		found_easy = false;
		solved_count = 0;
		
		for(uint y = 0; y < 9; y++) {
			for(uint x = 0; x < 9; x++) {
				struct Cell* cell = &(board->cell[y][x]);
				
				if(cell->value == 0) {
					solved_count++;
					
					// 'Entropy' - The number of possible solutions
					// 'Last'    - The 'last' possible solution to the cell.
					//             It is used for setting the cell.
					uint entropy = 0, last;
					for(uint i = 0; i < 9; i++) {
						if(cell->possible[i]) {
							entropy++;
							last = i;
						}
					}
					
					// Check if cell is unsolved AND it has no possible
					// solutions. If so, discard the board.
					if(entropy == 0) {
						return 1; // Unsolvable
					}
					
					// Solve the cell with only one possibility.
					if(entropy == 1) {
						found_easy = true;
						
						Board_set_cell(board, x, y, last + 1);
					}
				}
			}
		}
	} while(found_easy);
	
	// Check if board is completed
	if(solved_count == 0) {
		return 0; // Solved
	}
	
	/*
	 * This is the 'harder' algorithm. All it does is find the cell with
	 * the lowest entropy, and recursively searches it.
	 *
	 * This doesn't all need to be in a loop, because if the cell has no
	 * solutions, then the board is invalid!
	*/
	
	// X, Y, and entropy of the cell with the lowest entropy
	uint low_x, low_y, low_ent = 10;
	for(uint y = 0; y < 9; y++) {
		for(uint x = 0; x < 9; x++) {
			struct Cell* cell = &(board->cell[y][x]);
			
			if(cell->value == 0) {
				uint entropy = 0, last;
				for(uint i = 0; i < 9; i++) {
					if(cell->possible[i]) {
						entropy++;
						last = i;
					}
				}
				
				// Assume everything has entropy >1, because we're
				// now using recursion
				
				if(entropy < low_ent) {
					low_x = x;
					low_y = y;
					low_ent = entropy;
				}
			}
		}
	}
	
	// Cell is the cell with the lowest entropy
	struct Cell* cell = &(board->cell[low_y][low_x]);
	
	for(uint i = 0; i < 9; i++) {
		// If i+1 is a possible value
		if(cell->possible[i]) {
			// Make a copy of the board
			struct Board child;
			memcpy(&child, board, sizeof(struct Board));

			// Make the change in the board
			Board_set_cell(&child, low_x, low_y, i + 1);
			
			// If the attempt is a valid solution
			if(Board_solve(&child) == 0) {
				// Write solution to the board passed in
				memcpy(board, &child, sizeof(struct Board));
				
				return 0; // Solved!!!
			}
		}
	}
	
	return 1; // Impossible Board
}

bool Board_valid(struct Board* board) {
	uint buf_a[10], buf_b[10];

	for(uint a = 0; a < 9; a++) {
		memset(buf_a, 0, 10 * sizeof(uint));
		memset(buf_b, 0, 10 * sizeof(uint));

		for(uint b = 0; b < 9; b++) {
			buf_a[board->cell[a][b].value]++;
			buf_b[board->cell[b][a].value]++;
		}

		for(uint b = 1; b <= 9; b++) {
			if(buf_a[b] > 1) {
				return false;
			}
			if(buf_b[b] > 1) {
				return false;
			}
		}
	}

	for(uint a = 0; a < 9; a += 3) {
		for(uint b = 0; b < 9; b += 3) {
			memset(buf_a, 0, 10 * sizeof(uint));

			for(uint c = 0; c < 3; c++) {
				for(uint d = 0; d < 3; d++) {
					buf_a[board->cell[a+c][b+d].value]++;
				}
			}

			for(uint c = 1; c < 10; c++) {
				if(buf_a[c] > 1) {
					return false;
				}
			}
		}
	}

	return true;
}

void Board_set_cell(struct Board* board, uint x, uint y, uint value) {
	struct Cell* target = &(board->cell[y][x]);
	target->value = value;
	
	// Worried these will be optimized out
	uint tx = (x / 3) * 3, ty = (y / 3) * 3;
	
	for(uint i = 0; i < 9; i++) {
		target->possible[i] = false;
		
		board->cell[i][x].possible[value - 1] = false; // Vertical
		board->cell[y][i].possible[value - 1] = false; // Horizontal
		board->cell[ty + i/3][tx + i%3].possible[value - 1] = false; // 3x3 Cell
	}
	target->possible[value - 1] = true; // Don't know if nessessary
}

void Board_print(struct Board* board) {
	printf(" ___________\n|"); // Top Bar
	for(uint y = 0; y < 9; y++) {
		for(uint x = 0; x < 9; x++) {
			uint value = board->cell[y][x].value;
			
			if(value != 0) {
				printf("%u", board->cell[y][x].value);
			} else {
				printf(" ");
			}
			
			if(((x + 1) % 3 == 0) && (x < 8)) {
				printf("|"); // Line between 3x3 cells (horizontal)
			}
		}
		
		if(((y + 1) % 3 == 0) && (y < 8)) {
			printf("|\n|---|---|---"); // Line between 3x3 cells (vertical)
		}
		printf("|\n|"); // End of 'wall', newline, and start of next
	}
	printf("___|___|___|\n"); // Bottom Bar
}

int Board_load_from_file(struct Board* board, const char* path) {
	int res;
	if((res = Board_initialize(board)) != 0) {
		return res;
	}
	
	// One-line ordering *technically* undefined behavior, so not doing it
	const char* file_data = read_file(path);
	if(file_data == NULL) {
		return 1;
	}
	
	const char* file_read = (char*)file_data;
	
	// Each 'line' is a pointer to the start of the first 9 lines in the file
	const char* lines[9];
	
	for(uint line = 0; line < 9; line++) {
		lines[line] = file_read;
		
		// Advance to end of line
		for(; !ending_character(*file_read); file_read++);
		
		// Advance to start of next line
		while((*file_read == '\n') || (*file_read == '\r'))
			file_read++;
	}
	
	for(uint line = 0; line < 9; line++) {
		for(uint x = 0; x < 9; x++) {
			char c = *(lines[line] + x);
			if(c == 0)
				break;
			
			/*
			 * Any character that isn't 1-9 (inclusive) is a valid blank
			 * so ' ', '?', '*', etc all work!
			*/

			// Mades some assumtions about character set used, but it
			// should work for all UTF-8 and/or ASCII similar systems
			// AKA: All sane systems
			if((c >= '1') && (c <= '9')) {
				Board_set_cell(board, x, line, c - '0');
			}
		}
	}
	free((void*)file_data); // Cast to void* because MSVC is annoying

	return res;
}