# Sudoku Solver
This is a fun little project I threw together in an afternoon to avoid
having to do homework.

## Usage
```bash
SudokuSolver --path <path-to-board.txt>
```

## Board Format
```
XXXXXXXXX COMMENTS
XXXXXXXXX COMMENTS
XXXXXXXXX COMMENTS
XXXXXXXXX COMMENTS
XXXXXXXXX COMMENTS
XXXXXXXXX COMMENTS
XXXXXXXXX COMMENTS
XXXXXXXXX COMMENTS
XXXXXXXXX COMMENTS
```

Where 'X' is a cell. If it is a number from 1-9 (inclusive) the cell is set
to that number. Anything else, and its an unknown cell. Everything after the
first 9 character of each line are treated as comments, and don't effect the
board at all. If there isn't enough lines or characters, it should just fill
in the empty areas with unknown cells.

## Testing
I included various test boards in the ```tests/``` directory. I don't have a
seperate testing script, but the boards have comments which tell the expected
output.