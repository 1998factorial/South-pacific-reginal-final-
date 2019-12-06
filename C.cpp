#include <iostream>
#include <vector>
#include <algorithm>
#define MAXN 205
#define ID if (false)
using namespace std;

int n, k;
int grid[MAXN][MAXN];
int matrix[MAXN][MAXN];
vector<int> e[MAXN];
vector<int> corners;
int width, height;

bool is_corner(int at) {
  return e[at].size() == 2;
}

bool is_edge(int at) {
  return e[at].size() == 3;
}

bool is_body(int at) {
  return e[at].size() == 4;
}

// adds grid[r][c] only if it's inside the bounds
void add_if(vector<int>& out, int r, int c) {
  if (r < 0 || r >= height || c < 0 || c >= width) return;
  out.push_back(grid[r][c]);
}

// finds the unique element in nexts but not in nots
int find_not(vector<int>& nexts, vector<int>& nots) {
  int answer = 0;
  for (auto next : nexts) {
    bool ignore = false;
    for (auto nt : nots) {
      if (nt == next) {
        ignore = true;
      }
    }
    if (ignore) continue;

    if (answer) return 0;
    answer = next;
  }
  return answer;
}

// finds the unique element in nexts but not in nots
int find_not_4(vector<int>& nexts, vector<int>& nots) {
  for (auto next : nexts) {
    bool ignore = false;
    for (auto nt : nots) {
      if (nt == next) {
        ignore = true;
      }
    }
    if (ignore) continue;
    if (is_body(next)) continue;
    return next;
  }
  return 0;
}

void print_grid() {
  ID cout << "printing grid" << endl;
  cout << height << " " << width << endl;
  for (int r = 0; r < height; r++) {
    for (int c = 0; c < width; c++) cout << grid[r][c] << " ";
    cout << endl;
  }
}

// attempts to fill the first row with degree 3s
bool solve() {
  if (corners.size() != 4) return false;

  // corners must be connected to either edge or corner
  for (int corner : corners) {
    if (!is_corner(corner)) return false;
    for (int next : e[corner]) {
      if (!is_edge(next) && !is_corner(next)) return false;
    }
  }

  // nothing should border itself
  for (int i = 1; i <= n; i++) {
    for (int next : e[i]) {
      if (next == i) return false;
    }
  }

  grid[0][0] = corners[0];
  grid[0][1] = e[corners[0]][0];
  int c = 1;
  if (!is_corner(grid[0][1])) {

  }
  bool reached_corner = false;
  for (c = 2; true; ++c) {
    vector<int> nots;
    if (c != 1) nots.push_back(grid[0][c-2]);
    grid[0][c] = find_not_4(e[grid[0][c-1]], nots);
    if (grid[0][c] == 0) return false;
    if (is_body(grid[0][c])) return false;
    if (is_corner(grid[0][c])) {
      reached_corner = true;
      break;
    }
  }
  if (!reached_corner) return false;

  width = c+1;
  if (n % width != 0) return false;
  height = n / width;
  ID print_grid();

  for (int r = 1; r < height; r++) {
    // determine each grid cell based on the cell above it
    for (int c = 0; c < width; c++) {
      vector<int> nots;
      add_if(nots, r-1, c-1);
      add_if(nots, r-1, c+1);
      add_if(nots, r-2, c);
      grid[r][c] = find_not(e[grid[r-1][c]], nots);
      if (!grid[r][c]) return false;
    }
  }

  int count[MAXN];
  fill(count, count+MAXN, 0);
  for (int r = 0; r < height; r++) {
    for (int c = 0; c < width; c++) {
      count[grid[r][c]] += 1;
      bool redge = r == 0 || r == height-1;
      bool cedge = c == 0 || c == width-1;
      if (redge && cedge) {
        if (!is_corner(grid[r][c])) return false;
      } else if (redge || cedge) {
        if (!is_edge(grid[r][c])) return false;
      } else {
        if (!is_body(grid[r][c])) return false;
      }

      vector<int> next;
      add_if(next, r+1, c);
      add_if(next, r-1, c);
      add_if(next, r, c+1);
      add_if(next, r, c-1);
      sort(next.begin(), next.end());
      sort(e[grid[r][c]].begin(), e[grid[r][c]].end());
      if (next != e[grid[r][c]]) return false;
    }
  }

  for (int i = 1; i <= n; i++) {
    if (count[i] != 1) return false;
  }

  ID print_grid();
  return true;
}

int main() {
  cin >> n;
  for (int i = 1; i <= n; ++i) {
    cin >> k;
    for (int j = 0; j < k; ++j) {
      int x;
      cin >> x;
      matrix[i][x] = true;
      matrix[x][i] = true;
      e[i].push_back(x);
    }
  }

  // find the four corners
  for (int i = 1; i <= n; ++i) {
    if (e[i].size() == 2) corners.push_back(i);
  }

  if (!solve()) cout << -1 << endl;
  else print_grid();
}
