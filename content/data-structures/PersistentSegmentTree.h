/**
 * Author: IME ICPC Notebook and Noble Mushtak
 * Date: 2021-01-05
 * License: CC0
 * Source: me
 * Description: Persistent segment tree with ability to add values to points, and compute sum of intervals.
 * Time: O(\log N).
 * Status: stress-tested a bit
 */
#pragma once

const num MAXSZ = 5000000; //> 2*MAXN*log(MAXN)
num li[MAXSZ], ri[MAXSZ]; // [li(u), ri(u)] is the interval of node u
num st[MAXSZ], lc[MAXSZ], rc[MAXSZ]; // Value, left son and right son of node u
num stsz = 0; // Nodes used so far
num init(num i, num j) { //Returns root of tree, i and j are indices of first and last elements of tree
  num v = stsz++;
  li[v] = i, ri[v] = j;
  if (i == j) st[v] = 0;
  else {
    lc[v] = init(i, (i+j)/2);
    rc[v] = init((i+j)/2+1, j);
    st[v] = 0;
  }
  return v;
}
num sum(num u, num i, num j) { //Sum from indices i to j (inclusive), u is root of tree
  if (j < li[u] or ri[u] < i) return 0;
  if (i <= li[u] and ri[u] <= j) return st[u];
  return sum(lc[u], i, j) + sum (rc[u], i, j);
}
void clone(num i, num j) { //Clone node i into index j
  li[i] = li[j], ri[i] = ri[j];
  st[i] = st[j];
  lc[i] = lc[j], rc[i] = rc[j];
}
num update(num u, num i, num v) { //Adds v to index i, u is root of tree
  if (i < li[u] or ri[u] < i) return u;
  clone(stsz, u);
  u = stsz++;
  if (li[u] == ri[u]) st[u] += v;
  else {
      lc[u] = update(lc[u], i, v);
      rc[u] = update(rc[u], i, v);
      st[u] = st[lc[u]] + st[rc[u]];
  }
  return u;
}
num findLeastIdx(num u, num threshold) { //Returns least index li[u] <= k <= ri[u] such that sum of li[u],...,k >= threshold, or ri[u]+1 if no such index exists
    if (st[u] < threshold) return ri[u]+1;
    if (li[u] == ri[u]) return li[u];
    if (st[lc[u]] < threshold) return findLeastIdx(rc[u], threshold-st[lc[u]]);
    return findLeastIdx(lc[u], threshold);
}
