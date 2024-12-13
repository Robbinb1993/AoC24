#include <bits/stdc++.h>
using namespace std;

long long applyCramersRule(long long ax, long long ay, long long bx, long long by, long long px, long long py) {
   long long D = ax * by - ay * bx;
   long long numerator_n = px * by - py * bx;
   long long numerator_m = py * ax - px * ay;

   if (D != 0 && numerator_n % D == 0 && numerator_m % D == 0) {
      long long n0 = numerator_n / D;
      long long m0 = numerator_m / D;

      if (n0 >= 0 && m0 >= 0) {
         if (ax * n0 + bx * m0 == px && ay * n0 + by * m0 == py) {
            return 3 * n0 + m0;
         }
      }
   }
   return 0;
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   freopen("in.txt", "r", stdin);

   long long ans1 = 0, ans2 = 0;
   char buffer[256];
   while (fgets(buffer, sizeof(buffer), stdin)) {
      if (buffer[0] == '\n') continue;
      long long ax, ay, bx, by, px, py;
      sscanf(buffer, "Button A: X+%lld, Y+%lld", &ax, &ay);
      fgets(buffer, sizeof(buffer), stdin);
      sscanf(buffer, "Button B: X+%lld, Y+%lld", &bx, &by);
      fgets(buffer, sizeof(buffer), stdin);
      sscanf(buffer, "Prize: X=%lld, Y=%lld", &px, &py);

      ans1 += applyCramersRule(ax, ay, bx, by, px, py);
      ans2 += applyCramersRule(ax, ay, bx, by, px + 10000000000000, py + 10000000000000);
   }

   cout << ans1 << " " << ans2 << endl;

   return 0;
}
