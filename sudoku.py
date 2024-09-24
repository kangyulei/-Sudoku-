class Grid:
    def __init__(self, grid_str):
        self.BOX_SIZE = 3
        self.GRID_SIZE = 9
        self.grid = self.parse_grid(grid_str)

    def parse_grid(self, grid_str):
        """Parse the input string to create a 2D grid."""
        return [[int(grid_str[i * self.GRID_SIZE + j]) for j in range(self.GRID_SIZE)]
                for i in range(self.GRID_SIZE)]

    def get_row(self, row):
        """Get the specified row."""
        return self.grid[row]

    def get_column(self, col):
        """Get the specified column."""
        return [self.grid[row][col] for row in range(self.GRID_SIZE)]

    def get_box(self, row, col):
        """Get the 3x3 box containing the cell at (row, col)."""
        box_row_start = (row // self.BOX_SIZE) * self.BOX_SIZE
        box_col_start = (col // self.BOX_SIZE) * self.BOX_SIZE
        return [self.grid[r][c]
                for r in range(box_row_start, box_row_start + self.BOX_SIZE)
                for c in range(box_col_start, box_col_start + self.BOX_SIZE)]

class Sudoku(Grid):
    def get_inference(self):
        """Get possible candidates for each empty cell."""
        candidates = [[set(range(1, 10)) for _ in range(self.GRID_SIZE)] for _ in range(self.GRID_SIZE)]

        for row in range(self.GRID_SIZE):
            for col in range(self.GRID_SIZE):
                if self.grid[row][col] != 0:
                    candidates[row][col] = set()  # Cell is filled
                else:
                    # Eliminate values already present in the row, column, and box
                    for val in self.get_row(row):
                        candidates[row][col].discard(val)
                    for val in self.get_column(col):
                        candidates[row][col].discard(val)
                    for val in self.get_box(row, col):
                        candidates[row][col].discard(val)

        return candidates

# Example usage
if __name__ == "__main__":
    input_str = "017903600000080000900000507072010430000402070064370250701000065000030000005601720"
    sudoku = Sudoku(input_str)
    inferences = sudoku.get_inference()

    print("\nInferences (candidates for each empty cell):")
    for row in range(sudoku.GRID_SIZE):
        for col in range(sudoku.GRID_SIZE):
            if sudoku.get_row(row)[col] == 0:
                print(f"Cell ({row},{col}): {sorted(inferences[row][col])}")
