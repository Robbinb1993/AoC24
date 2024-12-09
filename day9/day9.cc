#include <bits/stdc++.h>
using namespace std;


int main() {
   freopen("in.txt", "r", stdin);

   string line;
   getline(cin, line);

   cout << line << endl;

   vector<int> M;
   int id = 0;

   for (int i = 0; i < int(line.size()); i++) {
      int v = line[i] - '0';

      if (i % 2 == 0) {
         for (int j = 0; j < v; j++) {
            M.push_back(id);
         }
         id++;
      }
      else {
         for (int j = 0; j < v; j++) {
            M.push_back(-1);
         }
      }
   }

   set<pair<int, int>> isFree;
   int idx = 0;
   while (idx < int(M.size())) {
      if (M[idx] == -1) {
         int totFree = 0;
         int st = idx;
         while (idx < int(M.size()) && M[idx] == -1) {
            totFree++;
            idx++;
         }
         isFree.insert({st, totFree});
      }
      else {
         idx++;
      }
   }

   idx = int(M.size()) - 1;
   while (idx >= 0) {
      if (M[idx] != -1) {
         int req = 0;
         int st = idx;
         while (idx >= 0 && M[idx] == M[st]) {
            req++;
            idx--;
         }

         for (auto it = isFree.begin(); it != isFree.end(); ) {
            if (it->first > st) {
               break;
            }
            if (it->second >= req) {
               for (int i = 0; i < req; i++) {
                  swap(M[it->first + i], M[st - i]);
               }
               int remFree = it->second - req;
               int nxtFree = it->first + req;
               it = isFree.erase(it);
               if (remFree > 0) {
                  isFree.insert({nxtFree, remFree});
               }
               break;
            }
            else {
               it++;
            }
         }
      }
      else {
         idx--;
      }
   }

   long long ans = 0;

   for (int i = 0; i < int(M.size()); i++) {
      if (M[i] != -1) {
         ans += i * M[i];
      }
   }

   cout << ans << endl;

   return 0;
}
