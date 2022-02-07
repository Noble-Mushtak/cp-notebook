/**
 * Author: Noble Mushtak
 * Date: 2022-01-05
 * Source: folklore
 * Description: A short self-balancing tree. It acts as a
 *  sequential container with log-time splits/joins, and
 *  is easy to augment with additional data.
 * Time: $O(\log N)$
 * Status: stress-tested
 */
#pragma once

#include <random> /** keep-include */
struct node;
struct treap { node *root; treap(node *r = nullptr) : root(r) {} };
std::mt19937 irand(std::random_device{}()); //std::mt19937_64 for 64-bit priority
struct node {
    num pri;
    num val, mn, mx, lz;
    int sze;
    treap left, right;
    node() {}
    node(num x) : pri(irand()), val(x), mn(x), mx(x), lz(0), sze(1), left(), right() { }
};
node* newNode(num val) { return new node(val); }
num mn(treap t) { return t.root ? t.root->mn : 1e18; }
num mx(treap t) { return t.root ? t.root->mx : -1e18; }
int sze(treap t) { return t.root ? t.root->sze : 0; }
void push(treap t) { //push should be O(1), MUST NOT recurse on left and right
    if (!t.root) return;
    num lz = t.root->lz;
    t.root->val += lz, t.root->mn += lz, t.root->mx += lz;
    if (t.root->left.root) t.root->left.root->lz += lz;
    if (t.root->right.root) t.root->right.root->lz += lz;
    t.root->lz = 0;
}
void update(treap t) {
    if (!t.root) return;
    treap &left = t.root->left, &right = t.root->right;
    push(left), push(right);
    t.root->sze = sze(left)+sze(right)+1;
    t.root->mn = std::min(t.root->val, std::min(mn(t.root->left), mn(t.root->right)));
    t.root->mx = std::max(t.root->val, std::max(mx(t.root->left), mx(t.root->right)));
}
treap merge(treap left, treap right) {
    push(left), push(right);
    treap n;
    if (!left.root || !right.root) n = left.root ? left : right;
    else if (left.root->pri > right.root->pri) left.root->right = merge(left.root->right, right), n = left;
    else right.root->left = merge(left, right.root->left), n = right;
    update(n);
    return n;
}
//pos: pos elements in left treap
//val: all elements in < val in left treap, all elements >= val in right treap
std::pair<treap, treap> split(treap t, int pos) {
    if (!t.root) return {{}, {}};
    push(t); //Add update(t) if updates affect size of left/right nodes
    treap &left = t.root->left, &right = t.root->right;
    if (sze(left) < pos) { //if (t.root->val < val) {
        auto pr = split(right, pos-sze(left)-1); //splitByVal(right, val);
        t.root->right = pr.first;
        update(t);
        return {{t.root}, pr.second};
    }
    auto pr = split(left, pos); //splitByVal(left, val);
    t.root->left = pr.second;
    update(t);
    return {pr.first, {t.root}};
}
int order(treap t, num val) {
    if (!t.root) return 0;
    push(t);
    treap &left = t.root->left, &right = t.root->right;
    if (t.root->val < val) return sze(left)+1+order(right, val);
    return order(left, val);
}
