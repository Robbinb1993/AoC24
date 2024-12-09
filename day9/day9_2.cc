#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

vector<int> fileSystem;
priority_queue<int, vector<int>, greater<int>> freeBlocks[10];

long long solvePart1() {
   vector<int> fileSystemCopy = fileSystem;
   int l = 0, r = (int)fileSystem.size() - 1;
   while (l <= r) {
      if (fileSystemCopy[r] != -1 && fileSystemCopy[l] == -1) {
         swap(fileSystemCopy[l], fileSystemCopy[r]);
      }
      if (fileSystemCopy[r] == -1) r--;
      if (fileSystemCopy[l] != -1) l++;
   }
   long long ans = 0;
   for (int i = 0; i < (int)fileSystemCopy.size(); i++) {
      if (fileSystemCopy[i] != -1) {
         ans += (long long)i * fileSystemCopy[i];
      }
   }
   return ans;
}

void initFreeBlocks() {
   int idx = 0;
   while (idx < (int)fileSystem.size()) {
      if (fileSystem[idx] == -1) {
         int totFree = 0;
         int st = idx;
         while (idx < (int)fileSystem.size() && fileSystem[idx] == -1) {
            totFree++;
            idx++;
         }
         freeBlocks[totFree].push(st);
      }
      else {
         idx++;
      }
   }
}

long long solvePart2() {
   initFreeBlocks();
   long long ans = 0;

   int idx = (int)fileSystem.size() - 1;
   while (idx >= 0) {
      if (fileSystem[idx] != -1) {
         int fileEndIdx = idx;
         while (idx >= 0 && fileSystem[idx] == fileSystem[fileEndIdx]) idx--;
         int fileStartIdx = idx + 1;
         int reqSpace = fileEndIdx - fileStartIdx + 1;

         int mnPos = INT_MAX;
         int freeBlockSpace = INT_MAX;
         for (int i = reqSpace; i <= 9; i++) {
            if (!freeBlocks[i].empty()) {
               int pos = freeBlocks[i].top();
               if (pos < fileStartIdx && pos < mnPos) {
                  mnPos = pos;
                  freeBlockSpace = i;
               }
            }
         }

         if (mnPos == INT_MAX)
            continue;

         int blockStartIdx = mnPos;
         if (blockStartIdx != -1 && blockStartIdx < fileStartIdx) {
            int itr = 0;
            for (int i = fileStartIdx; i <= fileEndIdx; i++) {
               swap(fileSystem[i], fileSystem[blockStartIdx + itr]);
               itr++;
            }
            int remSpace = freeBlockSpace - reqSpace;
            freeBlocks[freeBlockSpace].pop();
            if (remSpace > 0) {
               freeBlocks[remSpace].push(blockStartIdx + reqSpace);
            }
         }
      }
      else {
         idx--;
      }
   }

   for (int i = 0; i < (int)fileSystem.size(); i++) {
      if (fileSystem[i] != -1) {
         ans += (long long)i * fileSystem[i];
      }
   }

   return ans;
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   auto start = high_resolution_clock::now();

   freopen("aoc-2024-day-09-challenge-2.txt", "r", stdin);

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

   long long ansPart1 = solvePart1();
   long long ansPart2 = solvePart2();

   cout << "Part 1: " << ansPart1 << endl;
   cout << "Part 2: " << ansPart2 << endl;

   auto stop = high_resolution_clock::now();
   auto duration = duration_cast<milliseconds>(stop - start);

   cout << "Time taken: " << duration.count() << " milliseconds" << endl;

   return 0;
}
