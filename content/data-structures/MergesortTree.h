/**
 * Author: Noble Mushtak
 * Date: 2022-10-27
 * License: CC0
 * Source: me
 * Description: Mergesort tree with ability to query points in an interval.
 * Time: O(\log^2 N).
 * Status: stress-tested a bit
 */
#pragma once

template<class value> struct msorttree {
    num N;
    vector<vector<value>> data;
    msorttree(num n, const vector<value> &vals) : N(n), data(2*n) { build(vals, 0, 0, n-1); }
    void build(const vector<value> &vals, num idx, num left, num right) {
        if (left == right) { data[idx] = {vals[left]}; return; }
        num mid = (left+right)/2;
        build(vals, idx+1, left, mid);
        build(vals, idx+2*(mid-left+1), mid+1, right);
        data[idx] = vector<value>(sz(data[idx+1])+sz(data[idx+2*(mid-left+1)]));
        merge(data[idx+1].begin(), data[idx+1].end(), data[idx+2*(mid-left+1)].begin(), data[idx+2*(mid-left+1)].end(), data[idx].begin());
    }
    //Number of bounds with x-value in [leftQ, rightQ] and value in [lowBound, upBound]
    num query(num leftQ, num rightQ, value lowBound, value upBound, num idx = 0, num left = 0, num right = -1) {
        if (right == -1) right = N-1;
        if (right < leftQ || left > rightQ) return 0;
        if (left >= leftQ && right <= rightQ) {
            num idx1 = lower_bound(data[idx].begin(), data[idx].end(), lowBound)-data[idx].begin();
            num idx2 = upper_bound(data[idx].begin(), data[idx].end(), upBound)-data[idx].begin();
            return idx2-idx1;
        }
        num mid = (left+right)/2;
        return query(leftQ, rightQ, lowBound, upBound, idx+1, left, mid) +
            query(leftQ, rightQ, lowBound, upBound, idx+2*(mid-left+1), mid+1, right);
    }
};
