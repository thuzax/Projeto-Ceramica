#ifndef BOTTOM_LEFT_H_
#define BOTTOM_LEFT_H_

#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include <vector>

#include "structure.h"
#include "parser.h"

using namespace std;

// Calculate the D-function between line AB and the point P
int d_function(coord a, coord b, coord p);

// Sort items by height
vector<int> generateOrder(item *items, int numberOfItems);
void printOrder(item *items, vector<int> order);

// Get the minimum possible height for fitting an item in a new layer of the kiln
int minimumFittingHeight(double itemHeight, kiln *k);

// Solve the problem
solutionPool solve(item *items, int numberOfItems, vector<int> order, kiln *k);

// Check if an item is completely inside the the kiln
int checkBoundingBox(item *items, solution sol, kiln *k);
int checkBoundingBox(item *items, int id, double x, double y, int layer, kiln *k);

// NÃO ENTENDI ESSA NÃO
int checkPastSolutions(double x, double y, int layer, solutionPool sp);

// Check if a solution layer is feasible after inserting an item using reference point (x,y)
int checkSolution(item *items, int id, double x, double y, int layer, solutionPool sp);

void drawSolution(item *items, solutionPool sp, kiln *k, char *fileName, int numberOfItems);
void drawStep(item *items, solutionPool sp, vector<coord> pointsNFP, int newItem);

#endif