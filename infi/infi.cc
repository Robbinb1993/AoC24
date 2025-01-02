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

int getParamValue(const Instruction& instr, const int x, const int y, const int z) {
    if (holds_alternative<int>(instr.param)) {
        return get<int>(instr.param);
    }
    else if (holds_alternative<char>(instr.param)) {
        char var = get<char>(instr.param);
        switch (var) {
        case 'X': return x;
        case 'Y': return y;
        case 'Z': return z;
        default:
            throw invalid_argument("Invalid variable: " + string(1, var));
        }
    }
    throw invalid_argument("Parameter is neither int nor char.");
}

int solve(const int x, const int y, const int z) {
    int pc = 0;
    stack<int> S;

    while (true) {
        auto& instr = instructions[pc];
        switch (instr.type) {
        case InstructionType::PUSH:
            S.push(getParamValue(instr, x, y, z));
            break;

        case InstructionType::ADD:
            if (S.size() >= 2) {
                int a = S.top(); S.pop();
                int b = S.top(); S.pop();
                S.push(a + b);
            }
            break;

        case InstructionType::JMPOS:
            if (!S.empty()) {
                int topVal = S.top();
                S.pop();
                if (topVal >= 0) {
                    pc += getParamValue(instr, x, y, z);
                }
            }
            break;

        case InstructionType::RET:
            if (!S.empty()) {
                return S.top();
            }
            return 0;
        }

        pc++;

        if (pc >= instructions.size()) throw("Invalid program counter");
    }

    return 0;
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
