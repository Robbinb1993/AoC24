#include <bits/stdc++.h>
using namespace std;

long long startA, startB, startC;
vector<int> program;

long long getComboVal(const int idx, const long long A, const long long B, const long long C) {
   if (idx <= 3)
      return idx;
   else if (idx == 4)
      return A;
   else if (idx == 5)
      return B;
   else if (idx == 6)
      return C;
   else
      return -1;
}

int run(long long A) {
   long long B = 0;
   long long C = 0;
   int p = 0;

   while (p < int(program.size())) {
      bool incrementPointer = true;
      switch (program[p]) {
      case 0: {
         long long comboVal = getComboVal(program[p + 1], A, B, C);
         A >>= comboVal;
         break;
      }
      case 1: {
         long long literal = program[p + 1];
         B ^= literal;
         break;
      }
      case 2: {
         long long comboVal = getComboVal(program[p + 1], A, B, C);
         B = comboVal & 7;
         break;
      }
      case 3: {
         if (A == 0) {
            break;
         }
         p = program[p + 1];
         incrementPointer = false;
         break;
      }
      case 4: {
         B ^= C;
         break;
      }
      case 5: {
         long long comboVal = getComboVal(program[p + 1], A, B, C);
         return comboVal & 7;
      }
      case 6: {
         long long comboVal = getComboVal(program[p + 1], A, B, C);
         B = (A >> comboVal);
         break;
      }
      case 7: {
         long long comboVal = getComboVal(program[p + 1], A, B, C);
         C = (A >> comboVal);
         break;
      }
      }
      if (incrementPointer)
         p += 2;
   }

   return -1;
}

long long solve(const int idx, const long long A) {
   if (idx < 0)
      return A;

   for (int i = 0; i < 8; i++) {
      long long nextA = (A << 3) | i;
      if (run(nextA) == program[idx]) {
         long long result = solve(idx - 1, nextA);
         if (result != -1) return result;
      }
   }

   return -1;
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   freopen("in.txt", "r", stdin);

   string line, programInput;

   while (getline(cin, line)) {
      if (line.empty()) continue;
      if (line.find("Register A:") != string::npos) {
         sscanf(line.c_str(), "Register A: %lld", &startA);
      }
      else if (line.find("Register B:") != string::npos) {
         sscanf(line.c_str(), "Register B: %lld", &startB);
      }
      else if (line.find("Register C:") != string::npos) {
         sscanf(line.c_str(), "Register C: %lld", &startC);
      }
      else if (line.find("Program:") != string::npos) {
         programInput = line.substr(line.find(":") + 1);
         break;
      }
   }

   stringstream ss(programInput);
   string token;
   while (getline(ss, token, ',')) {
      if (!token.empty()) program.push_back(stoi(token));
   }

   cout << solve(int(program.size()) - 1, 0) << endl;

   return 0;
}
