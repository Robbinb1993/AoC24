#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;
const int DX[4] = {-1, 0, 1, 0};
const int DY[4] = {0, 1, 0, -1};

int N, M;
vector<string> grid;

struct State {
   int dist;
   int x, y, dir;

   bool operator<(const State& other) const {
      return dist > other.dist;
   }

   State(int dist, int x, int y, int dir)
      : dist(dist), x(x), y(y), dir(dir) {
   }
};

vector<vector<vector<vector<State>>>> parentStates;
vector<vector<vector<int>>> toDist;

int Dijkstra(const int sx, const int sy, const int ex, const int ey, const int dir) {
   State start(0, sx, sy, dir);
   toDist.assign(N, vector<vector<int>>(M, vector<int>(4, INF)));

   toDist[sx][sy][dir] = 0;
   int bestDist = INF;

   priority_queue<State> pq;
   pq.push(start);

   while (!pq.empty()) {
      auto curr = pq.top();
      pq.pop();

      int x = curr.x;
      int y = curr.y;
      int d = curr.dir;

      if (x == ex && y == ey) {
         if (curr.dist > bestDist)
            return bestDist;
         bestDist = curr.dist;
         continue;
      }
      if (toDist[x][y][d] < curr.dist) continue;

      int nx = x + DX[d];
      int ny = y + DY[d];

      if (nx >= 0 && nx < N && ny >= 0 && ny < M && grid[nx][ny] != '#') {
         if (toDist[nx][ny][d] >= toDist[x][y][d] + 1) {
            parentStates[nx][ny][d].push_back(State(toDist[x][y][d], x, y, d));
         }
         if (toDist[nx][ny][d] > toDist[x][y][d] + 1) {
            toDist[nx][ny][d] = toDist[x][y][d] + 1;
            pq.push(State(toDist[nx][ny][d], nx, ny, d));
         }
      }

      for (int i = -1; i <= 1; i++) {
         if (i == 0) continue;
         int nextDir = (d + i + 4) % 4;
         if (toDist[x][y][nextDir] >= toDist[x][y][d] + 1000) {
            parentStates[x][y][nextDir].push_back(State(toDist[x][y][d], x, y, d));
         }
         if (toDist[x][y][nextDir] > toDist[x][y][d] + 1000) {
            toDist[x][y][nextDir] = toDist[x][y][d] + 1000;
            pq.push(State(toDist[x][y][nextDir], x, y, nextDir));
         }
      }
   }

   return bestDist;
}

vector<vector<bool>> seen;
vector<vector<vector<bool>>> seenState;

void DFS(const int x, const int y, const int dir) {
   vector<tuple<int, int, int>> stack;

   stack.emplace_back(x, y, dir);

   seen[x][y] = true;
   seenState[dir][x][y] = true;

   while (!stack.empty()) {
      auto [cx, cy, cdir] = stack.back();
      stack.pop_back();

      seen[cx][cy] = true;

      for (auto& p : parentStates[cx][cy][cdir]) {
         int nx = p.x;
         int ny = p.y;
         int nd = p.dir;

         if (!seenState[nd][nx][ny]) {
            seenState[nd][nx][ny] = true;
            stack.emplace_back(nx, ny, nd);
         }
      }
   }
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   freopen("turnalot.txt", "r", stdin);

   string line;
   while (getline(cin, line)) {
      grid.push_back(line);
   }

   N = grid.size();
   M = grid[0].size();

   parentStates.assign(N, vector<vector<vector<State>>>(M, vector<vector<State>>(4)));

   int sx, sy, ex, ey;
   for (int i = 0; i < int(grid.size()); i++) {
      for (int j = 0; j < int(grid[i].size()); j++) {
         if (grid[i][j] == 'S') {
            sx = i;
            sy = j;
         }
         if (grid[i][j] == 'E') {
            ex = i;
            ey = j;
         }
      }
   }

   int minDist = Dijkstra(sx, sy, ex, ey, 1);

   seen.assign(N, vector<bool>(M, false));
   seenState.assign(4, vector<vector<bool>>(N, vector<bool>(M, false)));

   for (int i = 0; i < 4; i++) {
      if (toDist[ex][ey][i] == minDist)
         DFS(ex, ey, i);
   }

   int nodesOnBestRoute = 0;
   for (int i = 0; i < N; i++)
      for (int j = 0; j < M; j++)
         if (seen[i][j])
            nodesOnBestRoute++;

   cout << minDist << " " << nodesOnBestRoute << endl;

   return 0;
}
