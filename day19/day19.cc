#include <bits/stdc++.h>
using namespace std;

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

long long DP[100];

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

   freopen("in.txt", "r", stdin);

   string line, word;
   getline(cin, line);
   istringstream stream(line);
   while (getline(stream, word, ',')) {
      word.erase(0, word.find_first_not_of(' '));
      word.erase(word.find_last_not_of(' ') + 1);

      cout << word << endl;

      if (!word.empty()) {
         dictionary->insert(word);
      }
   }

   long long ans = 0;
   while (cin >> word) {
      memset(DP, -1, sizeof(DP));
      ans += solve(0, word);
   }

   cout << ans << endl;

   return 0;
}
