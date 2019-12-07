#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>
using namespace std;
typedef long long ll;
const int maxn = 1e5 + 10;
int N;
struct point{
  ll x , y;
} P[maxn];

unordered_map<int, int> s;

ll query(ll length, ll height) {
  if (height >= length || length == 0) return 0;
  ll x = length / height;

  ll complete = length / x;
  ll rem = length % x;
  return rem * complete + (complete * (complete - 1)) / 2 * x;
}

ll solve_1(vector<point> &pt, ll y) {
    for (int i = 0 ; i < pt.size(); i++) {
      cout << pt[i].x << " " << pt[i].y << endl;
    }
    ll ret = 0;
    int m = pt.size();
    for (int i = 1; i < m; i++) {
      ret += (pt[i].x - pt[i-1].x) * (min(pt[i].y, pt[i-1].y) -y);
      ret += query(pt[i].x - pt[i-1].x, abs(pt[i].y - pt[i-1].y));
    }
    return ret;
}

ll solve_2(vector<point> &pt, ll y) {

  cout << endl;
      for (int i = 0 ; i < pt.size(); i++) {
        cout << pt[i].x << " " << pt[i].y << endl;
      }
    ll ret = 0;
    int m = pt.size();
    for (int i = 1; i < m; i++) {
      ret += (pt[i].x - pt[i-1].x) * (y - max(pt[i].y, pt[i-1].y));
      ret += query(pt[i].x - pt[i-1].x, abs(pt[i].y - pt[i-1].y));
    }
    return ret;
}


int main(){
  vector<point> up, down;
  cin >> N;
  for(int i = 1; i <= N; ++i){
    cin >> P[i].x >> P[i].y;
  }
  sort(P + 1 , P + 1 + N , [&](point& a , point& b){
    if(a.x == b.x)return a.y < b.y;
    return a.x < b.x;
  });

  for (int i = 1; i <= N; ++i) {
    if (P[i].x != P[1].x) break;
    s[P[i].y] = i;
  }
  int l = 1, r = N;
  for (int i = N; i >= 1; i--) {
    if (P[i].x != P[N].x) break;
    if (s.find(P[i].y) != s.end()) {
      l = s[P[i].y];
      r = i;
      break;
    }
  }
  up.push_back(P[l]);
  down.push_back(P[l]);
  for (int i = 1; i <= N; i++) {
    if (i != l && i != r) {
      if (P[i].y > P[l].y) {
        up.push_back(P[i]);
      } else {
          down.push_back(P[i]);
      }
    }
  }

  up.push_back(P[r]);
  down.push_back(P[r]);

  cout << solve_1(up, P[r].y) + solve_2(down, P[r].y) << endl;
}
