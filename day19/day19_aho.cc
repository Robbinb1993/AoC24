#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

struct AhoNode {
   array<int, 26> next;
   int fail;
   vector<int> out;
   AhoNode() {
      next.fill(-1);
      fail = -1;
   }
};

class AhoCorasick {
public:
   vector<AhoNode> trie;
   AhoCorasick() { trie.push_back(AhoNode()); }

   void insert(const string& word) {
      int cur = 0;
      for (char c : word) {
         int idx = c - 'a';
         if (trie[cur].next[idx] == -1) {
            trie[cur].next[idx] = (int)trie.size();
            trie.push_back(AhoNode());
         }
         cur = trie[cur].next[idx];
      }
      trie[cur].out.push_back((int)word.size());
   }

   void build() {
      queue<int>q;
      for (int c = 0; c < 26; c++) {
         int nxt = trie[0].next[c];
         if (nxt != -1) {
            trie[nxt].fail = 0;
            q.push(nxt);
         }
         else {
            trie[0].next[c] = 0;
         }
      }

      while (!q.empty()) {
         int u = q.front(); q.pop();
         int f = trie[u].fail;

         for (auto& length : trie[f].out) {
            trie[u].out.push_back(length);
         }
         for (int c = 0; c < 26; c++) {
            int nxt = trie[u].next[c];
            if (nxt != -1) {
               trie[nxt].fail = trie[f].next[c];
               q.push(nxt);
            }
            else {
               trie[u].next[c] = trie[f].next[c];
            }
         }
      }
   }

   long long solve(const string& pattern) {
      vector<long long> DP(pattern.size() + 1, 0);
      DP[0] = 1;
      int curr = 0;

      for (int i = 0; i < (int)pattern.size(); i++) {
         int c = pattern[i] - 'a';
         curr = trie[curr].next[c];
         for (int length : trie[curr].out) {
            if (i + 1 - length >= 0)
               DP[i + 1] += DP[i + 1 - length];
         }
      }

      return DP[pattern.size()];
   }
};

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   auto start = high_resolution_clock::now();

   freopen("in.txt", "r", stdin);

   string line;
   getline(cin, line);

   istringstream stream(line);
   string word;

   AhoCorasick aho;

   while (getline(stream, word, ',')) {
      while (!word.empty() && isspace((unsigned char)word.front())) word.erase(word.begin());
      while (!word.empty() && isspace((unsigned char)word.back())) word.pop_back();

      if (!word.empty()) {
         aho.insert(word);
      }
   }

   aho.build();

   long long ans = 0;
   while (cin >> word) {
      ans += aho.solve(word);
   }

   cout << ans << "\n";

   auto stop = high_resolution_clock::now();
   auto duration = duration_cast<milliseconds>(stop - start);

   cerr << "Time: " << duration.count() << " ms\n";

   return 0;
}
