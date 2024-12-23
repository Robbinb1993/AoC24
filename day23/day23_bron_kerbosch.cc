#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

unordered_map<string, int> getId;
vector<string> getName;
int id = 0;

static const int MAXN = 700;
static bitset<MAXN> adj[MAXN], bestSet;
int bestSize = 0;

// P is the set of candidates for the maximal clique.
// R is the current set of vertices forming the growing clique.
void BronKerbosch(bitset<MAXN> R, bitset<MAXN> P, const int N, const int cliqueSize) {
    if (!P.any()) {
        if (cliqueSize > bestSize) {
            bestSize = cliqueSize;
            bestSet = R;
        }
        return;
    }

    if (cliqueSize + P.count() <= bestSize) return;

    // Pick first bit in P as the pivot.
    int pivot = P._Find_first();

    // Explore P \ neighbors(pivot).
    bitset<MAXN> toExplore = P & ~adj[pivot];
    for (int v = toExplore._Find_first(); v < N; v = toExplore._Find_next(v)) {
        bitset<MAXN> nextR = R;
        nextR.set(v);
        BronKerbosch(nextR, P & adj[v], N, cliqueSize + 1);
        P.reset(v);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    auto start = high_resolution_clock::now();

    freopen("aoc-2024-day-23-challenge-5.txt", "r", stdin);

    string line;
    while (getline(cin, line)) {
        string s1, s2;
        stringstream ss(line);
        getline(ss, s1, '-');
        getline(ss, s2, '-');
        if (getId.emplace(s1, id).second) { getName.push_back(s1); id++; }
        if (getId.emplace(s2, id).second) { getName.push_back(s2); id++; }
        int id1 = getId[s1], id2 = getId[s2];
        adj[id1].set(id2);
        adj[id2].set(id1);
    }

    bitset<MAXN> R, P;
    P.set();
    P >>= (MAXN - id);

    BronKerbosch(R, P, id, 0);

    vector<string> names;
    for (int i = bestSet._Find_first(); i < MAXN; i = bestSet._Find_next(i)) {
        names.push_back(getName[i]);
    }
    sort(names.begin(), names.end());

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << int(names.size()) << endl;

    cout << "Time: " << duration.count() << " milliseconds\n";
    for (int i = 0; i < (int)names.size(); i++) {
        if (i) cout << ",";
        cout << names[i];
    }
    cout << endl;
    return 0;
}
