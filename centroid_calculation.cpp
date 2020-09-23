vector<int> g[maxn];
int sz[maxn];

int c1 = -1, c2 = -1;
void centroid(int v, int p){
  sz[v] = 1;
  int mx = 0;
  for (int u: g[v]) if (u ^ p) {
    centroid(u, v), sz[v] += sz[u];
    mx = max(mx, sz[u]);
  }
  mx = max(mx, n - sz[v]);
  if (2 * sz[v] == n) {
        c1 = v;
        c2 = p;
  } else if (2 * mx < n) {
        c1 = v;    
  }
}
