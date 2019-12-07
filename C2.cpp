#include <bits/stdc++.h>
#include <set>
#include <vector>
#define NC 4
#define MAXN 205
#define INF 1000000
#define ID if (false)
using namespace std;

int n, k, x, w, h;
vector<int> edge[MAXN];
int matrix[MAXN][MAXN];
int answer[MAXN][MAXN];

int dist[MAXN];
int backtrack[MAXN];
vector<int> shortest_path(int start, int end) {
	fill(dist, dist+MAXN, INF);
	fill(backtrack, backtrack+MAXN, 0);
	dist[start] = 0;

	queue<int> q;
	q.push(start);
	while (q.size()) {
		int at = q.front();
		q.pop();
		if (at == end) {
			vector<int> path;
			while (at != 0) {
				path.push_back(at);
				at = backtrack[at];
			}
			reverse(path.begin(), path.end());
			return path;
		}
		for (int next : edge[at]) {
			if (dist[next] == INF) {
				backtrack[next] = at;
				dist[next] = dist[at] + 1;
				q.push(next);
			}
		}
	}
	return {};
}

vector<int> corners;
vector<int> sides;
vector<int> centers;

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
		if (size == 2) corners.push_back(i);
		if (size == 3) sides.push_back(i);
		if (size == 4) centers.push_back(i);
		if (size >= 5) return false;
	}

	if (corners.size() != NC) return false;
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

	w = (-b-((int) round(sqrt(d))))/(2*a);
	h = hf - w;
	if (w < 0) return false;
	if (h < 0) return false;
	if (w + h != hf) return false;
	if (w * h != centers.size()) return false;
	w += 2;
	h += 2;

	vector<int> paths[NC];
	for (int i = 1; i < NC; i++) {
		paths[i] = shortest_path(corners[0], corners[i]);
	}

	vector<int> indices{0,1,2,3};
	sort(indices.begin(), indices.end(), [&](int a, int b) {
		return paths[a].size() < paths[b].size();
	});

	answer[0][0] = corners[indices[0]];
	answer[0][w-1] = corners[indices[1]];
	answer[h-1][0] = corners[indices[2]];
	answer[h-1][w-1] = corners[indices[3]];

	if (shortest_path(answer[0][0], answer[0][w-1]).size() != w) return false;
	if (shortest_path(answer[0][0], answer[h-1][0]).size() != h) return false;
	if (shortest_path(answer[h-1][w-1], answer[0][w-1]).size() != h) return false;
	if (shortest_path(answer[h-1][w-1], answer[h-1][0]).size() != w) return false;

	ID cout << "decided first corner to be " << answer[0][0] << endl;
	ID cout << "decided second corner to be " << answer[0][w-1] << endl;
	ID cout << "decided third corner to be " << answer[h-1][0] << endl;
	ID cout << "decided fourth corner to be " << answer[h-1][w-1] << endl;

	vector<int> p1 = shortest_path(answer[0][0], answer[0][w-1]);
	vector<int> p2 = shortest_path(answer[0][0], answer[h-1][0]);
	vector<int> p3 = shortest_path(answer[0][w-1], answer[h-1][w-1]);
	vector<int> p4 = shortest_path(answer[h-1][0], answer[h-1][w-1]);

	for (int i = 1; i < w-1; i++) answer[0][i] = p1[i];
	for (int i = 1; i < h-1; i++) answer[i][0] = p2[i];
	for (int i = 1; i < h-1; i++) answer[i][w-1] = p3[i];
	for (int i = 1; i < w-1; i++) answer[h-1][i] = p4[i];

	for (int r = 1; r < h-1; r++) {
		for (int c = 1; c < w-1; c++) {
			answer[r][c] = find_unique(4, r-1, c);
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
