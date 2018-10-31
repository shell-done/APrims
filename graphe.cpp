#include "graphe.hpp"

using namespace std;

Graph::Graph(bool disp, std::string seed, unsigned int box, bool inst) : boxSize(box), nodes(NULL), dispWindow(disp), sizeX(1280), sizeY(720), instant(inst) {
	if (disp) {
		drawingEl.pWin = SDL_CreateWindow("Maze generator and solver - Prim's & A* Algorithms - Alexandre THOMAS (ISEN YNCREA OUEST RENNES)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);

		drawingEl.pRenderer = SDL_CreateRenderer(drawingEl.pWin, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		SDL_SetRenderDrawColor(drawingEl.pRenderer, COL_BLACK, 255);
		SDL_RenderClear(drawingEl.pRenderer);
		SDL_RenderPresent(drawingEl.pRenderer);

		SDL_Surface* icon = SDL_LoadBMP("icon.bmp");
		SDL_SetWindowIcon(drawingEl.pWin, icon);

		skip = 0;
		count = 0;

		int temp;

		if (seed == "NOSEED_NOBOXSIZE") {
			srand((unsigned int)time(NULL));
			temp = rand() % 4;
			boxSize = (unsigned int)pow(2, temp + 2);
		}
		else if (seed == "NOSEED") {
			srand((unsigned int)time(NULL));
		}
		else {
			this->seed = 0;
			for (unsigned int i = 0; i < seed.size(); i++)
				this->seed += seed[i];

			temp = this->seed % 5;
			boxSize = (unsigned int)pow(2, temp+2);
			srand(this->seed);
		}
	}
	
	maxY = new unsigned int[1280 / boxSize];
	minY = new unsigned int[1280 / boxSize];

	for (unsigned int i = 0; i < 1280 / boxSize; i++) {
		minY[i] = 0;
		maxY[i] = 720/boxSize;
	}
}

Graph::~Graph() {
  for(unsigned int i=0; i<sizeX; i++)
    delete[] nodes[i];

  delete[] nodes;
}

void Graph::loadFile(std::string file) {
  std::ifstream stream(file.c_str());
  std::string line;
  bool startAlreadyFound=false;
  bool stopAlreadyFound=false;

  sizeX=sizeY=0;

  while(getline(stream, line)) {
    if(sizeY!=0 && line.size()!=sizeX)
      std::cout << "[WARNING] Lines " << sizeY << " and " << sizeY+1 << " have not the same length." << std::endl;

    sizeX=line.size();
    sizeY++;
  }

  nodes = new Node*[sizeX];
  for(unsigned int i=0; i<sizeX; i++)
    nodes[i] = new Node[sizeY];

  stream.clear();
  stream.seekg(0, ios::beg);
  for(unsigned int i=0; i<sizeY; i++) {
    getline(stream, line);
    for(unsigned int j=0; j<sizeX; j++) {
      if(j>=line.size())
        nodes[j][i].initialize(j, i, true);
      else {
        if(line[j]=='#')
          nodes[j][i].initialize(j, i, true);
        else if(line[j]=='S' && !startAlreadyFound) {
          Node::setFirstNode(j, i);
          nodes[j][i].initialize(j, i, false);
          nodes[j][i].pushInOpenedList();
          openList.push_back(&(nodes[j][i]));
          startAlreadyFound=true;
        }
        else if(line[j]=='A' && !stopAlreadyFound) {
          Node::setFinalNode(j, i);
          nodes[j][i].initialize(j, i, false);
          stopAlreadyFound=true;
        }
        else if(line[j]=='-' || line[j]=='.')
          nodes[j][i].initialize(j, i, false);
        else if(startAlreadyFound && line[j]=='S') {
          std::cout << "[WARNING] At least 2 start points have been found." << std::endl;
          nodes[j][i].initialize(j, i, false);
        }
        else if(stopAlreadyFound && line[j]=='A') {
          std::cout << "[WARNING] At least 2 stops points have been found." << std::endl;
          nodes[j][i].initialize(j, i, false);
        } else {
          std::cout << "[WARNING] Unexcepted char : " << line[j] << ". Considering '-'" << std::endl;
          nodes[j][i].initialize(j, i, false);
        }
      }
    }
  }
}

void Graph::initialize() {
  openList[0]->analyze(NULL, 0);
}

Node* Graph::getBetterNodeInOpenedList() {
	if (openList.size() == 0)
		return NULL;

	unsigned int i = 0;
	int idx = 0;
	float betterH = openList[0]->getH();
	Node* bestNode = openList[0];

	if (openList.size() > 1) {
		for (i = 1; i < openList.size(); i++)
			if (betterH > openList[i]->getH()) {
				betterH = openList[i]->getH();
				idx = i;
			}
	}

	bestNode = openList[idx];
	openList[idx]->pushInClosedList();

	int temp=0;
	Node* currNode=bestNode;
	if (bestNode->getCoordX() == 1280 / boxSize - 2) {
		while (currNode) {
			if (currNode->getCoordY() > (int)minY[currNode->getCoordX()]) {
				currNode->setAsMinimY(true);
				minY[currNode->getCoordX()] = currNode->getCoordY();
			}
			currNode = currNode->getP();
		}
	}
	currNode = bestNode;
	if (bestNode->getCoordY() == 720 / boxSize - 2) {
		while (currNode) {
			if (currNode->getCoordY() < (int)maxY[currNode->getCoordX()]) {
				currNode->setAsMinimY(true);
				maxY[currNode->getCoordX()] = currNode->getCoordY();
			}
			currNode = currNode->getP();
		}
	}

	openList.erase(openList.begin() + idx);

	return bestNode;
}

void Graph::getBetterNodeAroundCurrent(Node* currentNode) {
	int xAxis = 0, yAxis = 0;
	unsigned int newX, newY;
	int newG;

	for (int i = 0; i < 4; i++) {
		if (i == 0) { xAxis = 0; yAxis = -1; } //Up
		if (i == 1) { xAxis = 0; yAxis = 1; } //Down
		if (i == 2) { xAxis = -1; yAxis = 0; } //Left
		if (i == 3) { xAxis = 1; yAxis = 0; } //Right

		newX = currentNode->getCoordX() + xAxis;
		newY = currentNode->getCoordY() + yAxis;

		if (!(newX<0 || newY<0 || newX>sizeX - 1 || newY>sizeY - 1)) {
			if (newY > minY[newX] && newY < maxY[newX]) {
				if (!nodes[newX][newY].isAnObstacle() && !nodes[newX][newY].isInClosedList()) {
					//If the node isn't in the opened list
					if (!nodes[newX][newY].isInOpenedList()) {
						nodes[newX][newY].analyze(currentNode, 1);
						nodes[newX][newY].pushInOpenedList();
						openList.push_back(&(nodes[newX][newY]));
					}
					else { //If the node is already in the openedList
						newG = currentNode->getG() + 1;
						if (newG < nodes[newX][newY].getG())
							nodes[newX][newY].analyze(currentNode, 1);
					}
				}
			}
		}
	}
}

void Graph::findPath() {
	Graph::initialize();

	int status = 0;
	Node* node=NULL;
	Graph::draw();
	Sleep(1000);

	SDL_Event ev;
	Uint32 delay = SDL_GetTicks();
	skip = 0;

	while (!status) {
		node = Graph::getBetterNodeInOpenedList();

		if (node)
			getBetterNodeAroundCurrent(node);

		if (instant && count >= skip) {
			count = 0;

			int evExist = SDL_PollEvent(&ev);
			if (ev.key.keysym.sym == SDLK_LEFT && skip > 10 && evExist)
				skip-=10;
			if (ev.key.keysym.sym == SDLK_RIGHT && skip < 190 && evExist)
				skip+=10;
			if (ev.key.keysym.sym == SDLK_PAGEUP && evExist)
				skip = 200;
			if (ev.key.keysym.sym == SDLK_PAGEDOWN && evExist)
				skip = 10;

			draw();
			SDL_Delay(0);
		}
		count++;

		if (openList.size() == 0)
			status = 1;
		for (unsigned int i = 0; i < openList.size(); i++)
			if (openList[i]->getH() == 0)
				status = 2;
	}
	
	int pathSize = 0;
	while (node->getP() != NULL) {
		node->setAsFinalPath();
		node = node->getP();
		pathSize++;
	}

	if (status == 1)
		std::cout << "No path found..." << std::endl;
	if (status == 2)
		std::cout << "Path found in " << pathSize << " blocks ! (" << (int)(SDL_GetTicks()-delay) << " ms)" << std::endl;

	bool cont = false;
	
	if (dispWindow) {
		std::cout << "Press ESCAPE, SPACE or RETURN on the window to quit" << std::endl;
		while (!cont) {
			display();
			SDL_Delay(32);

			SDL_PollEvent(&ev);
			if (ev.key.keysym.sym == SDLK_ESCAPE || ev.key.keysym.sym == SDLK_RETURN || ev.key.keysym.sym == SDLK_SPACE)
				cont = 1;
		}
	}
	else
		system("PAUSE");
}

void Graph::draw() const {
	system("cls");

	if (dispWindow)
		display();

	else {
		for (unsigned int i = 0; i < sizeY; i++) {
			for (unsigned int j = 0; j < sizeX; j++) {
				std::cout << (nodes[j][i].symb()) << " ";
			}
			std::cout << std::endl;
		}
	}
}

void Graph::display() const {
	char symb;
	SDL_Rect rect;

	for (unsigned int i = 0; i < sizeX; i++) {
		for (unsigned int j = 0; j < sizeY; j++) {
			symb = nodes[i][j].symb();
			rect.x = i * boxSize;
			rect.y = j * boxSize;
			rect.w = boxSize;
			rect.h = boxSize;

			if (symb == '.')
				SDL_SetRenderDrawColor(drawingEl.pRenderer, COL_GREEN, 255);
			else if (symb == '#')
				SDL_SetRenderDrawColor(drawingEl.pRenderer, COL_BLACK, 255);
			else if (symb == 'X')
				SDL_SetRenderDrawColor(drawingEl.pRenderer, COL_RED, 255);
			else if (symb == '?')
				SDL_SetRenderDrawColor(drawingEl.pRenderer, COL_GREEN, 255);
			else if (symb == ' ')
				SDL_SetRenderDrawColor(drawingEl.pRenderer, COL_PURPLE, 255);
			else if (symb == 'S')
				SDL_SetRenderDrawColor(drawingEl.pRenderer, COL_BLUE, 255);
			else if (symb == 'A')
				SDL_SetRenderDrawColor(drawingEl.pRenderer, COL_BLUE, 255);
			else if (symb == 'M')
				SDL_SetRenderDrawColor(drawingEl.pRenderer, COL_DARK_PURPLE, 255);
			else
				SDL_SetRenderDrawColor(drawingEl.pRenderer, COL_BLACK, 255);

			SDL_RenderFillRect(drawingEl.pRenderer, &rect);
		}
	}

	SDL_RenderPresent(drawingEl.pRenderer);
}

void Graph::generateMaze() {
	//Randomized Prim's algorithm

	int deb = 0;
	int randWall;
	int wallAround;
	SDL_Rect rect;
	std::vector<mazePart*> wallVector;

	mazePart** parts = new mazePart*[1280/boxSize];
	if (parts == NULL) {
		std::cout << "[ERROR] Can't allocate memory." << std::endl;
	}
	for (unsigned int i = 0; i < 1280 / boxSize; i++) {
		parts[i] = new mazePart[720 / boxSize];
		if (parts[i] == NULL) {
			std::cout << "[ERROR] Can't allocate memory (" << i << ")" << std::endl;
		}
	}


	for (unsigned int i = 0; i < 1280 / boxSize; i++)
		for (unsigned int j = 0; j < 720 / boxSize; j++) {
			parts[i][j].type = '#';
			parts[i][j].coordX = i;
			parts[i][j].coordY = j;
		}

	parts[1][1].type = '.';
	wallVector.push_back(&parts[1][2]);
	wallVector.push_back(&parts[2][1]);

	mazePart* p;
	SDL_Event ev;
	int evExist;
	while (wallVector.size() != 0) {
		deb++;
		randWall = rand() % wallVector.size();
		wallAround = 0;
		p = wallVector[randWall];
		wallVector.erase(wallVector.begin() + randWall);

		if (p->coordX - 1 > 0)
			if (parts[p->coordX - 1][p->coordY].type == '.')
				wallAround++;

		if (p->coordX + 1 < 1280 / (unsigned short)boxSize - 1)
			if (parts[p->coordX + 1][p->coordY].type == '.')
				wallAround++;

		if (p->coordY - 1 > 0)
			if (parts[p->coordX][p->coordY - 1].type == '.')
				wallAround++;

		if (p->coordY + 1 < 720 / (unsigned short)boxSize - 1)
			if (parts[p->coordX][p->coordY + 1].type == '.')
				wallAround++;

		if (wallAround < 2) {
			parts[p->coordX][p->coordY].type = '.';
			if (p->coordX - 1 > 0)
				if (parts[p->coordX - 1][p->coordY].type == '#')
					wallVector.push_back(&parts[p->coordX - 1][p->coordY]);

			if (p->coordX + 1 < 1280 / (unsigned short)boxSize - 1)
				if (parts[p->coordX + 1][p->coordY].type == '#')
					wallVector.push_back(&parts[p->coordX + 1][p->coordY]);

			if (p->coordY - 1 > 0)
				if (parts[p->coordX][p->coordY - 1].type == '#')
					wallVector.push_back(&parts[p->coordX][p->coordY - 1]);

			if (p->coordY + 1 < 720 / (unsigned short)boxSize - 1)
				if (parts[p->coordX][p->coordY + 1].type == '#')
					wallVector.push_back(&parts[p->coordX][p->coordY + 1]);
		}

		if (instant && count>=skip) {
			for (unsigned int i = 0; i < 1280 / boxSize; i++) {
				for (unsigned int j = 0; j < 720 / boxSize; j++) {
					rect.x = i * boxSize;
					rect.y = j * boxSize;
					rect.w = boxSize;
					rect.h = boxSize;

					if(parts[i][j].type == '.')
						SDL_SetRenderDrawColor(drawingEl.pRenderer, COL_GREEN, 255);
					else if (parts[i][j].type == '#')
						SDL_SetRenderDrawColor(drawingEl.pRenderer, COL_BLACK, 255);

					evExist = SDL_PollEvent(&ev);
					if (ev.key.keysym.sym == SDLK_LEFT && skip > 0 && evExist)
						skip--;
					if (ev.key.keysym.sym == SDLK_RIGHT && skip < 500 && evExist)
						skip++;
					if (ev.key.keysym.sym == SDLK_PAGEUP && evExist)
						skip = 500;
					if (ev.key.keysym.sym == SDLK_PAGEDOWN && evExist)
						skip = 10;

					count = 0;

					SDL_RenderFillRect(drawingEl.pRenderer, &rect);
				}
			}

			SDL_RenderPresent(drawingEl.pRenderer);
		}
		count++;
	}

	parts[1][1].type = 'S';
	parts[1280 / boxSize - 2][720 / boxSize - 2].type = 'A';
	if (parts[1280 / boxSize - 3][720 / boxSize - 2].type == '#' && parts[1280 / boxSize - 2][720 / boxSize - 3].type == '#')
		parts[1280 / boxSize - 3][720 / boxSize - 2].type = '.';

	ofstream stream("./generatedMaze.txt");
	for (unsigned int j = 0; j < 720 / boxSize; j++) {
		for (unsigned int i = 0; i < 1280 / boxSize; i++) {
			stream << parts[i][j].type;
		}
		stream << std::endl;
	}

	for (unsigned int i = 0; i < 1280 / boxSize; i++)
		delete[] parts[i];
	delete[] parts;
}

//TODO : Accelerer / visualisation generation / Enlever SDL_Image / Ajouter Icone