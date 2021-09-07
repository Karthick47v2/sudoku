#include <iostream>
#include <cmath>
#include <ctime>
#include <chrono>
#include "../include/Stack.h"

#define GRIDDIM 9 							//As the quest concerns abt 9x9 sudoku we can make them as const
#define BLOCKDIM 3 							//and block size also wont be changed

int sudokuGrid[GRIDDIM][GRIDDIM];			//Initialize sudoku grid
int sudokuCopy[GRIDDIM][GRIDDIM];			//Copy for restoring sudoku when user selects start from beginng when error is found


//---Sudoku Generating functions---///
void fillDiagonalBlocks();
bool fillNonDiagonalBlocks(int i, int j);

//---Sudoku Solving funcitons---///
bool solveSudoku(int i, int j, int c, int d, Stack *s);

//---Helper functions---///
void copyArray(int a[GRIDDIM][GRIDDIM], int b[GRIDDIM][GRIDDIM]);
void userInput(int i, int j, int c, Stack *s);
bool isFoundInBlock(int row, int col, int val);
bool isUniqueInRow(int row, int val);
bool isUniqueInCol(int col, int val);
bool isPlaceable(int i, int j, int k);
int randomNoGen();
void deleteElements(int i);
void printSudoku();

//---Testing func to measure time taken to genrate and solve sudoku---///
void test();
bool solveSudoku(int i, int j);
 
//check if element is already in same block
bool isFoundInBlock(int row, int col, int val){
	for(int i = 0; i < BLOCKDIM; i++){
		for(int j = 0; j < BLOCKDIM; j++){
			if(sudokuGrid[row+i][col+j] == val){
				return true;
			}
		}
	}
	return false;
}

//check if element is not in same row
bool isUniqueInRow(int row, int val){
	for(int i = 0; i < GRIDDIM; i++){
		if(sudokuGrid[row][i] == val) return false;
	}
	return true;
}

//check if element is not in same col
bool isUniqueInCol(int col, int val){
	for(int i = 0; i < GRIDDIM; i++){
		if(sudokuGrid[i][col] == val) return false;
	}
	return true;
}

//genrate random no
int randomNoGen(){
	return (rand() % 9);		//0 - 9
}

//fill diagonal blocks
void fillDiagonalBlocks(){
	int val;
	for(int i = 0; i < GRIDDIM;){
		for(int j = 0; j < BLOCKDIM; j++){
			for(int k = 0; k < BLOCKDIM; k++){
				while(isFoundInBlock(i, i, val = 1 + randomNoGen())){
					//just loop until find a unique value for val
				}
				//once unique val found assign it
				sudokuGrid[i+j][i+k] = val;
				
			}
		}
		i += BLOCKDIM;
	}
}

//check if element is placeable in grid
bool isPlaceable(int i, int j, int k){	
	return (isUniqueInRow(i, k) && isUniqueInCol(j, k) && !isFoundInBlock((i - (i%BLOCKDIM)), (j - (j%BLOCKDIM)), k));
}

//fill remainign blocks
bool fillNonDiagonalBlocks(int i, int j){
	if(j >= GRIDDIM && i < GRIDDIM - 1){									
		i++;
		j = 0;
	}
	
	if(i >= GRIDDIM && j >= GRIDDIM) return true;							//all filled
	
	//omit diagonal blocks
	if(i < BLOCKDIM){
		if(j < BLOCKDIM) j = BLOCKDIM;										
	}
	else if(i < GRIDDIM - BLOCKDIM){
		if(j == (int)(i/BLOCKDIM) * BLOCKDIM) j += BLOCKDIM;
	}
	else{
		if(j == GRIDDIM - BLOCKDIM){
			i++;
			j = 0; 
			if(i >= GRIDDIM) return true;
		}
	}
	
	//if all conditions are met then check if its placeable then place, or put 0 --- This fillNonDiagonal func is recursive func so if there is error then it backtrack to previous correct point
	for(int k = 1; k <= GRIDDIM; k++){
		if(isPlaceable(i, j, k)){
			sudokuGrid[i][j] = k;
			
			if(fillNonDiagonalBlocks(i, j+1)) return true;
			
			sudokuGrid[i][j] = 0;
		}
	}
	return false;
}

//create blank as per user req
void deleteElements(int i = 0){
	char c;
	int n;
	if(i == 0){
		do{
			std::cout << "How do you want to place blank ? (A) row wise  (B) col wise : ";
			std::cin >> c;
		}while(c != 'A' && c != 'B');
		
		do{
			std::cout << "Enter how many elements need to be blank in each " << (c == 'A' ? "row" : "col" ) <<  " (2 - 4) :";
			std::cin >> n;
		}while(n < 2 || n > 4);
	}
	else n = i;
	
	for(int i = 0; i < GRIDDIM; i++){
		for(int j = 0; j < n; j++){
			int k = randomNoGen();
			if(c == 'A') sudokuGrid[k][i] = 0;
			else sudokuGrid[i][k] = 0;
		}
	}	
}

//print 
void printSudoku(){
	std::cout << '\n';
	for(int i = 0; i < GRIDDIM; i++){
		if(i != 0 && i % 3 ==  0 ) std::cout << "--------------------\n"; 
		for(int j = 0; j < GRIDDIM; j++){
			if(j != 0 && j % 3 == 0) std::cout << "|"; 
			std::cout << sudokuGrid[i][j] << ' '; 
		}
		
		std::cout << '\n';
	}
}

//func for copying arr
void copyArray(int a[GRIDDIM][GRIDDIM], int b[GRIDDIM][GRIDDIM]){
	for(int i = 0; i < GRIDDIM; i++){
		for(int j = 0; j < GRIDDIM; j++){
			a[i][j] = b[i][j];
		}
	}
}

//recursive algo for solving sudoku -- if any error, it will backtrack to prev safe point
bool solveSudoku(int i, int j, int c, int d, Stack *s){
    if(i==9) return true;													//finished
    if(j==9) return solveSudoku(i+1, 0, c, d, s);							//go to nxt row
    if(sudokuGrid[i][j] != 0) return solveSudoku(i, j+1, c, d, s);			//go to next, if curr cell is not blank
	
	
	/*Here we push the predicted val to stack, in case if its wrong we can retrieve that val and increase 1 to it then retry.
	*/
	
	s -> push(d);	
	
    for(; c<=9; c++){
        if(isPlaceable(i, j, c)){												//check if its placeable
        
            sudokuGrid[i][j] = c;
            printSudoku();
            if(solveSudoku(i, j+1, 1, c, s)){    						//if all ok then return true  			    	
				return true;
			}
			printSudoku();
			userInput(i, j, c, s);
        }
    }
    return false;
}

//ask user for opts
void userInput(int i, int j, int c, Stack *s){
	static int trial = 0;
	trial++;
	char d;
	
	if(trial > 5){
		char a;
		do{
			std::cout << "Tired of being pressing key ??, Do you want to let system solve it automatically / Do you want to check if this algorithm is foolproof ? if yes press 'Y' or press 'N' to snooze this dialogue..\n";
			std::cin >> a;
		}while(a != 'Y' && a != 'N');
		trial = 0;
		if(a == 'Y'){
			copyArray(sudokuGrid, sudokuCopy);
			solveSudoku(0,0);
			printSudoku();
			std::cout << "\n\nSUDOKU SOLVED ...";
			system("pause");
			exit(0);
		}
	}
	
	do{
	std::cout << "Wrong value is placed at.. " << i << ',' << j << "  Do you want to start over (A) or start backtracking (B)?";
	std::cin >> d;
	}while(d != 'A'&& d != 'B');
	
	if(d == 'A'){
		copyArray(sudokuGrid, sudokuCopy);
		int a = s -> a[1];
		while(!s->isEmpty()) s -> pop();
		solveSudoku(0,0, a, a, s);
	}
	else{
		sudokuGrid[i][j] = 0;
		int a = s -> pop() + 1;
		solveSudoku(i, j, a, a, s);	
	}
}

bool solveSudoku(int i, int j){
    if(i==9) return true;								
    if(j==9) return solveSudoku(i+1, 0);				
    if(sudokuGrid[i][j] != 0) return solveSudoku(i, j+1);			
	
    for(int c = 1; c<=9; c++){
        if(isPlaceable(i, j, c)){									
        
            sudokuGrid[i][j] = c;
            if(solveSudoku(i, j+1)) return true;
			sudokuGrid[i][j] = 0;
        }
    }
    return false;
}

void test(){
	auto start1 = std::chrono::steady_clock::now();
	fillDiagonalBlocks();
	fillNonDiagonalBlocks(0, BLOCKDIM);
	auto end1 = std::chrono::steady_clock::now();
	deleteElements(5);
	auto end1_1 = std::chrono::steady_clock::now();	
	printSudoku();
	std::cout << "\ttime taken for generating sudoku  : " << std::chrono::duration_cast<std::chrono::nanoseconds>(end1 - start1).count() << "ns\n";
	std::cout << "\ttime taken for creating blank spaces : " << std::chrono::duration_cast<std::chrono::nanoseconds>(end1_1 - end1).count() << "ns\n";
	std::cout << "Sudoku puzzle generated, total time elapsed : " << std::chrono::duration_cast<std::chrono::nanoseconds>(end1_1 - start1).count() << "ns\n";
	
	auto start2 = std::chrono::steady_clock::now();
	solveSudoku(0,0);
	auto end2 = std::chrono::steady_clock::now();
	printSudoku();
	std::cout << "Sudoku puzzle solved, time elapsed : " << std::chrono::duration_cast<std::chrono::nanoseconds>(end2 - start2).count() << "ns\n";	
}

int main(){
	srand((unsigned) time(0));
	Stack s;
	
	/*Stratrgy we are going to use is, first we fill all the diagonal block,
	as we don't need to check if its valid or not
	
	for example,
	
	-------------------
	|x|x|x| | | | | | |
	-------------------
	|x|x|x| | | | | | |						For filling block with Xs we dont need to care about its rows or cols,
	-------------------						same for all other Y and Z blocks, so first we fill these 3 blocks without
	|x|x|x| | | | | | |						validation test, so we can save some unnessary computation time 
	-------------------
	| | | |y|y|y| | | |						After filling diagonal block, we fill other blocks recursively
	-------------------
	| | | |y|y|y| | | |
	-------------------
	| | | |y|y|y| | | |
	-------------------
	| | | | | | |z|z|z|
	-------------------
	| | | | | | |z|z|z|
	-------------------
	| | | | | | |z|z|z|
	-------------------                  */
	
	//THIS FUNC IS TO CHECK SPEED OF THE ALGORITHM, comment all other things below this funcs to check..
	//test();
	
	fillDiagonalBlocks();
	fillNonDiagonalBlocks(0, BLOCKDIM);
	
	deleteElements();
	std::cout << "SUDOKU GENERATED ...\n\n";
	printSudoku();
	
	
	copyArray(sudokuCopy, sudokuGrid);
	solveSudoku(0,0,1, 1, &s);
	
	printSudoku();
	std::cout << "\n\nSUDOKU SOLVED ...";
	system("pause");
	return 0;
}
