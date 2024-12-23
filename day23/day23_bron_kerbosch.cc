#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;
const int MAXN = 1000;

unordered_map<string, int> getId;
vector<string> getName;
int id = 0;
unordered_set<int> edges[MAXN];
unordered_set<int> maxClique = {};

int choosePivot(const unordered_set<int>& P, const unordered_set<int>& X) {
    int pivot = -1;
    int maxDegree = -1;

    unordered_set<int> candidates(P.begin(), P.end());
    candidates.insert(X.begin(), X.end());

    for (int u : candidates) {
        int degree = 0;
        for (int neighbor : edges[u]) {
            if (P.count(neighbor)) {
                degree++;
            }
        }
        if (degree > maxDegree) {
            maxDegree = degree;
            pivot = u;
        }
    }

    return pivot;
}

//P is the set of candidates for the clique, starting with all vertices
void BronKerbosch(unordered_set<int> R, unordered_set<int> P, unordered_set<int> X) {
    if (P.empty() && X.empty()) {
        if (R.size() > maxClique.size()) {
            maxClique = R;
        }
        return;
    }

    int pivot = choosePivot(P, X);

    unordered_set<int> candidates(P);
    for (int neighbor : edges[pivot]) {
        candidates.erase(neighbor);
    }

    for (auto candidate : candidates) {
        R.insert(candidate);

        unordered_set<int> newP, newX;

        for (int neighbor : edges[candidate]) {
            if (P.count(neighbor)) {
                newP.insert(neighbor);
            }
            if (X.count(neighbor)) {
                newX.insert(neighbor);
            }
        }

        BronKerbosch(R, newP, newX);

        R.erase(candidate);
        P.erase(candidate);
        X.insert(candidate);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    auto start = high_resolution_clock::now();

    freopen("aoc-2024-day-23-challenge-3.txt", "r", stdin);

    string line;
    while (getline(cin, line)) {
        string s1, s2;
        stringstream ss(line);
        getline(ss, s1, '-');
        getline(ss, s2, '-');

        if (getId.emplace(s1, id).second) {
            getName.push_back(s1);
            id++;
        }
        if (getId.emplace(s2, id).second) {
            getName.push_back(s2);
            id++;
        }

        int id1 = getId[s1];
        int id2 = getId[s2];

        edges[id1].insert(id2);
        edges[id2].insert(id1);
    }

    unordered_set<int> R, P, X;
    for (int i = 0; i < id; i++) {
        P.insert(i);
    }

    BronKerbosch(R, P, X);

    vector<string> names;
    for (int v : maxClique) {
        names.push_back(getName[v]);
    }

    sort(names.begin(), names.end());

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "Time: " << duration.count() << " milliseconds" << endl;

    for (int i = 0; i < int(names.size()); i++) {
        if (i)
            cout << ",";
        cout << names[i];
    }
    cout << endl;

    return 0;
}
