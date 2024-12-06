#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

int R, C;
vector<string> grid;
vector<vector<array<int, 4>>> nxt, visited;

const int DX[4] = {-1, 0, 1, 0};
const int DY[4] = {0, 1, 0, -1};

int oldVal[4];
int runItr = 1;

int oppositeDir(int d) {
   return (d + 2) % 4;
}

void preprocess() {
   R = (int)grid.size();
   C = (int)grid[0].size();
   nxt.assign(R, vector<array<int, 4>>(C, {-1,-1,-1,-1}));
   visited.assign(R, vector<array<int, 4>>(C, {0,0,0,0}));

   for (int dir = 0; dir < 4; dir++) {
      if (dir == 0) { // UP
         for (int j = 0; j < C; j++) {
            int lastObstacle = -2;
            for (int i = 0; i < R; i++) {
               if (grid[i][j] == '#') {
                  lastObstacle = i;
               }
               else {
                  nxt[i][j][0] = lastObstacle + 1;
               }
            }
         }
      }
      else if (dir == 1) { // RIGHT
         for (int i = 0; i < R; i++) {
            int lastObstacle = C + 1;
            for (int j = C - 1; j >= 0; j--) {
               if (grid[i][j] == '#') {
                  lastObstacle = j;
               }
               else {
                  nxt[i][j][1] = lastObstacle - 1;
               }
            }
         }
      }
      else if (dir == 2) { // DOWN
         for (int j = 0; j < C; j++) {
            int lastObstacle = R + 1;
            for (int i = R - 1; i >= 0; i--) {
               if (grid[i][j] == '#') {
                  lastObstacle = i;
               }
               else {
                  nxt[i][j][2] = lastObstacle - 1;
               }
            }
         }
      }
      else { // LEFT
         for (int i = 0; i < R; i++) {
            int lastObstacle = -2;
            for (int j = 0; j < C; j++) {
               if (grid[i][j] == '#') {
                  lastObstacle = j;
               }
               else {
                  nxt[i][j][3] = lastObstacle + 1;
               }
            }
         }
      }
   }
}

void addOrRemoveObstacle(int x, int y, int d, bool adding) {
   int od = oppositeDir(d);

   int r = x + DX[od], c = y + DY[od];

   bool firstCell = true;
   while (r >= 0 && r < R && c >= 0 && c < C && grid[r][c] != '#') {
      if (firstCell && adding) {
         oldVal[d] = nxt[r][c][d];
         firstCell = false;
      }

      if (adding) {
         if (d == 0) nxt[r][c][0] = x + 1;
         else if (d == 1) nxt[r][c][1] = y - 1;
         else if (d == 2) nxt[r][c][2] = x - 1;
         else nxt[r][c][3] = y + 1;
      }
      else {
         nxt[r][c][d] = oldVal[d];
      }

      r += DX[od];
      c += DY[od];
   }
}

void addObstacle(int x, int y) {
   // Place the obstacle
   grid[x][y] = '#';

   // Update each direction by walking in the opposite direction
   for (int d = 0; d < 4; d++) {
      addOrRemoveObstacle(x, y, d, true);
   }
}

void removeObstacle(int x, int y) {
   // Remove the obstacle
   grid[x][y] = '.';

   // Revert changes for each direction
   for (int d = 0; d < 4; d++) {
      addOrRemoveObstacle(x, y, d, false);
   }
}

int part1(int x, int y) {
   int d = 0;
   int ans = 0;
   while (true) {
      if (x < 0 || x >= R || y < 0 || y >= C) {
         return ans;
      }
      if (grid[x][y] != 'X') {
         ans++;
         grid[x][y] = 'X';
      }
      while (true) {
         x += DX[d];
         y += DY[d];
         if ((x >= 0 && x < R && y >= 0 && y < C) && grid[x][y] == '#') {
            x -= DX[d];
            y -= DY[d];
            d = (d + 1) % 4;
            continue;
         }
         break;
      }
   }
   throw("Should not reach here.");
}


bool part2(int x, int y, int d) {
   while (true) {
      if (x < 0 || x >= R || y < 0 || y >= C) {
         return false; // Escaped
      }
      if (visited[x][y][d] == runItr) {
         return true; // Cycle detected
      }
      visited[x][y][d] = runItr;

      int nextVal = nxt[x][y][d];
      int nxtX, nxtY;
      if (d == 0 || d == 2) { // up or down
         nxtX = nextVal;
         nxtY = y;
      }
      else { // right or left
         nxtX = x;
         nxtY = nextVal;
      }

      d = (d + 1) % 4; // rotate direction

      x = nxtX;
      y = nxtY;
   }
   throw("Should not reach here.");
}


int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   auto start = high_resolution_clock::now();

   freopen("aoc-2024-day-06-challenge-3.txt", "r", stdin);

   string line;
   while (cin >> line) {
      grid.push_back(line);
   }

   preprocess();

   int sx, sy;
   for (int i = 0; i < R; i++) {
      for (int j = 0; j < C; j++) {
         if (grid[i][j] == '^') {
            sx = i;
            sy = j;
         }
      }
   }

   cout << part1(sx, sy) << endl;

   int ansPart2 = 0;

   for (int i = 0; i < R; i++) {
      for (int j = 0; j < C; j++) {
         if (grid[i][j] != 'X') {
            continue;
         }
         addObstacle(i, j);
         ansPart2 += part2(sx, sy, 0);
         runItr++;
         removeObstacle(i, j);
      }
   }

   cout << ansPart2 << endl;

   auto stop = high_resolution_clock::now();
   auto duration = duration_cast<milliseconds>(stop - start);

   cout << "Time taken: " << duration.count() << " milliseconds" << endl;

   return 0;
}
