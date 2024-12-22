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

int sum[130321];
int seen[130321];
int seenItr;

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

   for (size_t s = 0; s < secretNumbers.size(); s++) {
      seenItr++;
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

         if (seen[index] != seenItr) {
            sum[index] += secretList[i] % 10;
            seen[index] = seenItr;
            if (sum[index] > ans) {
               ans = sum[index];
            }
         }
      }
   }

   auto end = high_resolution_clock::now();
   auto duration = duration_cast<milliseconds>(end - start);

   cout << ans << endl;
   cout << "Time: " << duration.count() << " ms" << endl;

   return 0;
}
