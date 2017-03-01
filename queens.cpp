/*
  Name: Anthony Absher
  Project: HW 3 - 8 Queens Genetic Algorithm
*/

#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;

//dimensions of chessboard, also number of queens
const int BOARD_SIZE = 8;
//number of various iterations of queen combinations in population
const int POP_SIZE = 10000;
//rate of being chosen for crossover (out of 100)
const int CROSSOVER_RATE = 40;
//rate of being chosen for mutation (out of 100)
const int MUTATION_RATE = 20;
//table to hold configuration of queens
int Population[BOARD_SIZE][POP_SIZE];
//chess board representation
int Board[BOARD_SIZE][BOARD_SIZE];
//fitness for each element of the population
int Fitness[POP_SIZE];


//clears chess board
void resetBoard(){
  for(int i = 0; i < BOARD_SIZE; i++){
	for(int j = 0; j < BOARD_SIZE; j++){
	  Board[i][j] = 0;
	}
  }
}

//fills chess board with a certain population
void fillBoard(int popIndex){
  resetBoard();
  for(int i=0; i < BOARD_SIZE; i++)
	Board[i][Population[i][popIndex]]=1;
}

//draws a current instance of a chess board to the screen
void drawBoard(){
  for(int i = 0; i < BOARD_SIZE; i++){
	for(int j = 0; j < BOARD_SIZE; j++){
	  cout << "|"<< Board[i][j] << "|";
	}
	cout << endl;
  }
}

//finds the fitness of a certain population
int fitness(int popIndex){
  int cost = 0;
  int queenSpot,m,n;
  fillBoard(popIndex);
  for(int i = 0; i < BOARD_SIZE; i++){
	queenSpot = Population[i][popIndex];

	m = i+1;
	n = queenSpot-1;
	
	//check first diagonal
	while(m < BOARD_SIZE && n >= 0){
	  if(Board[m][n] == 1)
		cost++;
	  m++;
	  n--;
	}
	m = i+1;
	n = queenSpot+1;

	//check second diagonal
	while(m < BOARD_SIZE && n < BOARD_SIZE){
	  if(Board[m][n] == 1)
		cost++;
	  m++;
	  n++;
	}

	//check third diagonal
	m = i-1;
	n = queenSpot-1;
	while(m >= 0 && n >= 0){
	  if(Board[m][n] == 1)
		cost++;
	  m--;
	  n--;
	}
	//check fourth diagonal
	m = i-1;
	n = queenSpot+1;
	while(m >= 0 && n < BOARD_SIZE){
	  if(Board[m][n] == 1)
		cost++;
	  m--;
	  n++;
	}
	//check same row
	for(int j = 0; j < i; j++){
	  if(Population[i][popIndex] == Population[j][popIndex])
		cost++;
	}
  }
  return cost;
}

void crossover(){
  int crossoverChance, chromosome1,
	chromosome2, crossoverPoint, temp;

  for(int i = 0; i < POP_SIZE; i++){
	crossoverChance = rand()%100;

	if(crossoverChance < CROSSOVER_RATE){
	  chromosome1 = rand()%POP_SIZE;
	  if(i > 0)
		chromosome2 = chromosome1 - 1;
	  else
		chromosome2 = chromosome1 + 1;

	  crossoverPoint = rand() % BOARD_SIZE;

	  for(int i = crossoverPoint; i < BOARD_SIZE; i++){
		temp = Population[i][chromosome1];
		Population[i][chromosome1] = Population[i][chromosome2];
		Population[i][chromosome2] = temp;
	  }
	  Fitness[chromosome1] = fitness(chromosome1);
	  Fitness[chromosome2] = fitness(chromosome2);
	}
  }
}



void mutation(){
  int mutationChance, mutIndex, newVal;
  
  for(int i = 0; i < POP_SIZE; i++){
	mutationChance = rand() % 100;
	if(mutationChance < MUTATION_RATE){
	  mutIndex = rand() % BOARD_SIZE;
	  newVal = rand() % BOARD_SIZE;

	  while(newVal == Population[mutIndex][i])
		newVal = rand() % BOARD_SIZE;

	  Population[mutIndex][i] = newVal;
	  Fitness[i] = fitness(i);
	}
  }
}


//initializes all the queen populations
void initialPop(){
  int random;
  for(int popIndex = 0; popIndex < POP_SIZE; popIndex++){
	for(int i = 0; i < BOARD_SIZE; i++){
	  random = rand()%BOARD_SIZE;
	  Population[i][popIndex] = random;
	}
	Fitness[popIndex] = fitness(popIndex);
  }

}

//swap 2 elements of the population, a helper funtion for the popSort
void swap(int i, int j){
  int tempFit = Fitness[i];
  Fitness[i] = Fitness[j];
  Fitness[j] = tempFit;
  int tempQ;
  for(int k = 0; k < BOARD_SIZE; k++){
	tempQ = Population[k][i];
	Population[k][i] = Population[k][j];
	Population[k][j] = tempQ;
  }

}

//function to sort the populations according to fitness
void popSort(){
  for(int i = 0; i < POP_SIZE; i++){
	for(int j = i+1; j < POP_SIZE; j++){
	  if(Fitness[i] > Fitness[j]){
		swap(i,j);
	  }
	}
  }
}

bool lifeCycle(){
  bool solFound = false;

  crossover();
  mutation();
  popSort();
  
  //check for solution
  if(Fitness[0] == 0)
	solFound = true;

  return solFound;
}

float averageFit(){
  float sum = 0, numElems = 0;
  for(int i = 0; i < POP_SIZE; i++){
	sum += Fitness[i];
	numElems++;
  }
  return (sum / numElems);
}
			   
int main(){
  int numCycles=0;
  srand(time(0));
  initialPop();

  cout << endl << endl << "Loading..." << endl << endl;

  while(lifeCycle() == false){
    numCycles++;
	//cout << "Average: " << averageFit() << endl;
  }

  cout << endl << endl
	   << "     SOLUTION BOARD" << endl
	   << "------------------------" << endl;
  
  fillBoard(0);
  drawBoard();
  cout << endl << "Number of Cycles: " << numCycles << endl << endl;
  
  return 0;

}
