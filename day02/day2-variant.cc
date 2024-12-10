#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

const int MAXN = 200000;
const int MAXV = 1000000;
const int OFFSET = 3;
const int IS_INCREASING = 0;
const int IS_DECREASING = 1;

int nxt[MAXV + 1];
int seen[MAXV + OFFSET + 1];
int DP[2][MAXN + 1];

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   auto start = high_resolution_clock::now();

   freopen("aoc-2024-day-2-challenge-3.txt", "r", stdin);

   int totAns = 0;
   int runItr = 1;
   string line;
   while (getline(cin, line)) {
      stringstream ss(line);
      string token;
      vector<int> numbers;

      int v;
      while (ss >> v) {
         numbers.push_back(v);
      }

      int N = int(numbers.size());

      int ans = INT_MAX;
      for (int i = N - 1; i >= 0; i--) {
         for (int dir = 0; dir <= 1; dir++) {
            int best = N - i - 1;

            for (int j = 1; j <= 3; j++) {
               int v = numbers[i] + (dir ? -j : j);
               if (seen[v + OFFSET] == runItr) {
                  int dist = nxt[v] - i;
                  int totElementsInBetween = dist - 1;
                  best = min(best, totElementsInBetween + DP[dir][nxt[v]]);
               }
            }
            DP[dir][i] = best;
         }

         ans = min(ans, i + min(DP[IS_INCREASING][i], DP[IS_DECREASING][i]));
         nxt[numbers[i]] = i;
         seen[numbers[i] + OFFSET] = runItr;
      }

      totAns += ans;
      runItr++;
   }

   auto stop = high_resolution_clock::now();
   auto duration = duration_cast<milliseconds>(stop - start);

   cout << totAns << endl;
   cout << "Time taken: " << duration.count() << " milliseconds" << endl;

   return 0;
}
