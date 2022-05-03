/**
 * Author: Philippe Legault
 * Date: 2016
 * License: MIT
 * Source: https://github.com/Bathlamos/delaunay-triangulation/
 * Description: Fast Delaunay triangulation.
 * Each circumcircle contains none of the input points.
 * There must be no duplicate points.
 * If all points are on a line, no triangles will be returned.
 * Should work for doubles as well, though there may be precision issues in 'circ'.
 * Returns triangles in order \{t[0][0], t[0][1], t[0][2], t[1][0], \dots\}, all counter-clockwise.
 * Time: O(n \log n)
 * Status: stress-tested
 */
#pragma once

#include "Point.h"
#include "../data-structures/UnionFindPathQueries.h"

typedef Point<ll> P;
typedef struct Quad* Q;
typedef __int128_t lll; // (can be ll if coords are < 2e4)
P arb(LLONG_MAX,LLONG_MAX); // not equal to any other point

struct Quad {
	Q rot, o; P p = arb; bool mark;
	P& F() { return r()->p; }
	Q& r() { return rot->rot; }
	Q prev() { return rot->o->rot; }
	Q next() { return r()->prev(); }
} *H;

bool circ(P p, P a, P b, P c) { // is p in the circumcircle?
	lll p2 = p.dist2(), A = a.dist2()-p2,
	    B = b.dist2()-p2, C = c.dist2()-p2;
	return p.cross(a,b)*C + p.cross(b,c)*A + p.cross(c,a)*B > 0;
}
Q makeEdge(P orig, P dest) {
	Q r = H ? H : new Quad{new Quad{new Quad{new Quad{0}}}};
	H = r->o; r->r()->r() = r;
	rep(i,0,4) r = r->rot, r->p = arb, r->o = i & 1 ? r : r->r();
	r->p = orig; r->F() = dest;
	return r;
}
void splice(Q a, Q b) {
	swap(a->o->rot->o, b->o->rot->o); swap(a->o, b->o);
}
Q connect(Q a, Q b) {
	Q q = makeEdge(a->F(), b->p);
	splice(q, a->next());
	splice(q->r(), b);
	return q;
}

pair<Q,Q> rec(const vector<P>& s) {
	if (sz(s) <= 3) {
		Q a = makeEdge(s[0], s[1]), b = makeEdge(s[1], s.back());
		if (sz(s) == 2) return { a, a->r() };
		splice(a->r(), b);
		auto side = s[0].cross(s[1], s[2]);
		Q c = side ? connect(b, a) : 0;
		return {side < 0 ? c->r() : a, side < 0 ? c : b->r() };
	}

#define H(e) e->F(), e->p
#define valid(e) (e->F().cross(H(base)) > 0)
	Q A, B, ra, rb;
	int half = sz(s) / 2;
	tie(ra, A) = rec({all(s) - half});
	tie(B, rb) = rec({sz(s) - half + all(s)});
	while ((B->p.cross(H(A)) < 0 && (A = A->next())) ||
	       (A->p.cross(H(B)) > 0 && (B = B->r()->o)));
	Q base = connect(B->r(), A);
	if (A->p == ra->p) ra = base->r();
	if (B->p == rb->p) rb = base;

#define DEL(e, init, dir) Q e = init->dir; if (valid(e)) \
		while (circ(e->dir->F(), H(base), e->F())) { \
			Q t = e->dir; \
			splice(e, e->prev()); \
			splice(e->r(), e->r()->prev()); \
			e->o = H; H = e; e = t; \
		}
	for (;;) {
		DEL(LC, base->r(), o);  DEL(RC, base, prev());
		if (!valid(LC) && !valid(RC)) break;
		if (!valid(LC) || (valid(RC) && circ(H(RC), H(LC))))
			base = connect(RC, base->r());
		else
			base = connect(base->r(), LC->r());
	}
	return { ra, rb };
}

vector<P> triangulate(vector<P> pts) {
	sort(all(pts));  assert(unique(all(pts)) == pts.end());
	if (sz(pts) < 2) return {};
	Q e = rec(pts).first;
	vector<Q> q = {e};
	int qi = 0;
	while (e->o->F().cross(e->F(), e->p) < 0) e = e->o;
#define ADD { Q c = e; do { c->mark = 1; pts.push_back(c->p); \
	q.push_back(c->r()); c = c->next(); } while (c != e); }
	ADD; pts.clear();
	while (qi < sz(q)) if (!(e = q[qi++])->mark) ADD;
	return pts;
}

//Euclidean MST:
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
