#include <bits/stdc++.h>
#include <set>
#include <vector>
#define MAXN 205
#define INF 1000000
#define ID if (true)
using namespace std;

int n, k, x, w, h;
vector<int> edge[MAXN];
int matrix[MAXN][MAXN];
int answer[MAXN][MAXN];
int dist[MAXN];

void bfs(int start) {
	fill(dist, dist+MAXN, INF);
	dist[start] = 0;
	queue<int> q;
	q.push(start);
	while (q.size()) {
		int at = q.top();
		q.pop();
		for (int next : edge[at]) {
			if (dist[next] == INF) {
				dist[next] = dist[at] + 1;
				q.push(next);
			}
		}
	}
}

unordered_set<int> corners;
unordered_set<int> sides;
unordered_set<int> centers;

void fail() {
	cout << -1 << endl;
	exit(EXIT_SUCCESS);
}

void add_if(unordered_set<int>& ignore, int r, int c) {
	if (r < 0 || r >= h || c < 0 || c >= w || answer[r][c] == 0) return;
	ignore.insert(answer[r][c]);
}

void check_neighbours(int r, int c) {
	int at = answer[r][c];
	unordered_set<int> grid_neighbours;
	add_if(grid_neighbours, r+1, c);
	add_if(grid_neighbours, r-1, c);
	add_if(grid_neighbours, r, c+1);
	add_if(grid_neighbours, r, c-1);

	unordered_set<int> original_neighbours;
	for (int next : edge[at]) original_neighbours.insert(next);
	if (grid_neighbours != original_neighbours) fail();
}

int find_unique(int size, int r, int c) {
	int next_to = answer[r][c];
	unordered_set<int> ignore;
	add_if(ignore, r+1, c);
	add_if(ignore, r-1, c);
	add_if(ignore, r, c+1);
	add_if(ignore, r, c-1);

	int found = 0;
	for (int next : edge[next_to]) {
		if (edge[next].size() == size && !ignore.count(next)) {
			if (found) {
				ID printf("duplicate size %d neighbour of %d, %d\n", size, r, c);
				fail();
			}
			found = next;
		}
	}
	if (found == 0) {
		ID printf("failed to find size %d neighbour of %d, %d\n", size, r, c);
		fail();
	}
	ID printf("found size %d neighbour of %d, %d = %d\n", size, r, c, found);
	return found;
}

bool solve() {
	// construct adjacency list
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> k;
		while (k--) {
			cin >> x;
			if (x == i) return false;
			edge[i].push_back(x);
			matrix[i][x] = true;
		}
	}

	// check if connectivity reflexive
	for (int a = 1; a <= n; a++) {
		for (int b = 1; b <= n; b++) {
			if (matrix[a][b] != matrix[b][a]) return false;
		}
	}

	// categorise and check if size invalid
	for (int i = 1; i <= n; i++) {
		int size = edge[i].size();
		if (size <= 1) return false;
		if (size == 2) corners.insert(i);
		if (size == 3) sides.insert(i);
		if (size == 4) centers.insert(i);
		if (size >= 5) return false;
	}

	if (corners.size() != 4) return false;
	if (sides.size() % 2 != 0) return false;
	int hf = sides.size() / 2;

	// solve quadratic equation to find w and h
	// (w+h)*2 = sides
	// w+h = hf
	// w*h = centers
	// w*(hf-w) = centers
	// w*hf - w*w = centers
	// w*w - hf*w + centers = 0
	int a = 1;
	int b = -hf;
	int c = centers.size();
	int d = b*b-4*a*c;
	if (d < 0) return false;

	w = (-b+((int) round(sqrt(d))))/(2*a);
	h = hf - w;
	if (w + h != hf) return false;
	if (w * h != centers.size()) return false;
	w += 2;
	h += 2;

	answer[0][0] = *corners.begin();
	bfs(answer[0][0]);

	ID cout << "decided first corner to be " << answer[0][0] << endl;
	if (w == 2 && h == 2) {
		answer[0][1] = edge[answer[0][0]][0];
		if (edge[answer[0][1]].size() != 2) return false;
	} else if (w == 2 && h != 2) {
		answer[0][1] = find_unique(2, 0, 0);
	} else {
		answer[0][1] = edge[answer[0][0]][0];
		if (edge[answer[0][1]].size() != 3) return false;
	}

	ID cout << "decided second corner to be " << answer[0][1] << endl;
	for (int c = 2; c < w; c++) {
		cout << c << " " << (w-1) << endl;
		if (c == w-1) {
			cout << "reached end" << endl;
			answer[0][c] = find_unique(2, 0, c-1);
		} else {
			answer[0][c] = find_unique(3, 0, c-1);
		}
	}
	ID cout << "first row complete" << endl;
	for (int r = 1; r < h; r++) {
		for (int c = 0; c < w; c++) {
			bool re = r == h-1;
			bool ce = c == 0 || c == w-1;
			if (re && ce) {
				answer[r][c] = find_unique(2, r-1, c);
			} else if (re || ce) {
				answer[r][c] = find_unique(3, r-1, c);
			} else {
				answer[r][c] = find_unique(4, r-1, c);
			}
		}
	}

	unordered_set<int> seen;
	for (int r = 0; r < h; r++) {
		for (int c = 0; c < w; c++) {
			check_neighbours(r, c);
			if (seen.count(answer[r][c])) return false;
			seen.insert(answer[r][c]);
		}
	}

	return true;
}

int main() {
	if (!solve()) cout << -1 << endl;
	else {
		cout << h << " " << w << endl;
		for (int r = 0; r < h; r++) {
			for (int c = 0; c < w; c++) {
				cout << answer[r][c] << " ";
			}
			cout << endl;
		}
	}
}
