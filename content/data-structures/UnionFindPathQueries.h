/**
 * Author: Noble Mushtak
 * Date: 2022-01-05
 * License: CC0
 * Source: me
 * Description: Union-find data structure with ability to find maximum of minimum edge weight along a path in a graph
 * Time: O(\log N).
 * Status: stress-tested a bit
 */
#pragma once

using vertex = num;
using weight = num;
const int MAXN = 2010;
const weight INF = 1e12+1;
const int MAXPATH = MAXN;
vertex path1Arr[MAXPATH], path2Arr[MAXPATH];
int p1Len, p2Len;
struct dsu {
    std::vector<vertex> parent, parentInfo;
    std::vector<int> size;
    dsu() : parent(MAXN, 0), parentInfo(MAXN, 0), size(MAXN, 1) { REPI(i, MAXN) parent[i] = i; }
    vertex find(vertex v) {
        while (parent[v] != v) v = parent[v];
        return v;
    }
    //Query minimum of maximum along all paths from v1->v2 -> unite vertices in increasing edge weight order
    //Query maximum of minimum -> unite vertices in decreasing edge weight order
    bool unite(vertex v1, vertex v2, weight info) {
        vertex par1 = find(v1);
        vertex par2 = find(v2);
        if (par1 == par2) return false;
        if (size[par1] < size[par2]) std::swap(par1, par2);
        parent[par2] = par1;
        parentInfo[par2] = info;
        size[par1] += size[par2];
        return true;
    }
    void pathToParent(vertex v, vertex* &path, int &len) {
        len = 0;
        while (parent[v] != v) {
            --path, *path = v, ++len;
            v = parent[v];
        }
        --path, *path = v, ++len;
    }
    weight findMin(vertex v1, vertex v2) {
        vertex *path1 = path1Arr+MAXPATH;
        vertex *path2 = path2Arr+MAXPATH;
        pathToParent(v1, path1, p1Len);
        pathToParent(v2, path2, p2Len);
        if (path1[0] != path2[0]) return -1; //Change if necessary!
        int idx = 0;
        while ((idx < min(p1Len, p2Len)) && (path1[idx] == path2[idx])) ++idx;
        --idx;        
        //Query minimum of maximum -> Change std::min to std::max, INF to -INF
        weight ans = INF;
        for (int j = p1Len-1; j > idx; --j) ans = std::min(ans, parentInfo[path1[j]]);
        for (int j = idx+1; j < p2Len; ++j) ans = std::min(ans, parentInfo[path2[j]]);
        return ans;
    }
};
