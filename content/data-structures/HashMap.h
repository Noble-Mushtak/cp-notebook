/**
 * Author: nealwu, Noble Mushtak
 * Date: 2022-03-03
 * License: CC0
 * Source: https://codeforces.com/blog/entry/62393
 * Description: Hash map with mostly the same API as unordered\_map, but safe against collision attacks
 * Status: Tested against https://codeforces.com/contest/1039/problem/C
 */
#pragma once

#include <bits/extc++.h> /** keep-include */
const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
struct chash {
	static uint64_t splitmix64(uint64_t x) {
		x += 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}
	size_t operator()(uint64_t x) const { return splitmix64(x + FIXED_RANDOM); }
};
template <class T> using hashset = __gnu_pbds::gp_hash_table<T, __gnu_pbds::null_type, chash>;
template <class K, class V> using hashmap = __gnu_pbds::gp_hash_table<K, V, chash>;
