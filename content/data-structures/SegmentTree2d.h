/**
 * Author: Noble Mushtak
 * Date: 2022-05-07
 * License: CC0
 * Source: folklore
 * Description: Computes minimum of a[i,j] for all x1<=i<=x2, y1<=j<=y2, and sets single elements a[x,y].
 * Time: $O(\log^2 N)$.
 * Status: stress-tested
 */
#pragma once

template<class value> struct segtree2d {
    int N, M;
    vector<vector<value>> data;
    segtree2d(int C, int R) : N(C), M(R), data(2*N, vector<value>(2*M, 1e18)) {} //Identity
    void update1(int idx, int y, value v) {
        int curIdx = y+M;
        if (idx >= N) data[idx][curIdx] = v; //Update
        else data[idx][curIdx] = min(data[2*idx][curIdx], data[2*idx+1][curIdx]); //Merge
        while (curIdx > 1) {
            curIdx >>= 1;
            data[idx][curIdx] = min(data[idx][2*curIdx], data[idx][2*curIdx+1]); //Merge
        }
    };
    void update(int x, int y, value v) {
        int curIdx = x+N;
        while (curIdx >= 1) {
            update1(curIdx, y, v);
            curIdx >>= 1;
        }
    };
    value query1(int idx, int y1, int y2) {
        int leftIdx = y1+M, rightIdx = y2+1+M;
        value ans = 1e18; //Identity
        while (leftIdx < rightIdx) {
            if (leftIdx & 1) ans = min(ans, data[idx][leftIdx]), leftIdx++; //Merge
            if (rightIdx & 1) --rightIdx, ans = min(ans, data[idx][rightIdx]); //Merge
            leftIdx >>= 1, rightIdx >>= 1;
        }
        return ans;
    };
    value query(int x1, int x2, int y1, int y2) {
        int leftIdx = x1+N, rightIdx = x2+1+N;
        value ans = 1e18; //Identity
        while (leftIdx < rightIdx) {
            if (leftIdx & 1) ans = min(ans, query1(leftIdx, y1, y2)), leftIdx++; //Merge
            if (rightIdx & 1) --rightIdx, ans = min(ans, query1(rightIdx, y1, y2)); //Merge
            leftIdx >>= 1, rightIdx >>= 1;
        }
        return ans;
    };
};
