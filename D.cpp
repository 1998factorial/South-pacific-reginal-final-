#include <iostream>
#include <string.h>
using namespace std;

int N, M;
string s;
int answer;

int main() {
  cin >> N >> M;
  while (N--) {
    cin >> s;
    bool exist[100];
    //fill(exist, exist+100, false);
    memset(exist, false, sizeof(exist));
    bool valid = true;
    for (int i = 0; i < s.length(); i++) {
      int c = s[i] - 'A';
      if (exist[c]) valid = false;
      exist[c] = true;
      if (c >= M) valid = false;
    }
    if (valid) answer += 1;
  }
  cout << answer << endl;
}
