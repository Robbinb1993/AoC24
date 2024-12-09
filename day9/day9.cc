#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

vector<int> fileSystem;
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

void buildSegmentTree() {
   int idx = 0;
   while (idx < int(fileSystem.size())) {
      if (fileSystem[idx] == -1) {
         int totFree = 0;
         int st = idx;
         while (idx < int(fileSystem.size()) && fileSystem[idx] == -1) {
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
}

void moveFileBlocks() {
   int idx = int(fileSystem.size()) - 1;
   while (idx >= 0) {
      if (fileSystem[idx] != -1) {
         int fileEndIdx = idx;
         while (idx >= 0 && fileSystem[idx] == fileSystem[fileEndIdx]) {
            idx--;
         }
         int fileStartIdx = idx + 1;

         int reqSpace = fileEndIdx - fileStartIdx + 1;
         auto res = query(reqSpace, 0, int(freeBlocks.size()) - 1, 1);

         int blockStartIdx = get<0>(res);
         int blockSize = get<1>(res);
         int blockIdx = get<2>(res);

         if (blockStartIdx != -1 && blockStartIdx < fileStartIdx) {
            int itr = 0;
            for (int i = fileStartIdx; i <= fileEndIdx; i++) {
               swap(fileSystem[i], fileSystem[blockStartIdx + itr]);
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
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   auto start = high_resolution_clock::now();

   freopen("in.txt", "r", stdin);

   string line;
   getline(cin, line);


   int id = 0;
   for (int i = 0; i < int(line.size()); i++) {
      int v = line[i] - '0';
      for (int j = 0; j < v; j++) {
         fileSystem.push_back(i % 2 == 0 ? id : -1);
      }
      id += (i % 2 == 0);
   }

   buildSegmentTree();
   moveFileBlocks();

   long long ans = 0;
   for (int i = 0; i < int(fileSystem.size()); i++) {
      if (fileSystem[i] != -1) {
         ans += i * fileSystem[i];
      }
   }

   auto stop = high_resolution_clock::now();
   auto duration = duration_cast<milliseconds>(stop - start);

   cout << ans << endl;
   cout << "Time taken: " << duration.count() << " milliseconds" << endl;

   return 0;
}
