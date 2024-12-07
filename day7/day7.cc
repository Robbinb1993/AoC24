#include <bits/stdc++.h>
using namespace std;
using namespace chrono;

bool possible(const int idx, const long long& rem, const vector<int>& V, const bool isPart2) {
   if (idx == 0) {
      return rem == V[0];
   }
   if (rem == 0 && V[idx] == 0) {
      return true;
   }
   if (rem >= V[idx]) {
      if (possible(idx - 1, rem - V[idx], V, isPart2)) {
         return true;
      }
   }
   if (V[idx] != 0 && rem % V[idx] == 0) {
      if (possible(idx - 1, rem / V[idx], V, isPart2)) {
         return true;
      }
   }
   if (isPart2) {
      string S = to_string(V[idx]);
      string M = to_string(rem);
      if (M.length() > S.length() && M.ends_with(S)) {
         M.erase(M.size() - S.size());
         if (possible(idx - 1, stoll(M), V, isPart2)) {
            return true;
         }
      }
   }

   return false;
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   auto start = high_resolution_clock::now();

   freopen("aoc-2024-day-07-challenge-3.txt", "r", stdin);

   vector<vector<int>> lists;
   vector<long long> totals;

   for (string line; getline(cin, line); ) {
      auto pos = line.find(':');
      if (pos == string::npos) {
         cerr << "Invalid format: " << line << '\n';
         continue;
      }
      totals.push_back(stoll(line.substr(0, pos)));

      vector<int> tempList;
      stringstream ss(line.substr(pos + 1));
      for (int val; ss >> val; ) tempList.push_back(val);

      lists.push_back(move(tempList));
   }

   long long ans[2] = {0, 0};
   for (int part = 1; part <= 2; part++) {
      for (int i = 0; i < int(totals.size()); i++) {
         if (possible(int(lists[i].size()) - 1, totals[i], lists[i], part == 2)) {
            ans[part - 1] += totals[i];
         }
      }
   }

   auto stop = high_resolution_clock::now();

   cout << ans[0] << " " << ans[1] << endl;
   cout << "Time taken: " << duration_cast<milliseconds>(stop - start).count() << "ms" << endl;

   return 0;
}
