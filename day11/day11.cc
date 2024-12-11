#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

const int MAXV = 1000;
const int MAXSTEPS = 75;
long long DP[MAXV + 1][MAXSTEPS + 1];

long long solve(long long stone, int remSteps) {
   if (remSteps == 0) {
      return 1;
   }

   if (stone <= MAXV && DP[stone][remSteps] != -1) {
      return DP[stone][remSteps];
   }

   long long tot = 0;
   if (stone == 0) {
      tot = solve(1, remSteps - 1);
   }
   else {
      long long numDigits = log10(stone) + 1;
      if (numDigits % 2 == 0) {
         long long halfDigits = numDigits / 2;
         long long divisor = pow(10, halfDigits);

         long long leftPart = stone / divisor;
         long long rightPart = stone % divisor;

         tot += solve(leftPart, remSteps - 1);
         tot += solve(rightPart, remSteps - 1);
      }
      else {
         tot = solve(stone * 2024, remSteps - 1);
      }
   }

   if (stone <= MAXV) {
      DP[stone][remSteps] = tot;
   }

   return tot;
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   auto start = high_resolution_clock::now();

   freopen("in.txt", "r", stdin);

   memset(DP, -1, sizeof(DP));

   int v;
   long long ans = 0;
   while (cin >> v) {
      ans += solve(v, 75);
   }

   auto stop = high_resolution_clock::now();
   auto duration = duration_cast<milliseconds>(stop - start);

   cout << ans << endl;
   cout << "Time taken: " << duration.count() << "ms" << endl;

   return 0;
}
