#include <bits/stdc++.h>
using namespace std;

const bool IS_PART1 = false;

int N, M;
vector<string> grid;
const int DX[4] = {-1, 0, 1, 0};
const int DY[4] = {0, 1, 0, -1};
unordered_map<char, int> directionMap = {{'^', 0}, {'>', 1}, {'v', 2}, {'<', 3}};

bool isHorizontalMove(const int dir) {
   return dir == 1 || dir == 3;
}

bool isBoxChar(const char c) {
   return c == '[' || c == ']' || c == 'O';
}

//Used to move 1x1 blocks or the 1x2 blocks horizontally.
pair<int, int> processSingleWidthMove(const int px, const int py, const int nx, const int ny, const int dir) {
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

//Used to move the 1x2 blocks vertically.
bool processMultiWidthMove(const int px, const int py, const int dir, const bool performMove) {
   if (grid[px][py] == '#') return false;
   if (grid[px][py] == '.') return true;

   int ly, ry;
   if (grid[px][py] == '[')
      ly = py, ry = py + 1;
   else
      ly = py - 1, ry = py;

   int nx = px + DX[dir];
   int nly = ly + DY[dir];
   int nry = ry + DY[dir];

   bool isRightHalfProcessed = (grid[nx][nry] == ']');
   // Check feasibility for both parts (left and right) of current [] block to be moved.
   bool canMoveLeftHalf = processMultiWidthMove(nx, nly, dir, performMove);
   bool canMoveRightHalf = !isRightHalfProcessed ? processMultiWidthMove(nx, nry, dir, performMove) : true;

   if (!canMoveLeftHalf || !canMoveRightHalf) return false;

   if (performMove) {
      // Perform actual move
      grid[nx][nly] = grid[px][ly];
      grid[px][ly] = '.';
      grid[nx][nry] = grid[px][ry];
      grid[px][ry] = '.';
   }

   return true;
}

pair<int, int> move(const int px, const int py, const int dir) {
   int nx = px + DX[dir];
   int ny = py + DY[dir];
   if (grid[nx][ny] == '#') return {px, py};
   if (IS_PART1 || isHorizontalMove(dir)) {
      return processSingleWidthMove(px, py, nx, ny, dir);
   }
   else {
      if (processMultiWidthMove(nx, ny, dir, false)) {
         processMultiWidthMove(nx, ny, dir, true);
         grid[nx][ny] = '@';
         grid[px][py] = '.';
         return {nx, ny};
      }
      return {px, py};
   }
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   freopen("in.txt", "r", stdin);

   string line;
   while (getline(cin, line) && line != "") {
      string transformedLine = "";
      for (auto c : line) {
         if (!IS_PART1) {
            if (c == '#') transformedLine += "##";
            else if (c == '.') transformedLine += "..";
            else if (c == 'O') transformedLine += "[]";
            else transformedLine += "@.";
         }
         else {
            transformedLine = line;
         }
      }
      grid.push_back(transformedLine);
   }

   string moves;
   while (getline(cin, line)) {
      moves += line;
   }

   N = grid.size();
   M = grid[0].size();

   pair<int, int> pos;
   for (int i = 0; i < N; i++) {
      for (int j = 0; j < M; j++) {
         if (grid[i][j] == '@') {
            pos.first = i;
            pos.second = j;
            break;
         }
      }
   }

   for (auto m : moves) {
      pos = move(pos.first, pos.second, directionMap[m]);
   }

   long long ans = 0;

   for (int i = 0; i < N; i++) {
      for (int j = 0; j < M; j++) {
         if (grid[i][j] == '[' || grid[i][j] == 'O') ans += 100 * i + j;
      }
   }

   cout << ans << endl;

   return 0;
}
