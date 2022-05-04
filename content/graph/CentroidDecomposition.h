/**
 * Author: Noble Mushtak
 * Date: 2022-05-03
 * License: CC0
 * Source: https://usaco.guide/plat/centroid
 * Description: Centroid decomposition which precomputes distances
 * Time: O(N \log N).
 * Status: Tested against https://codeforces.com/contest/342/problem/E
 */

void dfs(vector<vi> &adj, vi &par, vi &treesz, vector<bool> &vis, int r, int p) {
    treesz[r] = 1;
    for (int v : adj[r]) {
        if ((v == p) || vis[v]) continue;
        dfs(adj, par, treesz, vis, v, r);
        treesz[r] += treesz[v];
    }
}
int findCen(vector<vi> &adj, vi &treesz, vector<bool> &vis, int r, int p, int wholesz) {
    for (int v : adj[r]) {
        if ((v == p) || vis[v]) continue;
        if (treesz[v] > wholesz/2) return findCen(adj, treesz, vis, v, r, wholesz);
    }
    return r;
}
void fillDists(vector<vi> &adj, vector<unordered_map<int, int>> &dist, vector<bool> &vis, int r, int p, int cen) {
    for (int v : adj[r]) {
        if ((v == p) || vis[v]) continue;
        dist[cen][v] = dist[cen][r]+1;
        fillDists(adj, dist, vis, v, r, cen);
    }
}
int decomp(vector<vi> &adj, vi &par, vector<unordered_map<int, int>> &dist, vi &treesz, vector<bool> &vis, int r, int p) {
    dfs(adj, par, treesz, vis, r, -1);
    int cen = findCen(adj, treesz, vis, r, -1, treesz[r]);
    vis[cen] = true;
    par[cen] = p;
    dist[cen][cen] = 0;
    fillDists(adj, dist, vis, cen, -1, cen);
    for (int v : adj[cen]) {
        if (!vis[v]) decomp(adj, par, dist, treesz, vis, v, cen);
    }
    return cen;
}
int calcDecomp(vector<vi> &adj, vi &par, vector<unordered_map<int, int>> &dist) {
    vi treesz(sz(adj));
    vector<bool> vis(sz(adj), false);
    return decomp(adj, par, dist, treesz, vis, 0, -1);
}
