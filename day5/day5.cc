#include <bits/stdc++.h>
using namespace std;

int inOrder[100][100];
vector<int> edges[100];
int inDegree[100];

int topologicalSort(vector<int>& V) {
   unordered_map<int, bool> inList;
   memset(inDegree, 0, sizeof(inDegree));
   for (auto& v : V) {
      inList[v] = true;
   }
   for (auto& v : V) {
      for (auto& u : edges[v]) {
         inDegree[u]++;
      }
   }

   vector<int> stk;
   for (auto& v : V) {
      if (inDegree[v] == 0) {
         stk.push_back(v);
      }
   }

   int middleIdx = int(V.size()) / 2;
   int totSeen = 0;

   while (!stk.empty()) {
      int v = stk.back();
      if (totSeen == middleIdx) {
         return v;
      }
      totSeen++;
      stk.pop_back();
      for (auto& e : edges[v]) {
         if (inList[e]) {
            inDegree[e]--;
            if (inDegree[e] == 0) {
               stk.push_back(e);
            }
         }
      }
   }
   return -1;
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   freopen("aoc-2024-day-05-challenge-1.txt", "r", stdin);

   string line;
   while (getline(cin, line) && !line.empty()) {
      string left, right;
      stringstream ss(line);
      getline(ss, left, '|');
      getline(ss, right, '|');

      int v1 = stoi(left);
      int v2 = stoi(right);

      inOrder[v1][v2] = 1;
      edges[v1].push_back(v2);
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
         ans2 += topologicalSort(V);
      }
      else {
         ans1 += V[int(V.size()) / 2];
      }
   }

   cout << ans1 * ans2 << endl;

   return 0;
}
