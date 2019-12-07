#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#define LOGV 50
#define MAXN 200005
#define MAXV 3000000000L
#define ll long long int
#define iii tuple<ll, ll, ll>
using namespace std;

struct node {
  ll s, e, v;
  ll c[2];
};

node tree[LOGV*MAXN];
ll root;
ll tree_size = 1;

ll new_node(ll s, ll e) {
  tree[tree_size].s = s;
  tree[tree_size].e = e;
  tree[tree_size].v = 0;
  tree[tree_size].c[0] = 0;
  tree[tree_size].c[1] = 0;
  return tree_size++;
}

ll query(ll at, ll s, ll e) {
  if (at == 0) return 0;
  if (s > tree[at].e || e < tree[at].s) return 0;
  if (s <= tree[at].s && e >= tree[at].e) return tree[at].v;
  else {
    return query(tree[at].c[0], s, e) + query(tree[at].c[1], s, e);
  }
}

// place a tree at index
void update(ll at, ll index) {
  if (at == 0) return;
  if (index < tree[at].s || index > tree[at].e) return;
  tree[at].v += 1;
  if (tree[at].s != tree[at].e) {
    ll s = tree[at].s;
    ll e = tree[at].e;
    ll m = (s+e) / 2;
    if (tree[at].c[0] == 0) tree[at].c[0] = new_node(s, m);
    if (tree[at].c[1] == 0) tree[at].c[1] = new_node(m+1, e);
    update(tree[at].c[0], index);
    update(tree[at].c[1], index);
  }
}

// true iff all filled between l and r
bool filled(ll l, ll r) {
  ll count = (r-l+1);
  return query(root, l, r) == count;
}

ll N;
ll answer[MAXN];
vector<iii> s;

int main() {
  root = new_node(0, MAXV);
  cin >> N;
  for (ll i = 0; i < N; i++) {
    ll d, t;
    cin >> d >> t;
    ll l = d-t;
    ll r = d+t;
    if (l < 1) l = 1;
    s.push_back(make_tuple(l, r, i));
  }
  sort(s.begin(), s.end(), [&](iii& a, iii& b) {
    return get<1>(a) < get<1>(b);
  });

  for (auto p : s) {
    ll l = get<0>(p);
    ll r = get<1>(p);
    ll i = get<2>(p);
    ll lo = l;
    ll hi = r;
    ll best = -1;
    while (hi >= lo) {
      ll mid = (hi+lo) / 2;
      if (!filled(l, mid)) {
        best = mid;
        hi = mid - 1;
      } else {
        lo = mid + 1;
      }
    }
    update(root, best);
    answer[i] = best;
  }

  for (ll i = 0; i < N; i++) {
    cout << answer[i] << " ";
  }
  cout << endl;
}
