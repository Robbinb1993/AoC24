#include <bits/stdc++.h>
using namespace std;

const int N = 101;
const int M = 103;

int ctr[2][2];

vector<int> PX, PY, VX, VY;
int seen[N][M];
int runItr;

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   int px, py, vx, vy;

   string line;
   while (getline(cin, line)) {
      istringstream lineStream(line);
      char discard;

      lineStream >> discard >> discard >> px >> discard >> py;
      lineStream >> discard >> discard >> vx >> discard >> vy;

      PX.push_back(px);
      PY.push_back(py);

      VX.push_back(vx);
      VY.push_back(vy);
   }

   cin.clear();

   while (true) {
      runItr++;
      int totSeen = 0;

      for (int i = 0; i < PX.size(); i++) {
         PX[i] = ((PX[i] + VX[i]) % N + N) % N;
         PY[i] = ((PY[i] + VY[i]) % M + M) % M;
         if (seen[PX[i]][PY[i]] == runItr) {
            totSeen++;
         }
         seen[PX[i]][PY[i]] = runItr;
      }


      vector<vector<char>> grid(N, vector<char>(M, ' '));
      for (int i = 0; i < PX.size(); i++) {
         grid[PX[i]][PY[i]] = '#';
      }

      string targetPattern = "#################################";
      int patternLength = targetPattern.size();

      for (int i = 0; i < N; i++) {
         string row(grid[i].begin(), grid[i].end());
         if (row.find(targetPattern) != string::npos) {
            cout << "Pattern found after " << runItr << " seconds " << endl;
            for (int i = 0; i < N; i++) {
               for (int j = 0; j < M; j++) {
                  cout << grid[i][j];
               }
               cout << endl;
            }

            cout << "Press Enter to continue or type 'x' to stop: ";
            char c = cin.get(); // Wait for a single keypress
            if (c == 'x') {
               break; // Exit condition
            }

            // Clear the buffer if needed
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
         }
      }
   }

   return 0;
}
