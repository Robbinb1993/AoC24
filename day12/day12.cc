#include <bits/stdc++.h>
using namespace std;

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

pair<int, int> floodfill(const int x, const int y, const char c) {
   int sides = 0;

   for (int d = 0; d < 4; d++) {
      sides += getAndMarkLineSegment(x + DX[d], y + DY[d], d, c);
   }

   int area = 1;
   for (int i = 0; i < 4; i++) {
      int nx = x + DX[i];
      int ny = y + DY[i];
      if (grid[nx][ny] == c) {
         if (!seen[nx][ny]) {
            seen[nx][ny] = true;
            auto [s, a] = floodfill(nx, ny, c);
            area += a;
            sides += s;
         }
      }
   }

   return {sides, area};
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   freopen("in.txt", "r", stdin);
   string line;
   while (getline(cin, line)) {
      grid.push_back("*" + line + "*");
   }

   string border(grid[0].size(), '*');
   grid.insert(grid.begin(), border);
   grid.push_back(border);

   N = grid.size();
   M = grid[0].size();
   seen = vector<vector<bool>>(N, vector<bool>(M, false));

   for (int i = 0; i < 4; i++) {
      lineSegmentSeen[i].assign(N, vector<int>(M, 0));
   }

   long long ans = 0;

   for (int i = 1; i < N - 1; i++) {
      for (int j = 1; j < M - 1; j++) {
         if (!seen[i][j]) {
            seen[i][j] = true;
            runItr++;
            auto [sides, area] = floodfill(i, j, grid[i][j]);
            ans += area * sides;
         }
      }
   }

   cout << ans << endl;

   return 0;
}
