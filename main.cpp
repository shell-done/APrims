#include "graphe.hpp"

using namespace std;

int main(int argc, char** argv) {
	string file="generatedMaze.txt";
	string seed="test";
	int choice;
	int boxSize = 0;
	char type;
	bool instant;

	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cout << "[ERROR] Can't start SDL." << endl;
		SDL_ShowCursor(SDL_DISABLE);

		return 1;
	}

	cout << "Maze generator and solver - Prim's & A* Algorithms - Alexandre THOMAS (ISEN YNCREA OUEST RENNES)" << endl;
	cout << "------------------------------------------------------------------------------------------------" << endl;
	cout << "What do you want to do ?" << endl;
	cout << "\t1.Generate random maze and solve it" << endl;
	cout << "\t2.Generate random maze with a specified box size and solve it" << endl;
	cout << "\t3.Generate maze based on a seed and solve it" << endl;
	cout << "\t4.Load maze file and solve it" << endl;
	cout << "\t5.Quit" << endl;

	do {
		cout << " > ";
		cin >> choice;
		cin.ignore();

		if (!(choice > 0 && choice < 6)) {
			cout << "Invalid choice" << endl;
		}
	} while (!(choice > 0 && choice < 6));

	switch (choice) {
	case 1:
		seed = "NOSEED_NOBOXSIZE";
		break;

	case 2:
		seed = "NOSEED";
		cout << "Choose boxes size between 1 and 5" << endl;
		cout << " > ";
		cin >> boxSize;
		cin.ignore();

		if (boxSize < 1) {
			cout << "Wrong box size (<1) - Set to 1" << endl;
			boxSize = 1;
		}
		else if (boxSize > 5) {
			cout << "Wrong box size (>5) - Set to 5" << endl;
			boxSize = 5;
		}
		boxSize = (unsigned int)pow(2, boxSize + 1);
		break;

	case 3:
		cout << "Enter a seed" << endl;
		cout << " > ";
		cin >> seed;
		cin.ignore();
		break;

	case 4:
		cout << "Enter the file name" << endl;
		cout << " > ";
		cin >> file;
		cin.ignore();

		break;
	
	case 5:
		cout << "Quitting..." << endl;
		SDL_Quit();
		return 0;
		break;

	default:
		cout << "[ERROR] Impossible option" << endl;
		return 1;
	}

	ifstream stream(file.c_str());
	if (!stream && choice == 4) {
		cout << "Can't find file " << file << ".\nQuitting" << endl;
		system("PAUSE");
		SDL_Quit();

		stream.close();
		return 1;
	}

	cout << "Do you want to see the progression of algorithms ? (o/n)" << endl;
	cout << " > ";
	cin >> type;
	cin.ignore();
	if (type == 'O' || type == 'o')
		instant = true;
	else if (type == 'N' || type == 'n')
		instant = false;
	else {
		cout << "Invalid choice. Set on 'N'" << endl;
		instant = false;
	}

	Graph graph(true, seed, boxSize, instant);
	graph.generateMaze();
	graph.loadFile(file);

	graph.findPath();

	SDL_Quit();
	return 0;
}