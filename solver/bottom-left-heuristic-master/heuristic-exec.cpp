#include "heuristic-exec.h"

using namespace std;


int executeHeuristicBottomLeft(int argc, char** argv) {
	cout << "Iniciando Resolução do Problema" << stringEndline();
	declareGlobals();
	// Get the input parameters
    char *kilnFile = new char[1024];
    char *itemsFile = new char[1024];
    char *solutionFile = new char[1024];
    
    if (!kilnFile || !itemsFile || !solutionFile) {
        cout << "Error: Internal error while allocating memory for the configuration files. " << stringEndline();
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
	cout << "Inicializando variavel para o forno" << stringEndline();
	kiln *mainKiln = initKiln(kilnFile);
	cout << "OK" << stringEndline();
	//printKilnInformation(mainKiln);
    
	// Calculate heights for knapsack problem
	cout << "Calculando combinações de alturas dos suportes" << stringEndline();
    calculateAllPossibleHeights(mainKiln);
	cout << "OK" << stringEndline();
    //printAllPossibleHeights(mainKiln);

	//numberOfItems: number of different types of items
	int numberOfItems = -1;

	// Read items file and initialize the item structs
	cout << "Inicializando variaveis de peça" << stringEndline();
	item *items = initItems(itemsFile,&numberOfItems);
	cout << "OK" << stringEndline();
	//printItemsInformation(items, numberOfItems);

	// Initialize the input items bounding boxes
	cout << "Calculando caixa de limite ao redor da peça" << stringEndline();
	boundingBox(items, numberOfItems);
	cout << "OK" << stringEndline();
    //printBoundingBox(items, numberOfItems);
	
	// Calculate the input items area
	cout << "Calculando áreas das peças" << stringEndline();
    calculateArea(items, numberOfItems);
	cout << "OK" << stringEndline();
    //printArea(items, numberOfItems);

	// Calculate the NFP for each item
	cout << "Calculando No-fit polygon das peças" << stringEndline();
	calculateNFP(items, numberOfItems);	
	cout << "OK" << stringEndline();

	// Order the items by height
	cout << "Ordenando as peças" << stringEndline();
	vector<int> order = generateOrder(items, numberOfItems);
	cout << "OK" << stringEndline();
	//printOrder(items, order);

	// Solve the problem
	cout << "Resolvendo o problema" << stringEndline();
	solutionPool result = solve(items, numberOfItems, order, mainKiln);
	cout << "OK" << stringEndline();

	// Print Solution
	cout << "Solution: ";
	for (int l=0 ; l < mainKiln->layers.size() ; l++) {
		cout << " - Layer #" << l+1 << " - Length: " << result.layerLengths[l] << stringEndline();
		for (int i=0; i<result.solutions.size() ; i++) {
			if (result.solutions[i].layer == l && result.solutions[i].id != 0)
				cout << "   - Item #" << result.solutions[i].id << ": (" << result.solutions[i].p.x << ", " << result.solutions[i].p.y << ") " << stringEndline();
		}	
	}
	
	cout << stringEndline();
	// Draw solution in LaTEX
	cout << "Desenhando Solução" << stringEndline();
	drawSolution(items, result, mainKiln, solutionFile, numberOfItems);
	cout << "OK" << stringEndline();


	cout << "Limpando a Memória" << stringEndline();
	if (kilnFile != NULL) {
		delete[] kilnFile;
		kilnFile = NULL;
	}

	if (itemsFile != NULL) {
		delete[] itemsFile;
		itemsFile = NULL;
	}

	if (solutionFile != NULL) {
		delete[] solutionFile;
		solutionFile = NULL;
	}
    
	if (mainKiln != NULL) {
		delete mainKiln;
		mainKiln = NULL;
	}

	if (items != NULL) {
		delete[] items;
		items = NULL;
	}


	destroyGlobals();
	cout << "OK" << stringEndline();

	return 0;

}
