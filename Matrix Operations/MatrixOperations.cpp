#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct StrCounterPair {
	string str;
	int ctr;
	StrCounterPair(string str, int ctr) : str(str), ctr(ctr) {}
};

struct Cell {
	int row;
	int col;
	Cell(int row, int col) : row(row), col(col) {}
};


Cell findNextCell(Cell currentCell, int cur, int rowLength) {
	int r = currentCell.row, c = currentCell.col;
	switch (cur)
	{
	case 0: //go right
		c += 1;
		if (c >= rowLength) {
			r = -1;
			c = -1;
		}
		break;
	case 1: //go down
		r += 1;
		if (r >= rowLength) {
			r = -1;
			c = -1;
		}
		break;
	case 2: //go left
		c -= 1;
		if (c < 0) {
			r = -1;
			c = -1;
		}
		break;
	case 3: //go up
		r -= 1;
		if (r < 0) {
			r = -1;
			c = -1;
		}
		break;
	default:
		break;
	}
	return Cell(r, c);
}



void insertToVector(vector<StrCounterPair> & scp, string toInsert) {
	bool found = false;
	for (int i = 0; i < scp.size(); i++) {
		if (scp[i].str == toInsert) {
			found = true;
			scp[i].ctr += 1;
		}
	}
	if (!found) {
		scp.push_back(StrCounterPair(toInsert, 1));
	}
}

void printVector(vector<StrCounterPair> scp) {
	cout << "Given matrix includes: " << endl;
	for (int i = 0; i < scp.size(); i++) {
		cout << "[+] " << scp[i].str << " exists " << scp[i].ctr << " many times." << endl;
	}
}

int main() {
	ifstream ifs;
	string fileName, line;
	vector<vector<char>> inputMatrix;
	int givenRowCount, rowCount, wantedStringLength;

	cout << "Please enter the input file name: ";
	cin >> fileName;

	ifs.open(fileName);
	while (ifs.fail()) {  //checks if the file exists and successfully opened
		cout << "Couldn’t open file. Please enter again: ";
		cin >> fileName;
		ifs.open(fileName);
	}

	getline(ifs, line); //first line contains the row count (equal to column count) of matrix
	givenRowCount = stoi(line);
	inputMatrix.resize(givenRowCount);

	getline(ifs, line);
	rowCount = 0;
	while (line != "") {
		rowCount += 1;
		if (line.length() != givenRowCount) {
			cout << "Input file is not as expected." << endl;
			return 0;
		}
		for (int i = 0; i < line.length(); i++) { // construct the matrix
			if (line.at(i) <= 'Z' && line.at(i) >= 'A') {
				inputMatrix[rowCount - 1].push_back(line.at(i));
			}
			else {
				cout << "Input file is not as expected." << endl;
				return 0;
			}
		}
		getline(ifs, line);
	}

	if (rowCount != givenRowCount) {
		cout << "Input file is not as expected." << endl;
	}
	else {
		getline(ifs, line);
		wantedStringLength = stoi(line);

		vector<StrCounterPair> scp; //initialize the vector to store the result list
		for (int r = 0; r < rowCount; r++) {
			for (int c = 0; c < rowCount; c++) {
				Cell cell(r, c);

				for (int i = 0; i < (2 << (2 * wantedStringLength - 3)); i++) {
					int tmp = i;
					string newWord = "";
					vector<vector<int>> mapMarking(rowCount, vector<int>(rowCount, 0)); // vector to mark the points visited for a single word
					newWord += inputMatrix[r][c];
					mapMarking[r][c] = 1;
					Cell nextCell = cell;
					for (int j = 0; j < wantedStringLength - 1; j++) {
						int cur = tmp % 4;
						tmp /= 4;
						nextCell = findNextCell(nextCell, cur, rowCount);
						if (nextCell.row != -1 && mapMarking[nextCell.row][nextCell.col] != 1) { //next cell is reachable and not visited
							char toAppend = inputMatrix[nextCell.row][nextCell.col];
							newWord += toAppend;
							mapMarking[nextCell.row][nextCell.col] = 1;
						}
						else {
							break;
						}
					}
					if (newWord.length() == wantedStringLength) {
						insertToVector(scp, newWord);
					}
				}
			}
		}
		printVector(scp);
	}
	return 0;
}
