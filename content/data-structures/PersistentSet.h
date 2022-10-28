/**
 * Author: Noble Mushtak
 * Date: 2022-10-27
 * License: CC0
 * Source: me
 * Description: Persistent set with ability to add points and query size of set.
 * Time: O(\log N).
 * Status: stress-tested a bit
 */
#pragma once

struct pset {
    pset *left, *right;
    num sze;

    pset() : left(nullptr), right(nullptr), sze(0) {}
};

pair<pset*,bool> insertPset(const pset *curPset, num newNum, num leftBnd=0, num rightBnd=250005) {
    num oldSz = 0;
    pset *newLeft = nullptr;
    pset *newRight = nullptr;
    if (curPset != nullptr) {
        oldSz = curPset->sze;
        newLeft = curPset->left;
        newRight = curPset->right;
    }
    bool toAdd = false;
    if (leftBnd < rightBnd) {
        num sumBnd = (leftBnd+rightBnd);
        num mid = (sumBnd-(sumBnd & 1))/2;
        if (newNum <= mid) {
            tie(newLeft, toAdd) = insertPset(newLeft, newNum, leftBnd, mid);
        } else {
            tie(newRight, toAdd) = insertPset(newRight, newNum, mid+1, rightBnd);
        }
    } else {
        toAdd = (curPset == nullptr);
    }
    pset *newPset = new pset();
    newPset->sze = oldSz+toAdd;
    newPset->left = newLeft;
    newPset->right = newRight;
    return {newPset, toAdd};
}
