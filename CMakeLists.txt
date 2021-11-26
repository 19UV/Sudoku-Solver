cmake_minimum_required(VERSION 3.10)
project(SudokuSolver)

add_executable(SudokuSolver
	src/file.c
	src/sudoku.c
	src/main.c
)

set_property(TARGET SudokuSolver PROPERTY C_STANDARD 11)