#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

const int MOD = 16777216;
const int POW_19_3 = 19 * 19 * 19;

static int transform(unsigned long long secretNumber) {
    unsigned long long result = secretNumber * 64;
    secretNumber ^= result;
    secretNumber %= MOD;

    result = secretNumber / 32;
    secretNumber ^= result;
    secretNumber %= MOD;

    result = secretNumber * 2048;
    secretNumber ^= result;

    return secretNumber % MOD;
}

int sum[130321];
int seen[130321];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    auto start = high_resolution_clock::now();

    freopen("aoc-2024-day-22-challenge-1.txt", "r", stdin);

    long long ans1 = 0, ans2 = 0;
    int secret;
    int seenItr = 0;
    while (cin >> secret) {
        seenItr++;
        int lastPrice = secret % 10;
        int index = 0;
        for (int j = 0; j < 2000; j++) {
            secret = transform(secret);
            int price = secret % 10;
            int delta = price - lastPrice;
            lastPrice = price;
            index = (index % (POW_19_3)) * 19 + (delta + 9);
            if (j >= 2 && seen[index] != seenItr) {
                seen[index] = seenItr;
                sum[index] += price;
                if (sum[index] > ans2) {
                    ans2 = sum[index];
                }
            }
        }
        ans1 += secret;
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    cout << ans1 << " " << ans2 << endl;
    cout << "Time: " << duration.count() << " ms" << endl;

    return 0;
}
