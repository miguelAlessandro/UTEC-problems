vector<int> g[maxn];
int sz[maxn];
using pii = pair<int, int>;

void centroid(int v, int p){
  sz[v] = 1;
  int mx = 0;
  pii res = {-1, -1};
  for (int u: g[v]) if (u ^ p) {
    res = max(res, centroid(u, v));
    sz[v] += sz[u];
    mx = max(mx, sz[u]);
  }
  mx = max(mx, n - sz[v]);
  if (2 * sz[v] == n) {
        res = {v, p};
  } else if (2 * mx < n) {
        res = {v, -1};
  }
  return res;
}
