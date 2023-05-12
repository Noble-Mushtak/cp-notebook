/**
 * Author: CP-Algorithms
 * Date: 2022-06-08
 * License: CC0
 * Source: https://cp-algorithms.com/schedules/schedule_two_machines.html
 * Description: Computes optimal schedule of jobs which minimizes the total finish time
 * where each job must first be processed on the first machine and then must be processed on the second machine,
 * and job $i$ takes $a_i$ time on the first machine and $b_i$ time on the second machine.
 * Status: not tested
 */
#pragma once

struct Job {
    int a, b, idx;
    bool operator<(Job o) const { return min(a, b) < min(o.a, o.b); }
};
vector<Job> johnsons_rule(vector<Job> jobs) {
    sort(jobs.begin(), jobs.end());
    vector<Job> a, b;
    for (Job j : jobs) {
        if (j.a < j.b) a.push_back(j);
        else b.push_back(j);
    }
    a.insert(a.end(), b.rbegin(), b.rend());
    return a;
}
pair<int, int> finish_times(vector<Job> const& jobs) {
    int t1 = 0, t2 = 0;
    for (Job j : jobs) { t1 += j.a, t2 = max(t2, t1) + j.b; }
    return make_pair(t1, t2);
}
