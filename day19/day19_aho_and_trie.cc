#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

struct TrieNode {
   int left = -1;
   int right = -1;
   int parent = -1;
   long long DP = -1;
};

const int MAXN = 500000;
const int LOGN = 20;
array<TrieNode, MAXN> flatTrie;
int par[MAXN][LOGN];
int trieNodeCount = 1;

void insert(const string& word) {
   int node = 0; // Start at the root (index 0)
   for (char c : word) {
      int& child = (c == 'u') ? flatTrie[node].left : flatTrie[node].right;
      if (child == -1) {
         child = trieNodeCount++;
         flatTrie[child].parent = node;
      }
      node = child;
   }
}

void computeAncestors() {
   for (int node = 1; node <= trieNodeCount; node++) {
      par[node][0] = flatTrie[node].parent;
   }

   for (int k = 1; k < LOGN; k++) {
      for (int node = 1; node <= trieNodeCount; node++) {
         int mid = par[node][k - 1];
         par[node][k] = (mid == -1) ? -1 : par[mid][k - 1];
      }
   }
}

// Function to get the N'th parent using binary lifting
int getNthParent(int node, int N) {
   for (int k = 0; k < LOGN && node != -1; k++) {
      if (N & (1 << k)) {
         node = par[node][k];
      }
   }
   return node;
}

class AhoCorasick {
private:
   struct AhoNode {
      int next[2]; // Transitions for 'u' and 'w'
      int fail;    // Failure link
      vector<int> out; // Output lengths
      AhoNode() : fail(-1) { next[0] = next[1] = -1; }
   };

   vector<AhoNode> automaton;

public:
   AhoCorasick() { automaton.push_back(AhoNode()); }

   void insert(const string& word) {
      int curr = 0;
      for (char c : word) {
         int idx = (c == 'u') ? 0 : 1;
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
      for (int c = 0; c < 2; c++) {
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
         int u = Q.front();
         Q.pop();
         int f = automaton[u].fail;

         for (auto& length : automaton[f].out) {
            automaton[u].out.push_back(length);
         }

         for (int c = 0; c < 2; c++) {
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

   int nextState(const int curr, const char c) const {
      return automaton[curr].next[c == 'u' ? 0 : 1];
   }

   const vector<int>& getOutputs(const int state) const {
      return automaton[state].out;
   }
};

AhoCorasick aho;

long long solve(const string& pattern) {
   int curr = 0;
   int node = 0;

   for (char c : pattern) {
      curr = aho.nextState(curr, c); // Transition in the automaton
      node = (c == 'u') ? flatTrie[node].left : flatTrie[node].right;
      if (node == -1) return 0;

      if (flatTrie[node].DP == -1) {
         flatTrie[node].DP = 0;
         auto& ahoOutputs = aho.getOutputs(curr);
         for (int length : ahoOutputs) {
            int parentIdx = getNthParent(node, length);
            if (parentIdx != -1) {
               flatTrie[node].DP += flatTrie[parentIdx].DP;
            }
         }
      }
   }

   return flatTrie[node].DP;
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   auto start = high_resolution_clock::now();

   freopen("aoc-2024-day-19-challenge-3.txt", "r", stdin);

   string line;
   getline(cin, line);

   istringstream ss(line);
   string word;

   vector<string> patterns;

   while (getline(ss, word, ',')) {
      size_t start = word.find_first_not_of(" ");
      size_t end = word.find_last_not_of(" ");
      if (start != string::npos) {
         word = word.substr(start, end - start + 1);
         aho.insert(word);
      }
   }

   aho.build();

   flatTrie[0] = {};

   string pattern;
   while (cin >> pattern) {
      patterns.push_back(pattern);
      insert(pattern);
   }

   computeAncestors();

   flatTrie[0].DP = 1;
   long long ans = 0;
   for (const auto& pattern : patterns) {
      ans += solve(pattern);
   }

   auto stop = high_resolution_clock::now();
   auto duration = duration_cast<milliseconds>(stop - start);

   cout << ans << endl;
   cout << "Time taken: " << duration.count() << " milliseconds" << endl;

   return 0;
}
