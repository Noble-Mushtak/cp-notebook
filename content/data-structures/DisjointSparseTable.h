/**
 * Author: Noble Mushtak
 * Date: 2022-02-07
 * Source: https://codeforces.com/blog/entry/79108
 * Description: A sparse table which can handle any associative operation.
 * Time: $O(N\log N)$ build time, $O(1)$ query
 * Status: stress-tested against https://codeforces.com/contest/1208/problem/E
 */
#pragma once

using value = int64_t;
struct sp_table {
    vector<vector<value>> dsTable;
    value OP(value n1, value n2) { return max(n1, n2); }

    template<class value_it> sp_table(value_it beginIt, value_it endIt) {
        int length = endIt-beginIt, pow2 = 1, levels = 0;
        while (pow2 < length) pow2 <<= 1, ++levels;
        dsTable = vector<vector<value>>(levels, vector<value>(length));
        REPI(i, length) dsTable[0][i] = beginIt[i];
        for (int j = 1; j < levels; ++j) {
            int numPairsOfBlocks = 1 << (levels-j-1);
            REPI(i, numPairsOfBlocks) {
                int midIdx = (2*i+1) << j;
                value val;
                bool set = false;
                int startIdx = i << (j+1);
                for (int k = midIdx-1; k >= startIdx; --k) { if (k < length) {
                    dsTable[j][k] = val = set ? OP(val, dsTable[0][k]) : (set = true, dsTable[0][k]);
                } }
                set = false;
                int endIdx = (i+1) << (j+1);
                for (int k = midIdx; k < endIdx; ++k) { if (k < length) {
                    dsTable[j][k] = val = set ? OP(val, dsTable[0][k]) : (set = true, dsTable[0][k]);
                } }
            }
        }
    }
    value query(int leftIdx, int rightIdx) {
        if (leftIdx == rightIdx) return dsTable[0][leftIdx];
        int level = (sizeof(int) * CHAR_BIT) - 1 - __builtin_clz(leftIdx ^ rightIdx);
        return OP(dsTable[level][leftIdx], dsTable[level][rightIdx]);
    }
};
