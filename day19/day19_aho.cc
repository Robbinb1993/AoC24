#include <bits/stdc++.h>
#include <cstddef>
#include <chrono>
using namespace std;
using namespace chrono;

class AhoCorasick {
private:
   struct AhoNode {
      array<int, 26> next;
      int fail;
      vector<int> out;
      AhoNode() {
         next.fill(-1);
         fail = -1;
      }
   };

   vector<AhoNode> automaton;

public:
   AhoCorasick() { automaton.push_back(AhoNode()); }

   void insert(const string& word) {
      int curr = 0;
      for (char c : word) {
         int idx = c - 'a';
         if (automaton[curr].next[idx] == -1) {
            automaton[curr].next[idx] = (int)automaton.size();
            automaton.push_back(AhoNode());
         }
         curr = automaton[curr].next[idx];
      }
      automaton[curr].out.push_back((int)word.size());
   }

   void build() {
      queue<int> Q;
      for (int c = 0; c < 26; c++) {
         int nxt = automaton[0].next[c];
         if (nxt != -1) {
            automaton[nxt].fail = 0;
            Q.push(nxt);
         }
         else {
            automaton[0].next[c] = 0;
         }
      }

      while (!Q.empty()) {
         int u = Q.front(); Q.pop();
         int f = automaton[u].fail;

         for (auto& length : automaton[f].out) {
            automaton[u].out.push_back(length);
         }
         for (int c = 0; c < 26; c++) {
            int nxt = automaton[u].next[c];
            if (nxt != -1) {
               automaton[nxt].fail = automaton[f].next[c];
               Q.push(nxt);
            }
            else {
               automaton[u].next[c] = automaton[f].next[c];
            }
         }
      }
   }

   int nextState(int curr, char c) const {
      return automaton[curr].next[c - 'a'];
   }

   const vector<int>& getOutputs(int state) const {
      return automaton[state].out;
   }
};

AhoCorasick aho;

long long solve(const string& pattern) {
   vector<long long> DP(pattern.size() + 1, 0); // DP[i] = number of ways to match the prefix of length i
   DP[0] = 1; // 1 way to match an empty prefix
   int curr = 0;

   for (size_t i = 0; i < pattern.size(); i++) {
      char c = pattern[i];
      curr = aho.nextState(curr, c); // Transition in the automaton
      for (int length : aho.getOutputs(curr)) {
         if (i + 1 - length >= 0)
            DP[i + 1] += DP[i + 1 - length];
      }
   }

   return DP[pattern.size()];
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   auto start = high_resolution_clock::now();

   freopen("in.txt", "r", stdin);

   string line;
   getline(cin, line);

   istringstream ss(line);
   string word;

   while (getline(ss, word, ',')) {
      size_t start = word.find_first_not_of(" \t");
      size_t end = word.find_last_not_of(" \t");

      if (start != string::npos) {
         word = word.substr(start, end - start + 1);
         aho.insert(word);
      }
   }

   aho.build();

   long long ans = 0;
   while (cin >> word) {
      ans += solve(word);
   }

   cout << ans << "\n";

   auto stop = high_resolution_clock::now();
   auto duration = duration_cast<milliseconds>(stop - start);

   cerr << "Time: " << duration.count() << " ms\n";

   return 0;
}
