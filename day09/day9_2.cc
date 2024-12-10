#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

const int MAX_SEGMENT_SIZE = 9;
vector<int> fileSystem;
priority_queue<int, vector<int>, greater<int>> freeBlocks[MAX_SEGMENT_SIZE + 1];

__int128 solvePart1() {
   vector<int> fileSystemCopy = fileSystem;
   int l = 0, r = (int)fileSystem.size() - 1;
   while (l <= r) {
      if (fileSystemCopy[r] != -1 && fileSystemCopy[l] == -1) {
         swap(fileSystemCopy[l], fileSystemCopy[r]);
      }
      if (fileSystemCopy[r] == -1) r--;
      if (fileSystemCopy[l] != -1) l++;
   }
   __int128 ans = 0;
   for (int i = 0; i < (int)fileSystemCopy.size(); i++) {
      if (fileSystemCopy[i] != -1) {
         ans += (__int128)i * (__int128)fileSystemCopy[i];
      }
   }
   return ans;
}

void preprocessPart2() {
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

__int128 solvePart2() {
   preprocessPart2();
   __int128 ans = 0;

   int idx = (int)fileSystem.size() - 1;
   while (idx >= 0) {
      if (fileSystem[idx] != -1) {
         int fileEndIdx = idx;
         while (idx >= 0 && fileSystem[idx] == fileSystem[fileEndIdx]) idx--;
         int fileStartIdx = idx + 1;
         int reqSpace = fileEndIdx - fileStartIdx + 1;

         int mnPos = INT_MAX;
         int freeBlockSpace = 0;
         for (int i = reqSpace; i <= MAX_SEGMENT_SIZE; i++) {
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
         ans += (__int128)i * (__int128)fileSystem[i];
      }
   }

   return ans;
}

void print128(__int128 x) {
   if (x == 0) {
      cout << "0";
      return;
   }
   string s = "";
   while (x > 0) {
      s += (char)(x % 10 + '0');
      x /= 10;
   }
   reverse(s.begin(), s.end());
   cout << s << endl;
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

   __int128 ansPart1 = solvePart1();
   __int128 ansPart2 = solvePart2();

   print128(ansPart1);
   print128(ansPart2);

   auto stop = high_resolution_clock::now();
   auto duration = duration_cast<milliseconds>(stop - start);

   cout << "Time taken: " << duration.count() << " milliseconds" << endl;

   return 0;
}
