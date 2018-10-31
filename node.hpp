#ifndef NODES
#define NODES

#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>
#include <SDL.h>

#define TIME 10

typedef struct drawingElements {
	SDL_Window* pWin;
	SDL_Renderer* pRenderer;
} drawingElements;

class Node {
public:
	Node();
	~Node();

	void initialize(int startX, int startY, bool isAnObstacle);
	void analyze(Node* parent, int distToParent);
	Node* getP() const;
	int getG() const;
	float getH() const;
	float getF() const;
	int getCoordX() const;
	int getCoordY() const;

	char symb() const;
	bool isInClosedList() const;
	void pushInClosedList();
	bool isInOpenedList() const;
	void pushInOpenedList();
	bool isAnObstacle() const;
	void setAsObstacle();
	void setAsMinimY(bool b);
	bool getResearchBorder();

	void setAsFinalPath();

	static void setFirstNode(int x, int y);
	static void setFinalNode(int x, int y);

private:
	Node * P; //Parent node
	int G; //Distance from beginning
	float H; //Distance to arrival
	float F; //Poid : G+H

	int coordX;
	int coordY;
	bool inClosedList;
	bool inOpenedList;
	bool isObstacle;
	bool isFinalPath;
	bool isResearchBorder;

	static int startX;
	static int startY;
	static int finalX;
	static int finalY;
};

#endif
