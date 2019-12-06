#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#define MAXN 262144
#define INF 1152921504606846976L
#define ll long long int
#define ii pair<ll, ll>
#define ID if (false)
using namespace std;

const ll inf = 1e18;
const int maxn = 2e5 + 10;
ll N, K;
vector<ii> g[maxn];
vector<int> judge;
ll dis[maxn];
bool vis[maxn];

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

struct init_node {
  ll s, e, d;
  ll parent;
  ll dist_parent;
  ll lo, hi;
  ll hi_exclude; // longest distance from parent excluding this node an it's children
  vector<ii> next;
};

init_node init[MAXN];
// ll N, K;
ll thejudge[MAXN];

ll next_s = 0;
ll dfs(ll at, ll prev, ll dist, ll dist_parent) {
  init[at].s = next_s++;
  init[at].e = init[at].s;
  init[at].d = dist;
  init[at].dist_parent = dist_parent;
  init[at].parent = prev;
  for (auto p : init[at].next) {
    ll next = p.first;
    ll w = p.second;
    if (next == prev) continue;
    init[at].e = max(init[at].e, dfs(next, at, dist + w, w));
  }
  return init[at].e;
}

struct node {
  ll s, e, lo, hi;
};

node tree[MAXN*2];
void init_tree(ll at, ll s, ll e) {
  tree[at].s = s;
  tree[at].e = e;
  tree[at].lo = INF;
  tree[at].hi = 0;
  if (s != e) {
    ll m = (s + e) / 2;
    init_tree(at*2+1, s, m);
    init_tree(at*2+2, m+1, e);
  }
}

void update_tree(ll at, ll i, ll v) {
  if (i < tree[at].s || i > tree[at].e) return;
  tree[at].lo = min(tree[at].lo, v);
  tree[at].hi = max(tree[at].hi, v);
  if (tree[at].s != tree[at].e) {
    update_tree(at*2+1, i, v);
    update_tree(at*2+2, i, v);
  }
}

ll query_tree_max(ll at, ll s, ll e) {
  if (e < s) return 0;
  if (s > tree[at].e || e < tree[at].s) return 0;
  if (s <= tree[at].s && e >= tree[at].e) return tree[at].hi;
  else {
    if (tree[at].s == tree[at].e) return 0;
    return max(
      query_tree_max(at*2+1, s, e),
      query_tree_max(at*2+2, s, e)
    );
  }
}

void calc(ll at, ll prev) {
  init[at].hi = 0;
  if (at != 1) {
    init[at].hi = max(init[at].hi, init[at].dist_parent + query_tree_max(0, init[init[at].parent].s, init[at].s-1) - init[init[at].parent].d);
    init[at].hi = max(init[at].hi, init[at].dist_parent + query_tree_max(0, init[at].e+1, init[init[at].parent].e) - init[init[at].parent].d);
    if (init[at].parent != 1) {
      init[at].hi = max(init[at].hi, init[at].dist_parent + init[init[at].parent].hi_exclude);
    }
  }
  init[at].hi_exclude = init[at].hi;
  init[at].hi = max(init[at].hi, query_tree_max(0, init[at].s, init[at].e) - init[at].d);
  ID cout << at << " [" << init[at].s << " " << init[at].e << "] " << init[at].hi << endl;
  for (auto p : init[at].next) if (p.first != prev) calc(p.first, at);
}

ll gcd(ll a , ll b){
  return b ? gcd(b , a % b) : a;
}

int main() {
  cin >> N >> K;
  while (K--) {
    ll x;
    cin >> x;
    judge.push_back(x);
    thejudge[x] = true;
  }

  for (ll i = 1; i < N; ++i) {
    ll u, v, w;
    cin >> u >> v >> w;
    init[u].next.push_back(make_pair(v, w));
    init[v].next.push_back(make_pair(u, w));
    g[u].emplace_back(v, w);
    g[v].emplace_back(u, w);
  }

  dijkstra();
  dfs(1, 0, 0, 0);
  init_tree(0, 0, MAXN-1);

  for (int i = 1; i <= N; ++i) {
    if (thejudge[i]) {
      // cout << i << " is a thejudge with dist " << init[i].d << endl;
      update_tree(0, init[i].s, init[i].d);
    }
  }
  calc(1, 0);

  vector<ll> ns;
  for (ll i = 1; i <= N; ++i) {
    ns.push_back(i);
    ID cout << i << " " << dis[i] << " " << init[i].hi << endl;
  }
  sort(ns.begin(), ns.end(), [&](ll a, ll b) {
    ll a_lo = dis[a];
    ll a_hi = init[a].hi;
    ll b_lo = dis[b];
    ll b_hi = init[b].hi;
    return a_lo*b_hi > b_lo*a_hi;
  });
  ID cout << ns[0] << endl;

  ll a = ns[0];
  ll a_lo = dis[a];
  ll a_hi = init[a].hi;

  if (a_lo == 0) {
    cout << "0/1" << endl;
  } else {
    ll cmn = gcd(a_lo, a_hi);
    a_lo /= cmn;
    a_hi /= cmn;
    cout << a_lo << "/" << a_hi << endl;
  }

  // cout << "nofault" << endl;
  // calc(1, 0);
  // for (int i = 1; i <= N; ++i) {
  //   cout << i << " " << init[i].hi << endl;
  // }
  // cout << endl;

}
