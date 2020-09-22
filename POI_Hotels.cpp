/**
 * @author Miguel Mini
 * @tag centroid_decomposition, dsu, dp, ladder_trick
 * @complexity O(nlogn)
**/
#include <bits/stdc++.h>
using namespace std;

const int maxn = 5e3 + 10;
using ll=long long;
int L[maxn], R[maxn];
int len[maxn];
deque<int> Ld[maxn];

struct CentroidDecomposition {
	vector<vector<int>> T;
	vector<int> sz, P;
	vector<bool> block;
	CentroidDecomposition(int n):
		T(n), sz(n), P(n, -1), block(n) {}
	size_t size() {
	    return T.size();
	}
	void add_edge(int a, int b) {
		T[a].emplace_back(b);
		T[b].emplace_back(a);
	}
	bool valid(int x, int p) {
		return x != p && !block[x];
	}
	//calculate sizes
	int dfs(int x, int p=-1) {
		sz[x] = 1;
		for (int v : T[x]) if (valid(v, p))
			sz[x] += dfs(v, x);
		return sz[x];
	}
	//gradient descendt algorithm
	int get_centroid(int x, int p, int n) {
		for (int v : T[x]) 
		    if (valid(v, p) && 2 * sz[v] > n)
				return get_centroid(v, x, n);
		return x;
	}
    void dfsL(int x, int p, int v, int h) {
        L[h] += v;    
        for (int u : T[x]) {
            if (!valid(u, p)) continue;
            dfsL(u, x, v, h+1);
        }
    }
    void dfsR(int x, int p, int v, int h) {
        R[h] += v;
        for (int u : T[x]) {
            if (!valid(u, p)) continue;
            dfsR(u, x, v, h+1);
        }
    }
    ll dfsC(int x, int p, int h) {
        ll ans = L[h] * R[h];
        for (int u : T[x]) {
            if (!valid(u, p)) continue;
            ans += dfsC(u, x, h+1);
        }
        return ans;
    }
    void dfs_len(int x, int p) {
        len[x] = 0;
        for (int u : T[x]) {
            if (!valid(u, p)) continue;
            dfs_len(u, x);
            len[x] = max(len[x], len[u]+1);
        }
    }
    ll dsu(int x, int p, int h=0) {
        Ld[x].clear();
        ll ans = 0;
        int big = -1;
        for (int v : T[x]) {
            if (!valid(v, p)) continue;
            ans += dsu(v, x, h+1);
            if (big == -1 || len[big]<len[v]) {
                big = v;
            }
        }
        if (~big) swap(Ld[x], Ld[big]);
        Ld[x].push_front(1);
        for (int v : T[x]) {
            if (v == big || !valid(v, p)) continue;
            for (int i = 0; i < Ld[v].size(); ++i) {
                if (i+1 >= h) {
                    ans += Ld[v][i] *1ll* Ld[x][i+1] 
                        *(L[i+1-h] + R[i+1-h]);
                }
                Ld[x][i+1] += Ld[v][i];
            }
        }
        return ans;
    }
	ll decompose(int x=0, int p=-1) {
		int n = dfs(x);
		int cen = get_centroid(x, -1, n);
		R[0] = -1; L[0] = 1;
		dfsR(cen, -1, 1, 0);
		dfs_len(cen, -1);
		ll ans = 0;
		for (auto v : T[cen]) {
		    if (block[v]) continue;
		    dfsR(v, cen, -1, 1);
		    ans += dfsC(v, cen, 1);
		    ans += dsu(v, cen, 1);
		    dfsL(v, cen, 1, 1);
		}
		dfsL(cen, -1, -1, 0);
		P[cen] = p; //centroid parent 
		block[cen] = 1; //block node
		for (auto v : T[cen]) {
			if (block[v]) continue;
			ans += decompose(v, cen);
		}
		return ans;
	}
};

int main() {
    int n;  
    scanf("%d", &n);
    CentroidDecomposition solver(n);
    for (int i = 0; i < n-1; ++i) {
        int a, b;
        scanf("%d %d", &a, &b);
        solver.add_edge(a-1, b-1);
    }
    printf("%lld\n", solver.decompose());
    return 0;
}
