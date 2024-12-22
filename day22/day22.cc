#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

const int MOD = 16777216;
const int POWERS_OF_19[4] = {19 * 19 * 19, 19 * 19, 19, 1};

int transform(long long secretNumber) {
   secretNumber ^= (secretNumber << 6);
   secretNumber &= MOD - 1;

   secretNumber ^= (secretNumber >> 5);
   secretNumber &= MOD - 1;

   secretNumber ^= (secretNumber << 11);
   return secretNumber & (MOD - 1);
}

int sum[130321];
int seen[130321];
int seenItr;
int ans;

void updateAns(const int index, const int value) {
   if (seen[index] != seenItr) {
      seen[index] = seenItr;
      sum[index] += value;
      ans = max(ans, sum[index]);
   }
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

   for (size_t s = 0; s < secretNumbers.size(); s++) {
      seenItr++;
      int secret = secretNumbers[s];
      vector<int> secretList;
      secretList.push_back(secret);
      for (int j = 0; j < 2000; j++) {
         secret = transform(secret);
         secretList.push_back(secret);
      }

      int index = 0;
      for (int i = 0; i < 4; i++) {
         int priceDiff = secretList[i + 1] % 10 - secretList[i] % 10;
         index += (priceDiff + 9) * POWERS_OF_19[i];
      }

      updateAns(index, secretList[4] % 10);

      for (size_t i = 5; i < secretList.size(); i++) {
         int oldPriceDiff = secretList[i - 4] % 10 - secretList[i - 5] % 10;
         int newPriceDiff = secretList[i] % 10 - secretList[i - 1] % 10;
         index = ((index - (oldPriceDiff + 9) * POWERS_OF_19[0]) * 19 + (newPriceDiff + 9));
         updateAns(index, secretList[i] % 10);
      }
   }

   auto end = high_resolution_clock::now();
   auto duration = duration_cast<milliseconds>(end - start);

   cout << ans << endl;
   cout << "Time: " << duration.count() << " ms" << endl;

   return 0;
}
