#include <bits/stdc++.h>
using namespace std;

const int MAX_X = 30;
const int MAX_Y = 30;
const int MAX_Z = 30;

enum InstructionType {
    PUSH,
    ADD,
    JMPOS,
    RET
};

struct Instruction {
    InstructionType type;
    variant<monostate, int, char> param;
};

vector<Instruction> instructions;

void readInstructions() {
    freopen("in.txt", "r", stdin);
    string type, param;
    while (cin >> type) {
        Instruction instr;
        if (type == "push") instr.type = PUSH;
        else if (type == "add") instr.type = ADD;
        else if (type == "jmpos") instr.type = JMPOS;
        else if (type == "ret") instr.type = RET;

        if (instr.type == PUSH || instr.type == JMPOS) {
            cin >> param;
            if (isdigit(param[0]) || param[0] == '-') instr.param = stoi(param);
            else instr.param = param[0];
        }
        instructions.push_back(instr);
    }
}

int solve(const int x, const int y, const int z) {
    int pc = 0;
    stack<int> S;

    while (pc < instructions.size()) {
        const auto& instr = instructions[pc];
        switch (instr.type) {
        case PUSH:
            S.push(holds_alternative<int>(instr.param) ? get<int>(instr.param)
                : (get<char>(instr.param) == 'X' ? x
                    : get<char>(instr.param) == 'Y' ? y : z));
            break;
        case ADD:
            if (S.size() >= 2) {
                int a = S.top(); S.pop();
                int b = S.top(); S.pop();
                S.push(a + b);
            }
            break;
        case JMPOS:
            if (!S.empty()) {
                if (S.top() >= 0) {
                    pc += get<int>(instr.param);
                }
                S.pop();
            }
            break;
        case RET:
            return S.empty() ? 0 : S.top();
        }

        pc++;
    }

    throw("No return instruction found");
}

bool isNewCloud[MAX_X][MAX_Y][MAX_Z];
const int DX[6] = {1, -1, 0, 0, 0, 0};
const int DY[6] = {0, 0, 1, -1, 0, 0};
const int DZ[6] = {0, 0, 0, 0, 1, -1};

void floodFill(const int x, const int y, const int z) {
    if (x < 0 || x >= MAX_X || y < 0 || y >= MAX_Y || z < 0 || z >= MAX_Z) return;
    if (!isNewCloud[x][y][z]) return;

    isNewCloud[x][y][z] = false;

    for (int i = 0; i < 6; i++) {
        floodFill(x + DX[i], y + DY[i], z + DZ[i]);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    readInstructions();

    long long ansPart1 = 0;
    for (int x = 0; x < MAX_X; x++) {
        for (int y = 0; y < MAX_Y; y++) {
            for (int z = 0; z < MAX_Z; z++) {
                int currAns = solve(x, y, z);
                if (currAns > 0) {
                    isNewCloud[x][y][z] = true;
                }
                ansPart1 += currAns;
            }
        }
    }

    int ansPart2 = 0;
    for (int x = 0; x < MAX_X; x++) {
        for (int y = 0; y < MAX_Y; y++) {
            for (int z = 0; z < MAX_Z; z++) {
                if (isNewCloud[x][y][z]) {
                    ansPart2++;
                    floodFill(x, y, z);
                }
            }
        }
    }

    cout << ansPart1 << " " << ansPart2 << endl;

    return 0;
}
