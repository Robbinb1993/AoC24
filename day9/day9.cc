#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

vector<pair<int, int>> freeBlocks;
vector<int> freeSpace, startPos;

void build(const int L, const int R, const int idx) {
   if (L == R) {
      freeSpace[idx] = freeBlocks[L].second;
      startPos[idx] = freeBlocks[L].first;
      return;
   }
   int M = (L + R) / 2;
   build(L, M, 2 * idx);
   build(M + 1, R, 2 * idx + 1);

   freeSpace[idx] = max(freeSpace[2 * idx], freeSpace[2 * idx + 1]);
}

void upd(const int p, const int remSpace, const int L, const int R, const int idx) {
   if (L == R) {
      int filledSpace = freeSpace[idx] - remSpace;
      startPos[idx] += filledSpace;
      freeSpace[idx] = remSpace;
      return;
   }
   int M = (L + R) / 2;
   if (p <= M) {
      upd(p, remSpace, L, M, 2 * idx);
   }
   else {
      upd(p, remSpace, M + 1, R, 2 * idx + 1);
   }
   freeSpace[idx] = max(freeSpace[2 * idx], freeSpace[2 * idx + 1]);
}

tuple<int, int, int> query(const int reqSpace, const int L, const int R, const int idx) {
   if (freeSpace[idx] < reqSpace) {
      return {-1, -1, -1};
   }
   if (L == R) {
      return {startPos[idx], freeSpace[idx], L};
   }
   if (freeSpace[2 * idx] >= reqSpace) {
      return query(reqSpace, L, (L + R) / 2, 2 * idx);
   }
   else {
      return query(reqSpace, (L + R) / 2 + 1, R, 2 * idx + 1);
   }
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   auto start = high_resolution_clock::now();

   freopen("in.txt", "r", stdin);

   string line;
   getline(cin, line);

   vector<int> M;
   int id = 0;

   for (int i = 0; i < int(line.size()); i++) {
      int v = line[i] - '0';
      for (int j = 0; j < v; j++) {
         M.push_back(i % 2 == 0 ? id : -1);
      }
      id += (i % 2 == 0);
   }

   int idx = 0;
   while (idx < int(M.size())) {
      if (M[idx] == -1) {
         int totFree = 0;
         int st = idx;
         while (idx < int(M.size()) && M[idx] == -1) {
            totFree++;
            idx++;
         }
         freeBlocks.push_back({st, totFree});
      }
      else {
         idx++;
      }
   }

   freeSpace.assign(4 * int(freeBlocks.size()), 0);
   startPos.assign(4 * int(freeBlocks.size()), 0);

   build(0, int(freeBlocks.size()) - 1, 1);

   idx = int(M.size()) - 1;
   while (idx >= 0) {
      if (M[idx] != -1) {
         int fileEndIdx = idx;
         while (idx >= 0 && M[idx] == M[fileEndIdx]) {
            idx--;
         }
         int fileStartIdx = idx + 1;

         int reqSpace = fileEndIdx - fileStartIdx + 1;
         auto res = query(reqSpace, 0, int(freeBlocks.size()) - 1, 1);

         int blockStartPos = get<0>(res);
         int blockSize = get<1>(res);
         int blockIdx = get<2>(res);

         if (blockStartPos != -1 && blockStartPos < fileStartIdx) {
            int itr = 0;
            for (int i = fileStartIdx; i <= fileEndIdx; i++) {
               swap(M[i], M[blockStartPos + itr]);
               itr++;
            }
            int newRemainingSpace = blockSize - reqSpace;
            upd(blockIdx, newRemainingSpace, 0, int(freeBlocks.size()) - 1, 1);
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

   auto stop = high_resolution_clock::now();
   auto duration = duration_cast<milliseconds>(stop - start);

   cout << ans << endl;
   cout << "Time taken: " << duration.count() << " milliseconds" << endl;

   return 0;
}
