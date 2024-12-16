#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

static const int DX[4] = {-1, 0, 1, 0};
static const int DY[4] = {0, 1, 0, -1};
static int directionMap[128];

inline bool isHorizontalMove(const int dir) {
   return dir == 1 || dir == 3;
}

inline bool isBoxChar(const char c) {
   return c == '[' || c == ']' || c == 'O';
}

int N, M;
vector<vector<char>> grid;
vector<vector<int>> seen;
int runItr = 0;

inline pair<int, int> processSingleWidthMove(const int px, const int py, const int nx, const int ny, const int dir) {
   int ex = nx;
   int ey = ny;
   while (isBoxChar(grid[ex][ey])) {
      ex += DX[dir];
      ey += DY[dir];
   }
   if (grid[ex][ey] == '#') return {px, py};
   while (!(ex == nx && ey == ny)) {
      grid[ex][ey] = grid[ex - DX[dir]][ey - DY[dir]];
      ex -= DX[dir];
      ey -= DY[dir];
   }
   grid[nx][ny] = '@';
   grid[px][py] = '.';
   return {nx, ny};
}

inline pair<int, int> processMultiWidthMove(const int px, const int py, const int dir) {
   runItr++;
   vector<pair<int, int>> movedTo;
   movedTo.reserve(2000);
   queue<pair<int, int>> Q;
   Q.emplace(px + DX[dir], py);

   while (!Q.empty()) {
      auto [cx, cy] = Q.front();
      Q.pop();

      if (seen[cx][cy] == runItr) continue;
      seen[cx][cy] = runItr;

      movedTo.emplace_back(cx, cy);

      if (grid[cx][cy] == '#') return {px, py};
      if (grid[cx][cy] == '.') continue;
      if (grid[cx][cy] == ']') cy--;

      int nextX = cx + DX[dir];
      Q.emplace(nextX, cy);
      Q.emplace(nextX, cy + 1);
   }

   for (int i = (int)movedTo.size() - 1; i >= 0; i--) {
      auto [bx, by] = movedTo[i];
      grid[bx][by] = grid[bx - DX[dir]][by];
      grid[bx - DX[dir]][by] = '.';
   }

   return {px + DX[dir], py};
}

inline pair<int, int> movePlayer(const int px, const int py, const int dir, const bool isPart1) {
   int nx = px + DX[dir];
   int ny = py + DY[dir];
   if (grid[nx][ny] == '#') return {px, py};
   if (isPart1 || isHorizontalMove(dir)) {
      return processSingleWidthMove(px, py, nx, ny, dir);
   }
   else {
      return processMultiWidthMove(px, py, dir);
   }
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   auto start = chrono::high_resolution_clock::now();

   directionMap['^'] = 0;
   directionMap['>'] = 1;
   directionMap['v'] = 2;
   directionMap['<'] = 3;

   freopen("aoc-2024-day-15-challenge-4.txt", "r", stdin);

   string line;
   vector<vector<char>> tempGrid;
   while (true) {
      if (!std::getline(cin, line) || line.empty()) break;
      tempGrid.emplace_back(line.begin(), line.end());
   }
   grid = move(tempGrid);


   auto gridCopy = grid;

   string moves;
   string line;
   while (std::getline(cin, line)) {
      moves += line;
   }

   for (int part = 1; part <= 2; part++) {
      if (part == 2) {
         for (int l = 0; l < (int)gridCopy.size(); l++) {
            string transformedLine;
            transformedLine.reserve(gridCopy[l].size() * 2);
            for (auto c : gridCopy[l]) {
               if (c == '#') transformedLine += "##";
               else if (c == '.') transformedLine += "..";
               else if (c == 'O') transformedLine += "[]";
               else transformedLine += "@.";
            }
            vector<char> newRow(transformedLine.begin(), transformedLine.end());
            grid[l] = move(newRow);
         }
      }

      N = (int)grid.size();
      M = (int)grid[0].size();

      seen.assign(N, vector<int>(M, 0));

      pair<int, int> pos;
      {
         bool found = false;
         for (int i = 0; i < N && !found; i++) {
            for (int j = 0; j < M && !found; j++) {
               if (grid[i][j] == '@') {
                  pos = {i, j};
                  found = true;
               }
            }
         }
      }

      for (auto m : moves) {
         pos = movePlayer(pos.first, pos.second, directionMap[m], part == 1);
      }

      long long ans = 0;
      for (int i = 0; i < N; i++) {
         for (int j = 0; j < M; j++) {
            char c = grid[i][j];
            if (c == '[' || c == 'O') ans += 100LL * i + j;
         }
      }

      cout << ans << "\n";

      if (part == 1) {
         grid = gridCopy;
      }
   }

   auto stop = high_resolution_clock::now();
   auto duration = duration_cast<milliseconds>(stop - start);
   cout << "Time taken: "
      << duration.count() << " milliseconds" << endl;

   return 0;
}
