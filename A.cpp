#include <iostream>
using namespace std;

int N, l1 , r1 , l2 , r2;
int a[1005];

bool solve(){
  int v1 = 0;
  int v2 = 0;
  int ones = 0;
  for (int i = 1; i <= N; ++i) {
    int half = a[i] / 2;
    v1 += half;
    v2 += half;
    if (a[i] % 2 == 1) ones += 1;
  }

  while (v1 < l1 && ones > 0) {
    v1 += 1;
    ones -= 1;
  }

  while (v2 < l2 && ones > 0) {
    v2 += 1;
    ones -= 1;
  }

  while (ones > 0 && v1 < r1) {
    v1 += 1;
    ones -= 1;
  }

  while (ones > 0 && v2 < r2) {
    v2 += 1;
    ones -= 1;
  }

  if (v1 < l1 || v1 > r1 || v2 < l2 || v2 > r2 || ones > 0) return false;
  return true;
}

int main(){
  cin >> N;
  cin >> l1 >> r1;
  cin >> l2 >> r2;
  for (int i = 1; i <= N; ++i) cin >> a[i];
  if (solve()) cout << "Yes\n";
  else cout << "No\n";
  return 0;
}
