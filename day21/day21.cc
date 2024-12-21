#include <bits/stdc++.h>
using namespace std;

const int MAX_LEVEL = 25;
const long long INF = 1e15;
const vector<vector<char>> NUM_PAD = {
    {'7', '8', '9'},
    {'4', '5', '6'},
    {'1', '2', '3'},
    {' ', '0', 'A'}
};

const vector<vector<char>> KEY_PAD = {
    {' ', '^', 'A'},
    {'<', 'v', '>'}
};

char getCharFromId(int id) {
   return "^>v<A0123456789"[id];
}

int moveId[256];
vector<string> minPaths[15][15];

pair<int, int> findLocation(const vector<vector<char>>& pad, char key) {
   for (int i = 0; i < pad.size(); ++i) {
      for (int j = 0; j < pad[i].size(); ++j) {
         if (pad[i][j] == key) {
            return {i, j};
         }
      }
   }
   return {-1, -1};
}

void genMinPaths(const int from, const int to, const vector<vector<char>>& pad) {
   if (from == to && minPaths[from][to].empty()) {
      minPaths[from][to].push_back("A");
      return;
   }

   auto [r1, c1] = findLocation(pad, getCharFromId(from));
   auto [r2, c2] = findLocation(pad, getCharFromId(to));

   int dr = r2 - r1;
   int dc = c2 - c1;

   vector<string> paths;

   if (pad[r2][c1] != ' ') {
      paths.push_back(string(abs(dr), dr > 0 ? 'v' : '^') +
         string(abs(dc), dc > 0 ? '>' : '<'));
   }

   if (pad[r1][c2] != ' ') {
      paths.push_back(string(abs(dc), dc > 0 ? '>' : '<') +
         string(abs(dr), dr > 0 ? 'v' : '^'));
   }

   for (const auto& path : paths) {
      string newPath = path;
      if (minPaths[from][to].empty() || minPaths[from][to].back() != newPath) {
         minPaths[from][to].push_back(newPath + "A");
      }
   }
}

void init() {
   moveId['^'] = 0;
   moveId['>'] = 1;
   moveId['v'] = 2;
   moveId['<'] = 3;
   moveId['A'] = 4;
   for (char c = '0'; c <= '9'; c++) {
      moveId[c] = 5 + c - '0';
   }
   for (int i = 0; i <= 4; i++) {
      for (int j = 0; j <= 4; j++) {
         genMinPaths(i, j, KEY_PAD);
      }
   }
   for (int i = 4; i <= 14; i++) {
      for (int j = 4; j <= 14; j++) {
         genMinPaths(i, j, NUM_PAD);
      }
   }
}

long long DP[15][15][MAX_LEVEL + 1];

long long solve(const int from, const int to, const int level) {
   if (level > MAX_LEVEL) {
      return 1;
   }

   if (DP[from][to][level] != -1) {
      return DP[from][to][level];
   }

   long long bestDist = INF;
   for (auto& path : minPaths[from][to]) {
      //After every previous move performed a layer higher, the current robot will be at the 'A' position as he had to 
      //perform the actual move of the robot a layer up by pressing the 'A' key on the keypad. If the first move is yet 
      //to be performed, we know the robot will be at the starting position 'A' aswell. Thus we should always first move
      //the current robot from 'A' to the start of the next path.
      long long currDist = solve(moveId['A'], moveId[path[0]], level + 1);
      //Now we will reposition the robot a layer higher to the next move required and press 'A' at the end 
      for (int i = 0; i < int(path.size()) - 1; i++) {
         currDist += solve(moveId[path[i]], moveId[path[i + 1]], level + 1);
      }
      bestDist = min(bestDist, currDist);
   }

   return DP[from][to][level] = bestDist;
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   init();

   auto start = chrono::high_resolution_clock::now();

   freopen("in.txt", "r", stdin);

   vector<string> codes;
   string code;
   while (cin >> code) {
      codes.push_back(code);
   }

   memset(DP, -1, sizeof(DP));

   long long ans = 0;

   for (int i = 0; i < int(codes.size()); i++) {
      string code = codes[i];

      long long totDist = solve(moveId['A'], moveId[code[0]], 0);
      for (int i = 0; i < int(code.size()) - 1; i++) {
         totDist += solve(moveId[code[i]], moveId[code[i + 1]], 0);
      }

      int codeVal = 0;
      for (auto& c : code) {
         if (isdigit(c))
            codeVal = codeVal * 10 + c - '0';
      }

      ans += codeVal * totDist;
   }

   auto end = chrono::high_resolution_clock::now();
   auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

   cout << ans << endl;
   cout << "Time: " << duration.count() << " ms" << endl;

   return 0;
}
