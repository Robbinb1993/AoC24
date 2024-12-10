#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

const int DX[8] = {1, -1, -1, 1, 1, 0, -1, 0};
const int DY[8] = {1, 1, -1, -1, 0, 1, 0, -1};

vector<string> grid;
int N, M;

int solvePart1() {
   int totXmas = 0;
   string target = "XMAS";
   for (int i = 0; i < N; i++) {
      for (int j = 0; j < M; j++) {
         if (grid[i][j] == 'X') {
            for (int k = 0; k < 8; k++) {
               int x = i, y = j;
               for (int p = 1; p < 4; p++) {
                  x += DX[k];
                  y += DY[k];
                  if (x < 0 || x >= N || y < 0 || y >= M) {
                     break;
                  }
                  if (grid[x][y] != target[p]) {
                     break;
                  }
                  if (p == 3) {
                     totXmas++;
                  }
               }
            }
         }
      }
   }
   return totXmas;
}

int solvePart2() {
   int totXmas = 0;
   string target = "MAS";
   vector<vector<bool>> visited(N, vector<bool>(M, false));
   for (int i = 0; i < N; i++) {
      for (int j = 0; j < M; j++) {
         if (grid[i][j] == 'M') {
            for (int k = 0; k < 4; k++) {
               int x = i, y = j;
               for (int p = 1; p < 3; p++) {
                  x += DX[k];
                  y += DY[k];
                  if (x < 0 || x >= N || y < 0 || y >= M) {
                     break;
                  }
                  if (grid[x][y] != target[p]) {
                     break;
                  }
                  if (p == 2) {
                     int xA = i + DX[k], yA = j + DY[k];
                     if (visited[xA][yA]) {
                        totXmas++;
                     }
                     else {
                        visited[xA][yA] = true;
                     }
                  }
               }
            }
         }
      }
   }
   return totXmas;
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   auto start = high_resolution_clock::now();

   freopen("aoc-2024-day-04-challenge-1.txt", "r", stdin);
   string line;

   while (cin >> line) {
      grid.push_back(line);
   }

   N = int(grid.size());
   M = grid[0].size();

   int ansPart1 = solvePart1();
   int ansPart2 = solvePart2();

   auto end = high_resolution_clock::now();
   auto totalDuration = duration_cast<milliseconds>(end - start).count();


   cout << "Elapsed time: " << totalDuration << "ms" << endl;
   cout << "Ans part 1: " << ansPart1 << endl;
   cout << "Ans part 2: " << ansPart2 << endl;

   return 0;
}
