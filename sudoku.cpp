#include <iostream>
#include <vector>
#include <set>
#include <sstream>

using namespace std;

const int BOX_SIZE = 3;
const int GRID_SIZE = 9;

class Grid {
protected:
    vector<vector<int> > grid;
public:
    Grid() : grid(GRID_SIZE, vector<int>(GRID_SIZE, 0)) {}

    // Initialize grid with a string input
    Grid(const string& input) {
        grid.resize(GRID_SIZE, vector<int>(GRID_SIZE, 0));
        for (int i = 0; i < GRID_SIZE * GRID_SIZE; ++i) {
            grid[i / GRID_SIZE][i % GRID_SIZE] = input[i] - '0';
        }
    }

    // Get row from the grid
    vector<int> getRow(int row) const {
        return grid[row];
    }

    // Get column from the grid
    vector<int> getColumn(int col) const {
        vector<int> column(GRID_SIZE);
        for (int i = 0; i < GRID_SIZE; ++i) {
            column[i] = grid[i][col];
        }
        return column;
    }

    // Get box from the grid based on row and column
    vector<int> getBox(int row, int col) const {
        vector<int> box;
        int startRow = (row / BOX_SIZE) * BOX_SIZE;
        int startCol = (col / BOX_SIZE) * BOX_SIZE;
        for (int i = 0; i < BOX_SIZE; ++i) {
            for (int j = 0; j < BOX_SIZE; ++j) {
                box.push_back(grid[startRow + i][startCol + j]);
            }
        }
        return box;
    }

    // Display the grid for debugging
	void display() const {
	    for (size_t i = 0; i < grid.size(); ++i) {
	        for (size_t j = 0; j < grid[i].size(); ++j) {
	            cout << grid[i][j] << " ";
	        }
	        cout << endl;
	    }
	}

};

class Sudoku : public Grid {
public:
    Sudoku(const string& input) : Grid(input) {}

    // Get possible candidates for each empty cell (0 represents an empty cell)
    vector<vector<set<int> > > getInference() const {
        vector<vector<set<int> > > candidates(GRID_SIZE, vector<set<int> >(GRID_SIZE));
        
        for (int row = 0; row < GRID_SIZE; ++row) {
            for (int col = 0; col < GRID_SIZE; ++col) {
                if (grid[row][col] == 0) {
                    // Get candidates for empty cell
                    set<int> possibleValues = getPossibleValues(row, col);
                    candidates[row][col] = possibleValues;
                }
            }
        }

        return candidates;
    }

    // Clone the current Sudoku board
	Sudoku clone() const {
	    stringstream ss;
	    for (int row = 0; row < grid.size(); ++row) {
	        for (int col = 0; col < grid[row].size(); ++col) {
	            ss << grid[row][col];
	        }
	    }
	    return Sudoku(ss.str());
	}

private:
    set<int> getPossibleValues(int row, int col) const {
        set<int> possibleValues;
		for (int i = 1; i <= 9; ++i) {
		    possibleValues.insert(i);
		}

        // Eliminate values already present in the row
		const vector<int>& rowValues = getRow(row);
		for (vector<int>::const_iterator it = rowValues.begin(); it != rowValues.end(); ++it) {
		    possibleValues.erase(*it);
		}

		// Eliminate values already present in the column
		const vector<int>& columnValues = getColumn(col);
		for (vector<int>::const_iterator it = columnValues.begin(); it != columnValues.end(); ++it) {
		    possibleValues.erase(*it);
		}
		
		// Eliminate values already present in the box
		const vector<int>& boxValues = getBox(row, col);
		for (vector<int>::const_iterator it = boxValues.begin(); it != boxValues.end(); ++it) {
		    possibleValues.erase(*it);
		}

        return possibleValues;
    }
};

// Test function
void testSudokuSolver() {
    string puzzle = "017903600000080000900000507072010430000402070064370250701000065000030000005601720";

    Sudoku sudoku(puzzle);
    cout << "Initial Sudoku Grid:" << endl;
    sudoku.display();

    // Get inferences
	vector<vector<set<int> > > inferences = sudoku.getInference();

    // Display candidates for each empty cell
	cout << "\nInferences (candidates for each empty cell):" << endl;
	for (int row = 0; row < GRID_SIZE; ++row) {
	    for (int col = 0; col < GRID_SIZE; ++col) {
	        if (sudoku.getRow(row)[col] == 0) {
	            cout << "Cell (" << row << "," << col << "): ";
	            // 使用迭代器遍历 inferences[row][col]
	            std::set<int>::const_iterator it = inferences[row][col].begin();
	            while (it != inferences[row][col].end()) {
	                cout << *it << " ";
	                ++it;
	            }
	            cout << endl;
	        }
	    }
	}

    // Clone test
    Sudoku clonedSudoku = sudoku.clone();
    cout << "\nCloned Sudoku Grid (should match the initial grid):" << endl;
    clonedSudoku.display();
}

int main() {
    testSudokuSolver();
    return 0;
}

