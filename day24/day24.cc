#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1000;

unordered_map<string, int> getId;
vector<string> getName;
int lastId = 0;

vector<int> edges[MAXN];
vector<int> parents[MAXN];

enum Operator {
    AND,
    OR,
    XOR,
};

struct Operation {
    int lhsId, rhsId;
    Operator op;
    Operation() : lhsId(-1), rhsId(-1), op(AND) {}
    Operation(int lhsId, int rhsId, Operator op) : lhsId(lhsId), rhsId(rhsId), op(op) {}
};

Operation operations[MAXN];
vector<int> values;

int getOrCreateId(const string& name) {
    if (getId.emplace(name, lastId).second) {
        getName.push_back(name);
        values.push_back(-1);
        lastId++;
    }
    return getId[name];
}

void calculate(const int varId, const string par) {
    if (values[varId] != -1) {
        return;
    }
    Operation operation = operations[varId];
    Operator op = operation.op;
    int lhsVal = values[operation.lhsId];
    if (lhsVal == -1) {
        calculate(operation.lhsId, par);
        lhsVal = values[operation.lhsId];
    }
    int rhsVal = values[operation.rhsId];
    if (rhsVal == -1) {
        calculate(operation.rhsId, par);
        rhsVal = values[operation.rhsId];
    }

    int val = op == Operator::AND ? lhsVal & rhsVal :
        op == Operator::OR ? lhsVal | rhsVal : lhsVal ^ rhsVal;

    values[varId] = val;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    freopen("in.txt", "r", stdin);

    string line;

    vector<string> operationLines;

    while (getline(cin, line)) {
        stringstream ss(line);
        if (line.find(":") != string::npos) {
            string var;
            int val;
            getline(ss, var, ':');
            ss >> val;
            var = var.substr(0, var.find_last_not_of(' ') + 1);

            int varId = getOrCreateId(var);
            values[varId] = val;
            cout << "Assigned: " << var << " (" << varId << ") = " << val << endl;
        }
        else if (line.find("->") != string::npos) {
            string lhs, op, rhs, arrow, result;
            ss >> lhs >> op >> rhs >> arrow >> result;

            if (lhs < rhs) {
                swap(lhs, rhs);
            }

            operationLines.push_back(lhs + " " + op + " " + rhs + " -> " + result);

            int lhsId = getOrCreateId(lhs);
            int rhsId = getOrCreateId(rhs);
            int resultId = getOrCreateId(result);

            edges[lhsId].push_back(resultId);
            edges[rhsId].push_back(resultId);
            parents[resultId].push_back(lhsId);
            parents[resultId].push_back(rhsId);

            Operator opr = op == "AND" ? Operator::AND : op == "OR" ? Operator::OR : Operator::XOR;
            operations[resultId] = Operation(lhsId, rhsId, opr);
        }
    }

    sort(operationLines.begin(), operationLines.end());
    for (string opLine : operationLines) {
        cout << opLine << endl;
    }
    cout << endl;

    long long ans = 0;

    for (int varId = 0; varId < lastId; varId++) {
        calculate(varId, getName[varId]);
        if (values[varId] == 1) {
            string var = getName[varId];
            if (var[0] == 'z') {
                int zValue = stoi(var.substr(1));
                ans += (1LL << zValue);
            }
        }
    }

    cout << ans << endl;

    return 0;
}
