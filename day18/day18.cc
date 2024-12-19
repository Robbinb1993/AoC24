#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

const int N = 2501;
const int M = 2501;
const int INF = 1e9;
const int DX[4] = {-1, 0, 1, 0};
const int DY[4] = {0, 1, 0, -1};

class DSU {
private:
   vector<int> parent, size;

public:
   DSU(const int sz) {
      parent.assign(sz, 0);
      size.assign(sz, 1);
      iota(parent.begin(), parent.end(), 0);
   }
   int getSetSize(int v);
   int findSetParent(int v);
   void uniteSets(int a, int b);
};

int DSU::getSetSize(int v) {
   int p = findSetParent(v);
   return size[p];
}

int DSU::findSetParent(int v) {
   if (v == parent[v])
      return v;
   return parent[v] = findSetParent(parent[v]);
}

void DSU::uniteSets(int a, int b) {
   a = findSetParent(a);
   b = findSetParent(b);
   if (a != b) {
      if (size[a] < size[b])
         swap(a, b);
      parent[b] = a;
      size[a] += size[b];
   }
}

vector<vector<char>> grid(N, vector<char>(M, '.'));
vector<pair<int, int>> bytes;
DSU dsu(N* M);

int solve() {
   for (auto [x, y] : bytes) {
      grid[x][y] = '#';
   }

   for (int x = 0; x < N; x++) {
      for (int y = 0; y < M; y++) {
         if (grid[x][y] == '#')
            continue;
         for (int k = 0; k < 4; k++) {
            int nx = x + DX[k];
            int ny = y + DY[k];
            if (nx >= 0 && nx < N && ny >= 0 && ny < M && grid[nx][ny] == '.') {
               int fromId = x * M + y;
               int toId = nx * M + ny;
               dsu.uniteSets(fromId, toId);
            }
         }
      }
   }

   if (dsu.findSetParent(0) == dsu.findSetParent(N * M - 1))
      return int(bytes.size());

   for (int i = int(bytes.size()) - 1; i >= 0; i--) {
      auto [x, y] = bytes[i];
      grid[x][y] = '.';
      for (int k = 0; k < 4; k++) {
         int nx = x + DX[k];
         int ny = y + DY[k];

         if (nx >= 0 && nx < N && ny >= 0 && ny < M && grid[nx][ny] == '.') {
            int fromId = x * M + y;
            int toId = nx * M + ny;
            dsu.uniteSets(fromId, toId);
         }
      }
      if (dsu.findSetParent(0) == dsu.findSetParent(N * M - 1))
         return i;
   }

   return -1;
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   auto start = high_resolution_clock::now();

   freopen("aoc-2024-day-18-challenge-3-2500x2500.txt", "r", stdin);

   string input_line;

   while (getline(cin, input_line)) {
      stringstream ss(input_line);
      int first, second;
      char comma;
      while (ss >> first >> comma >> second) {
         bytes.emplace_back(first, second);
      }
   }

   int ans = solve();

   auto stop = high_resolution_clock::now();
   auto duration = duration_cast<milliseconds>(stop - start);

   cout << bytes[ans].first << " " << bytes[ans].second << " " << bytes[ans].first * bytes[ans].second << endl;
   cout << "Time: " << duration.count() << " milliseconds" << endl;

   return 0;
}
