/**
 * Author: Noble Mushtak
 * Date: 2022-03-01
 * License: CC0
 * Description: Prime sieve with data structure for computing all factors of a number.
 * Time: MAXN=2e7+1 $\approx$ 300 ms on CodeForces
 * Status: Tested against https://codeforces.com/contest/1499/problem/D, https://codeforces.com/contest/1593/problem/D2, https://codeforces.com/contest/1614/problem/D2
 */
#pragma once

const int32_t MAXN = 2e7+1;
vector<pair<int32_t, int8_t>> primeFact(MAXN);
vector<int32_t> primes;
vector<int8_t> numPrimes(MAXN);
primes.reserve(1270607);
for (int32_t p = 2; p < MAXN; ++p) {
    if (primeFact[p].first == 0) {
        primes.push_back(p);
        primeFact[p] = {p, 1};
        numPrimes[p] = 1;
    }
    for (num p2 : primes) {
        if (p*p2 >= MAXN) break;
        if (p % p2 == 0) {
            primeFact[p*p2] = primeFact[p];
            ++primeFact[p*p2].second;
            numPrimes[p*p2] = numPrimes[p];
            break;
        } else {
            primeFact[p*p2] = {p2, 1};
            numPrimes[p*p2] = numPrimes[p]+1;
        }
    }
}
void allFactors(const vector<pair<int32_t, int8_t>> &primeFact, vector<num> &res, num n) {
    if (n == 1) { res.push_back(1); return; }
    num curP, curPow;
    tie(curP, curPow) = primeFact[n];
    num nextN = n;
    REPI(i, curPow) nextN /= curP;
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
