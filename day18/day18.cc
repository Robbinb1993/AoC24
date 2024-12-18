#include <bits/stdc++.h>
using namespace std;

const int N = 71;
const int M = 71;
const int INF = 1e9;
const int DX[4] = {-1, 0, 1, 0};
const int DY[4] = {0, 1, 0, -1};

vector<vector<char>> grid(N, vector<char>(M, '.'));
vector<pair<int, int>> bytes;

int BFS() {
   queue<pair<int, int>> Q;
   vector<vector<int>> dist(N, vector<int>(M, INF));
   dist[0][0] = 0;
   Q.emplace(0, 0);

   while (!Q.empty()) {
      auto [x, y] = Q.front();

      if (x == N - 1 && y == M - 1) {
         return dist[x][y];
      }

      Q.pop();

      for (int i = 0; i < 4; i++) {
         int nx = x + DX[i];
         int ny = y + DY[i];

         if (nx < 0 || nx >= N || ny < 0 || ny >= M) continue;
         if (grid[nx][ny] == '#') continue;

         if (dist[nx][ny] > dist[x][y] + 1) {
            dist[nx][ny] = dist[x][y] + 1;
            Q.emplace(nx, ny);
         }
      }
   }

   return -1;
}

int BFS(const int prevBytes, const int totBytes) {
   for (int i = prevBytes + 1; i <= totBytes; i++) {
      auto [x, y] = bytes[i];
      grid[x][y] = '#';
   }
   for (int i = totBytes + 1; i <= prevBytes; i++) {
      auto [x, y] = bytes[i];
      grid[x][y] = '.';
   }

   return BFS();
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   freopen("in.txt", "r", stdin);

   string input_line;

   while (getline(cin, input_line)) {
      stringstream ss(input_line);
      int first, second;
      char comma;
      while (ss >> first >> comma >> second) {
         bytes.emplace_back(first, second);
      }
   }

   int prv = -1;
   int L = 0;
   int R = int(bytes.size()) - 1;
   int ans;

   while (L <= R) {
      int mid = (L + R) / 2;
      if (BFS(prv, mid) != -1) {
         L = mid + 1;
         ans = mid;
      }
      else {
         R = mid - 1;
      }
      prv = mid;
   }

   cout << BFS(prv, 1024) << " " << bytes[ans + 1].first << " " << bytes[ans + 1].second << endl;

   return 0;
}
