#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;
const int MAXN = 3500;

unordered_map<string, int> getId;
vector<string> getName;
int id = 0;
bool isConnected[MAXN][MAXN];
vector<int> edges[MAXN];

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

    vector<int> res;
    int best = 0;

    for (int i = 0; i < id; i++) {
        int totEdges = int(edges[i].size());
        for (int bm = 0; bm < (1 << totEdges); bm++) {
            if (best >= __builtin_popcount(bm)) {
                continue;
            }

            vector<int> nodes;

            for (int bit = 0; bit < totEdges; bit++) {
                if (bm & (1 << bit)) {
                    nodes.push_back(edges[i][bit]);
                }
            }

            bool valid = true;

            for (int i = 0; i < int(nodes.size()) && valid; i++) {
                for (int j = i + 1; j < int(nodes.size()) && valid; j++) {
                    if (!isConnected[nodes[i]][nodes[j]]) {
                        valid = false;
                    }
                }
            }

            if (!valid)
                continue;

            best = int(nodes.size());
            nodes.push_back(i);
            res = nodes;
        }
    }

    vector<string> names;
    for (int i = 0; i < int(res.size()); i++) {
        names.push_back(getName[res[i]]);
    }

    sort(names.begin(), names.end());

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "Time: "
        << duration.count() << " milliseconds" << endl;

    for (int i = 0; i < int(names.size()); i++) {
        cout << names[i] << ",";
    }

    cout << endl;

    return 0;
}
