#include <bits/stdc++.h>
using namespace std;
using namespace chrono;

const int MAX_N = 500000;
const int MAX_LIST_SIZE = 100;
int DP[MAX_LIST_SIZE][MAX_N];
int runItr = 1;

vector<int> P;

bool possible(const int idx, const long long rem, const vector<int>& V, const bool isPart2) {
   if (idx == 0) {
      return rem == V[0];
   }
   if (rem == 0 && V[idx] == 0) {
      return true;
   }
   if (rem < MAX_N) {
      if (DP[idx][rem] == runItr) return false;
      DP[idx][rem] = runItr;
   }
   if (isPart2) {
      long long p = P[idx];
      if (rem >= p && rem % p == V[idx]) {
         if (possible(idx - 1, rem / p, V, isPart2)) return true;
      }
   }
   if (V[idx] != 0 && rem % V[idx] == 0 && possible(idx - 1, rem / V[idx], V, isPart2)) return true;
   if (rem >= V[idx] && possible(idx - 1, rem - V[idx], V, isPart2)) return true;
   return false;
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   auto start = high_resolution_clock::now();

   freopen("aoc-2024-day-07-challenge-2.txt", "r", stdin);

   vector<vector<int>> lists;
   vector<long long> totals;

   for (string line; getline(cin, line); ) {
      auto pos = line.find(':');
      totals.push_back(stoll(line.substr(0, pos)));

      vector<int> tempList;
      stringstream ss(line.substr(pos + 1));
      for (int val; ss >> val; ) tempList.push_back(val);

      lists.push_back(move(tempList));
   }

   long long ans[2] = {0, 0};
   for (int part = 1; part <= 2; part++) {
      for (int i = 0; i < int(totals.size()); i++) {
         if (part == 2) {
            P.assign(lists[i].size(), 0);
            for (int j = 0; j < (int)lists[i].size(); j++) {
               int suffix = lists[i][j];
               long long p = (suffix == 0) ? 10 : 1;
               for (int x = suffix; x > 0; x /= 10) p *= 10;
               P[j] = p;
            }
         }
         if (possible(int(lists[i].size()) - 1, totals[i], lists[i], part == 2)) {
            ans[part - 1] += totals[i];
         }
         runItr++;
      }
   }

   auto stop = high_resolution_clock::now();

   cout << ans[0] << " " << ans[1] << endl;
   cout << "Time taken: " << duration_cast<milliseconds>(stop - start).count() << "ms" << endl;

   return 0;
}
