/**
 * Author: Noble Mushtak
 * Date: 2022
 * License: MIT
 * Description: Computes Euclidean minimum spanning tree using fast Delaunay triangulation.
 * Time: O(n \log n)
 * Status: stress-tested
 */
#pragma once

#include "FastDelaunay.h"
#include "../data-structures/UnionFindPathQueries.h"

struct edge { num u, v, w; };
static inline num sqr(num x) { return x*x; }
static inline num dist(P p1, P p2) {
    return sqr(p1.x-p2.x)+sqr(p1.y-p2.y);
}
void findMst(num N, vector<P> pts) {
    map<P, num> idxs;
    REPI(i, N) idxs[pts[i]] = i;
    vector<P> ans = triangulate(pts);
    vector<edge> allEdges;
    allEdges.reserve(ans.size()+10);
    for (num i = 0; i < sz(ans); i += 3) {
        vector<pair<num, num>> shfts = {{0,1},{1,2},{0,2}};
        for (auto shft : shfts) {
            auto pt1 = ans[i+shft.first];
            auto pt2 = ans[i+shft.second];
            auto idx1 = idxs[pt1];
            auto idx2 = idxs[pt2];
            allEdges.push_back({ idx1, idx2, dist(pt1, pt2) });
        }
    }
    if (ans.size() == 0) {
        std::sort(pts.begin(), pts.end());
        REPI(i, sz(pts)-1) {
            allEdges.push_back({ idxs[pts[i]], idxs[pts[i+1]], dist(pts[i], pts[i+1]) });
        }
    }
    dsu curDsu;
    std::sort(allEdges.begin(), allEdges.end(), [](edge e1, edge e2) { return e1.w < e2.w; });
    for (auto e : allEdges) {
        if (curDsu.unite(e.u, e.v, e.w)) { /* this edge is part of the MST */ }
    }
}
