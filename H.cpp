#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <queue>
#include <string.h>
#include <cmath>
using namespace std;
typedef long long ll;

const int maxn = 1500;
const ll INF = 1e12;
const double ep = 1e-8;
int N , M , K;
vector<int> adj[maxn];

struct Edge{
  int from , to;
  ll cap , flow;
  Edge(int u ,  int v , ll c , ll f): from(u) , to(v) , cap(c) , flow(f) {}
};

struct Dinic{
  int n , m , s , t;
  vector<Edge> edges;
  vector<int> G[maxn];
  int d[maxn] , cur[maxn];
  bool vis[maxn];

  void init(int n){
    this->n = n - 1;
    for(int i = 0; i < maxn; ++i)G[i].clear();
    edges.clear();
  }

  void add(int from , int to , ll cap){
    edges.emplace_back(from , to , cap , 0);
    edges.emplace_back(to , from , 0 , 0);
    m = edges.size();
    G[from].push_back(m - 2);
    G[to].push_back(m - 1);
  }

  bool BFS(){
    memset(vis , 0 , sizeof(vis));
    queue<int> Q;
    Q.push(s);
    d[s] = 0; vis[s] = 1;
    while(!Q.empty()){
      int x = Q.front();
      Q.pop();
      for(int i = 0; i < G[x].size(); ++i){
        Edge& e = edges[G[x][i]];
        if(!vis[e.to] && e.cap > e.flow){
          vis[e.to] = 1;
          d[e.to] = d[x] + 1;
          Q.push(e.to);
        }
      }
    }
    return vis[t];
  }

  ll DFS(int x , ll a){
    if(x == t || a == 0)return a;
    ll flow = 0 , f;
    for(int& i = cur[x]; i < G[x].size(); ++i){
      Edge& e = edges[G[x][i]];
      if(d[x] + 1 == d[e.to] && (f = DFS(e.to , min(a , e.cap - e.flow))) > 0){
        e.flow += f;
        edges[G[x][i] ^ 1].flow -= f;
        flow += f;
        a -= f;
        if(a == 0)break;
      }
    }
    return flow;
  }

  ll maxFlow(int s , int t){
    this->s = s;
    this->t = t;
    double flow = 0;
    while(BFS()){
      memset(cur , 0 , sizeof(cur));
      flow += DFS(s , INF);
    }
    return flow;
  }

  void show(){
    for(auto& e : edges){
      printf("(%d -> %d) = %lld , %lld\n" , e.from , e.to , e.cap , e.flow);
    }
  }

};

bool check(int mid){
  Dinic g;
  g.init(2 * mid + 3);
  int s = 0 , t = 2 * mid + 2;
  for(int i = 1; i <= mid; ++i){
    if(adj[i].size()){
      g.add(s , 2 * i , INF);
      g.add(2 * i , 2 * i + 1 , 1);
      for(int j : adj[i]){
        if(j > i && j <= mid){
          g.add(2 * i + 1 , 2 * j , 1);
        }
      }
    }
    g.add(2 * i , t , 1);
  }
  ll ret = g.maxFlow(s , t);
  return ret == mid;
}

int main(){
  cin >> N;
  for(int i = 1; i <= N; ++i){
    cin >> M;
    for(int j = 1; j <= M; ++j){
      int x; cin >> x;
      adj[i].push_back(x);
    }
  }
  int l = 1 , r = N , best = 1;
  while(l <= r){
    int mid = (l + r) >> 1;
    if(check(mid)){
      best = mid;
      l = mid + 1;
    }
    else{
      r = mid - 1;
    }
  }
  cout << best << endl;
  return 0;
}
