#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

const int INF = 1e9;
const int DX[4] = {-1, 0, 1, 0};
const int DY[4] = {0, 1, 0, -1};
const int MAX_EDGE_WEIGHT = 1000;
const int TOT_BUCKETS = MAX_EDGE_WEIGHT + 1;

int N, M;
vector<string> grid;
vector<int> bestDist;
vector<vector<int>> bucketQueue;

inline int getId(const int x, const int y, const int d) {
   return (x * M + y) * 4 + d;
}

int Dijkstra(const int sx, const int sy, const int ex, const int ey, const int dir) {
   bestDist.assign(N * M * 4, INF);
   vector<bool> visited(N * M * 4, false);

   int startId = getId(sx, sy, dir);
   bestDist[startId] = 0;

   bucketQueue.assign(TOT_BUCKETS, vector<int>());
   bucketQueue[0].push_back(startId);

   int idx = 0;
   int lastAdded = 0;

   while (idx - lastAdded <= MAX_EDGE_WEIGHT) {
      auto& bucket = bucketQueue[idx % TOT_BUCKETS];
      while (!bucket.empty()) {
         int currId = bucket.back();
         bucket.pop_back();

         if (visited[currId]) continue;
         visited[currId] = true;

         int currDist = bestDist[currId];
         int x = currId / (M * 4);
         int y = (currId / 4) % M;
         int d = currId % 4;

         if (x == ex && y == ey) {
            return currDist;
         }

         int nx = x + DX[d];
         int ny = y + DY[d];
         if (grid[nx][ny] != '#') {
            int nextId = getId(nx, ny, d);
            if (bestDist[nextId] > bestDist[currId] + 1) {
               bestDist[nextId] = bestDist[currId] + 1;
               lastAdded = idx;
               bucketQueue[bestDist[nextId] % TOT_BUCKETS].push_back(nextId);
            }
         }

         for (int i = -1; i <= 1; i += 2) {
            int nextDir = (d + i + 4) % 4;
            int nextId = getId(x, y, nextDir);
            int newDist = bestDist[currId] + 1000;
            if (bestDist[nextId] > newDist) {
               bestDist[nextId] = newDist;
               lastAdded = idx;
               bucketQueue[newDist % TOT_BUCKETS].push_back(nextId);
            }
         }
      }
      idx++;
   }

   return INF;
}

vector<vector<bool>> seen;
vector<int> seenState;

void DFS(const int endId) {
   vector<int> stack;
   stack.push_back(endId);

   while (!stack.empty()) {
      int currId = stack.back();
      stack.pop_back();

      int x = currId / (M * 4);
      int y = (currId / 4) % M;
      int dir = currId % 4;

      seen[x][y] = true;

      int nx = x - DX[dir];
      int ny = y - DY[dir];
      int nxtId = getId(nx, ny, dir);
      if (grid[nx][ny] != '#' && !seenState[nxtId]) {
         seenState[nxtId] = true;
         if (bestDist[nxtId] == bestDist[currId] - 1) {
            stack.push_back(nxtId);
         }
      }

      for (int i = -1; i <= 1; i += 2) {
         int nextDir = (dir + i + 4) % 4;
         int nxtId = getId(x, y, nextDir);
         if (!seenState[nxtId]) {
            seenState[nxtId] = true;
            if (bestDist[nxtId] == bestDist[currId] - 1000) {
               stack.push_back(nxtId);
            }
         }
      }
   }
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   auto start = high_resolution_clock::now();

   freopen("maze-large.txt", "r", stdin);

   string line;
   while (getline(cin, line)) {
      grid.push_back(line);
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
         if (grid[i][j] == 'E') {
            ex = i;
            ey = j;
         }
      }
   }

   int minDist = Dijkstra(sx, sy, ex, ey, 1);

   seen.assign(N, vector<bool>(M, false));
   seenState.assign(4 * N * M, 0);

   for (int i = 0; i < 4; i++) {
      int endId = getId(ex, ey, i);
      if (bestDist[endId] == minDist)
         DFS(endId);
   }

   int nodesOnBestRoute = 0;
   for (int i = 0; i < N; i++)
      for (int j = 0; j < M; j++)
         if (seen[i][j])
            nodesOnBestRoute++;

   cout << minDist << " " << nodesOnBestRoute << endl;

   auto stop = high_resolution_clock::now();
   auto duration = duration_cast<milliseconds>(stop - start);
   cout << "Time passed: " << duration.count() << "ms" << endl;

   return 0;
}
