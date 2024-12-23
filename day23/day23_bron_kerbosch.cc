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

// This method greedily colors the subgraph induced by P
// and returns the number of colors used. No adjacent vertices
// in P will have the same color and the colors used are minimal.
// It will be an upper bound for the size of the largest clique in P.
int colorBound(const bitset<MAXN>& P, int N) {
    static int color[MAXN];
    memset(color, 0, sizeof(color));

    vector<int> vertices;
    for (int v = P._Find_first(); v < N; v = P._Find_next(v)) {
        vertices.push_back(v);
    }

    int usedColors = 0;
    // Greedy color each vertex in P
    for (int v : vertices) {
        // For each color, check if we can color vertex v with that color
        // by verifying it does not conflict with neighbors of v that are 
        // already colored with the same color.
        bool placed = false;
        for (int c = 1; c <= usedColors; c++) {
            bool conflict = false;
            // Check adjacency with other vertices that have color c
            for (int u : vertices) {
                if (color[u] == c && adj[v].test(u)) {
                    conflict = true;
                    break;
                }
            }
            if (!conflict) {
                color[v] = c;
                placed = true;
                break;
            }
        }
        // If we found no color that fits, we add a new color
        if (!placed) {
            usedColors++;
            color[v] = usedColors;
        }
    }

    return usedColors;
}

void BronKerbosch(
    bitset<MAXN> R, // current clique
    bitset<MAXN> P, // candidate vertices that can still join R
    int N,          // total vertices
    int cliqueSize  // current clique size
) {
    // If P is empty, we have found a clique that cannot be expanded.
    if (!P.any()) {
        if (cliqueSize > bestSize) {
            bestSize = cliqueSize;
            bestSet = R;
        }
        return;
    }

    if (cliqueSize + (int)P.count() <= bestSize) {
        return;
    }

    //Color bound will be the an upperbound for the size of the largest clique remaining in P.
    int cBound = colorBound(P, N);
    if (cliqueSize + cBound <= bestSize) {
        return;
    }

    int pivot = P._Find_first();

    // Explore P \ neighbors(pivot)
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

    cout << id << endl;

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
