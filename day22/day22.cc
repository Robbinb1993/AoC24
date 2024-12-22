#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

const int MOD = 16777216;

int transform(long long secretNumber) {
   secretNumber ^= (secretNumber << 6);
   secretNumber &= MOD - 1;

   secretNumber ^= (secretNumber >> 5);
   secretNumber &= MOD - 1;

   secretNumber ^= (secretNumber << 11);
   return secretNumber & (MOD - 1);
}

int powersOf19[4] = {19 * 19 * 19, 19 * 19, 19, 1};

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

      int window[4] = {
         0,
         secretList[1] % 10 - secretList[0] % 10,
         secretList[2] % 10 - secretList[1] % 10,
         secretList[3] % 10 - secretList[2] % 10
      };

      int index = 0;
      for (int k = 0; k < 4; k++) {
         index += (window[k] + 9) * powersOf19[k];
      }

      for (size_t i = 4; i < secretList.size(); i++) {
         int newPriceDiff = secretList[i] % 10 - secretList[i - 1] % 10;
         index -= (window[0] + 9) * powersOf19[0];
         index *= 19;
         index += (newPriceDiff + 9);

         for (int k = 0; k < 3; k++) {
            window[k] = window[k + 1];
         }
         window[3] = newPriceDiff;

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
