/**
 * @author Miguel Mini
 * @tag centroid_decomposition, priority_queue
 *
**/

#include <bits/stdc++.h>
using namespace std;
 
const int inf = 1e9; 
struct CentroidDecomposition {
	vector<vector<int>> T;
	vector<int> sz, P, rank;
	vector<bool> block;
	vector<vector<int>> dist;
	vector<multiset<int>> res;
	vector<int> color;
	int root;
	CentroidDecomposition(int n):
		T(n)
		, sz(n)
		, P(n, -1)
		, block(n)
		, rank(n)
		, root(0)
		, dist((int)log2(n) + 1, vector<int>(n))
		, res(n)
		, color(n)
	{}
	void add_edge(int a, int b) {
		T[a].emplace_back(b);
		T[b].emplace_back(a);
	}
	bool valid(int x, int p) {
		return x != p and not block[x];
	}
	int dfs(int x, int p=-1) {
		sz[x] = 1;
		for (int v : T[x]) {
			if (valid(v, p)) {
				sz[x] += dfs(v, x);
			}
		}
		return sz[x];
	}
	int get_centroid(int x, int p, int n) {
		for (int v : T[x]) {
			if (valid(v, p) and sz[v] > n>>1) {
				return get_centroid(v, x, n);
			}
		}
		return x;
	}
	void calc_dist(int x, int r, int p=-1, int h=0) {
		dist[r][x] = h;
		for (int v : T[x]) {
			if (valid(v, p)) {
				calc_dist(v, r, x, h+1);
			}
		}
	}
	void decompose(int x, int p=-1, int r=0) {
		int n = dfs(x);
		int cen = get_centroid(x, -1, n);
		calc_dist(cen, r);
		P[cen] = p; 
		block[cen] = 1; 
		rank[cen] = r;
		for (auto v : T[cen]) {
			if (block[v]) continue;
			decompose(v, cen, r+1);
		}
	}
	int query(int x) {
		int ans = inf, root = x;
		while (root != -1) {
			if (!res[root].empty()) {
				ans = min(ans, dist[rank[root]][x] + *res[root].begin());
			}
			root = P[root];
		}
		return ans;
	}
	void update(int x) {
		int root = x;
		color[x] ^= 1;
		while (root != -1) {
			if (color[x]) {
				res[root].insert(dist[rank[root]][x]);
			} else {
				res[root].erase(res[root].lower_bound(dist[rank[root]][x]));
			}
			root = P[root];
		}
	}
};
 
int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
	int n;
	cin >> n;
	CentroidDecomposition tree(n);
	for (int i = 0; i < n-1; ++i) {
		int u, v;
		cin >> u >> v;
		tree.add_edge(u-1, v-1);
	}
	tree.decompose(tree.root);
	int q;
	cin >> q;
	while (q--) {
		int type, v;
		cin >> type >> v;
		if (!type) {
			tree.update(v-1);
		} else {
			int ans = tree.query(v-1);
			cout << (ans != inf ? ans : -1) << endl;
		}
	}
	return 0;
} 
