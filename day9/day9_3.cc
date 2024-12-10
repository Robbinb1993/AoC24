#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

vector<int> fileSystem, jump;
const int MAX_FREE_SLOTS = 9;
int nextFree[MAX_FREE_SLOTS + 1];

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

void initPart2() {
   for (int i = 1; i <= MAX_FREE_SLOTS; i++) {
      nextFree[i] = INT_MAX;
   }

   int idx = 0;
   while (idx < (int)fileSystem.size()) {
      if (fileSystem[idx] == -1) {
         int totFree = 0;
         int st = idx;
         while (idx < (int)fileSystem.size() && fileSystem[idx] == -1) {
            totFree++;
            idx++;
         }
         for (int i = 1; i <= totFree; i++) {
            nextFree[i] = min(nextFree[i], st);
         }
         if (totFree >= MAX_FREE_SLOTS) {
            break;
         }
      }
      else {
         idx++;
      }
   }

   jump.resize((int)fileSystem.size());
   int lastFileEnd = INT_MAX;
   int lastFreeSpaceEnd = INT_MAX;

   for (int i = int(fileSystem.size()) - 1; i >= 0; i--) {
      if (fileSystem[i] == -1) {
         if (lastFreeSpaceEnd == INT_MAX) {
            lastFreeSpaceEnd = i;
         }
         jump[i] = lastFreeSpaceEnd + 1;
         lastFileEnd = INT_MAX;
      }
      else {
         if (lastFileEnd == INT_MAX) {
            lastFileEnd = i;
         }
         jump[i] = lastFileEnd + 1;
         lastFreeSpaceEnd = INT_MAX;
      }
   }
}

inline void findNextFreeSpace(int from, const int to, const int idx, const int until) {
   if (nextFree[to] < idx) return;

   for (int i = idx; i < until;) {
      if (fileSystem[i] != -1) {
         i = jump[i];
         continue;
      }

      int j = jump[i];
      int freeSpaceSz = j - i;

      if (freeSpaceSz >= from) {
         for (int k = from; k <= freeSpaceSz; k++) {
            if (nextFree[k] > i) {
               nextFree[k] = i;
            }
         }
      }

      if (freeSpaceSz >= to) return;

      i = j;
   }
}

inline void updateFreeSpace(const int idx, const int taken, const int until) {
   int initialFreeSpace = jump[idx] - idx;

   int from = INT_MAX;
   for (int i = 1; i <= initialFreeSpace; i++) {
      if (nextFree[i] == idx) {
         if (from == INT_MAX) from = i;
         nextFree[i] = INT_MAX;
      }
   }

   findNextFreeSpace(from, initialFreeSpace, idx + taken, until);
}

__int128 solvePart2() {
   initPart2();
   __int128 ans = 0;

   int idx = (int)fileSystem.size() - 1;
   while (idx >= 0) {
      if (fileSystem[idx] != -1) {
         int fileEndIdx = idx;
         while (idx >= 0 && fileSystem[idx] == fileSystem[fileEndIdx]) idx--;
         int fileStartIdx = idx + 1;
         int reqSpace = fileEndIdx - fileStartIdx + 1;

         if (nextFree[reqSpace] < fileStartIdx) {
            int stFree = nextFree[reqSpace];
            int edFree = stFree + reqSpace - 1;
            int itr = 0;
            updateFreeSpace(stFree, reqSpace, idx);
            for (int i = stFree; i <= edFree; i++) {
               swap(fileSystem[i], fileSystem[fileStartIdx + itr]);
               jump[i] = edFree + 1;
               itr++;
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
