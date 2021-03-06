#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "sudoku.h"

#define MAX_PATH_SIZE 512
struct Config {
	char file_path[MAX_PATH_SIZE];
};

struct Config parse_config(int argc, char* argv[]) {
	struct Config config;
	
	STR_COPY(config.file_path, "board.txt", MAX_PATH_SIZE);
	
	for(int i = 1; i < argc; i++) {
		char* curr = argv[i];
		if(STR_EQ(curr, "--path") || STR_EQ(curr, "-p")) {
			if((i + 1) == argc) {
				printf("Not Enough Inputs!\n");
				break;
			}
			STR_COPY(config.file_path, argv[i+1], MAX_PATH_SIZE);
			i++; // Skip Next
		} else if(STR_EQ(curr, "--help") || STR_EQ(curr, "-help")) {
			fprintf(stderr, "SudokuSolver [--path <path-to-board>]\n");
			*(config.file_path) = 0; // Hacky exit
		} else {
			printf("Unknown Command: %s\n", curr);
			break;
		}
	}
	
	return config;
}

int main(int argc, char* argv[]) {
	int res;
	
	struct Config config = parse_config(argc, argv);
	if(*(config.file_path) == 0) {
		// Used help menu, so not attempting the board
		return 0;
	}
	fprintf(stderr, "Board Path: %s\n", config.file_path);
	
	struct Board board;
	if((res = Board_load_from_file(&board, config.file_path)) != 0) {
		printf("Failed to read file!\n");
		return res;
	}

	if(!Board_valid(&board)) {
		printf("Board Invalid!\n");
		return 1;
	}

	res = Board_solve(&board);
	if(res != 0) {
		printf("Board Not Solvable!\n");
		return res;
	}

	Board_print(&board);

	return 0;
}