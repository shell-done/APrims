#ifndef GRAPH
#define GRAPH

#define COL_GREEN 139,180,74 
#define COL_BLUE 0,148,255
#define COL_BLACK 106,106,106
#define COL_RED 204, 81, 81
#define COL_PURPLE 169,17,196
#define COL_DARK_PURPLE 87,0,127

#include "node.hpp"

typedef struct mazePart {
	char type;
	unsigned short coordX, coordY;
} mazePart;

class Graph {
public:
	Graph(bool disp, std::string seed, unsigned int box, bool inst);
	~Graph();

	void loadFile(std::string file);
	void draw() const;
	void initialize();
	Node* getBetterNodeInOpenedList();
	void getBetterNodeAroundCurrent(Node* currentNode);
	void findPath();
	void display() const;
	void generateMaze();

private:
	unsigned int sizeX, sizeY;
	unsigned int boxSize;
	unsigned int seed;
	bool dispWindow;
	bool instant;

	int skip, count;

	unsigned int* minY, *maxY;
	drawingElements drawingEl;
	Node** nodes;
	std::vector<Node*> openList;
};

#endif
