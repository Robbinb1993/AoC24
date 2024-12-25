#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> keys, locks;

void processBlock(vector<string>& currentBlock) {
    if (!currentBlock.empty()) {
        vector<int> colSizes;
        for (int i = 0; i < int(currentBlock[0].size()); i++) {
            colSizes.push_back(0);
        }
        for (int i = 0; i < int(currentBlock.size()); i++) {
            for (int j = 0; j < int(currentBlock[i].size()); j++) {
                colSizes[j] += currentBlock[i][j] == '#';
            }
        }

        bool isKey = currentBlock[0][0] == '.';
        if (isKey) {
            keys.push_back(colSizes);
        }
        else {
            locks.push_back(colSizes);
        }
    }
    currentBlock.clear();
}

void readInput() {
    freopen("in.txt", "r", stdin);

    string line;
    vector<string> currentBlock;

    while (getline(cin, line)) {
        if (line.empty()) {
            processBlock(currentBlock);
        }
        else {
            currentBlock.push_back(line);
        }
    }
    processBlock(currentBlock);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    readInput();

    int ans = 0;
    for (auto& key : keys) {
        for (auto& lock : locks) {
            bool valid = true;
            for (int i = 0; i < int(key.size()) && valid; i++) {
                if (key[i] + lock[i] > 7) {
                    valid = false;
                }
            }
            ans += valid;
        }
    }

    cout << ans << endl;

    return 0;
}
