#include "node.hpp"

int Node::startX=0;
int Node::startY=0;
int Node::finalX=0;
int Node::finalY=0;

Node::Node() {
	coordX = coordY = 0;
	P = NULL;
	F = 0;
	G = 0;
	H = 0;
	inClosedList = false;
	inOpenedList = false;
	isObstacle = false;
	isFinalPath = false;
}
Node::~Node() {}

void Node::initialize(int startX, int startY, bool isAnObstacle) {
	coordX = startX;
	coordY = startY;

	P = NULL;
	F = 0;
	G = 0;
	H = 0;
	isResearchBorder = false;
	inClosedList = false;
	inOpenedList = false;
	isObstacle = isAnObstacle;
}

void Node::analyze(Node* parent, int distToParent) {
	P = parent;

	if (parent != NULL)
		G = parent->G + distToParent;
	else
		G = 0;

	H = (float)sqrt(pow(finalX - coordX, 2) + pow(finalY - coordY, 2));
	F = H + G;
}

Node* Node::getP() const {return P;}
int Node::getG() const {return G;}
float Node::getH() const {return H;}
float Node::getF() const {return F;}
int Node::getCoordX() const {return coordX;}
int Node::getCoordY() const {return coordY;}
char Node::symb() const {
	if (coordX == startX && coordY == startY)
		return 'S';
	if (coordX == finalX && coordY == finalY)
		return 'A';
	if (isFinalPath)
		return ' ';
	if (isObstacle)
		return '#';
	if (isResearchBorder)
		return 'M';
	if (inClosedList)
		return 'X';
	if (inOpenedList)
		return '?';

	return '.';
}

bool Node::isInClosedList() const {return inClosedList;}
bool Node::isAnObstacle() const {return isObstacle;}
bool Node::isInOpenedList() const {return inOpenedList;}
void Node::setAsObstacle() {isObstacle=true;}
void Node::pushInClosedList() {inClosedList=true;}
void Node::pushInOpenedList() {inOpenedList=true;}

void Node::setFirstNode(int x, int y) {
	startX = x;
	startY = y;
}
void Node::setFinalNode(int x, int y) {
	finalX = x;
	finalY = y;
}

void Node::setAsFinalPath() { isFinalPath = true; }

void Node::setAsMinimY(bool b) {
	isResearchBorder = b;
}

bool Node::getResearchBorder() { return isResearchBorder; }