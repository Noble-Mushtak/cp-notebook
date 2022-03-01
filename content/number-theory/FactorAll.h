/**
 * Author: Noble Mushtak
 * Date: 2022-03-01
 * License: CC0
 * Description: Prime sieve with data structure for computing all factors of a number.
 * Time: MAXN=2e7+1 $\approx$ 700 ms on CodeForces
 * Status: Tested against https://codeforces.com/contest/1499/problem/D, https://codeforces.com/contest/1593/problem/D2
 */
#pragma once

const num MAXN = 2e7+1;
vector<num> smallestPrime(MAXN, 0);
vector<num> numPrimes(MAXN, 0);
vector<pair<num, pair<num, num>>> primeFact(MAXN);
for (num p = 2; p < MAXN; ++p) {
    if (smallestPrime[p] == 0) {
        for (num j = 2*p; j < MAXN; j += p) {
            if (smallestPrime[j] == 0) smallestPrime[j] = p;
        }
        primeFact[p] = {1, {p, 1}};
        numPrimes[p] = 1;
        continue;
    }
    num curPrime = smallestPrime[p];
    primeFact[p] = primeFact[p/curPrime];
    numPrimes[p] = numPrimes[p/curPrime];
    if (primeFact[p].second.first != curPrime) {
        primeFact[p] = {p/curPrime, {curPrime, 1}};
        ++numPrimes[p];
    } else {
        ++primeFact[p].second.second;
    }
}
void allFactors(const vector<pair<num, pair<num, num>>> &primeFact, vector<num> &res, num n) {
    if (n == 1) { res.push_back(1); return; }
    num nextN, curP, curPow;
    pair<num, num> tmp;
    tie(nextN, tmp) = primeFact[n];
    tie(curP, curPow) = tmp;
    allFactors(primeFact, res, nextN);
    num maxIdx = sz(res);
    num pPow = 1;
    for (num j = 1; j <= curPow; ++j) {
        pPow *= curP;
        REPI(i, maxIdx) {
            res.push_back(res[i]*pPow);
        }
    }
}
