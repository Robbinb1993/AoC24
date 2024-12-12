#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

const int DX[4] = {1, -1, 0, 0};
const int DY[4] = {0, 0, 1, -1};

int N, M;
vector<vector<bool>> seen;
vector<vector<int>> lineSegmentSeen[4];
int runItr = 0;
vector<string> grid;

bool getAndMarkLineSegment(const int x, const int y, const int dir, const char pChar) {
   char cChar = grid[x][y];
   if (cChar == pChar) {
      return false;
   }

   bool isNewLine = lineSegmentSeen[dir][x][y] != runItr;
   if (!isNewLine) {
      return false;
   }

   auto markLine = [&](int cx, int cy, int stepX, int stepY) {
      while (cx >= 0 && cx < N && cy >= 0 && cy < M) {
         int px = cx - DX[dir];
         int py = cy - DY[dir];
         if (grid[px][py] != pChar || grid[cx][cy] == pChar) {
            break;
         }
         lineSegmentSeen[dir][cx][cy] = runItr;
         cx += stepX;
         cy += stepY;
      }
      };

   if (dir <= 1) { //Horizontally touching neighbours
      markLine(x, y + DY[2], 0, DY[2]);
      markLine(x, y + DY[3], 0, DY[3]);
   }
   else { // Vertically touching neighbours
      markLine(x + DX[0], y, DX[0], 0);
      markLine(x + DX[1], y, DX[1], 0);
   }

   lineSegmentSeen[dir][x][y] = runItr;
   return isNewLine;
}

tuple<int, int, int> floodfill(const int startX, const int startY, const char c) {
   vector<pair<int, int>> stack;
   stack.push_back({startX, startY});
   seen[startX][startY] = true;

   int area = 0;
   int sides = 0;
   int perimeter = 0;

   while (!stack.empty()) {
      auto [x, y] = stack.back();
      stack.pop_back();
      area++;

      for (int d = 0; d < 4; d++) {
         int nx = x + DX[d];
         int ny = y + DY[d];
         if (grid[nx][ny] != c) {
            perimeter++;
            sides += getAndMarkLineSegment(nx, ny, d, c);
         }
         else if (!seen[nx][ny]) {
            seen[nx][ny] = true;
            stack.push_back({nx, ny});
         }
      }
   }

   return {perimeter, sides, area};
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   auto start = high_resolution_clock::now();

   freopen("combined-1.txt", "r", stdin);
   string line;
   while (getline(cin, line)) {
      grid.push_back("*" + line + "*");
   }

   string border(grid[0].size(), '*');
   grid.insert(grid.begin(), border);
   grid.push_back(border);

   N = grid.size();
   M = grid[0].size();
   seen.assign(N, vector<bool>(M, false));

   for (int i = 0; i < 4; i++) {
      lineSegmentSeen[i].assign(N, vector<int>(M, 0));
   }

   long long ans1 = 0;
   long long ans2 = 0;

   for (int i = 1; i < N - 1; i++) {
      for (int j = 1; j < M - 1; j++) {
         if (!seen[i][j]) {
            seen[i][j] = true;
            runItr++;
            auto [perimeter, sides, area] = floodfill(i, j, grid[i][j]);
            ans1 += (long long)perimeter * area;
            ans2 += (long long)area * sides;
         }
      }
   }

   auto stop = high_resolution_clock::now();
   auto duration = duration_cast<milliseconds>(stop - start);

   cout << ans1 << " " << ans2 << endl;
   cout << "Time taken: "
      << duration.count() << "ms" << endl;

   return 0;
}
