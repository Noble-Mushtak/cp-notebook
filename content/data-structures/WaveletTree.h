/**
 * Author: Noble Mushtak
 * Date: 2021-01-05
 * License: CC0
 * Source: me
 * Description: A standard wavelet tree
 * Time: O(\log N).
 * Status: stress-tested a bit
 */
#pragma once

using num_items = int;
template<class num> struct tree {
    num minVal, maxVal;
    tree *leftChild, *rightChild;
    //itemsInLeftChild[i] denotes how many elements from arr[0..i] are in the leftChild of this wavelet tree
    //NOTE: arr[0..i] does not include arr[i]
    std::vector<num_items> itemsInLeftChild;
    template<class num_it> tree(num_it beginIt, num_it endIt, num minV, num maxV) {
        leftChild = nullptr;
        rightChild = nullptr;
        reconstruct(beginIt, endIt, minV, maxV);
    }
    //Reconstruct a wavelet tree to represent the elements [beginIt, endIt), where every element is in the interval [minV, maxV]
    template<class num_it> void reconstruct(num_it beginIt, num_it endIt, num minV, num maxV) {
        minVal = minV, maxVal = maxV;
        itemsInLeftChild.clear();
        if ((minVal == maxVal) || (beginIt >= endIt)) { 
            leftChild = rightChild = nullptr;
            return;
        }
        itemsInLeftChild.reserve(endIt-beginIt+1);
        itemsInLeftChild.push_back(0);
        //Must be adjusted for negative values to use floor instead of truncated division:
        num midVal = (minVal+maxVal)/2;
        for (num_it it = beginIt; it != endIt; ++it) {
            itemsInLeftChild.push_back(itemsInLeftChild.back() + (*it <= midVal));
        }
        num_it pivotIt = std::stable_partition(beginIt, endIt, [midVal](num x) { return x <= midVal; });
        leftChild = new tree(beginIt, pivotIt, minVal, midVal);
        rightChild = new tree(pivotIt, endIt, midVal+1, maxVal);
    }
    //Find the kth smallest value in arr[leftIndex..rightIndex]
    //NOTE: leftIndex, rightIndex, k are 0-based, i.e. the smallest value is the 0th smallest value
    num kthSmallest(num_items leftIndex, num_items rightIndex, num_items k) {
        if (minVal == maxVal) return minVal;
        num_items leftIndexOfLeftChild = itemsInLeftChild[leftIndex];
        num_items rightIndexOfLeftChild = itemsInLeftChild[rightIndex];
        num_items itemsInLeftChild = rightIndexOfLeftChild-leftIndexOfLeftChild;
        if (k < itemsInLeftChild) {
            return leftChild->kthSmallest(leftIndexOfLeftChild, rightIndexOfLeftChild, k);
        } else {
            return rightChild->kthSmallest(leftIndex-leftIndexOfLeftChild, rightIndex-rightIndexOfLeftChild, k-itemsInLeftChild);
        }
    }
    //Count number of elements in arr[leftIndex..rightIndex] in the interval [queryMin, queryMax]
    num_items withinInterval(num_items leftIndex, num_items rightIndex, num queryMin, num queryMax) {
        if ((leftIndex >= rightIndex) || (queryMax < queryMin) || (queryMax < minVal) || (maxVal < queryMin)) return 0;
        if ((queryMin <= minVal) && (maxVal <= queryMax)) return rightIndex-leftIndex;
        num_items leftIndexOfLeftChild = itemsInLeftChild[leftIndex];
        num_items rightIndexOfLeftChild = itemsInLeftChild[rightIndex];
        return leftChild->withinInterval(
            leftIndexOfLeftChild, rightIndexOfLeftChild, queryMin, queryMax
        )+rightChild->withinInterval(
            leftIndex-leftIndexOfLeftChild, rightIndex-rightIndexOfLeftChild, queryMin, queryMax
        );
    }
};
