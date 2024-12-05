#include <bits/stdc++.h>
using namespace std;

int inOrder[100][100];

bool order(vector<int>& V) {
   bool valid = true;
   for (int i = 0; i < int(V.size()) && valid; i++) {
      for (int j = i + 1; j < int(V.size()); j++) {
         if (inOrder[V[j]][V[i]]) {
            swap(V[i], V[j]);
            valid = false;
            break;
         }
      }
   }
   return valid;
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   freopen("aoc-2024-day-05-challenge-1.txt", "r", stdin);
   //get line until line is empty
   string line;

   while (getline(cin, line) && !line.empty()) {
      string left, right;
      stringstream ss(line);
      getline(ss, left, '|');
      getline(ss, right, '|');

      int v1 = stoi(left);
      int v2 = stoi(right);

      inOrder[v1][v2] = 1;
   }

   int ans1 = 0, ans2 = 0;

   while (getline(cin, line)) {
      stringstream ss(line);
      vector<int> V;
      string temp;

      while (getline(ss, temp, ',')) {
         V.push_back(stoi(temp));
      }
      bool valid = true;
      for (int i = 0; i < int(V.size()); i++) {
         for (int j = i + 1; j < int(V.size()); j++) {
            if (inOrder[V[j]][V[i]]) {
               valid = false;
               break;
            }
         }
      }
      if (!valid) {
         while (!order(V));
         ans2 += V[int(V.size()) / 2];
      }
      else {
         ans1 += V[int(V.size()) / 2];
      }
   }

   cout << ans1 * ans2 << endl;

   return 0;
}
