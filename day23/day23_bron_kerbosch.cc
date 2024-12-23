#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

static const int MAXN = 700;

unordered_map<string, int> getId;
vector<string> getName;
int id = 0;
unordered_set<int> edges[MAXN];
unordered_set<int> maxClique;

static bitset<MAXN> adj[MAXN];
static bitset<MAXN> bestSet;
int bestSize = 0;

void BronKerbosch(bitset<MAXN> R, bitset<MAXN> P, bitset<MAXN> X) {
    if (!P.any() && !X.any()) {
        int sz = (int)R.count();
        if (sz > bestSize) {
            bestSize = sz;
            bestSet = R;
        }
        return;
    }
    int pivot = -1;

    // pick first bit in (P|X)
    bitset<MAXN> px = (P | X);
    pivot = px._Find_first();

    // explore P \ neighbors(pivot)
    bitset<MAXN> toExplore = P & ~adj[pivot];
    for (int v = toExplore._Find_first(); v < MAXN; v = toExplore._Find_next(v)) {
        bitset<MAXN> R2 = R; R2.set(v);
        bitset<MAXN> P2 = P & adj[v];
        bitset<MAXN> X2 = X & adj[v];
        BronKerbosch(R2, P2, X2);
        P.reset(v);
        X.set(v);
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
        edges[id1].insert(id2);
        edges[id2].insert(id1);
    }

    for (int i = 0; i < id; i++)
        for (auto& n : edges[i]) adj[i].set(n);

    bitset<MAXN> R, P, X;
    P.set();
    P >>= (MAXN - id);

    BronKerbosch(R, P, X);

    vector<string> names;
    for (int i = bestSet._Find_first(); i < MAXN; i = bestSet._Find_next(i)) {
        names.push_back(getName[i]);
    }
    sort(names.begin(), names.end());

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "Time: " << duration.count() << " milliseconds\n";
    for (int i = 0; i < (int)names.size(); i++) {
        if (i) cout << ",";
        cout << names[i];
    }
    cout << endl;
    return 0;
}
