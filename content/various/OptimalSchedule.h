/**
 * Author: CP-Algorithms
 * Date: 2022-06-08
 * License: CC0
 * Source: https://cp-algorithms.com/schedules/schedule-with-completion-duration.html
 * Description: Computes schedule of jobs with duration and deadline
 * which maximizes the number of jobs which is possible to carry out.
 * Returns list of indices of jobs in the schedule, sort by deadline to get the order of the jobs.
 * Status: not tested
 */
#pragma once

struct Job {
    int deadline, duration, idx;
    bool operator<(Job o) const { return deadline < o.deadline; }
};
vector<int> compute_schedule(vector<Job> jobs) {
    sort(jobs.begin(), jobs.end());
    set<pair<int,int>> s;
    vector<int> schedule;
    for (int i = jobs.size()-1; i >= 0; i--) {
        int t = jobs[i].deadline-(i ? jobs[i-1].deadline : 0);
        s.insert(make_pair(jobs[i].duration, jobs[i].idx));
        while (t && !s.empty()) {
            auto it = s.begin();
            if (it->first <= t) {
                t -= it->first; schedule.push_back(it->second);
            } else {
                s.insert(make_pair(it->first - t, it->second));
                t = 0;
            }
            s.erase(it);
        }
    }
    return schedule;
}
