#include <bits/stdc++.h>
using namespace std;

const int DX[4] = {-1, 0, 1, 0};
const int DY[4] = {0, 1, 0, -1};
const int INF = 1e9;
const int MAX_CHEAT_DIST = 20;
const int DIST_DIFF_REQ = 100;

int N, M;
vector<vector<char>> grid;
vector<vector<int>> distStart, distEnd;
vector<vector<bool>> cheated;

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

   freopen("in.txt", "r", stdin);
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

   int ans = 0;
   for (int i = 0; i < N; i++) {
      for (int j = 0; j < M; j++) {
         if (grid[i][j] == '#') {
            continue;
         }
         for (int k = -MAX_CHEAT_DIST; k <= MAX_CHEAT_DIST; k++) {
            int kDist = abs(k);
            for (int l = -MAX_CHEAT_DIST + kDist; l <= MAX_CHEAT_DIST - kDist; l++) {
               int nx = i + k;
               int ny = j + l;

               if (nx < 0 || nx >= N || ny < 0 || ny >= M || grid[nx][ny] == '#') {
                  continue;
               }

               int dist = abs(nx - i) + abs(ny - j);

               if (dist == 0 || dist > MAX_CHEAT_DIST || distStart[i][j] == -1 || distEnd[nx][ny] == -1) {
                  continue;
               }

               int currDist = distStart[i][j] + dist + distEnd[nx][ny];
               if (minDist - currDist >= DIST_DIFF_REQ) {
                  ans++;
               }
            }
         }
      }
   }

   cout << ans << endl;

   return 0;
}
