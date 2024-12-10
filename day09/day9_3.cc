#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

vector<int> fileSystem;
const int MAX_FREE_SLOTS = 9;
int freeSegmentIdx = 0;
vector<int> freeSegmentStart, freeSegmentEnd;
vector<int> freeSegments[MAX_FREE_SLOTS + 1];

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

void preprocessPart2(const int maxSegments) {
   freeSegmentStart.reserve(maxSegments);
   freeSegmentEnd.reserve(maxSegments);

   for (int i = 1; i <= MAX_FREE_SLOTS; i++) {
      freeSegments[i].reserve(maxSegments);
   }

   int idx = (int)fileSystem.size() - 1;
   while (idx >= 0) {
      if (fileSystem[idx] == -1) {
         int totFree = 0;
         int ed = idx;
         while (idx >= 0 && fileSystem[idx] == -1) {
            totFree++;
            idx--;
         }
         int st = idx + 1;

         freeSegmentStart.push_back(st);
         freeSegmentEnd.push_back(ed);

         for (int i = 1; i <= totFree; i++) {
            freeSegments[i].push_back(freeSegmentIdx);
         }

         freeSegmentIdx++;
      }
      else {
         idx--;
      }
   }
}

__int128 solvePart2() {
   __int128 ans = 0;

   int idx = (int)fileSystem.size() - 1;
   while (idx >= 0) {
      if (fileSystem[idx] != -1) {
         int fileEndIdx = idx;
         while (idx >= 0 && fileSystem[idx] == fileSystem[fileEndIdx]) idx--;
         int fileStartIdx = idx + 1;
         int reqSpace = fileEndIdx - fileStartIdx + 1;

         while (!freeSegments[reqSpace].empty()) {
            int freeSegmentIdx = freeSegments[reqSpace].back();
            freeSegments[reqSpace].pop_back();

            int freeSegmentStartIdx = freeSegmentStart[freeSegmentIdx];
            int freeSegmentEndIdx = freeSegmentEnd[freeSegmentIdx];
            int availableSpace = freeSegmentEndIdx - freeSegmentStartIdx + 1;

            if (freeSegmentStartIdx < fileStartIdx && availableSpace >= reqSpace) {
               for (int i = 0; i < reqSpace; i++) {
                  fileSystem[freeSegmentStartIdx + i] = fileSystem[fileStartIdx + i];
                  fileSystem[fileStartIdx + i] = -1;
               }

               freeSegmentStart[freeSegmentIdx] += reqSpace;
               availableSpace -= reqSpace;

               if (availableSpace >= reqSpace) {
                  freeSegments[reqSpace].push_back(freeSegmentIdx);
               }

               break;
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
   preprocessPart2(int(line.size()) / 2);
   __int128 ansPart2 = solvePart2();

   print128(ansPart1);
   print128(ansPart2);

   auto stop = high_resolution_clock::now();
   auto duration = duration_cast<milliseconds>(stop - start);

   cout << "Time taken: " << duration.count() << " milliseconds" << endl;

   return 0;
}
