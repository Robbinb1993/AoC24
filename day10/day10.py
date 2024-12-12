import sys
from functools import lru_cache

def read_grid_from_stdin():
    return [line.strip() for line in sys.stdin if line.strip()]

grid = read_grid_from_stdin()

N, M = len(grid), len(grid[0])

DX = [-1, 1, 0, 0]
DY = [0, 0, -1, 1]

@lru_cache(None)
def solve(x, y):
    if grid[x][y] == '9':  # Base case
        return 1

    total_paths = 0
    for dx, dy in zip(DX, DY):  # Check all 4 directions
        nx, ny = x + dx, y + dy
        if 0 <= nx < N and 0 <= ny < M and int(grid[nx][ny]) == int(grid[x][y]) + 1:
            total_paths += solve(nx, ny)

    return total_paths

# Compute total paths for cells starting with '0'
ans_part2 = sum(solve(i, j) for i in range(N) for j in range(M) if grid[i][j] == '0')

print("Total paths starting with '0':", ans_part2)
