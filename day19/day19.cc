#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

class Trie {
public:
   unique_ptr<Trie> children[26];
   bool isEndOfWord;

   Trie() : isEndOfWord(false) {}

   void insert(const string& word) {
      Trie* node = this;
      for (char c : word) {
         if (!node->children[c - 'a'])
            node->children[c - 'a'] = make_unique<Trie>();
         node = node->children[c - 'a'].get();
      }
      node->isEndOfWord = true;
   }
};

unique_ptr<Trie> dictionary = make_unique<Trie>();

vector<long long> DP;
long long solve(const int idx, const string& word) {
   if (idx == word.size()) return 1;

   if (DP[idx] != -1) return DP[idx];
   long long tot = 0;

   Trie* node = dictionary.get();
   for (int i = idx; i < word.size(); i++) {
      char c = word[i];

      if (!node->children[c - 'a']) break;

      node = node->children[c - 'a'].get();

      if (node->isEndOfWord) {
         tot += solve(i + 1, word);
      }
   }

   return DP[idx] = tot;
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   auto start = high_resolution_clock::now();

   freopen("in.txt", "r", stdin);

   string line, word;
   getline(cin, line);
   istringstream stream(line);
   while (getline(stream, word, ',')) {
      word.erase(0, word.find_first_not_of(' '));
      word.erase(word.find_last_not_of(' ') + 1);

      if (!word.empty()) {
         dictionary->insert(word);
      }
   }

   long long ans = 0;
   while (cin >> word) {
      DP.assign(word.size(), -1);
      ans += solve(0, word);
   }

   auto stop = high_resolution_clock::now();
   auto duration = duration_cast<milliseconds>(stop - start);

   cout << ans << endl;
   cout << "Time: " << duration.count() << " ms" << endl;

   return 0;
}
