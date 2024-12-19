#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

class Trie;

vector<string> patterns;
vector<Trie*> flatTrie;
int trieNodeCount = 1;

class Trie {
public:
   unique_ptr<Trie> children[26];
   Trie* parent;                 // Pointer to the parent node
   vector<Trie*> ancestors;      // Binary lifting table
   long long DP;                 // DP value for this node
   int idx;

   Trie() : parent(nullptr), DP(-1), idx(0) {}

   void insert(const string& word) {
      Trie* node = this;
      for (char c : word) {
         if (!node->children[c - 'a']) {
            node->children[c - 'a'] = make_unique<Trie>();
            Trie* newNode = node->children[c - 'a'].get();
            newNode->parent = node;
            newNode->idx = trieNodeCount++;
            flatTrie.push_back(newNode);
         }
         node = node->children[c - 'a'].get();
      }
   }
};

Trie root;
vector<vector<int>> par;
int LOGN;

void computeAncestors() {
   LOGN = ceil(log2(trieNodeCount)) + 1;
   par.assign(LOGN, vector<int>(trieNodeCount, -1));

   for (Trie* node : flatTrie) {
      if (node->parent) {
         par[0][node->idx] = node->parent->idx; // 2^0 ancestor
      }
   }

   // Binary lifting for higher levels
   for (int k = 1; k < LOGN; k++) {
      for (int i = 0; i < trieNodeCount; i++) {
         if (par[k - 1][i] != -1) {
            par[k][i] = par[k - 1][par[k - 1][i]];
         }
      }
   }
}

//Get N'th parent in O(log(N)) time using binary lifting
int getNthParent(int nodeIndex, int N) {
   for (int k = 0; k < LOGN; k++) {
      if (N & (1 << k)) {
         nodeIndex = par[k][nodeIndex];
         if (nodeIndex == -1) break;
      }
   }
   return nodeIndex;
}

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

   int nextState(const int curr, const char c) const {
      return automaton[curr].next[c - 'a'];
   }

   const vector<int>& getOutputs(const int state) const {
      return automaton[state].out;
   }
};

AhoCorasick aho;

long long solve(const string& pattern) {
   int curr = 0;
   Trie* node = &root;

   for (size_t i = 0; i < pattern.size(); i++) {
      char c = pattern[i];
      curr = aho.nextState(curr, c); // Transition in the automaton
      node = node->children[c - 'a'].get();

      // Check if DP is known for the current node, if not compute it
      if (node->DP == -1) {
         node->DP = 0;
         auto& ahoOutputs = aho.getOutputs(curr);
         for (int length : ahoOutputs) {
            int parentIdx = getNthParent(node->idx, length);

            if (parentIdx != -1) {
               node->DP += flatTrie[parentIdx]->DP;
            }
         }
      }
   }

   return node->DP;
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

   while (getline(ss, word, ',')) {
      size_t start = word.find_first_not_of(" ");
      size_t end = word.find_last_not_of(" ");

      if (start != string::npos) {
         word = word.substr(start, end - start + 1);
         aho.insert(word);
      }
   }

   aho.build();

   flatTrie.reserve(1e6);
   flatTrie.push_back(&root);

   long long ans = 0;
   string pattern;
   while (cin >> pattern) {
      patterns.push_back(pattern);
      root.insert(pattern);
   }

   computeAncestors();

   root.DP = 1; // 1 way to match an empty prefix

   for (auto& pattern : patterns) {
      ans += solve(pattern);
   }

   auto stop = high_resolution_clock::now();
   auto duration = duration_cast<milliseconds>(stop - start);

   cout << ans << endl;
   cout << "Time taken: " << duration.count() << " milliseconds" << endl;

   return 0;
}
