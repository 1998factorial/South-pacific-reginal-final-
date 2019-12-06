#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;

const ll inf = 8e18;
ll dp[205][205];
int N , K;
ll c[205];

int main(){
  cin >> N >> K;
  for(int i = 1; i <= N; ++i)cin >> c[i];
  for(int j = 0; j <= K; ++j){
    for(int i = 1; i <= N; ++i){
      dp[i][j] = inf;
    }
  }
  sort(c + 1 , c + N + 1);
  dp[0][0] = 0;
  for(int j = 1; j <= K; ++j){
    for(int i = 1; i <= N; ++i){
      for(int k = 0; k < i; ++k){
        dp[i][j] = min(dp[i][j] , dp[k][j - 1] + (c[i] - c[k + 1]) * (c[i] - c[k + 1]));
      }
    }
  }
  cout << dp[N][K] << endl;
  return 0;
}
