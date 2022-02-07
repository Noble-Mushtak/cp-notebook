/**
 * Author: Noble Mushtak
 * Date: 2022-01-05
 * License: CC0
 * Source: me
 * Description: Segment tree with ability to add values of large intervals, and compute sum of intervals.
 * Can be changed to other things.
 * Time: O(\log N).
 * Status: stress-tested a bit
 */
#pragma once

using value = int64_t;
using upd = int64_t;
const value IDENT = 0; //Change as needed
const upd NONE = 0;
struct segtree {
    value mergeValues(value val1, value val2) { return val1+val2; }
    void updNode(value &curValue, num left, num right, upd newUpd) { curValue += (right-left+1)*newUpd; }
    void mergeUpdates(upd &origUpd, upd newUpd) { origUpd += newUpd; }

    num N;
    vector<value> nodes;
    vector<upd> lazy;
    segtree(num n = 0) : N(n), nodes(2*n, IDENT), lazy(2*n, NONE) {}
    segtree(num n, const vector<value> &vals) : segtree(n) { build(vals, 0, 0, n-1); }
    void build(const vector<value> &vals, num idx, num left, num right) {
        if (left == right) { nodes[idx] = vals[left]; return; }
        num mid = (left+right)/2;
        build(vals, idx+1, left, mid);
        build(vals, idx+2*(mid-left+1), mid+1, right);
        nodes[idx] = mergeValues(nodes[idx+1], nodes[idx+2*(mid-left+1)]);
    }
    void push(num idx, num left, num right) {
        if (lazy[idx] != NONE) {
            updNode(nodes[idx], left, right, lazy[idx]);
            num mid = (left+right)/2;
            if (left != right) mergeUpdates(lazy[idx+1], lazy[idx]), mergeUpdates(lazy[idx+2*(mid-left+1)], lazy[idx]);
            lazy[idx] = NONE;
        }
    }
    value query(num leftQ, num rightQ, num idx = 0, num left = 0, num right = -1) {
        if (right == -1) right = N-1;
        push(idx, left, right);
        if (right < leftQ || left > rightQ) return IDENT;
        if (left >= leftQ && right <= rightQ) return nodes[idx];
        num mid = (left+right)/2;
        return mergeValues(query(leftQ, rightQ, idx+1, left, mid),
                           query(leftQ, rightQ, idx+2*(mid-left+1), mid+1, right));
    }
    void update(num leftU, num rightU, upd newUpd, num idx = 0, num left = 0, num right = -1) {
        if (right == -1) right = N-1;
        push(idx, left, right);
        if (right < leftU || left > rightU) return;
        if (left >= leftU && right <= rightU) { lazy[idx] = newUpd; push(idx, left, right); return; }
        num mid = (left+right)/2;
        update(leftU, rightU, newUpd, idx+1, left, mid);
        update(leftU, rightU, newUpd, idx+2*(mid-left+1), mid+1, right);
        nodes[idx] = mergeValues(nodes[idx+1], nodes[idx+2*(mid-left+1)]);
    }
};
