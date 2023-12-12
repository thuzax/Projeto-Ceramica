#include "main.h"
#include "globals.h"

using namespace std;

int main(int argc, char *argv[]) {
	declare_globals();

	// Get the input parameters
    char *kilnFile = (char *)malloc(1024 * sizeof(char));
    char *itemsFile = (char *)malloc(1024 * sizeof(char));
    char *solutionFile = (char *)malloc(1024 * sizeof(char));
    
    if (!kilnFile || !itemsFile || !solutionFile) {
        cout << "Error: Internal error while allocating memory for the configuration files. " << string_endline();
		exit(EXIT_FAILURE);
    }
    
    if (argc >= 4) {
        strcpy(kilnFile, argv[1]);
        strcpy(itemsFile, argv[2]);
        strcpy(solutionFile, argv[3]);
    } else {
        strcpy(kilnFile, "kiln.txt");
        strcpy(itemsFile, "items.txt");
        strcpy(solutionFile, "solution.tex");
    }


	// Read kiln definition file and initialize the problem kiln struct
    cout << string_endline();
	kiln *mainKiln = initKiln(kilnFile);	
	//printKilnInformation(mainKiln);
    
	// Calculate heights for knapsack problem
    calculateAllPossibleHeights(mainKiln);
    //printAllPossibleHeights(mainKiln);

	//numberOfItems: number of different types of items
	int numberOfItems = -1;

	// Read items file and initialize the item structs
	item *items = initItems(itemsFile,&numberOfItems);
	//printItemsInformation(items, numberOfItems);

	// Initialize the input items bounding boxes
	boundingBox(items, numberOfItems);
    //printBoundingBox(items, numberOfItems);
	
	// Calculate the input items area
    calculateArea(items, numberOfItems);
    //printArea(items, numberOfItems);

	// Calculate the NFP for each item
	calculateNFP(items, numberOfItems);	

	// Order the items by height
	vector<int> order = generateOrder(items, numberOfItems);
	//printOrder(items, order);

	// Solve the problem
	solutionPool result = solve(items, numberOfItems, order, mainKiln);

	// Print Solution
	cout << "Solution: ";
	for (int l=0 ; l < mainKiln->layers.size() ; l++) {
		cout << " - Layer #" << l+1 << " - Length: " << result.layerLengths[l] << string_endline();
		for (int i=0; i<result.solutions.size() ; i++) {
			if (result.solutions[i].layer == l && result.solutions[i].id != 0)
				cout << "   - Item #" << result.solutions[i].id << ": (" << result.solutions[i].p.x << ", " << result.solutions[i].p.y << ") " << string_endline();
		}	
	}
	
	cout << string_endline();
	// Draw solution in LaTEX
	drawSolution(items, result, mainKiln, solutionFile, numberOfItems);

	free(kilnFile);
	free(itemsFile);
	free(solutionFile);
    free(mainKiln);
	free(items);

	destroy_globals();

	return 0;
}