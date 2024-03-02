#ifndef STRUCTURE_H_
#define STRUCTURE_H_

#include <string.h>
#include <iostream>
#include <vector>

#define ZERO 1E-8
#define EPS 1E-4
#define PI 3.14159265359

using namespace std;

typedef struct {

    long double length;
    long double width;
    long double height;
    
} layer;


typedef struct {

    long double length;
    long double width;
    long double height;

    vector<layer> layers;
    vector<long double> differentHeights;
    vector<long double> possibleHeights;

} kiln;


typedef struct {

    long double x;
    long double y;

} coord;


typedef enum {

    RECTANGLE = 1,
    TRIANGLE = 2,
    CIRCLE = 3

} itemType;


typedef struct {

    int numberOfVertices;
    vector<coord> vertices;
    
} polygon;


typedef struct {
    
    int id;
    char description[100];
    int type;
    int quantity;
    long double height;

    // Circle
    long double radius;
    
    // Rectangle
    long double length;
    long double width;
    
    // Equilateral triangle
    long double side;
    
    struct {
        coord lu, rb;
    } boundingBox;
    
    vector<polygon> NFP;
    
    long double area;

} item;


typedef struct {

    int id;
    coord p;
    int layer;

} solution;

typedef struct {

    vector<solution> solutions;

    //stores the length of each layer (i.e., the solution value)
    vector <long double> layerLengths;

} solutionPool;

#endif