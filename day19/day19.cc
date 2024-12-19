#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

struct TrieNode {
   int children[26]; // Indices of child nodes ('a' to 'z')
   bool isEndOfWord = false;

   TrieNode() {
      fill(begin(children), end(children), -1);
   }
};

const int MAXN = 500000;
vector<TrieNode> trie(MAXN);
int trieNodeCount = 1;

void insert(const string& word) {
   int node = 0; // Start at the root
   for (char c : word) {
      int& child = trie[node].children[c - 'a'];
      if (child == -1) {
         child = trieNodeCount++;
      }
      node = child;
   }
   trie[node].isEndOfWord = true;
}

vector<long long> DP;

long long solve(const int idx, const string& word) {
   if (idx == word.size()) return 1;

   if (DP[idx] != -1) return DP[idx];
   long long tot = 0;

   int node = 0;
   for (int i = idx; i < word.size(); i++) {
      char c = word[i];

      if (trie[node].children[c - 'a'] == -1) break;

      node = trie[node].children[c - 'a'];

      if (trie[node].isEndOfWord) {
         tot += solve(i + 1, word);
      }
   }

   return DP[idx] = tot;
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   auto start = high_resolution_clock::now();

   freopen("aoc-2024-day-19-challenge-2.txt", "r", stdin);

   string line;
   getline(cin, line);

   istringstream ss(line);
   string word;

   while (getline(ss, word, ',')) {
      size_t start = word.find_first_not_of(" ");
      size_t end = word.find_last_not_of(" ");

      if (start != string::npos) {
         word = word.substr(start, end - start + 1);
         insert(word);
      }
   }

   long long ans = 0;

   while (cin >> word) {
      DP.assign(word.size(), -1);
      ans += solve(0, word);
      cout << ans << endl;
   }

   auto stop = high_resolution_clock::now();
   auto duration = duration_cast<milliseconds>(stop - start);

   cout << ans << endl;
   cout << "Time: " << duration.count() << " ms" << endl;

   return 0;
}
