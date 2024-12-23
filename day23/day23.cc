#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;
const int MAXN = 1000;

unordered_map<string, int> getId;
vector<string> getName;
int id = 0;
bool isConnected[MAXN][MAXN];
vector<int> edges[MAXN];
vector<int> maxClique = {};

void generateMaxClique(const int idx, const vector<int>& candidates, vector<int>& clique) {
    if (idx == int(candidates.size())) {
        if (clique.size() > maxClique.size()) {
            maxClique = clique;
        }
        return;
    }

    if (clique.size() + candidates.size() - idx <= maxClique.size()) {
        return;
    }

    int v = candidates[idx];
    bool isClique = true;
    for (int i = 0; i < int(clique.size()); i++) {
        if (!isConnected[v][clique[i]]) {
            isClique = false;
            break;
        }
    }

    if (isClique) {
        clique.push_back(v);
        generateMaxClique(idx + 1, candidates, clique);
        clique.pop_back();
    }

    generateMaxClique(idx + 1, candidates, clique);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    auto start = high_resolution_clock::now();

    freopen("in.txt", "r", stdin);

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

        isConnected[id1][id2] = true;
        isConnected[id2][id1] = true;
        edges[id1].push_back(id2);
        edges[id2].push_back(id1);
    }

    for (int i = 0; i < id; i++) {
        vector<int> clique = {i};
        generateMaxClique(0, edges[i], clique);
    }

    vector<string> names;
    for (int i = 0; i < int(maxClique.size()); i++) {
        names.push_back(getName[maxClique[i]]);
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
