/*
	Damien Edward Blasko
	28 January 2020

	Langton's Ant (please reference the wikipedia entry) is a classic 
	demonstration of "two-dimensional universal Turing machine with 
	a very simple set of rules but complex emergent behavior."

	During each demonstration, we'll chose an arbitrary number of 
	iterations for the ant to follow.

	The simulation involves a board made up of squares that can be 
	white or black. Initially the board is entirely white. An the ant
	is places on a random square. The simulation abides the following
	steps:

		(1) The Ant flips the color of the square it's sitting on from
			black to white, or white to black.
		(2) If the ant had been sitting on a white space, it turns 
			clockwise 90 degrees. If it had been sitting on a black 
			space, it turns counter clockwise by 90 degrees.
		(3) The ant moves forward one square.
		(4) The process keeps repeating, until all iterations of the 
			previous steps are complete
*/

#define BLACK_SQUARE '#'
#define WHITE_SQUARE ' '
#define ANT '*'
#define MAX_ROWS 50
#define MAX_COLS 50
#define MAX_TURNS 1000
#define UNREASONABLE_ROWS (num_rows < 1 || num_rows > MAX_ROWS)
#define UNREASONABLE_COLS (num_cols < 1 || num_cols > MAX_COLS) 
#define UNREASONABLE_TURNS (turns < 0 || turns > MAX_TURNS)
						

#include <stdio.h>
#include <stdlib.h>
#include <time.h> //used for random number generation

enum Space_Color {black_ = 1, white_ = 0}; 

/* We will need these Nodes to create a Doubly- */
/* linked-list to keep track of the orientation */
/* of our ant */
typedef struct Node * lnk;
struct Node {
	int x_add;
	int y_add;
	lnk clockwise;
	lnk c_clockwise;
};

/* We'll only need one 'board' struct */
/* to model the state (and history of states) */
/* i.e, the path, of our Ant */
/* In other words, the 'board' is what we'll be */
/* drawing to std output at the end of each turn */
struct board {
	int x, y, row_of_ant, column_of_ant;
	enum Space_Color ant_square_color;
	char ** grid;
};

/* Default color of board spaces is white */
/* But the ant will change white spaces to black as */ 
/* it moves out of them */
void fill_chars(struct board * board_){
	for(int i = 0; i < board_->y; i++){
		for(int j = 0; j < board_->x; j++){
			board_->grid[i][j] = WHITE_SQUARE;
		}
	}
}

/* Initializes a new board */
/* Only explicit use of 'malloc', so we can specifiy */
/* different board sizes via command-line input it also	*/
/* also calls 'fill_chars' and draws the initial placement of the ant */
struct board * init_board(int x_, int y_, int r, int c) {
	struct board * ptr = malloc(sizeof(struct board));
	ptr->x = x_;
	ptr->y = y_;
	ptr->row_of_ant = r;
	ptr->column_of_ant = c;
	ptr->grid = malloc(y_ * sizeof(char*));
	for(int i = 0; i < y_; i++){
		ptr->grid[i] = calloc(x_ + 1, sizeof(char));
	}
	fill_chars(ptr);
	ptr->grid[r][c] = ANT;
	ptr->ant_square_color = white_;

	return ptr;
}

void print_board(struct board * board_){
	for(int i = 0; i <= board_->x + 1; i++){
		printf("-");
	}
	printf("\n");
	for(int i = 0; i < board_->y; i++){
		printf("|%s|\n", board_->grid[i]);
	}
	for(int i = 0; i <= board_->x + 1; i++){
		printf("-");
	}
	printf("\n");
}

void free_board(struct board * board_){
	for(int i = 0; i < board_->y; i++){
		free(board_->grid[i]); 
	}
	free(board_->grid);
	free(board_);
}

/* If the Ant tries to walk out of bounds, it simply */
/* pops back out on the other side of the board */
/* Another way to think of this is that the board is the */
/* Surface of a sphere */
int adjust_bounds(int coord, int lower_bound, int upper_bound){
	if (coord < lower_bound) coord = upper_bound;
	if (coord > upper_bound) coord = lower_bound;
	return coord;
}

/* Moves the ant horizontally by a given amount, and */
/* updates the 'ant_square_color' property to the color */
/* of the space just moved into. The board must keep track */
/* of the color of all squares, even the one whose surface we */
/* cannot see b/c the ant is standing on it */
/* 'z' can be positive or negative */
void horizontal_move(struct board * board_, int z){
	int r = board_->row_of_ant;
	int c = board_->column_of_ant;
	if(board_->ant_square_color == black_) {
		board_->grid[r][c] = BLACK_SQUARE;
	} else {
		board_->grid[r][c] = WHITE_SQUARE;
	}
	c += z;
	c = adjust_bounds(c, 0, board_->x - 1);
	if(board_->grid[r][c] == WHITE_SQUARE){
		board_->ant_square_color = white_;
	} else {
		board_->ant_square_color = black_;
	}
	board_->grid[r][c] = ANT;
	board_->column_of_ant = c;
}

/* Same as horizontal move, but along perpendicular axis */
void vertical_move(struct board * board_, int z){
	int r = board_->row_of_ant;
	int c = board_->column_of_ant;
	if(board_->ant_square_color == black_) {
		board_->grid[r][c] = BLACK_SQUARE;
	} else {
		board_->grid[r][c] = WHITE_SQUARE;
	}

	r += z;
	r = adjust_bounds(r, 0, board_->y - 1);
	if(board_->grid[r][c] == WHITE_SQUARE){
		board_->ant_square_color = white_;
	} else {
		board_->ant_square_color = black_;
	}
	/* We mustn't forget to draw our ant */
	board_->grid[r][c] = ANT;
	/* Or to keep track of it's new location */
	board_->row_of_ant = r;
}

/* We need to consolidate the two 'move' functions so */
/* that they can be called with the values from any node */
/* in the main method. This will become clear below */
void move(struct board * board_, int x, int y){
	vertical_move(board_, y);
	horizontal_move(board_, x);
}

int main(int argc, char *argv[]){
	/* We'll start by checking to see that there are the correct number*/
	/* of command-line arguments to main */
	if (argc < 4 || argc > 5) {
		fprintf(stderr, "Wrong number of arguments. There should be three.\n");
		fprintf(stderr, "(1) number of rows, (2) number of columns, and \n");
		fprintf(stderr, "(3) number of times the ant moves.\n");
		exit(EXIT_FAILURE);
	}

	/* we'll start by extracting the three necessary arguments */
	/* from the command-line */
	int num_rows = atoi(argv[1]);
	int num_cols = atoi(argv[2]);
	int turns = atoi(argv[3]);

	if(UNREASONABLE_ROWS || UNREASONABLE_COLS || UNREASONABLE_TURNS){
		fprintf(stderr, "Numerical arguments are out of bounds.\n");
		fprintf(stderr, "Acceptable: [1, %d] [1, %d] [0, %d]\n",
			MAX_ROWS, MAX_COLS, MAX_TURNS);
		exit(EXIT_FAILURE);
	}




	/* now, we need a random set of coordinates to place our ant */
	srand(time(NULL));
	int x_start = rand() % num_cols;
	int y_start = rand() % num_rows;

	/* We'll use a doubly-linked list to keep track of and control */
	/* the orientation of the Ant, rather than switches or 'if-else...*/
	/* ...if-else' statements */

	/* BEGINNING OF DOUBLY LINKED LIST CONSTRUCTION */
	struct Node north, south, east, west;

	north.x_add = 0;
	north.y_add = -1;
	north.clockwise = &east;
	north.c_clockwise = &west;

	east.x_add = 1;
	east.y_add = 0;
	east.clockwise = &south;
	east.c_clockwise = &north;

	south.x_add = 0;
	south.y_add = 1;
	south.clockwise = &west;
	south.c_clockwise = &east;

	west.x_add = -1;
	west.y_add = 0;
	west.clockwise = &north;
	west.c_clockwise = &south;

	/* Now we have to have a 'cursor' to access elements of the list */
	lnk cursor = &north;

	/* END OF CONSTRUCTION OF DOUBLY LINKED LIST */

	/* We think of the ant as having a finite number of states: it can face */
	/* North, South, East, or West AND it can sit on a white square or a */
	/* black square. The position of the cursor, i.e., which direction the ant */
	/* faces, is the CURRENT STATE of our program, while the 'board', aside from */
	/* giving us one piece of further info about the current state--the color of */
	/* the current square--is a record of WHERE the ant has been: all previous */
	/* states of the program */


	struct board * myBoard = init_board(num_cols, num_rows, y_start, x_start);
	print_board(myBoard);
	for(int i = 0; i < turns; i++){
		/* STEP 1: Turn the Ant */
		/* STEP 2: Flip the Color of the Ant's square */
		if(myBoard->ant_square_color == white_){
			cursor = cursor->clockwise;
			myBoard->ant_square_color = black_;
		} else {
			cursor = cursor->c_clockwise;
			myBoard->ant_square_color = white_;
		}
		/* STEP 3: Move the Ant */
		move(myBoard, cursor->x_add, cursor->y_add);
		print_board(myBoard);
	}
	free_board(myBoard);
	return EXIT_SUCCESS;
}