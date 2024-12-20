#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

const int DX[4] = {-1, 0, 1, 0};
const int DY[4] = {0, 1, 0, -1};
const int DIST_DIFF_REQ = 100;

int N, M;
vector<vector<char>> grid;
vector<vector<int>> distStart, distEnd;

int BFS(const int sx, const int sy, const int ex, const int ey, vector<vector<int>>& dist) {
   dist.assign(N, vector<int>(M, -1));

   queue<pair<int, int>> Q;
   Q.push({sx, sy});
   dist[sx][sy] = 0;

   while (!Q.empty()) {
      auto [x, y] = Q.front();
      Q.pop();

      for (int i = 0; i < 4; i++) {
         int nx = x + DX[i];
         int ny = y + DY[i];

         if (nx >= 0 && nx < grid.size() && ny >= 0 && ny < grid[0].size() && grid[nx][ny] != '#' && dist[nx][ny] == -1) {
            dist[nx][ny] = dist[x][y] + 1;
            Q.push({nx, ny});
         }
      }
   }

   return dist[ex][ey];
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   auto start = high_resolution_clock::now();

   freopen("aoc-2024-day-20-challenge-3.txt", "r", stdin);
   string line;
   while (getline(cin, line)) {
      grid.push_back(vector<char>(line.begin(), line.end()));
   }

   N = grid.size();
   M = grid[0].size();

   int sx, sy, ex, ey;
   for (int i = 0; i < N; i++) {
      for (int j = 0; j < M; j++) {
         if (grid[i][j] == 'S') {
            sx = i;
            sy = j;
         }
         else if (grid[i][j] == 'E') {
            ex = i;
            ey = j;
         }
      }
   }

   int minDist = BFS(sx, sy, ex, ey, distStart);
   BFS(ex, ey, sx, sy, distEnd);

   for (int part = 1; part <= 2; part++) {
      int ans = 0;
      int MAX_CHEAT_DIST = part == 1 ? 2 : 20;
      for (int x = 0; x < N; x++) {
         for (int y = 0; y < M; y++) {
            if (distStart[x][y] == -1) {
               continue;
            }
            for (int dx = -MAX_CHEAT_DIST; dx <= MAX_CHEAT_DIST; dx++) {
               int xDist = abs(dx);
               int from = -MAX_CHEAT_DIST + xDist;
               int to = MAX_CHEAT_DIST - xDist;
               for (int dy = from; dy <= to; dy++) {
                  if (dx == 0 && dy == 0) {
                     continue;
                  }
                  int nx = x + dx;
                  int ny = y + dy;

                  if (nx < 0 || nx >= N || ny < 0 || ny >= M || grid[nx][ny] == '#') {
                     continue;
                  }

                  if (distEnd[nx][ny] == -1) {
                     continue;
                  }

                  int dist = xDist + abs(dy);
                  int currDist = distStart[x][y] + dist + distEnd[nx][ny];
                  if (minDist - currDist >= DIST_DIFF_REQ) {
                     ans++;
                  }
               }
            }
         }
      }

      cout << ans << endl;
   }

   auto stop = high_resolution_clock::now();
   auto duration = duration_cast<milliseconds>(stop - start);

   cout << duration.count() << "ms" << endl;

   return 0;
}
