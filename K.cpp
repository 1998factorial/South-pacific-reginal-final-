#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <set>
#include <string.h>
using namespace std;
typedef long long ll;
typedef pair<int , ll> ii;

const ll inf = 1e18;
const int maxn = 2e5 + 10;
int N , K;
vector<ii> g[maxn];
vector<int> judge;
ll dis[maxn];
bool vis[maxn];
ll dp[maxn][2];
ll maxdis[maxn];
ll cnt[maxn];

void dijkstra(){
  for(int i = 1; i <= N; ++i)dis[i] = inf , vis[i] = false;
  priority_queue<ii , vector<ii> , greater<ii>> q;
  for(int i : judge){
    q.push(make_pair(0 , i));
  }
  while(!q.empty()){
    auto cur = q.top(); q.pop();
    if(vis[cur.second])continue;
    vis[cur.second] = true;
    dis[cur.second] = cur.first;
    for(auto v : g[cur.second]){
      if(!vis[v.first] && dis[v.first] > dis[cur.second] + v.second){
        dis[v.first] = dis[cur.second] + v.second;
        q.push(make_pair(dis[v.first] , v.first));
      }
    }
  }
}

void DFS(int v , int p){
  if(vis[v])dp[v][0] = 0 , cnt[v] = 1;
  for(auto& e : g[v]){
    int u = e.first;
    ll w = e.second;
    if(u != p){
      DFS(u , v);
      if(cnt[u]){
        cnt[v] += cnt[u];
        dp[v][0] = max(dp[v][0] , dp[u][0] + w);
      }
    }
  }
}

void DFS2(int v , int p){
  multiset<ll> vals;
  for(auto& e : g[v]){
    ll u = e.first , w = e.second;
    if(u != p){
      if(cnt[u])
        vals.insert(dp[u][0] + w);
    }
  }
  for(auto& e : g[v]){
    ll u = e.first , w = e.second;
    if(u != p){
      if(cnt[1] - cnt[v] + vis[v])dp[u][1] = dp[v][1] + w;
      if(cnt[u])vals.erase(vals.find(dp[u][0] + w));
      if(vals.size())dp[u][1] = max(dp[u][1] , *vals.rbegin() + w);
      if(cnt[u])vals.insert(dp[u][0] + w);
    }
  }
  for(auto& e : g[v]){
    ll u = e.first;
    if(u != p){
      DFS2(u , v);
    }
  }
}

ll gcd(ll a , ll b){
  return b ? gcd(b , a % b) : a;
}

int main(){
  cin >> N >> K;
  for(int i = 1; i <= K; ++i){
    int x; cin >> x;
    judge.push_back(x);
  }
  for(int i = 1; i <= N - 1; ++i){
    int x , y , w; cin >> x >> y >> w;
    g[x].emplace_back(y , w);
    g[y].emplace_back(x , w);
  }
  dijkstra(); // for each node , find it's minimal distance to judge
  memset(vis , 0 , sizeof(vis));
  for(int i : judge)vis[i] = 1;
  dp[1][1] = 0;
  DFS(1 , 1);
  DFS2(1 , 1);
  vector<int> id;
  for(int i = 1; i <= N; ++i)id.push_back(i) , maxdis[i] = max(dp[i][0] , dp[i][1]);
  sort(id.begin() , id.end() , [&](int x , int y){
    return dis[x] * maxdis[y] > dis[y] * maxdis[x];
  });
  ll d = gcd(dis[id[0]] , maxdis[id[0]]);
  cout << dis[id[0]] / d << "/" << maxdis[id[0]] / d << endl;
}
