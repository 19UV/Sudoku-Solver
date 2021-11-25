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
			strcpy_s(config.file_path, MAX_PATH_SIZE, argv[i+1]);
			i++; // Skip Next
		} else {
			printf("Unknown Command: %s\n", curr);
			break;
		}
	}
	
	return config;
}

int main(int argc, char* argv[]) {
	struct Config config = parse_config(argc, argv);
	fprintf(stderr, "Board Path: %s\n", config.file_path);
	
	struct Board board;
	Board_load_from_file(&board, config.file_path);
	
	int res = Board_solve(&board);
	if(res != 0) {
		printf("Board Not Solvable!\n");
		return res;
	}
	
	Board_print(&board);
	
	return 0;
}