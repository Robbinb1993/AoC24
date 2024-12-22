#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

const int MOD = 16777216;

int transform(long long secretNumber) {
   long long result = secretNumber * 64;
   secretNumber ^= result;
   secretNumber %= MOD;

   result = secretNumber / 32;
   secretNumber ^= result;
   secretNumber %= MOD;

   result = secretNumber * 2048;
   secretNumber ^= result;

   return secretNumber % MOD;
}

int calculateIndex(int i, int j, int k, int l) {
   return ((i + 9) * 19 * 19 * 19) + ((j + 9) * 19 * 19) + ((k + 9) * 19) + (l + 9);
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   auto start = high_resolution_clock::now();

   freopen("in.txt", "r", stdin);

   vector<int> secretNumbers;
   int secretNumber;

   while (cin >> secretNumber) {
      secretNumbers.push_back(secretNumber);
   }

   long long ans = 0;

   vector<vector<int>> mp(secretNumbers.size(), vector<int>(130321, -1));

   for (size_t s = 0; s < secretNumbers.size(); s++) {
      int secret = secretNumbers[s];
      vector<int> secretList;
      secretList.push_back(secret);
      for (int j = 0; j < 2000; j++) {
         secret = transform(secret);
         secretList.push_back(secret);
      }

      for (size_t i = 4; i < secretList.size(); i++) {
         int index = calculateIndex(
            secretList[i - 3] % 10 - secretList[i - 4] % 10,
            secretList[i - 2] % 10 - secretList[i - 3] % 10,
            secretList[i - 1] % 10 - secretList[i - 2] % 10,
            secretList[i] % 10 - secretList[i - 1] % 10);

         if (mp[s][index] == -1) {
            mp[s][index] = secretList[i] % 10;
         }
      }
   }

   int best = 0;
   for (int i = -9; i <= 9; i++) {
      for (int j = -9; j <= 9; j++) {
         for (int k = -9; k <= 9; k++) {
            for (int l = -9; l <= 9; l++) {
               int index = calculateIndex(i, j, k, l);
               int curr = 0;
               for (size_t m = 0; m < mp.size(); m++) {
                  if (mp[m][index] != -1) {
                     curr += mp[m][index];
                  }
               }
               best = max(best, curr);
            }
         }
      }
   }

   auto end = high_resolution_clock::now();
   auto duration = duration_cast<milliseconds>(end - start);

   cout << best << endl;
   cout << "Time: " << duration.count() << " ms" << endl;

   return 0;
}
