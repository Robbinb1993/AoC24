#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   auto start = high_resolution_clock::now();

   freopen("in.txt", "r", stdin);
   vector<string> grid;
   string line;
   while (getline(cin, line)) {
      grid.push_back(line);
   }
   vector<vector<bool>> isAntinode(grid.size(), vector<bool>(grid[0].size(), 0));

   map<char, vector<pair<int, int>>> antennas;
   for (int i = 0; i < grid.size(); i++) {
      for (int j = 0; j < grid[i].size(); j++) {
         if (grid[i][j] != '.') {
            antennas[grid[i][j]].push_back({i, j});
         }
      }
   }

   for (auto& [antenna, p] : antennas) {
      for (int i = 0; i < int(p.size()); i++) {
         for (int j = i + 1; j < int(p.size()); j++) {
            int dx = p[i].first - p[j].first;
            int dy = p[i].second - p[j].second;
            int nx = p[i].first;
            int ny = p[i].second;
            do {
               isAntinode[nx][ny] = true;
               nx += dx;
               ny += dy;
            } while (nx >= 0 && nx < grid.size() && ny >= 0 && ny < grid[0].size());


            nx = p[j].first;
            ny = p[j].second;
            do {
               isAntinode[nx][ny] = true;
               nx -= dx;
               ny -= dy;
            } while (nx >= 0 && nx < grid.size() && ny >= 0 && ny < grid[0].size());
         }
      }
   }

   int totAntinodes = 0;
   for (int i = 0; i < grid.size(); i++) {
      for (int j = 0; j < grid[i].size(); j++) {
         totAntinodes += (isAntinode[i][j] == true);
      }
   }

   auto stop = high_resolution_clock::now();

   cout << "Total antinodes: " << totAntinodes << endl;
   auto duration = duration_cast<milliseconds>(stop - start);
   cout << "Execution time: " << duration.count() << "ms" << endl;

   return 0;
}
