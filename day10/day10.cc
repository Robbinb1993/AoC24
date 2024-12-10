#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

const int DX[4] = {0, 0, 1, -1};
const int DY[4] = {1, -1, 0, 0};

int N, M;
vector<string> grid;
vector<vector<int>> seen, DP;
int runItr;

int solvePart1(const int x, const  int y) {
   if (seen[x][y] == runItr) {
      return 0;
   }
   seen[x][y] = runItr;
   if (grid[x][y] == '9') {
      return 1;
   }

   int tot = 0;
   for (int i = 0; i < 4; i++) {
      int nx = x + DX[i];
      int ny = y + DY[i];
      if (nx >= 0 && nx < N && ny >= 0 && ny < M && grid[nx][ny] == grid[x][y] + 1) {
         tot += solvePart1(nx, ny);
      }
   }

   return tot;
}

int solvePart2(const int x, const int y) {
   if (grid[x][y] == '9') {
      return 1;
   }
   if (DP[x][y] != -1) {
      return DP[x][y];
   }
   int tot = 0;
   for (int i = 0; i < 4; i++) {
      int nx = x + DX[i];
      int ny = y + DY[i];
      if (nx >= 0 && nx < N && ny >= 0 && ny < M && grid[nx][ny] == grid[x][y] + 1) {
         tot += solvePart2(nx, ny);
      }
   }

   return DP[x][y] = tot;
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   auto start = high_resolution_clock::now();

   freopen("aoc-2024-day-10-challenge-1.txt", "r", stdin);

   string line;
   while (getline(cin, line)) {
      grid.push_back(line);
   }

   N = grid.size();
   M = grid[0].size();

   seen.assign(N, vector<int>(M, 0));
   DP.assign(N, vector<int>(M, -1));

   int ansPart1 = 0, ansPart2 = 0;
   for (int i = 0; i < N; i++) {
      for (int j = 0; j < M; j++) {
         if (grid[i][j] == '0') {
            runItr++;
            ansPart1 += solvePart1(i, j);
            ansPart2 += solvePart2(i, j);
         }
      }
   }

   auto stop = high_resolution_clock::now();
   auto duration = duration_cast<milliseconds>(stop - start);

   cout << ansPart1 << " " << ansPart2 << endl;
   cout << "Time: " << duration.count() << " milliseconds" << endl;

   return 0;
}
