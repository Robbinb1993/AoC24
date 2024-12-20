#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

const int MAXV = 1000;
const int MAXSTEPS = 75;
__int128 DP[MAXV + 1][MAXSTEPS + 1];

__int128 solve(long long stoneVal, int remSteps) {
   if (remSteps == 0) {
      return 1;
   }

   if (stoneVal <= MAXV && DP[stoneVal][remSteps] != -1) {
      return DP[stoneVal][remSteps];
   }

   long long tot = 0;
   if (stoneVal == 0) {
      tot = solve(1, remSteps - 1);
   }
   else {
      long long numDigits = log10(stoneVal) + 1;
      if (numDigits % 2 == 0) {
         long long halfDigits = numDigits / 2;
         long long divisor = pow(10, halfDigits);

         long long leftPart = stoneVal / divisor;
         long long rightPart = stoneVal % divisor;

         tot += solve(leftPart, remSteps - 1);
         tot += solve(rightPart, remSteps - 1);
      }
      else {
         tot = solve(stoneVal * 2024, remSteps - 1);
      }
   }

   if (stoneVal <= MAXV) {
      DP[stoneVal][remSteps] = tot;
   }

   return tot;
}

ostream& operator<<(ostream& os, __int128 num) {
   if (num == 0) {
      os << '0';
      return os;
   }

   bool isNegative = num < 0;
   if (isNegative) num = -num;

   string str = "";
   while (num > 0) {
      str += static_cast<char>('0' + (num % 10));
      num /= 10;
   }

   if (isNegative) str += '-';

   reverse(str.begin(), str.end());
   os << str;
   return os;
}


int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   auto start = high_resolution_clock::now();

   freopen("in.txt", "r", stdin);

   memset(DP, -1, sizeof(DP));

   long long v;
   __int128 ans = 0;
   while (cin >> v) {
      ans += solve(v, 75);
   }

   auto stop = high_resolution_clock::now();
   auto duration = duration_cast<milliseconds>(stop - start);

   cout << ans << endl;
   cout << "Time taken: " << duration.count() << "ms" << endl;

   return 0;
}
