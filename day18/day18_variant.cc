#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

const int N = 2501;
const int M = 2501;
const int DX[4] = {-1, 0, 1, 0};
const int DY[4] = {0, 1, 0, -1};

vector<pair<int, int>> bytes;
bool seen[N][M];
bool touchingSeen[N][M];
char grid[N][M];

bool floodFill(const int startX, const int startY) {
   vector<pair<int, int>> stk;
   stk.emplace_back(startX, startY);
   seen[startX][startY] = true;

   while (!stk.empty()) {
      auto [x, y] = stk.back();
      stk.pop_back();

      if (x == 0 && y == 0)
         return true;

      for (int i = 0; i < 4; i++) {
         int nx = x + DX[i];
         int ny = y + DY[i];

         if (nx >= 0 && nx < N && ny >= 0 && ny < M && !seen[nx][ny]) {
            if (grid[nx][ny] == '.') {
               seen[nx][ny] = true;
               stk.emplace_back(nx, ny);
            }
            else {
               touchingSeen[nx][ny] = true;
            }
         }
      }
   }

   return false;
}

int solve() {
   for (int i = 0; i < N; i++) {
      for (int j = 0; j < M; j++) {
         grid[i][j] = '.';
      }
   }
   for (auto [x, y] : bytes) {
      grid[x][y] = '#';
   }

   floodFill(N - 1, M - 1);

   for (size_t i = bytes.size() - 1; i >= 0; i--) {
      auto [x, y] = bytes[i];
      grid[x][y] = '.';

      for (int j = 0; j < 4; j++) {
         if (touchingSeen[x][y] && floodFill(x, y)) {
            return i;
         }
      }
   }

   return -1;
}

void fastInput() {
   char c;
   int num = 0;
   bool isX = true;
   pair<int, int> currentPair;

   freopen("aoc-2024-day-18-challenge-4-2500x2500.txt", "r", stdin);

   //Use getchar_unlocked on Linux
   while ((c = _getchar_nolock()) != EOF) {
      if (c >= '0' && c <= '9') {
         num = num * 10 + (c - '0');
      }
      else if (c == ',') {
         currentPair.first = num;
         num = 0;
         isX = false;
      }
      else if (c == '\n') {
         currentPair.second = num;
         bytes.emplace_back(currentPair);
         num = 0;
         isX = true;
      }
   }

   if (!isX) {
      currentPair.second = num;
      bytes.emplace_back(currentPair);
   }
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   auto start = high_resolution_clock::now();

   fastInput();

   int ans = solve();

   auto stop = high_resolution_clock::now();
   auto duration = duration_cast<milliseconds>(stop - start);

   cout << bytes[ans].first << " " << bytes[ans].second << " " << bytes[ans].first * bytes[ans].second << endl;
   cout << "Time: " << duration.count() << " milliseconds" << endl;

   return 0;
}
