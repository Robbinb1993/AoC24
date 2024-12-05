#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;

bool inOrder[100][100];

auto customComparator = [](int a, int b) {
   if (inOrder[a][b]) return true;
   if (inOrder[b][a]) return false;
   return a < b;
   };

int processInput(const vector<vector<int>>& inputData, function<int(vector<int>&)> operation) {
   int totalSum = 0;
   for (auto values : inputData) {
      totalSum += operation(values);
   }
   return totalSum;
}

int solvePart1(vector<int>& values) {
   int medianSum = 0;
   if (is_sorted(values.begin(), values.end(), customComparator)) {
      medianSum += values[int(values.size()) / 2];
   }
   return medianSum;
}

int solvePart2(vector<int>& values) {
   int medianSum = 0;
   if (!is_sorted(values.begin(), values.end(), customComparator)) {
      auto mid = values.begin() + values.size() / 2;
      nth_element(values.begin(), mid, values.end(), customComparator);
      medianSum += *mid;
   }
   return medianSum;
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   auto start = high_resolution_clock::now();

   freopen("in.txt", "r", stdin);
   string line;

   while (getline(cin, line) && !line.empty()) {
      string left, right;
      stringstream ss(line);
      getline(ss, left, '|');
      getline(ss, right, '|');

      int v1 = stoi(left);
      int v2 = stoi(right);

      inOrder[v1][v2] = true;
   }

   vector<vector<int>> inputData;
   while (getline(cin, line)) {
      stringstream ss(line);
      vector<int> values;
      string temp;

      while (getline(ss, temp, ',')) {
         values.push_back(stoi(temp));
      }

      inputData.push_back(values);
   }

   cout << processInput(inputData, solvePart1) << " " << processInput(inputData, solvePart2) << endl;

   auto stop = high_resolution_clock::now();
   auto duration = duration_cast<milliseconds>(stop - start);
   cout << "Time: " << duration.count() << "ms" << endl;

   return 0;
}
