#ifndef DSU_H
#define DSU_H

#include <vector>
#include <algorithm>
#include <numeric>

class DSU {
private:
   std::vector<int> parent, size;

public:
   DSU(const int N) {
      parent.assign(N, 0);
      size.assign(N, 1);
      std::iota(parent.begin(), parent.end(), 0);
   }
   int getSetSize(int v);
   int findSetParent(int v);
   void uniteSets(int a, int b);
};

int DSU::getSetSize(int v) {
   int p = findSetParent(v);
   return size[p];
}

int DSU::findSetParent(int v) {
   if (v == parent[v])
      return v;
   return parent[v] = findSetParent(parent[v]);
}

void DSU::uniteSets(int a, int b) {
   a = findSetParent(a);
   b = findSetParent(b);
   if (a != b) {
      if (size[a] < size[b])
         std::swap(a, b);
      parent[b] = a;
      size[a] += size[b];
   }
}

#endif // DSU_H
