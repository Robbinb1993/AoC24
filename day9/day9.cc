#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

vector<int> fileSystem;

int treeSize;
int base;
vector<int> freeSpace, startPos;

void buildSegmentTree() {
   vector<pair<int, int>> freeBlocks;
   int idx = 0;
   while (idx < (int)fileSystem.size()) {
      if (fileSystem[idx] == -1) {
         int totFree = 0;
         int st = idx;
         while (idx < (int)fileSystem.size() && fileSystem[idx] == -1) {
            totFree++;
            idx++;
         }
         freeBlocks.push_back({st, totFree});
      }
      else {
         idx++;
      }
   }

   treeSize = (int)freeBlocks.size();
   if (treeSize == 0) return;
   base = 1;
   while (base < treeSize) base <<= 1;

   freeSpace.assign(2 * base, 0);
   startPos.assign(2 * base, 0);

   for (int i = 0; i < treeSize; i++) {
      freeSpace[base + i] = freeBlocks[i].second;
      startPos[base + i] = freeBlocks[i].first;
   }

   for (int i = treeSize; i < base; i++) {
      freeSpace[base + i] = 0;
      startPos[base + i] = 0;
   }

   for (int i = base - 1; i > 0; i--) {
      freeSpace[i] = max(freeSpace[i << 1], freeSpace[i << 1 | 1]);
   }
}

void upd(int p, int remSpace) {
   p += base;
   int filled = freeSpace[p] - remSpace;
   startPos[p] += filled;
   freeSpace[p] = remSpace;
   p >>= 1;
   while (p > 0) {
      freeSpace[p] = max(freeSpace[p << 1], freeSpace[p << 1 | 1]);
      p >>= 1;
   }
}

tuple<int, int, int> query(int reqSpace) {
   if (freeSpace[1] < reqSpace) return {-1, -1, -1};
   int idx = 1;
   int L = 0, R = base - 1;
   while (L != R) {
      int M = (L + R) >> 1;
      if (freeSpace[idx << 1] >= reqSpace) {
         idx = idx << 1;
         R = M;
      }
      else {
         idx = idx << 1 | 1;
         L = M + 1;
      }
   }
   return {startPos[idx], freeSpace[idx], L};
}

void moveFileBlocks() {
   if (treeSize == 0) return;
   int idx = (int)fileSystem.size() - 1;
   while (idx >= 0) {
      if (fileSystem[idx] != -1) {
         int fileEndIdx = idx;
         while (idx >= 0 && fileSystem[idx] == fileSystem[fileEndIdx]) idx--;
         int fileStartIdx = idx + 1;
         int reqSpace = fileEndIdx - fileStartIdx + 1;
         auto res = query(reqSpace);
         int blockStartIdx = get<0>(res);
         int blockSize = get<1>(res);
         int blockIdx = get<2>(res);
         if (blockStartIdx != -1 && blockStartIdx < fileStartIdx) {
            int itr = 0;
            for (int i = fileStartIdx; i <= fileEndIdx; i++) {
               swap(fileSystem[i], fileSystem[blockStartIdx + itr]);
               itr++;
            }
            upd(blockIdx, blockSize - reqSpace);
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

   freopen("aoc-2024-day-09-challenge-3.txt", "r", stdin);

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
