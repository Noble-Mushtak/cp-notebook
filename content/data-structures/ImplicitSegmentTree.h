/**
 * Author: Noble Mushtak
 * Date: 2022-10-28
 * License: CC0
 * Source: me
 * Description: Implicit segment tree with ability to set values of large intervals, and compute sum of intervals.
 * Can be changed to other things.
 * Time: O(\log N).
 * Status: stress-tested a bit
 */
#pragma once

using value = int64_t;
using upd = int64_t;
const value IDENT = 0; //Change as needed
const upd NONE = 0;
static value mergeValues(value val1, value val2) { return val1+val2; }
static void updNode(value &curValue, num left, num right, upd newUpd) { curValue = (right-left+1)*newUpd; }
static void mergeUpdates(upd &origUpd, upd newUpd) { origUpd = newUpd; }
constexpr num MAXQ = 200000;
struct node;
struct nalloc {
    vector<node> nodes;
    num newNode();
    nalloc() { nodes.reserve(64*MAXQ); }
};
struct node {
    value val; upd lz; num l, r;
    node() : val(IDENT), lz(NONE), l(-1), r(-1) {}
};
num nalloc::newNode() {
    nodes.push_back(node());
    return sz(nodes)-1;
}
struct segtree {
    num N, root;
    nalloc *na;
    segtree(nalloc *ourNa, num n) : N(n), na(ourNa) { root = na->newNode(); }
    void create(num &nidx) { if (nidx == -1) { nidx = na->newNode(); } }
    void push(num nidx, num left, num right) {
        if (na->nodes[nidx].lz != NONE) {
            updNode(na->nodes[nidx].val, left, right, na->nodes[nidx].lz);
            if (left != right) {
                create(na->nodes[nidx].l), create(na->nodes[nidx].r);
                mergeUpdates(na->nodes[na->nodes[nidx].l].lz, na->nodes[nidx].lz);
                mergeUpdates(na->nodes[na->nodes[nidx].r].lz, na->nodes[nidx].lz);
            }
            na->nodes[nidx].lz = 0;
        }
    }
    value query(num leftQ, num rightQ, num nidx = -2, num left = 0, num right = -1) {
        if (nidx == -2) nidx = root, right = N-1;
        assert(nidx != -1);
        push(nidx, left, right);
        if (right < leftQ || left > rightQ) return IDENT;
        if (left >= leftQ && right <= rightQ) return na->nodes[nidx].val;
        num mid = (left+right)/2;
        create(na->nodes[nidx].l), create(na->nodes[nidx].r);
        return mergeValues(query(leftQ, rightQ, na->nodes[nidx].l, left, mid), query(leftQ, rightQ, na->nodes[nidx].r, mid+1, right));
    }
    void update(num leftU, num rightU, upd newUpd, num nidx = -2, num left = 0, num right = -1) {
        if (nidx == -2) nidx = root, right = N-1;
        assert(nidx != -1);
        push(nidx, left, right);
        if (right < leftU || left > rightU) return;
        if (left >= leftU && right <= rightU) { na->nodes[nidx].lz = newUpd; push(nidx, left, right); return; }
        num mid = (left+right)/2;
        create(na->nodes[nidx].l), create(na->nodes[nidx].r);
        update(leftU, rightU, newUpd, na->nodes[nidx].l, left, mid);
        update(leftU, rightU, newUpd, na->nodes[nidx].r, mid+1, right);
        na->nodes[nidx].val = mergeValues(na->nodes[na->nodes[nidx].l].val, na->nodes[na->nodes[nidx].r].val);
    }
};
